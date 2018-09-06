/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "state.h"

#include "shmem/defs.h"

#include <unistd.h>
#include <string.h>

#include <ucp/api/ucp.h>

/*
 * -- helpers ----------------------------------------------------------------
 */

/*
 * shortcut to look up the UCP endpoint of a context
 */
inline static ucp_ep_h
lookup_ucp_ep(shmemc_context_h ch, int pe)
{
    NO_WARN_UNUSED(ch);

    return proc.comms.eps[pe];
}

/*
 * find rkey for memory "region" on PE "pe"
 */
inline static ucp_rkey_h
lookup_rkey(size_t region, int pe)
{
    return proc.comms.regions[region].minfo[pe].racc.rkey;
}

/*
 * where the heap lives on PE "pe"
 */
inline static uint64_t
get_base(size_t region, int pe)
{
    return proc.comms.regions[region].minfo[pe].base;
}

/*
 * -- translation helpers ---------------------------------------------------
 */

/*
 * is the given address in this memory region?  Non-zero if yes, 0 if
 * not.
 */
inline static int
in_region(uint64_t addr, size_t region, int pe)
{
    const mem_info_t mi = proc.comms.regions[region].minfo[pe];

    return (mi.base <= addr) && (addr < mi.end);
}

/*
 * find memory region that ADDR is in, or -1 if none
 */
inline static long
lookup_region(uint64_t addr, int pe)
{
    long r;

    /*
     * Let's search down from top heap to globals (#0) under
     * assumption most data in heaps and newest one is most likely
     * (may need to revisit)
     */
    for (r = proc.comms.nregions - 1; r >= 0; r -= 1) {
        if (in_region(addr, (size_t) r, pe)) {
            return r;
            /* NOT REACHED */
        }
    }

    return -1L;
}

/*
 * translate remote address:
 *
 * if all addresses aligned, remote always == local
 *
 * otherwise globals are always aligned, but translate shmalloc'ed
 * variables
 */
inline static uint64_t
translate_address(uint64_t local_addr, size_t region, int pe)
{
#ifdef ENABLE_ALIGNED_ADDRESSES
    NO_WARN_UNUSED(region);
    NO_WARN_UNUSED(pe);

    return local_addr;
#else
    if (region == 0) {
        return local_addr;
    }
    else {
        const uint64_t my_offset = local_addr - get_base(region, proc.rank);
        const uint64_t remote_addr = my_offset + get_base(region, pe);

        return remote_addr;
    }
#endif /* ENABLE_ALIGNED_ADDRESSES */
}

/*
 * All ops here need to find remote keys and addresses
 */
inline static void
get_remote_key_and_addr(uint64_t local_addr, int pe,
                        ucp_rkey_h *rkey_p, uint64_t *raddr_p)
{
    const long r = lookup_region(local_addr, proc.rank);

    shmemu_assert(r >= 0, "can't find memory region for %p", local_addr);

    *rkey_p = lookup_rkey(r, pe);
    *raddr_p = translate_address(local_addr, r, pe);
}

/*
 * -- ordering -----------------------------------------------------------
 */

/*
 * fence and quiet only do something on storable contexts
 */

#define SHMEMC_FENCE_QUIET(_op, _ucp_op)                            \
    void                                                            \
    shmemc_ctx_##_op(shmem_ctx_t ctx)                               \
    {                                                               \
        if (ctx != SHMEM_CTX_INVALID) {                             \
            shmemc_context_h ch = (shmemc_context_h) ctx;           \
                                                                    \
            if (! ch->attr.nostore) {                               \
                const ucs_status_t s = ucp_worker_##_ucp_op(ch->w); \
                                                                    \
                shmemu_assert(s == UCS_OK,                          \
                              "%s() failed (status: %s)", #_op,     \
                              ucs_status_string(s));                \
            }                                                       \
        }                                                           \
    }

SHMEMC_FENCE_QUIET(fence, fence)
SHMEMC_FENCE_QUIET(quiet, flush)

#ifdef ENABLE_EXPERIMENTAL

/*
 * This should be correct, but not optimal.  Gets us going.
 */

int
shmemc_ctx_fence_test(shmem_ctx_t ctx)
{
    shmemc_ctx_fence(ctx);
    return 1;
}

int
shmemc_ctx_quiet_test(shmem_ctx_t ctx)
{
    shmemc_ctx_quiet(ctx);
    return 1;
}

