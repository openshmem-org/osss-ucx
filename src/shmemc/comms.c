/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "state.h"

#include "shmem/defs.h"

#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <ucp/api/ucp.h>

/*
 * shortcut to look up the UCP endpoint of a context
 */
inline static ucp_ep_h
lookup_ucp_ep(shmem_ctx_t ctx, int pe)
{
#if 0
    const shmem_context_h ch = (shmem_context_h) ctx;

    return proc.comms.ctxts[ch->id].eps[pe];
#endif
    return proc.comms.eps[pe];
}

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
    size_t r;

    for (r = 0; r < proc.comms.nregions; r += 1) {
        if (in_region(addr, r, pe)) {
            return (long) r;
            /* NOT REACHED */
        }
    }

    return -1;
}

/*
 * find remote rkey
 */
inline static ucp_rkey_h
lookup_rkey(size_t region, int pe)
{
    return proc.comms.regions[region].minfo[pe].racc.rkey;
}

inline static uint64_t
get_base(size_t region, int pe)
{
    return proc.comms.regions[region].minfo[pe].base;
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
 * Return non-zero if PE is a valid rank, 0 otherwise
 */
inline static int
is_valid_pe_number(int pe)
{
    return (proc.nranks > pe) && (pe >= 0);
}

/*
 * All ops here need to find remote keys and addresses
 */
inline static void
get_remote_key_and_addr(uint64_t laddr, int pe,
                        ucp_rkey_h *rkey, uint64_t *raddr)
{
    const long r = lookup_region(laddr, proc.rank);

    shmemu_assert("remote key/address lookup", r >= 0);

    *rkey = lookup_rkey(r, pe);
    *raddr = translate_address(laddr, r, pe);
}

/*
 * See if addr is reachable using given context.  Return usable
 * address if so, otherwise NULL.
 */
inline static void *
shmemc_ptr_helper(shmem_ctx_t ctx,
                  const void *addr, int pe)
{
    /* check to see if UCX is new enough */
#ifdef HAVE_UCP_RKEY_PTR
    uint64_t r_addr;            /* address on other PE */
    ucp_rkey_h rkey;            /* rkey for remote address */
    void *usable_addr = NULL;
    ucs_status_t s;

    get_remote_key_and_addr((uint64_t) addr, pe, &rkey, &r_addr);

    s = ucp_rkey_ptr(rkey, r_addr, &usable_addr);
    if (s == UCS_OK) {
        return usable_addr;
    }
#endif  /* HAVE_UCP_RKEY_PTR */

    return NULL;
}

/**
 * API
 *
 **/

/*
 * -- ordering -----------------------------------------------------------
 */

void
shmemc_ctx_fence(shmem_ctx_t ctx)
{
    const ucs_status_t s = ucp_worker_fence(proc.comms.wrkr);
    assert(s == UCS_OK);
}

void
shmemc_ctx_quiet(shmem_ctx_t ctx)
{
    const ucs_status_t s = ucp_worker_flush(proc.comms.wrkr);
    assert(s == UCS_OK);
}

/*
 * -- accessible memory pointers -----------------------------------------
 */

void *
shmemc_ctx_ptr(shmem_ctx_t ctx, const void *addr, int pe)
{
    return shmemc_ptr_helper(ctx, addr, pe);
}

/*
 * Return non-zero if adddress is remotely accessible, 0 otherwise
 */
int
shmemc_addr_accessible(const void *addr, int pe)
{
    uint64_t ua = (uint64_t) addr;
    const long r = lookup_region(ua, proc.rank);

    return (r >= 0);
}

/*
 * Return non-zero if a valid PE #, 0 otherwise
 */
int
shmemc_pe_accessible(int pe)
{
    return is_valid_pe_number(pe);
}

/*
 * -- puts & gets --------------------------------------------------------
 */

void
shmemc_ctx_put(shmem_ctx_t ctx,
               void *dest, const void *src,
               size_t nbytes, int pe)
{
    uint64_t r_dest;            /* address on other PE */
    ucp_rkey_h rkey;            /* rkey for remote address */
    ucs_status_t s;

    get_remote_key_and_addr((uint64_t) dest, pe, &rkey, &r_dest);

    s = ucp_put(lookup_ucp_ep(ctx, pe), src, nbytes, r_dest, rkey);
    assert(s == UCS_OK);
}

void
shmemc_ctx_get(shmem_ctx_t ctx,
               void *dest, const void *src,
               size_t nbytes, int pe)
{
    uint64_t r_src;
    ucp_rkey_h rkey;
    ucs_status_t s;

    get_remote_key_and_addr((uint64_t) src, pe, &rkey, &r_src);

    s = ucp_get(lookup_ucp_ep(ctx, pe), dest, nbytes, r_src, rkey);
    assert(s == UCS_OK);
}

/**
 * Return values from put_nbi/get_nbi probably need more handling
 *
 */

void
shmemc_ctx_put_nbi(shmem_ctx_t ctx,
                   void *dest, const void *src,
                   size_t nbytes, int pe)
{
    uint64_t r_dest;
    ucp_rkey_h rkey;
    ucs_status_t s;

    get_remote_key_and_addr((uint64_t) dest, pe, &rkey, &r_dest);

    s = ucp_put_nbi(lookup_ucp_ep(ctx, pe), src, nbytes, r_dest, rkey);
    assert(s == UCS_OK || s == UCS_INPROGRESS);
}

void
shmemc_ctx_get_nbi(shmem_ctx_t ctx,
                   void *dest, const void *src,
                   size_t nbytes, int pe)
{
    uint64_t r_src;
    ucp_rkey_h rkey;
    ucs_status_t s;

    get_remote_key_and_addr((uint64_t) src, pe, &rkey, &r_src);

    s = ucp_get_nbi(lookup_ucp_ep(ctx, pe), dest, nbytes, r_src, rkey);
    assert(s == UCS_OK || s == UCS_INPROGRESS);
}

/*
 * -- atomics ------------------------------------------------------------
 */

/* TODO: repeated patterns here, maybe some kind of template? */

/*
 * helpers
 */

#define HELPER_FADD(_size)                                              \
    inline static uint##_size##_t                                       \
    helper_atomic_fetch_add##_size(shmem_ctx_t ctx,                     \
                                   uint64_t t, uint##_size##_t v,       \
                                   int pe)                              \
    {                                                                   \
        uint64_t r_t;                                                   \
        ucp_rkey_h rkey;                                                \
        uint##_size##_t ret;                                            \
        ucs_status_t s;                                                 \
                                                                        \
        get_remote_key_and_addr(t, pe, &rkey, &r_t);                    \
                                                                        \
        s = ucp_atomic_fadd##_size(lookup_ucp_ep(ctx, pe),              \
                                   v, r_t, rkey, &ret);                 \
        assert(s == UCS_OK);                                            \
                                                                        \
        return ret;                                                     \
    }

