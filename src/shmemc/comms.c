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

#if 0
    logger(LOG_MEMORY,
           "region %d for PE %d: addr = %lu, base = %lu, end = %lu",
           region, pe,
           addr, mi.base, mi.end);
#endif

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
    const uint64_t my_offset = local_addr - get_base(region, proc.rank);
    const uint64_t remote_addr = my_offset + get_base(region, pe);

    return remote_addr;
}

/**
 * API
 *
 **/

/*
 * -- puts & gets --------------------------------------------------------
 */

void
shmemc_put(void *dest, const void *src,
           size_t nbytes, int pe)
{
    long r;
    uint64_t ud = (uint64_t) dest;
    uint64_t r_dest;            /* address on other PE */
    ucp_rkey_h rkey;            /* rkey for remote address */
    ucp_ep_h ep;
    ucs_status_t s;

    r = lookup_region(ud, proc.rank);
    assert(r >= 0);

    r_dest = translate_address(ud, r, pe);
    assert(r_dest != 0);

    rkey = lookup_rkey(r, pe);
    assert(rkey != NULL);

    ep = lookup_ucp_ep(pe);

    s = ucp_put(ep, src, nbytes, r_dest, rkey);
    assert(s == UCS_OK);
}

void
shmemc_get(void *dest, const void *src,
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
    assert(r_src != 0);

    rkey = lookup_rkey(r, pe);
    assert(rkey != NULL);

    ep = lookup_ucp_ep(pe);

    s = ucp_get(ep, dest, nbytes, r_src, rkey);
    assert(s == UCS_OK);
}

/**
 * Return values from put_nbi/get_nbi probably need more handling
 *
 */

void
shmemc_put_nbi(void *dest, const void *src,
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
    assert(r_dest != 0);

    rkey = lookup_rkey(r, pe);
    assert(rkey != NULL);

    ep = lookup_ucp_ep(pe);

    s = ucp_put_nbi(ep, src, nbytes, r_dest, rkey);
    assert(s == UCS_OK || s == UCS_INPROGRESS);
}

void
shmemc_get_nbi(void *dest, const void *src,
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
    assert(r_src != 0);

    rkey = lookup_rkey(r, pe);
    assert(rkey != NULL);

    ep = lookup_ucp_ep(pe);

    s = ucp_get_nbi(ep, dest, nbytes, r_src, rkey);
    assert(s == UCS_OK || s == UCS_INPROGRESS);
}

/*
 * -- atomics ------------------------------------------------------------
 */

/*
 * helpers
 */

inline static uint32_t
helper_fadd32(uint64_t t, uint32_t v, int pe)
{
    long r;
    uint64_t r_t;
    ucp_rkey_h rkey;
    ucp_ep_h ep;
    uint32_t ret;
    ucs_status_t s;

    r = lookup_region(t, proc.rank);
    r_t = translate_address(t, r, pe);
    rkey = lookup_rkey(r, pe);
    ep = lookup_ucp_ep(pe);

    s = ucp_atomic_fadd32(ep, v, r_t, rkey, &ret);
    assert(s == UCS_OK);

    return ret;
}

inline static uint64_t
helper_fadd64(uint64_t t, uint64_t v, int pe)
{
    long r;
    uint64_t r_t;
    ucp_rkey_h rkey;
    ucp_ep_h ep;
    uint64_t ret;
    ucs_status_t s;

    r = lookup_region(t, proc.rank);
    r_t = translate_address(t, r, pe);
    rkey = lookup_rkey(r, pe);
    ep = lookup_ucp_ep(pe);

    s = ucp_atomic_fadd64(ep, v, r_t, rkey, &ret);
    assert(s == UCS_OK);

    return ret;
}