#endif  /* ENABLE_EXPERIMENTAL */

/*
 * a dummy callback that does nothing
 */

static void
noop_callback(void *request, ucs_status_t status)
{
    NO_WARN_UNUSED(request);
    NO_WARN_UNUSED(status);
}

/*
 * wait for some non-blocking request to complete on a worker
 *
 * TODO: possible consolidation with EP disconnect code
 */

#ifdef HAVE_UCP_REQUEST_CHECK_STATUS
# define UCX_REQUEST_CHECK(_request) ucp_request_check_status(_request)
#else
# define UCX_REQUEST_CHECK(_request) ucp_request_test(_request, NULL)
#endif  /* HAVE_UCP_REQUEST_CHECK_STATUS */

inline static ucs_status_t
check_wait_for_request(shmemc_context_h ch, void *req)
{
    if (req == NULL) {          /* completed */
        return UCS_OK;
    }
    else if (UCS_PTR_IS_ERR(req)) {
        ucp_request_cancel(ch->w, req);
        return UCS_PTR_STATUS(req);
    }
    else {                      /* wait for completion */
        ucs_status_t s;

        do {
            ucp_worker_progress(ch->w);

            s = UCX_REQUEST_CHECK(req);
        } while (s == UCS_INPROGRESS);
        ucp_request_free(req);
        return s;
    }
}

/*
 *  -- helpers for atomics -----------------------------------------------
 */

/*
 * post-or-fetch-and-wait AMO to target address "t" on PE "pe" with
 * value "v"
 */

inline static ucs_status_t
ucx_atomic_post_op(ucp_atomic_post_op_t uapo,
                   shmemc_context_h ch,
                   uint64_t t,
                   uint64_t v,    /* encapsulate 32/64-bit value */
                   size_t vs,     /* actual size of value */
                   int pe)
{
    uint64_t r_t;
    ucp_rkey_h r_key;
    ucp_ep_h ep;

    get_remote_key_and_addr(t, pe, &r_key, &r_t);
    ep = lookup_ucp_ep(ch, pe);

    return ucp_atomic_post(ep, uapo, v, vs, r_t, r_key);
}

inline static ucs_status_t
ucx_atomic_fetch_op(ucp_atomic_fetch_op_t uafo,
                    shmemc_context_h ch,
                    uint64_t t,
                    uint64_t v,    /* encapsulate 32/64-bit value */
                    size_t vs,     /* actual size of value */
                    int pe,
                    uint64_t *result)
{
    uint64_t r_t;
    ucp_rkey_h r_key;
    ucp_ep_h ep;
    ucs_status_ptr_t sp;

    get_remote_key_and_addr(t, pe, &r_key, &r_t);
    ep = lookup_ucp_ep(ch, pe);

    sp = ucp_atomic_fetch_nb(ep, uafo, v, result, vs, r_t, r_key,
                             noop_callback);

    return check_wait_for_request(ch, sp);
}

/*
 * adds
 */

#define HELPER_FADD(_size)                                          \
    inline static uint##_size##_t                                   \
    helper_atomic_fetch_add##_size(shmemc_context_h ch,             \
                                   uint64_t t, uint##_size##_t v,   \
                                   int pe)                          \
    {                                                               \
        uint##_size##_t ret = 0;                                    \
        ucs_status_t s;                                             \
        uint64_t r_t;                                               \
        ucp_rkey_h r_key;                                           \
        ucp_ep_h ep;                                                \
                                                                    \
        get_remote_key_and_addr(t, pe, &r_key, &r_t);               \
        ep = lookup_ucp_ep(ch, pe);                                 \
                                                                    \
        s = ucp_atomic_fadd##_size(ep, v, r_t, r_key, &ret);        \
        /* value came back? */                                      \
        shmemu_assert(s == UCS_OK,                                  \
                      "AMO fetch-add failed (status: %s)",          \
                      ucs_status_string(s));                        \
                                                                    \
        return ret;                                                 \
    }

HELPER_FADD(32)
HELPER_FADD(64)