HELPER_FADD(32)
HELPER_FADD(64)

#define HELPER_ADD(_size)                                           \
    inline static void                                              \
    helper_atomic_add##_size(shmem_ctx_t ctx,                       \
                             uint64_t t, uint##_size##_t v,         \
                             int pe)                                \
    {                                                               \
        uint64_t r_t;                                               \
        ucp_rkey_h rkey;                                            \
        ucs_status_t s;                                             \
                                                                    \
        get_remote_key_and_addr(t, pe, &rkey, &r_t);                \
                                                                    \
        s = ucp_atomic_add##_size(lookup_ucp_ep(ctx, pe),           \
                                  v, r_t, rkey);                    \
        assert(s == UCS_OK);                                        \
    }

HELPER_ADD(32)
HELPER_ADD(64)

/*
 * swaps
 */

#define HELPER_SWAP(_size)                                              \
    inline static uint##_size##_t                                       \
    helper_atomic_swap##_size(shmem_ctx_t ctx,                          \
                              uint64_t t, uint##_size##_t v,            \
                              int pe)                                   \
    {                                                                   \
        uint64_t r_t;                                                   \
        ucp_rkey_h rkey;                                                \
        uint##_size##_t ret;                                            \
        ucs_status_t s;                                                 \
                                                                        \
        get_remote_key_and_addr(t, pe, &rkey, &r_t);                    \
                                                                        \
        s = ucp_atomic_swap##_size(lookup_ucp_ep(ctx, pe),              \
                                   v, r_t, rkey, &ret);                 \
        assert(s == UCS_OK);                                            \
                                                                        \
        return ret;                                                     \
    }

HELPER_SWAP(32)
HELPER_SWAP(64)