inline static void
helper_add32(uint64_t t, uint32_t v, int pe)
{
    long r;
    uint64_t r_t;
    ucp_rkey_h rkey;
    ucp_ep_h ep;
    ucs_status_t s;

    r = lookup_region(t, proc.rank);
    r_t = translate_address(t, r, pe);
    rkey = lookup_rkey(r, pe);
    ep = lookup_ucp_ep(pe);

    s = ucp_atomic_add32(ep, v, r_t, rkey);
    assert(s == UCS_OK);
}

inline static void
helper_add64(uint64_t t, uint64_t v, int pe)
{
    long r;
    uint64_t r_t;
    ucp_rkey_h rkey;
    ucp_ep_h ep;
    ucs_status_t s;

    r = lookup_region(t, proc.rank);
    r_t = translate_address(t, r, pe);
    rkey = lookup_rkey(r, pe);
    ep = lookup_ucp_ep(pe);

    s = ucp_atomic_add64(ep, v, r_t, rkey);
    assert(s == UCS_OK);
}

inline static uint32_t
helper_swap32(uint64_t t, uint32_t v, int pe)
{
    long r;
    uint64_t r_t;
    ucp_rkey_h rkey;
    ucp_ep_h ep;
    uint32_t ret;
    ucs_status_t s;

    r = lookup_region(t, proc.rank);
    r_t = translate_address(t, r, pe);
    rkey = lookup_rkey(r, pe);
    ep = lookup_ucp_ep(pe);

    s = ucp_atomic_swap32(ep, v, r_t, rkey, &ret);
    assert(s == UCS_OK);

    return ret;
}

inline static uint64_t
helper_swap64(uint64_t t, uint64_t v, int pe)
{
    long r;
    uint64_t r_t;
    ucp_rkey_h rkey;
    ucp_ep_h ep;
    uint64_t ret;
    ucs_status_t s;

    r = lookup_region(t, proc.rank);
    r_t = translate_address(t, r, pe);
    rkey = lookup_rkey(r, pe);
    ep = lookup_ucp_ep(pe);

    s = ucp_atomic_swap64(ep, v, r_t, rkey, &ret);
    assert(s == UCS_OK);

    return ret;
}

inline static uint32_t
helper_cswap32(uint64_t t, uint32_t c, uint32_t v, int pe)
{
    long r;
    uint64_t r_t;
    ucp_rkey_h rkey;
    ucp_ep_h ep;
    uint32_t ret;
    ucs_status_t s;

    r = lookup_region(t, proc.rank);
    r_t = translate_address(t, r, pe);
    rkey = lookup_rkey(r, pe);
    ep = lookup_ucp_ep(pe);

    s = ucp_atomic_cswap32(ep, c, v, r_t, rkey, &ret);
    assert(s == UCS_OK);

    return ret;
}

inline static uint64_t
helper_cswap64(uint64_t t, uint64_t c, uint64_t v, int pe)
{
    long r;
    uint64_t r_t;
    ucp_rkey_h rkey;
    ucp_ep_h ep;
    uint64_t ret;
    ucs_status_t s;

    r = lookup_region(t, proc.rank);
    r_t = translate_address(t, r, pe);
    rkey = lookup_rkey(r, pe);
    ep = lookup_ucp_ep(pe);

    s = ucp_atomic_cswap64(ep, c, v, r_t, rkey, &ret);
    assert(s == UCS_OK);

    return ret;
}

/**
 * API
 **/

/*
 * add
 */

#define SHMEMC_TYPED_ADD(_name, _type, _size)                   \
    void                                                        \
    shmemc_##_name##_add(_type *t, _type v, int pe)             \
    {                                                           \
        helper_add##_size((uint64_t) t, v, pe);                 \
    }

SHMEMC_TYPED_ADD(int, int, 32)
SHMEMC_TYPED_ADD(long, long, 64)
SHMEMC_TYPED_ADD(longlong, long long, 64)

/*
 * inc is just "add 1"
 */