#define HELPER_ADD(_size)                                       \
    inline static void                                          \
    helper_atomic_add##_size(shmemc_context_h ch,               \
                             uint64_t t, uint##_size##_t v,     \
                             int pe)                            \
    {                                                           \
        ucs_status_t s;                                         \
        uint64_t r_t;                                           \
        ucp_rkey_h r_key;                                       \
        ucp_ep_h ep;                                            \
                                                                \
        get_remote_key_and_addr(t, pe, &r_key, &r_t);           \
        ep = lookup_ucp_ep(ch, pe);                             \
                                                                \
        s = ucp_atomic_add##_size(ep, v, r_t, r_key);           \
        /* could still be in flight */                          \
        shmemu_assert((s == UCS_OK) || (s == UCS_INPROGRESS),   \
                      "AMO add op failed (status: %s)",         \
                      ucs_status_string(s));                    \
    }

HELPER_ADD(32)
HELPER_ADD(64)

/*
 * increments use add
 */

#define HELPER_FINC(_size)                                      \
    inline static uint##_size##_t                               \
    helper_atomic_fetch_inc##_size(shmemc_context_h ch,         \
                                   uint64_t t, int pe)          \
    {                                                           \
        return helper_atomic_fetch_add##_size(ch, t, 1, pe);    \
    }

HELPER_FINC(32)
HELPER_FINC(64)

#define HELPER_INC(_size)                                       \
    inline static void                                          \
    helper_atomic_inc##_size(shmemc_context_h ch,               \
                             uint64_t t, int pe)                \
    {                                                           \
        helper_atomic_add##_size(ch, t, 1, pe);                 \
    }

HELPER_INC(32)
HELPER_INC(64)

/*
 * swaps
 */

#define HELPER_SWAP(_size)                                      \
    inline static uint##_size##_t                               \
    helper_atomic_swap##_size(shmemc_context_h ch,              \
                              uint64_t t, uint##_size##_t v,    \
                              int pe)                           \
    {                                                           \
        uint64_t r_t;                                           \
        ucp_rkey_h r_key;                                       \
        uint##_size##_t ret = 0;                                \
        ucp_ep_h ep;                                            \
        ucs_status_t s;                                         \
                                                                \
        get_remote_key_and_addr(t, pe, &r_key, &r_t);           \
        ep = lookup_ucp_ep(ch, pe);                             \
                                                                \
        s = ucp_atomic_swap##_size(ep, v, r_t, r_key, &ret);    \
        shmemu_assert(s == UCS_OK,                              \
                      "AMO swap failed (status: %s)",           \
                      ucs_status_string(s));                    \
                                                                \
        return ret;                                             \
    }

HELPER_SWAP(32)
HELPER_SWAP(64)

#define HELPER_CSWAP(_size)                                             \
    inline static uint##_size##_t                                       \
    helper_atomic_cswap##_size(shmemc_context_h ch,                     \
                               uint64_t t,                              \
                               uint##_size##_t c, uint##_size##_t v,    \
                               int pe)                                  \
    {                                                                   \
        uint64_t r_t;                                                   \
        ucp_rkey_h r_key;                                               \
        uint##_size##_t ret = 0;                                        \
        ucp_ep_h ep;                                                    \
        ucs_status_t s;                                                 \
                                                                        \
        get_remote_key_and_addr(t, pe, &r_key, &r_t);                   \
        ep = lookup_ucp_ep(ch, pe);                                     \
                                                                        \
        s = ucp_atomic_cswap##_size(ep, c, v, r_t, r_key, &ret);        \
        shmemu_assert(s == UCS_OK,                                      \
                      "AMO conditional swap failed (status: %s)",       \
                      ucs_status_string(s));                            \
                                                                        \
        return ret;                                                     \
    }

HELPER_CSWAP(32)
HELPER_CSWAP(64)

/*
 * bitwise helpers
 *
 * Newer versions of UCX now have native support for bitwise atomics:
 * we detect this during configure.
 *
 */

#ifdef HAVE_UCP_BITWISE_ATOMICS

#define MAKE_UCP_FETCH_OP(_op)     UCP_ATOMIC_FETCH_OP_F##_op
#define MAKE_UCP_POST_OP(_op)      UCP_ATOMIC_POST_OP_##_op