#define HELPER_CSWAP(_size)                                             \
    inline static uint##_size##_t                                       \
    helper_atomic_cswap##_size(shmem_ctx_t ctx,                         \
                               uint64_t t,                              \
                               uint##_size##_t c, uint##_size##_t v,    \
                               int pe)                                  \
    {                                                                   \
        uint64_t r_t;                                                   \
        ucp_rkey_h rkey;                                                \
        uint##_size##_t ret;                                            \
        ucs_status_t s;                                                 \
                                                                        \
        get_remote_key_and_addr(t, pe, &rkey, &r_t);                    \
                                                                        \
        s = ucp_atomic_cswap##_size(lookup_ucp_ep(ctx, pe),             \
                                    c, v, r_t, rkey, &ret);             \
        assert(s == UCS_OK);                                            \
                                                                        \
        return ret;                                                     \
    }

HELPER_CSWAP(32)
HELPER_CSWAP(64)

/*
 * bitwise helpers
 */

#ifndef HAVE_UCP_BITWISE_ATOMICS

/* NB UCX currently doesn't have API support for these ops */

#define NOTUCP_ATOMIC_BITWISE_OP(_op, _opname, _size)                   \
    inline static ucs_status_t                                          \
    ucp_atomic_##_opname##_size(ucp_ep_h ep,                            \
                                uint##_size##_t val,                    \
                                uint64_t remote_addr,                   \
                                ucp_rkey_h rkey,                        \
                                uint##_size##_t *result)                \
    {                                                                   \
        uint##_size##_t rval, rval_orig, ret;                           \
        ucs_status_t s;                                                 \
                                                                        \
        do {                                                            \
            s = ucp_get(ep, &rval_orig, sizeof(rval_orig),              \
                        remote_addr, rkey);                             \
            assert(s == UCS_OK);                                        \
                                                                        \
            rval = (rval_orig) _op (val);                               \
                                                                        \
            s = ucp_atomic_cswap##_size(ep, rval_orig, rval,            \
                                        remote_addr, rkey, &ret);       \
            assert(s == UCS_OK);                                        \
        } while (ret != rval_orig);                                     \
                                                                        \
        *result = ret;                                                  \
        return UCS_OK;                                                  \
    }

NOTUCP_ATOMIC_BITWISE_OP(&, and, 32)
NOTUCP_ATOMIC_BITWISE_OP(&, and, 64)
NOTUCP_ATOMIC_BITWISE_OP(|, or, 32)
NOTUCP_ATOMIC_BITWISE_OP(|, or, 64)
NOTUCP_ATOMIC_BITWISE_OP(^, xor, 32)
NOTUCP_ATOMIC_BITWISE_OP(^, xor, 64)

#endif  /* ! HAVE_UCP_BITWISE_ATOMICS */

#define HELPER_FETCH_BITWISE_OP(_op, _opname, _size)                    \
    inline static uint##_size##_t                                       \
    helper_atomic_fetch_##_opname##_size(shmem_ctx_t ctx,               \
                                         uint64_t t,                    \
                                         uint##_size##_t v,             \
                                         int pe)                        \
    {                                                                   \
        uint64_t r_t;                                                   \
        uint##_size##_t ret;                                            \
        ucp_rkey_h rkey;                                                \
        ucs_status_t s;                                                 \
                                                                        \
        get_remote_key_and_addr(t, pe, &rkey, &r_t);                    \
                                                                        \
        s = ucp_atomic_##_opname##_size(lookup_ucp_ep(ctx, pe),         \
                                        v, r_t, rkey, &ret);            \
        assert(s == UCS_OK);                                            \
                                                                        \
        return ret;                                                     \
    }

HELPER_FETCH_BITWISE_OP(&, and, 32)
HELPER_FETCH_BITWISE_OP(&, and, 64)
HELPER_FETCH_BITWISE_OP(|, or, 32)
HELPER_FETCH_BITWISE_OP(|, or, 64)
HELPER_FETCH_BITWISE_OP(^, xor, 32)
HELPER_FETCH_BITWISE_OP(^, xor, 64)

/**
 * AMO API
 **/

/*
 * add
 */

#define SHMEMC_CTX_ADD(_size)                                       \
    void                                                            \
    shmemc_ctx_add##_size(shmem_ctx_t ctx,                          \
                          void *t, uint64_t v, int pe)              \
    {                                                               \
        helper_atomic_add##_size(ctx, (uint64_t) t, v, pe);         \
    }

SHMEMC_CTX_ADD(32)
SHMEMC_CTX_ADD(64)

