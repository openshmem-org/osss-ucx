/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"
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
    return ch->eps[pe];
}

/*
 * find rkey for memory "region" on PE "pe"
 */
inline static ucp_rkey_h
lookup_rkey(shmemc_context_h ch, size_t region, int pe)
{
    return ch->racc[region].rinfo[pe].rkey;
}

/*
 * -- translation helpers ---------------------------------------------------
 */

/*
 * is the given address in this memory region?  Non-zero if yes, 0 if
 * not.
 */
inline static int
in_region(uint64_t addr, size_t region)
{
    const mem_info_t *mip = & proc.comms.regions[region].minfo[proc.rank];

    return (mip->base <= addr) && (addr < mip->end);
}

/*
 * find memory region that ADDR is in, or -1 if none
 */
inline static long
lookup_region(uint64_t addr)
{
    long r;

    /*
     * Let's search down from top heap to globals (#0) under
     * assumption most data in heaps and newest one is most likely
     * (may need to revisit)
     */
    for (r = proc.comms.nregions - 1; r >= 0; --r) {
        if (in_region(addr, (size_t) r)) {
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
#ifdef ENABLE_ALIGNED_ADDRESSES
# define translate_region_address(_local_addr, _region, _pe) (_local_addr)
# define translate_address(_local_addr, _pe) (_local_addr)
#else

/*
 * where the heap lives on PE "pe"
 */
# define get_base(_region, _pe) proc.comms.regions[_region].minfo[_pe].base

inline static uint64_t
translate_region_address(uint64_t local_addr, size_t region, int pe)
{
    if (region == 0) {
        return local_addr;
    }
    else {
        const long my_offset = local_addr - get_base(region, proc.rank);

        if (my_offset < 0) {
            return 0;
        }

        return my_offset + get_base(region, pe);
    }
}

inline static uint64_t
translate_address(uint64_t local_addr, int pe)
{
    long r = lookup_region(local_addr);

    if (r < 0) {
        return 0;
    }

    return translate_region_address(local_addr, r, pe);
}
#endif  /* ENABLE_ALIGNED_ADDRESSES */

/*
 * All ops here need to find remote keys and addresses
 */
inline static void
get_remote_key_and_addr(shmemc_context_h ch,
                        uint64_t local_addr, int pe,
                        ucp_rkey_h *rkey_p, uint64_t *raddr_p)
{
    const long r = lookup_region(local_addr);

    shmemu_assert(r >= 0,
                  "can't find memory region for %p",
                  (void *) local_addr);

    *rkey_p = lookup_rkey(ch, r, pe);
    *raddr_p = translate_region_address(local_addr, r, pe);
}

/*
 * -- ordering -----------------------------------------------------------
 */

/*
 * fence and quiet only do something on storable contexts, but
 * currently, progress is on the default context
 */

#define SHMEMC_FENCE_QUIET(_op, _ucp_op)                                \
    void                                                                \
    shmemc_ctx_##_op(shmem_ctx_t ctx)                                   \
    {                                                                   \
        if (ctx != SHMEM_CTX_INVALID) {                                 \
            shmemc_context_h ch = (shmemc_context_h) ctx;               \
                                                                        \
            if (! ch->attr.nostore) {                                   \
                const ucs_status_t s = ucp_worker_##_ucp_op(ch->w);     \
                                                                        \
                shmemu_assert(s == UCS_OK,                              \
                              "%s() failed (status: %s)", #_op,         \
                              ucs_status_string(s));                    \
            }                                                           \
        }                                                               \
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

void
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
    NO_WARN_UNUSED(ch);

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

static ucs_status_t
helper_posted_amo(shmemc_context_h ch,
                  ucp_atomic_post_op_t uapo,
                  void *t, void *vp, size_t vs,
                  int pe)
{
    uint64_t r_t;
    ucp_rkey_h r_key;
    ucp_ep_h ep;
    uint64_t rv = *(uint64_t *) vp;

    get_remote_key_and_addr(ch, (uint64_t) t, pe, &r_key, &r_t);
    ep = lookup_ucp_ep(ch, pe);

    return ucp_atomic_post(ep, uapo, rv, vs, r_t, r_key);
}

/*
 * fetching AMOs: target t, (optional) condition c, value v
 */

static ucs_status_t
helper_fetching_amo(shmemc_context_h ch,
                    ucp_atomic_fetch_op_t op,
                    void *t, void *vp, size_t vs,
                    int pe,
                    void *retp)
{
    ucp_rkey_h r_key;
    uint64_t r_t;
    ucp_ep_h ep;
    uint64_t rv = *(uint64_t *) vp;
    ucs_status_ptr_t sp;

    get_remote_key_and_addr(ch, (uint64_t) t, pe, &r_key, &r_t);
    ep = lookup_ucp_ep(ch, pe);

    sp = ucp_atomic_fetch_nb(ep,
                             op,
                             rv, retp, vs,
                             r_t, r_key, noop_callback);

    return check_wait_for_request(ch, sp);
}

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
    /* check to see if UCX is new enough */
#ifdef HAVE_UCP_RKEY_PTR
    shmemc_context_h ch = (shmemc_context_h) ctx;
    uint64_t r_addr;            /* address on other PE */
    ucp_rkey_h r_key;            /* rkey for remote address */
    void *usable_addr = NULL;
    ucs_status_t s;

    get_remote_key_and_addr(ch, (uint64_t) addr, pe, &r_key, &r_addr);

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
 * Return non-zero if adddress is remotely accessible, 0 otherwise.
 *
 */
int
shmemc_addr_accessible(const void *addr, int pe)
{
    uint64_t uaddr = (uint64_t) addr;

    NO_WARN_UNUSED(pe);         /* if aligned addresses */

    return translate_address(uaddr, pe) > 0;
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

    get_remote_key_and_addr(ch, (uint64_t) dest, pe, &r_key, &r_dest);
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

    get_remote_key_and_addr(ch, (uint64_t) src, pe, &r_key, &r_src);
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

    get_remote_key_and_addr(ch, (uint64_t) dest, pe, &r_key, &r_dest);
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

    get_remote_key_and_addr(ch, (uint64_t) src, pe, &r_key, &r_src);
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

void
shmemc_ctx_add(shmem_ctx_t ctx,
               void *t, void *vp, size_t vs,
               int pe)
{
    shmemc_context_h ch = (shmemc_context_h) ctx;

    helper_posted_amo(ch, UCP_ATOMIC_POST_OP_ADD, t, vp, vs, pe);
}

/*
 * inc = add 1
 */

/*
 * fetch-and-add
 */

void
shmemc_ctx_fadd(shmem_ctx_t ctx,
                void *t, void *vp, size_t vs,
                int pe,
                void *retp)
{
    shmemc_context_h ch = (shmemc_context_h) ctx;

    helper_fetching_amo(ch,
                        UCP_ATOMIC_FETCH_OP_FADD,
                        t, vp, vs, pe, retp);
}

/*
 * fetch-and-inc: finc = fadd 1
 */

/*
 * swaps
 */

void
shmemc_ctx_swap(shmem_ctx_t ctx,
                void *t, void *vp, size_t vs,
                int pe,
                void *retp)
{
    shmemc_context_h ch = (shmemc_context_h) ctx;
    ucs_status_t s;

    s = helper_fetching_amo(ch,
                            UCP_ATOMIC_FETCH_OP_SWAP,
                            t, vp, vs,
                            pe,
                            retp);

    shmemu_assert(s == UCS_OK,
                  "AMO swap failed (status: %s)",
                  ucs_status_string(s));
}

void
shmemc_ctx_cswap(shmem_ctx_t ctx,
                 void *t, void *c, void *vp, size_t vs,
                 int pe,
                 void *retp)
{
    shmemc_context_h ch = (shmemc_context_h) ctx;
    ucs_status_t s;

    memcpy(retp, vp, vs);       /* prime the value */

    s = helper_fetching_amo(ch,
                            UCP_ATOMIC_FETCH_OP_CSWAP,
                            t, c, vs,
                            pe,
                            retp);

    shmemu_assert(s == UCS_OK,
                  "AMO conditional swap failed (status: %s)",
                  ucs_status_string(s));
}

/*
 * fetch handled via typed-0-swap
 */

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

#define HELPER_BITWISE_FETCH_ATOMIC(_ucp_op, _opname)                   \
    inline static void                                                  \
    helper_atomic_fetch_##_opname(shmemc_context_h ch,                  \
                                  void *t, void *vp, size_t vs,         \
                                  int pe,                               \
                                  void *retp)                           \
    {                                                                   \
        const ucs_status_t s =                                          \
            helper_fetching_amo(ch,                                     \
                                MAKE_UCP_FETCH_OP(_ucp_op),             \
                                t, vp, vs,                              \
                                pe,                                     \
                                retp);                                  \
                                                                        \
        /* value came back? */                                          \
        shmemu_assert(s == UCS_OK,                                      \
                      "AMO fetch op \"%s\" failed (status: %s)",        \
                      #_opname, ucs_status_string(s));                  \
    }

HELPER_BITWISE_FETCH_ATOMIC(AND, and)
HELPER_BITWISE_FETCH_ATOMIC(OR,  or)
HELPER_BITWISE_FETCH_ATOMIC(XOR, xor)

#define HELPER_BITWISE_ATOMIC(_ucp_op, _opname)                         \
    inline static void                                                  \
    helper_atomic_##_opname(shmemc_context_h ch,                        \
                            void *t, void *vp, size_t vs,               \
                            int pe)                                     \
    {                                                                   \
        const ucs_status_t s =                                          \
            helper_posted_amo(ch,                                       \
                              MAKE_UCP_POST_OP(_ucp_op),                \
                              t, vp, vs,                                \
                              pe);                                      \
                                                                        \
        shmemu_assert(s == UCS_OK,                                      \
                      "AMO post op \"%s\" failed (status: %s)",         \
                      #_opname, ucs_status_string(s));                  \
    }

HELPER_BITWISE_ATOMIC(AND, and)
HELPER_BITWISE_ATOMIC(OR,  or)
HELPER_BITWISE_ATOMIC(XOR, xor)

#else  /* ! HAVE_UCP_BITWISE_ATOMICS */

#define HELPER_BITWISE_FETCH_ATOMIC(_op, _opname)                       \
    inline static void                                                  \
    helper_atomic_fetch_##_opname(shmemc_context_h ch,                  \
                                  void *t, void *vp, size_t vs,         \
                                  int pe,                               \
                                  void *retp)                           \
    {                                                                   \
        /* make sure zeroed for smaller types */                        \
        uint64_t ret = 0, rval = 0, rval_orig = 0, vcomp = 0;           \
        ucs_status_t s;                                                 \
        uint64_t r_t;                                                   \
        ucp_rkey_h r_key;                                               \
        ucp_ep_h ep;                                                    \
                                                                        \
        memcpy(&vcomp, vp, vs); /* save comparator */                   \
        get_remote_key_and_addr(ch, (uint64_t) t, pe, &r_key, &r_t);    \
        ep = lookup_ucp_ep(ch, pe);                                     \
                                                                        \
        do {                                                            \
            s = ucp_get(ep, &rval_orig, sizeof(rval_orig),              \
                        r_t, r_key);                                    \
            shmemu_assert(s == UCS_OK,                                  \
                          "AMO fetch failed in CAS (status: %s)",       \
                          ucs_status_string(s));                        \
            rval = (rval_orig) _op vcomp;                               \
                                                                        \
            s = ucp_atomic_cswap64(ep, rval_orig, rval,                 \
                                   r_t, r_key, &ret);                   \
        } while (ret != rval_orig);                                     \
                                                                        \
        memcpy(retp, &ret, vs); /* copy out answer */                   \
    }

HELPER_BITWISE_FETCH_ATOMIC(|, or)
HELPER_BITWISE_FETCH_ATOMIC(&, and)
HELPER_BITWISE_FETCH_ATOMIC(^, xor)

#define HELPER_BITWISE_ATOMIC(_opname)                                  \
    inline static void                                                  \
    helper_atomic_##_opname(shmemc_context_h ch,                        \
                            void *t, void *vp, size_t vs,               \
                            int pe)                                     \
    {                                                                   \
        uint64_t discard;                                               \
        (void) helper_atomic_fetch_##_opname(ch, t, vp, vs, pe, &discard); \
    }

HELPER_BITWISE_ATOMIC(or)
HELPER_BITWISE_ATOMIC(and)
HELPER_BITWISE_ATOMIC(xor)

#endif  /* HAVE_UCP_BITWISE_ATOMICS */

/*
 * fetched-bitwise
 */

#define SHMEMC_CTX_FETCH_BITWISE(_op)                                   \
    void                                                                \
    shmemc_ctx_fetch_##_op(shmem_ctx_t ctx,                             \
                           void *t, void *vp, size_t vs,                \
                           int pe,                                      \
                           void *retp)                                  \
    {                                                                   \
        shmemc_context_h ch = (shmemc_context_h) ctx;                   \
                                                                        \
        helper_atomic_fetch_##_op(ch,                                   \
                                  t, vp, vs,                            \
                                  pe,                                   \
                                  retp);                                \
    }

SHMEMC_CTX_FETCH_BITWISE(and)
SHMEMC_CTX_FETCH_BITWISE(or)
SHMEMC_CTX_FETCH_BITWISE(xor)

/*
 * bitwise
 */

#define SHMEMC_CTX_BITWISE(_op)                                     \
    void                                                            \
    shmemc_ctx_##_op(shmem_ctx_t ctx,                               \
                     void *t, void *vp, size_t vs, int pe)          \
    {                                                               \
        shmemc_context_h ch = (shmemc_context_h) ctx;               \
                                                                    \
        helper_atomic_##_op(ch, t, vp, vs, pe);                     \
    }

SHMEMC_CTX_BITWISE(and)
SHMEMC_CTX_BITWISE(or)
SHMEMC_CTX_BITWISE(xor)