#define HELPER_BITWISE_FETCH_ATOMIC(_ucp_op, _opname, _size)            \
    inline static uint##_size##_t                                       \
    helper_atomic_fetch_##_opname##_size(shmemc_context_h ch,           \
                                         uint64_t t, uint##_size##_t v, \
                                         int pe)                        \
    {                                                                   \
        uint64_t ret = 0;                                               \
        const ucs_status_t s =                                          \
            ucx_atomic_fetch_op(MAKE_UCP_FETCH_OP(_ucp_op),             \
                                ch,                                     \
                                t,                                      \
                                v, sizeof(v),                           \
                                pe,                                     \
                                &ret);                                  \
                                                                        \
        /* value came back? */                                          \
        shmemu_assert(s == UCS_OK,                                      \
                      "AMO fetch op \"%s\" failed (status: %s)",        \
                      #_opname, ucs_status_string(s));                  \
                                                                        \
        return (uint##_size##_t) ret;                                   \
    }

HELPER_BITWISE_FETCH_ATOMIC(AND, and, 32)
HELPER_BITWISE_FETCH_ATOMIC(AND, and, 64)
HELPER_BITWISE_FETCH_ATOMIC(OR,  or,  32)
HELPER_BITWISE_FETCH_ATOMIC(OR,  or,  64)
HELPER_BITWISE_FETCH_ATOMIC(XOR, xor, 32)
HELPER_BITWISE_FETCH_ATOMIC(XOR, xor, 64)

#define HELPER_BITWISE_ATOMIC(_ucp_op, _opname, _size)              \
    inline static void                                              \
    helper_atomic_##_opname##_size(shmemc_context_h ch,             \
                                   uint64_t t, uint##_size##_t v,   \
                                   int pe)                          \
    {                                                               \
        const ucs_status_t s =                                      \
            ucx_atomic_post_op(MAKE_UCP_POST_OP(_ucp_op),           \
                               ch,                                  \
                               t,                                   \
                               v, sizeof(v),                        \
                               pe);                                 \
                                                                    \
        shmemu_assert((s == UCS_OK) || (s == UCS_INPROGRESS),       \
                      "AMO post op \"%s\" failed (status: %s)",     \
                      #_opname, ucs_status_string(s));              \
    }

HELPER_BITWISE_ATOMIC(AND, and, 32)
HELPER_BITWISE_ATOMIC(AND, and, 64)
HELPER_BITWISE_ATOMIC(OR,  or,  32)
HELPER_BITWISE_ATOMIC(OR,  or,  64)
HELPER_BITWISE_ATOMIC(XOR, xor, 32)
HELPER_BITWISE_ATOMIC(XOR, xor, 64)

#else  /* ! HAVE_UCP_BITWISE_ATOMICS */

#define HELPER_BITWISE_FETCH_ATOMIC(_op, _opname, _size)                \
    inline static uint##_size##_t                                       \
    helper_atomic_fetch_##_opname##_size(shmemc_context_h ch,           \
                                         uint64_t t, uint##_size##_t v, \
                                         int pe)                        \
    {                                                                   \
        uint##_size##_t ret = 0;                                        \
        uint##_size##_t rval, rval_orig;                                \
        ucs_status_t s;                                                 \
        uint64_t r_t;                                                   \
        ucp_rkey_h r_key;                                               \
        ucp_ep_h ep;                                                    \
                                                                        \
        get_remote_key_and_addr(t, pe, &r_key, &r_t);                   \
        ep = lookup_ucp_ep(ch, pe);                                     \
                                                                        \
        do {                                                            \
            s = ucp_get(ep, &rval_orig, sizeof(rval_orig),              \
                        r_t, r_key);                                    \
            shmemu_assert(s == UCS_OK,                                  \
                          "AMO fetch failed in CAS (status: %s)",       \
                          ucs_status_string(s));                        \
            rval = (rval_orig) _op v;                                   \
                                                                        \
            s = ucp_atomic_cswap##_size(ep, rval_orig, rval,            \
                                        r_t, r_key, &ret);              \
        } while (ret != rval_orig);                                     \
                                                                        \
        return ret;                                                     \
    }

HELPER_BITWISE_FETCH_ATOMIC(|, or,  32)
HELPER_BITWISE_FETCH_ATOMIC(|, or,  64)
HELPER_BITWISE_FETCH_ATOMIC(&, and, 32)
HELPER_BITWISE_FETCH_ATOMIC(&, and, 64)
HELPER_BITWISE_FETCH_ATOMIC(^, xor, 32)
HELPER_BITWISE_FETCH_ATOMIC(^, xor, 64)