/*
 * inc is just "add 1"
 */

#define SHMEMC_CTX_INC(_size)                                       \
    void                                                            \
    shmemc_ctx_inc##_size(shmem_ctx_t ctx,                          \
                          void *t, int pe)                          \
    {                                                               \
        helper_atomic_add##_size(ctx, (uint64_t) t, 1, pe);         \
    }

SHMEMC_CTX_INC(32)
SHMEMC_CTX_INC(64)

/*
 * fetch-and-add
 */

#define SHMEMC_CTX_FADD(_size)                                          \
    uint64_t                                                            \
    shmemc_ctx_fadd##_size(shmem_ctx_t ctx,                             \
                           void *t, uint64_t v, int pe)                 \
    {                                                                   \
        return helper_atomic_fetch_add##_size(ctx, (uint64_t) t, v, pe); \
    }

SHMEMC_CTX_FADD(32)
SHMEMC_CTX_FADD(64)

/*
 * finc is just "fadd 1"
 */

#define SHMEMC_CTX_FINC(_size)                                          \
    uint64_t                                                            \
    shmemc_ctx_finc##_size(shmem_ctx_t ctx,                             \
                           void *t, int pe)                             \
    {                                                                   \
        return helper_atomic_fetch_add##_size(ctx, (uint64_t) t, 1, pe); \
    }

SHMEMC_CTX_FINC(32)
SHMEMC_CTX_FINC(64)

/*
 * swaps
 */

#define SHMEMC_CTX_SWAP(_size)                                      \
    uint64_t                                                        \
    shmemc_ctx_swap##_size(shmem_ctx_t ctx,                         \
                           void *t, uint64_t v, int pe)             \
    {                                                               \
        return helper_atomic_swap##_size(ctx, (uint64_t) t, v, pe); \
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
 * TODO: UCX really does have a fetch_nb, but for now do something
 * simple
 *
 */

#define SHMEMC_CTX_FETCH(_size)                                         \
    uint64_t                                                            \
    shmemc_ctx_fetch##_size(shmem_ctx_t ctx,                            \
                            void *t, int pe)                            \
    {                                                                   \
        return helper_atomic_fetch_add##_size(ctx, (uint64_t) t, 0, pe); \
    }

SHMEMC_CTX_FETCH(32)
SHMEMC_CTX_FETCH(64)

/*
 * TODO: use swap and ignore return?
 */
#define SHMEMC_CTX_SET(_size)                                           \
    void                                                                \
    shmemc_ctx_set##_size(shmem_ctx_t ctx,                              \
                          void *t, uint64_t v, int pe)                  \
    {                                                                   \
        (void) helper_atomic_swap##_size(ctx, (uint64_t) t, v, pe);     \
    }

SHMEMC_CTX_SET(32)
SHMEMC_CTX_SET(64)

/*
 * fetched-bitwise
 */

#define SHMEMC_CTX_FETCH_BITWISE(_op, _size)                            \
    uint64_t                                                            \
    shmemc_ctx_fetch_##_op##_size(shmem_ctx_t ctx,                      \
                                  void *t, uint64_t v, int pe)          \
    {                                                                   \
        return helper_atomic_fetch_##_op##_size(ctx, (uint64_t) t, v, pe); \
    }

SHMEMC_CTX_FETCH_BITWISE(and, 32)
SHMEMC_CTX_FETCH_BITWISE(and, 64)

SHMEMC_CTX_FETCH_BITWISE(or, 32)
SHMEMC_CTX_FETCH_BITWISE(or, 64)

SHMEMC_CTX_FETCH_BITWISE(xor, 32)
SHMEMC_CTX_FETCH_BITWISE(xor, 64)

/*
 * bitwise
 */

#define SHMEMC_CTX_BITWISE(_op, _size)                                  \
    void                                                                \
    shmemc_ctx_##_op##_size(shmem_ctx_t ctx,                            \
                            void *t, uint64_t v, int pe)                \
    {                                                                   \
        (void) helper_atomic_fetch_##_op##_size(ctx, (uint64_t) t, v, pe); \
    }

SHMEMC_CTX_BITWISE(and, 32)
SHMEMC_CTX_BITWISE(and, 64)

SHMEMC_CTX_BITWISE(or, 32)
SHMEMC_CTX_BITWISE(or, 64)

SHMEMC_CTX_BITWISE(xor, 32)
SHMEMC_CTX_BITWISE(xor, 64)
