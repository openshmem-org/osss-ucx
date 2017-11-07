/* For license: see LICENSE file at top-level */

#include "shmemu.h"
#include "state.h"

#include "shmem/defs.h"

#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <ucp/api/ucp.h>

/*
 * shortcut to look up the UCP endpoint
 */
inline static ucp_ep_h
lookup_ucp_ep(int pe)
{
    return proc.comms.eps[pe];
}

inline static int
in_region(uint64_t addr, size_t region, int pe)
{
    const mem_info_t mi = proc.comms.regions[region].minfo[pe];

    if ( (mi.base <= addr) && (addr < mi.end) ) {
        return 1;
    }

    return 0;
}

/*
 * find memory region that ADDR is in, or -1 if none
 */
inline static long
lookup_region(uint64_t addr, int pe)
{
    size_t i;

    for (i = 0; i < proc.comms.nregions; i += 1) {
        if (in_region(addr, i, pe)) {
            return (long) i;
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
get_base(int region, int pe)
{
    return proc.comms.regions[region].minfo[pe].base;
}

/*
 * translate remote address
 */
inline static uint64_t
translate_address(uint64_t local_addr, size_t region, int pe)
{
#ifdef ENABLE_FIXED_ADDRESSES
    return local_addr;
#else
    const uint64_t my_offset = local_addr - get_base(region, proc.rank);
    const uint64_t remote_addr = my_offset + get_base(region, pe);

    return remote_addr;
#endif /* ENABLE_FIXED_ADDRESSES */
}

/*
 * 1 if PE is a valid rank, 0 otherwise
 */
inline static int
valid_pe_number(int pe)
{
    return ( (proc.nranks > pe) && (pe >= 0) ) ? 1 : 0;
}

/*
 * see if addr is reachable using given context
 */
inline static void *
shmemc_ptr_helper(shmem_ctx_t ctx,
           const void *addr, int pe)
{
    long r;
    uint64_t ua = (uint64_t) addr;
    uint64_t r_addr;            /* address on other PE */
    ucp_rkey_h rkey;            /* rkey for remote address */
    void *usable_addr = NULL;
    ucs_status_t s;

    r = lookup_region(ua, proc.rank);
    r_addr = translate_address(ua, r, pe);
    rkey = lookup_rkey(r, pe);

    s = ucp_rkey_ptr(rkey, r_addr, &usable_addr);
    if (s == UCS_OK) {
        return usable_addr;
    }

    return NULL;
}

/**
 * API
 *
 **/

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
shmemc_ptr(const void *addr, int pe)
{
    return shmemc_ptr_helper(SHMEM_CTX_DEFAULT, addr, pe);
}

/*
 * 1 if adddress is remotely accessible, 0 otherwise
 */
int
shmemc_addr_accessible(const void *addr, int pe)
{
    uint64_t ua = (uint64_t) addr;
    const long r = lookup_region(ua, proc.rank);

    return (r >= 0) ? 1 : 0;
}

/*
 * 1 if a valid PE #, 0 otherwise
 */
int
shmemc_pe_accessible(int pe)
{
    return valid_pe_number(pe);
}

/*
 * -- puts & gets --------------------------------------------------------
 */

void
shmemc_ctx_put(shmem_ctx_t ctx,
               void *dest, const void *src,
               size_t nbytes, int pe)
{
    long r;
    uint64_t ud = (uint64_t) dest;
    uint64_t r_dest;            /* address on other PE */
    ucp_rkey_h rkey;            /* rkey for remote address */
    ucp_ep_h ep;
    ucs_status_t s;

    r = lookup_region(ud, proc.rank);
    r_dest = translate_address(ud, r, pe);
    rkey = lookup_rkey(r, pe);
    ep = lookup_ucp_ep(pe);

    s = ucp_put(ep, src, nbytes, r_dest, rkey);
    assert(s == UCS_OK);
}

void
shmemc_ctx_get(shmem_ctx_t ctx,
               void *dest, const void *src,
               size_t nbytes, int pe)
{
    long r;
    uint64_t us = (uint64_t) src;
    uint64_t r_src;
    ucp_rkey_h rkey;
    ucp_ep_h ep;
    ucs_status_t s;

    r = lookup_region(us, proc.rank);
    r_src = translate_address(us, r, pe);
    rkey = lookup_rkey(r, pe);
    ep = lookup_ucp_ep(pe);

    s = ucp_get(ep, dest, nbytes, r_src, rkey);
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
    long r;
    uint64_t ud = (uint64_t) dest;
    uint64_t r_dest;
    ucp_rkey_h rkey;
    ucp_ep_h ep;
    ucs_status_t s;

    r = lookup_region(ud, proc.rank);
    r_dest = translate_address(ud, r, pe);
    rkey = lookup_rkey(r, pe);
    ep = lookup_ucp_ep(pe);

    s = ucp_put_nbi(ep, src, nbytes, r_dest, rkey);
    assert(s == UCS_OK || s == UCS_INPROGRESS);
}

void
shmemc_ctx_get_nbi(shmem_ctx_t ctx,
                   void *dest, const void *src,
                   size_t nbytes, int pe)
{
    long r;
    uint64_t us = (uint64_t) src;
    uint64_t r_src;
    ucp_rkey_h rkey;
    ucp_ep_h ep;
    ucs_status_t s;

    r = lookup_region(us, proc.rank);
    r_src = translate_address(us, r, pe);
    rkey = lookup_rkey(r, pe);
    ep = lookup_ucp_ep(pe);

    s = ucp_get_nbi(ep, dest, nbytes, r_src, rkey);
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
    helper_atomic_fetch_add##_size(uint64_t t, uint##_size##_t v, int pe) \
    {                                                                   \
        long r;                                                         \
        uint64_t r_t;                                                   \
        ucp_rkey_h rkey;                                                \
        ucp_ep_h ep;                                                    \
        uint##_size##_t ret;                                            \
        ucs_status_t s;                                                 \
                                                                        \
        r = lookup_region(t, proc.rank);                                \
        r_t = translate_address(t, r, pe);                              \
        rkey = lookup_rkey(r, pe);                                      \
        ep = lookup_ucp_ep(pe);                                         \
                                                                        \
        s = ucp_atomic_fadd##_size(ep, v, r_t, rkey, &ret);             \
        assert(s == UCS_OK);                                            \
                                                                        \
        return ret;                                                     \
    }

HELPER_FADD(32)
HELPER_FADD(64)

#define HELPER_ADD(_size)                                           \
    inline static void                                              \
    helper_atomic_add##_size(uint64_t t, uint##_size##_t v, int pe) \
    {                                                               \
        long r;                                                     \
        uint64_t r_t;                                               \
        ucp_rkey_h rkey;                                            \
        ucp_ep_h ep;                                                \
        ucs_status_t s;                                             \
                                                                    \
        r = lookup_region(t, proc.rank);                            \
        r_t = translate_address(t, r, pe);                          \
        rkey = lookup_rkey(r, pe);                                  \
        ep = lookup_ucp_ep(pe);                                     \
                                                                    \
        s = ucp_atomic_add##_size(ep, v, r_t, rkey);                \
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
                              uint64_t t, uint##_size##_t v, int pe)    \
    {                                                                   \
        long r;                                                         \
        uint64_t r_t;                                                   \
        ucp_rkey_h rkey;                                                \
        ucp_ep_h ep;                                                    \
        uint##_size##_t ret;                                            \
        ucs_status_t s;                                                 \
                                                                        \
        r = lookup_region(t, proc.rank);                                \
        r_t = translate_address(t, r, pe);                              \
        rkey = lookup_rkey(r, pe);                                      \
        ep = lookup_ucp_ep(pe);                                         \
                                                                        \
        s = ucp_atomic_swap##_size(ep, v, r_t, rkey, &ret);             \
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
        long r;                                                         \
        uint64_t r_t;                                                   \
        ucp_rkey_h rkey;                                                \
        ucp_ep_h ep;                                                    \
        uint##_size##_t ret;                                            \
        ucs_status_t s;                                                 \
                                                                        \
        r = lookup_region(t, proc.rank);                                \
        r_t = translate_address(t, r, pe);                              \
        rkey = lookup_rkey(r, pe);                                      \
        ep = lookup_ucp_ep(pe);                                         \
                                                                        \
        s = ucp_atomic_cswap##_size(ep, c, v, r_t, rkey, &ret);         \
        assert(s == UCS_OK);                                            \
                                                                        \
        return ret;                                                     \
    }

HELPER_CSWAP(32)
HELPER_CSWAP(64)

/*
 * bitwise helpers
 */

/* NB UCX currently doesn't have API support for these ops */

#define HELPER_FETCH_BITWISE_OP(_op, _opname, _size)                    \
    inline static uint##_size##_t                                       \
    helper_atomic_fetch_##_opname##_size(uint64_t t,                    \
                                         uint##_size##_t v,             \
                                         int pe)                        \
    {                                                                   \
        long r;                                                         \
        uint64_t r_t;                                                   \
        uint##_size##_t rval, rval_orig, ret;                           \
        ucp_rkey_h rkey;                                                \
        ucp_ep_h ep;                                                    \
        ucs_status_t s;                                                 \
                                                                        \
        r = lookup_region(t, proc.rank);                                \
        r_t = translate_address(t, r, pe);                              \
        rkey = lookup_rkey(r, pe);                                      \
        ep = lookup_ucp_ep(pe);                                         \
                                                                        \
        do {                                                            \
            s = ucp_get(ep, &rval_orig, sizeof(rval_orig), r_t, rkey);  \
            assert(s == UCS_OK);                                        \
                                                                        \
            rval = (rval_orig) _op (v);                                 \
                                                                        \
            s = ucp_atomic_cswap##_size(ep, rval_orig, rval,            \
                                        r_t, rkey, &ret);               \
            assert(s == UCS_OK);                                        \
        } while (ret != rval_orig);                                     \
                                                                        \
        return ret;                                                     \
    }

/*
 * and
 */
HELPER_FETCH_BITWISE_OP(&, and, 32)
HELPER_FETCH_BITWISE_OP(&, and, 64)

/*
 * or
 */
HELPER_FETCH_BITWISE_OP(|, or, 32)
HELPER_FETCH_BITWISE_OP(|, or, 64)

/*
 * xor
 */
HELPER_FETCH_BITWISE_OP(^, xor, 32)
HELPER_FETCH_BITWISE_OP(^, xor, 64)

/**
 * AMO API
 **/

/*
 * add
 */

#define SHMEMC_ADD(_size)                                       \
    void                                                        \
    shmemc_add##_size(void *t, uint64_t v, int pe)              \
    {                                                           \
        (void) helper_atomic_add##_size((uint64_t) t, v, pe);   \
    }

SHMEMC_ADD(32)
SHMEMC_ADD(64)

/*
 * inc is just "add 1"
 */

#define SHMEMC_INC(_size)                                       \
    void                                                        \
    shmemc_inc##_size(void *t, int pe)                          \
    {                                                           \
        (void) helper_atomic_add##_size((uint64_t) t, 1, pe);   \
    }

SHMEMC_INC(32)
SHMEMC_INC(64)

/*
 * fetch-and-add
 */

#define SHMEMC_FADD(_size)                                          \
    uint64_t                                                        \
    shmemc_fadd##_size(void *t, uint64_t v, int pe)                 \
    {                                                               \
        return helper_atomic_fetch_add##_size((uint64_t) t, v, pe); \
    }

SHMEMC_FADD(32)
SHMEMC_FADD(64)

/*
 * finc is just "fadd 1"
 */

#define SHMEMC_FINC(_size)                                          \
    uint64_t                                                        \
    shmemc_finc##_size(void *t, int pe)                             \
    {                                                               \
        return helper_atomic_fetch_add##_size((uint64_t) t, 1, pe); \
    }

SHMEMC_FINC(32)
SHMEMC_FINC(64)

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

#define SHMEMC_FETCH(_size)                                         \
    uint64_t                                                        \
    shmemc_fetch##_size(void *t, int pe)                            \
    {                                                               \
        return helper_atomic_fetch_add##_size((uint64_t) t, 0, pe); \
    }

SHMEMC_FETCH(32)
SHMEMC_FETCH(64)

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

#define SHMEMC_FETCH_BITWISE(_op, _size)                                \
    uint64_t                                                            \
    shmemc_fetch_##_op##_size(void *t, uint64_t v, int pe)              \
    {                                                                   \
        return helper_atomic_fetch_##_op##_size((uint64_t) t, v, pe);   \
    }

SHMEMC_FETCH_BITWISE(and, 32)
SHMEMC_FETCH_BITWISE(and, 64)

SHMEMC_FETCH_BITWISE(or, 32)
SHMEMC_FETCH_BITWISE(or, 64)

SHMEMC_FETCH_BITWISE(xor, 32)
SHMEMC_FETCH_BITWISE(xor, 64)

/*
 * bitwise
 */

#define SHMEMC_BITWISE(_op, _size)                                      \
    void                                                                \
    shmemc_##_op##_size(void *t, uint64_t v, int pe)                    \
    {                                                                   \
        (void) helper_atomic_fetch_##_op##_size((uint64_t) t, v, pe);   \
    }

SHMEMC_BITWISE(and, 32)
SHMEMC_BITWISE(and, 64)

SHMEMC_BITWISE(or, 32)
SHMEMC_BITWISE(or, 64)

SHMEMC_BITWISE(xor, 32)
SHMEMC_BITWISE(xor, 64)