#define HELPER_BITWISE_ATOMIC(_op, _opname, _size)                      \
    inline static void                                                  \
    helper_atomic_##_opname##_size(shmemc_context_h ch,                 \
                                   uint64_t t, uint##_size##_t v,       \
                                   int pe)                              \
    {                                                                   \
        (void) helper_atomic_fetch_##_opname##_size(ch, t, v, pe);      \
    }

HELPER_BITWISE_ATOMIC(|, or,  32)
HELPER_BITWISE_ATOMIC(|, or,  64)
HELPER_BITWISE_ATOMIC(&, and, 32)
HELPER_BITWISE_ATOMIC(&, and, 64)
HELPER_BITWISE_ATOMIC(^, xor, 32)
HELPER_BITWISE_ATOMIC(^, xor, 64)

#endif  /* HAVE_UCP_BITWISE_ATOMICS */

/**
 * API
 *
 **/

/*
 * make progress on a context
 */

inline static void
helper_ctx_progress(shmem_ctx_t ctx)
{
    shmemc_context_h ch = (shmemc_context_h) ctx;

    (void) ucp_worker_progress(ch->w);
}

inline static void
helper_progress(void)
{
    helper_ctx_progress(SHMEM_CTX_DEFAULT);
}

void
shmemc_ctx_progress(shmem_ctx_t ctx)
{
    helper_ctx_progress(ctx);
}

void
shmemc_progress(void)
{
    helper_ctx_progress(SHMEM_CTX_DEFAULT);
}

/*
 * -- accessible memory pointers -----------------------------------------
 */

/*
 * See if addr is reachable using given context.  Return usable
 * address if so, otherwise NULL.
 */

void *
shmemc_ctx_ptr(shmem_ctx_t ctx, const void *addr, int pe)
{
    NO_WARN_UNUSED(ctx);

    /* check to see if UCX is new enough */
#ifdef HAVE_UCP_RKEY_PTR
    uint64_t r_addr;            /* address on other PE */
    ucp_rkey_h r_key;            /* rkey for remote address */
    void *usable_addr = NULL;
    ucs_status_t s;

    get_remote_key_and_addr((uint64_t) addr, pe, &r_key, &r_addr);

    s = ucp_rkey_ptr(r_key, r_addr, &usable_addr);
    if (s == UCS_OK) {
        return usable_addr;
        /* NOT REACHED */
    }
    /*
     * fall through and ...
     */
#endif  /* HAVE_UCP_RKEY_PTR */

    return NULL;
}

/*
 * Return non-zero if adddress is remotely accessible, 0 otherwise
 */
int
shmemc_addr_accessible(const void *addr, int pe)
{
    return lookup_region((uint64_t) addr, pe) >= 0;
}

/*
 * Return non-zero if a valid PE #, 0 otherwise.
 *
 * N.B. this function was originally meant to test which ranks were
 * SHMEM in mixed SHMEM/MPI programs.  Here we just check the rank
 * number is valid and assume reachability, although might want to
 * revisit for FT/resilience purposes.
 */
int
shmemc_pe_accessible(int pe)
{
    return IS_VALID_PE_NUMBER(pe);
}

/*
 * -- puts & gets --------------------------------------------------------
 */

void
shmemc_ctx_put(shmem_ctx_t ctx,
               void *dest, const void *src,
               size_t nbytes, int pe)
{
    shmemc_context_h ch = (shmemc_context_h) ctx;
    uint64_t r_dest;            /* address on other PE */
    ucp_rkey_h r_key;            /* rkey for remote address */
    ucp_ep_h ep;
#ifdef HAVE_UCP_PUT_NB
    ucs_status_ptr_t sp;
#endif /* HAVE_UCP_PUT_NB */
    ucs_status_t s;

    get_remote_key_and_addr((uint64_t) dest, pe, &r_key, &r_dest);
    ep = lookup_ucp_ep(ch, pe);

#ifdef HAVE_UCP_PUT_NB
    sp = ucp_put_nb(ep, src, nbytes, r_dest, r_key,
                    noop_callback);
    s = check_wait_for_request(ch, sp);
#else
    s = ucp_put(ep, src, nbytes, r_dest, r_key);
#endif /* HAVE_UCP_PUT_NB */

    shmemu_assert(s == UCS_OK,
                  "put failed (status: %s)",
                  ucs_status_string(s));
}