#define SHMEMC_TYPED_INC(_name, _type, _size)                   \
    void                                                        \
    shmemc_##_name##_inc(_type *t, int pe)                      \
    {                                                           \
        helper_add##_size((uint64_t) t, 1, pe);                 \
    }

SHMEMC_TYPED_INC(int, int, 32)
SHMEMC_TYPED_INC(long, long, 64)
SHMEMC_TYPED_INC(longlong, long long, 64)

/*
 * fetch-and-add
 */

#define SHMEMC_TYPED_FADD(_name, _type, _size)                  \
    _type                                                       \
    shmemc_##_name##_fadd(_type *t, _type v, int pe)            \
    {                                                           \
        return (_type) helper_fadd##_size((uint64_t) t, v, pe); \
    }

SHMEMC_TYPED_FADD(int, int, 32)
SHMEMC_TYPED_FADD(long, long, 64)
SHMEMC_TYPED_FADD(longlong, long long, 64)

/*
 * finc is just "fadd 1"
 */

#define SHMEMC_TYPED_FINC(_name, _type, _size)                  \
    _type                                                       \
    shmemc_##_name##_finc(_type *t, int pe)                     \
    {                                                           \
        return (_type) helper_fadd##_size((uint64_t) t, 1, pe); \
    }

SHMEMC_TYPED_FINC(int, int, 32)
SHMEMC_TYPED_FINC(long, long, 64)
SHMEMC_TYPED_FINC(longlong, long long, 64)

/*
 * swaps
 */

#define SHMEMC_TYPED_SWAP(_name, _type, _size)                  \
    _type                                                       \
    shmemc_##_name##_swap(_type *t, _type v, int pe)            \
    {                                                           \
        return (_type) helper_swap##_size((uint64_t) t, v, pe); \
    }                                                           \

SHMEMC_TYPED_SWAP(int, int, 32)
SHMEMC_TYPED_SWAP(long, long, 64)
SHMEMC_TYPED_SWAP(longlong, long long, 64)
SHMEMC_TYPED_SWAP(float, float, 32)
SHMEMC_TYPED_SWAP(double, double, 64)

#define SHMEMC_TYPED_CSWAP(_name, _type, _size)                     \
    _type                                                           \
    shmemc_##_name##_cswap(_type *t, _type c, _type v, int pe)      \
    {                                                               \
        return (_type) helper_cswap##_size((uint64_t) t, c, v, pe); \
    }                                                               \

SHMEMC_TYPED_CSWAP(int, int, 32)
SHMEMC_TYPED_CSWAP(long, long, 64)
SHMEMC_TYPED_CSWAP(longlong, long long, 64)

/*
 * fetch & set
 *
 * TODO: UCX really does have a fetch_nb, but for now do something
 * simple
 *
 */

#define SHMEMC_TYPED_FETCH(_name, _type, _size)                 \
    _type                                                       \
    shmemc_##_name##_fetch(_type *t, int pe)                    \
    {                                                           \
        return (_type) helper_fadd##_size((uint64_t) t, 0, pe); \
    }

SHMEMC_TYPED_FETCH(int, int, 32)
SHMEMC_TYPED_FETCH(long, long, 64)
SHMEMC_TYPED_FETCH(longlong, long long, 64)
SHMEMC_TYPED_FETCH(float, float, 32)
SHMEMC_TYPED_FETCH(double, double, 64)

/*
 * TODO: use swap and ignore return?
 */
#define SHMEMC_TYPED_SET(_name, _type, _size)           \
    void                                                \
    shmemc_##_name##_set(_type *t, _type v, int pe)     \
    {                                                   \
        (void) helper_swap##_size((uint64_t) t, v, pe); \
    }

SHMEMC_TYPED_SET(int, int, 32)
SHMEMC_TYPED_SET(long, long, 64)
SHMEMC_TYPED_SET(longlong, long long, 64)
SHMEMC_TYPED_SET(float, float, 32)
SHMEMC_TYPED_SET(double, double, 64)