void
shmemc_ctx_get(shmem_ctx_t ctx,
               void *dest, const void *src,
               size_t nbytes, int pe)
{
    shmemc_context_h ch = (shmemc_context_h) ctx;
    uint64_t r_src;
    ucp_rkey_h r_key;
    ucp_ep_h ep;
#ifdef HAVE_UCP_GET_NB
    ucs_status_ptr_t sp;
#endif /* HAVE_UCP_GET_NB */
    ucs_status_t s;

    get_remote_key_and_addr((uint64_t) src, pe, &r_key, &r_src);
    ep = lookup_ucp_ep(ch, pe);

#ifdef HAVE_UCP_GET_NB
    sp = ucp_get_nb(ep, dest, nbytes, r_src, r_key,
                    noop_callback);
    s = check_wait_for_request(ch, sp);
#else
    s = ucp_get(ep, dest, nbytes, r_src, r_key);
#endif /* HAVE_UCP_GET_NB */

    shmemu_assert(s == UCS_OK,
                  "get failed (status: %s)",
                  ucs_status_string(s));
}

/*
 * strided ops currently build on put/get in upper API
 */

/**
 * Return status from UCP nbi routines probably needs more handling
 *
 */

void
shmemc_ctx_put_nbi(shmem_ctx_t ctx,
                   void *dest, const void *src,
                   size_t nbytes, int pe)
{
    shmemc_context_h ch = (shmemc_context_h) ctx;
    uint64_t r_dest;
    ucp_rkey_h r_key;
    ucp_ep_h ep;
    ucs_status_t s;

    get_remote_key_and_addr((uint64_t) dest, pe, &r_key, &r_dest);
    ep = lookup_ucp_ep(ch, pe);

    s = ucp_put_nbi(ep, src, nbytes, r_dest, r_key);
    shmemu_assert((s == UCS_OK) || (s == UCS_INPROGRESS),
                  "non-blocking put failed (status: %s)",
                  ucs_status_string(s));
}

void
shmemc_ctx_get_nbi(shmem_ctx_t ctx,
                   void *dest, const void *src,
                   size_t nbytes, int pe)
{
    shmemc_context_h ch = (shmemc_context_h) ctx;
    uint64_t r_src;
    ucp_rkey_h r_key;
    ucp_ep_h ep;
    ucs_status_t s;

    get_remote_key_and_addr((uint64_t) src, pe, &r_key, &r_src);
    ep = lookup_ucp_ep(ch, pe);

    s = ucp_get_nbi(ep, dest, nbytes, r_src, r_key);
    shmemu_assert((s == UCS_OK) || (s == UCS_INPROGRESS),
                  "non-blocking get failed (status: %s)",
                  ucs_status_string(s));
 }

#define SHMEMC_PUTGET_SIGNAL(_op)                                       \
    void                                                                \
    shmemc_ctx_##_op##_signal(shmem_ctx_t ctx,                          \
                              void *dest, const void *src,              \
                              size_t nbytes,                            \
                              uint64_t *s_target, uint64_t s_val,       \
                              int pe)                                   \
    {                                                                   \
        shmemc_ctx_##_op(ctx, dest, src, nbytes, pe);                   \
        shmemc_ctx_fence(ctx);                                          \
        shmemc_ctx_##_op(ctx, s_target, &s_val, sizeof(s_val), pe);     \
    }

SHMEMC_PUTGET_SIGNAL(put)
SHMEMC_PUTGET_SIGNAL(get)

/*
 * -- atomics ------------------------------------------------------------
 */

/**
 * AMO API
 **/

/*
 * add
 */

#define SHMEMC_CTX_ADD(_size)                               \
    void                                                    \
    shmemc_ctx_add##_size(shmem_ctx_t ctx,                  \
                          void *t, uint64_t v, int pe)      \
    {                                                       \
        helper_atomic_add##_size(ctx, (uint64_t) t, v, pe); \
    }

SHMEMC_CTX_ADD(32)
SHMEMC_CTX_ADD(64)

/*
 * inc
 */

#define SHMEMC_CTX_INC(_size)                               \
    void                                                    \
    shmemc_ctx_inc##_size(shmem_ctx_t ctx,                  \
                          void *t, int pe)                  \
    {                                                       \
        helper_atomic_inc##_size(ctx, (uint64_t) t, pe);    \
    }

SHMEMC_CTX_INC(32)
SHMEMC_CTX_INC(64)

/*
 * fetch-and-add
 */

#define SHMEMC_CTX_FADD(_size)                                      \
    uint64_t                                                        \
    shmemc_ctx_fadd##_size(shmem_ctx_t ctx,                         \
                           void *t, uint64_t v, int pe)             \
    {                                                               \
        return helper_atomic_fetch_add##_size(ctx, (uint64_t) t,    \
                                              v, pe);               \
    }

SHMEMC_CTX_FADD(32)
SHMEMC_CTX_FADD(64)

/*
 * fetch-and-inc
 */

#define SHMEMC_CTX_FINC(_size)                                      \
    uint64_t                                                        \
    shmemc_ctx_finc##_size(shmem_ctx_t ctx,                         \
                           void *t, int pe)                         \
    {                                                               \
        return helper_atomic_fetch_inc##_size(ctx, (uint64_t) t,    \
                                              pe);                  \
    }

SHMEMC_CTX_FINC(32)
SHMEMC_CTX_FINC(64)

/*
 * swaps
 */

#define SHMEMC_CTX_SWAP(_size)                                          \
    uint64_t                                                            \
    shmemc_ctx_swap##_size(shmem_ctx_t ctx,                             \
                           void *t, uint64_t v, int pe)                 \
    {                                                                   \
        return helper_atomic_swap##_size(ctx, (uint64_t) t, v, pe);     \
    }

SHMEMC_CTX_SWAP(32)
SHMEMC_CTX_SWAP(64)

#define SHMEMC_CTX_CSWAP(_size)                                         \
    uint64_t                                                            \
    shmemc_ctx_cswap##_size(shmem_ctx_t ctx,                            \
                            void *t, uint64_t c, uint64_t v, int pe)    \
    {                                                                   \
        return helper_atomic_cswap##_size(ctx, (uint64_t) t, c, v, pe); \
    }

SHMEMC_CTX_CSWAP(32)
SHMEMC_CTX_CSWAP(64)

/*
 * fetch & set
 *
 */

#define SHMEMC_CTX_FETCH(_size)                                     \
    uint64_t                                                        \
    shmemc_ctx_fetch##_size(shmem_ctx_t ctx,                        \
                            void *t, int pe)                        \
    {                                                               \
        return helper_atomic_fetch_add##_size(ctx, (uint64_t) t,    \
                                              0, pe);               \
    }

SHMEMC_CTX_FETCH(32)
SHMEMC_CTX_FETCH(64)

/*
 * set/fetch
 */
#define SHMEMC_CTX_SET(_size)                                       \
    void                                                            \
    shmemc_ctx_set##_size(shmem_ctx_t ctx,                          \
                          void *t, uint64_t v, int pe)              \
    {                                                               \
        (void) helper_atomic_swap##_size(ctx, (uint64_t) t, v, pe); \
    }

SHMEMC_CTX_SET(32)
SHMEMC_CTX_SET(64)

/*
 * fetched-bitwise
 */

#define SHMEMC_CTX_FETCH_BITWISE(_op, _size)                        \
    uint64_t                                                        \
    shmemc_ctx_fetch_##_op##_size(shmem_ctx_t ctx,                  \
                                  void *t, uint64_t v, int pe)      \
    {                                                               \
        return helper_atomic_fetch_##_op##_size(ctx, (uint64_t) t,  \
                                                v, pe);             \
    }

SHMEMC_CTX_FETCH_BITWISE(and, 32)
SHMEMC_CTX_FETCH_BITWISE(and, 64)
SHMEMC_CTX_FETCH_BITWISE(or,  32)
SHMEMC_CTX_FETCH_BITWISE(or,  64)
SHMEMC_CTX_FETCH_BITWISE(xor, 32)
SHMEMC_CTX_FETCH_BITWISE(xor, 64)

/*
 * bitwise
 */

#define SHMEMC_CTX_BITWISE(_op, _size)                              \
    void                                                            \
    shmemc_ctx_##_op##_size(shmem_ctx_t ctx,                        \
                            void *t, uint64_t v, int pe)            \
    {                                                               \
        helper_atomic_##_op##_size(ctx, (uint64_t) t,               \
                                   v, pe);                          \
    }

SHMEMC_CTX_BITWISE(and, 32)
SHMEMC_CTX_BITWISE(and, 64)
SHMEMC_CTX_BITWISE(or,  32)
SHMEMC_CTX_BITWISE(or,  64)
SHMEMC_CTX_BITWISE(xor, 32)
SHMEMC_CTX_BITWISE(xor, 64)
