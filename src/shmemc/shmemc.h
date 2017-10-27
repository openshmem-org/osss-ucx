/* For license: see LICENSE file at top-level */

#ifndef _SHMEMC_H
#define _SHMEMC_H 1

#include "shmem/defs.h"

#include <sys/types.h>          /* size_t */
#include <stddef.h>             /* ptrdiff_t */
#include <stdint.h>
#include <unistd.h>

/**
 * API
 *
 **/

void shmemc_init(void);
void shmemc_finalize(void);

char *shmemc_getenv(const char *name);

int shmemc_my_pe(void);
int shmemc_n_pes(void);

void shmemc_quiet(void);
void shmemc_fence(void);

void *shmemc_ptr(const void *target, int pe);

void shmemc_put(void *dest, const void *src, size_t nbytes, int pe);
void shmemc_get(void *dest, const void *src, size_t nbytes, int pe);

void shmemc_put_nbi(void *dest, const void *src, size_t nbytes, int pe);
void shmemc_get_nbi(void *dest, const void *src, size_t nbytes, int pe);

/*
 * swappity
 */

#define SHMEMC_DECL_SWAP(_size)                                         \
    uint64_t shmemc_swap##_size(void *target, uint64_t value, int pe);

SHMEMC_DECL_SWAP(32)
SHMEMC_DECL_SWAP(64)

#define SHMEMC_DECL_CSWAP(_size)                                \
    uint64_t shmemc_cswap##_size(void *target,                  \
                                 uint64_t cond, uint64_t value, \
                                 int pe);

SHMEMC_DECL_CSWAP(32)
SHMEMC_DECL_CSWAP(64)

/*
 * adds and incs
 */

#define SHMEMC_DECL_ADD(_size)                                      \
    void shmemc_add##_size(void *target, uint64_t value, int pe);

#define SHMEMC_DECL_INC(_size)                                      \
    void shmemc_inc##_size(void *target, int pe);

SHMEMC_DECL_ADD(32)
SHMEMC_DECL_ADD(64)

SHMEMC_DECL_INC(32)
SHMEMC_DECL_INC(64)

#define SHMEMC_DECL_FETCH_ADD(_size)                                    \
    uint64_t shmemc_fadd##_size(void *target, uint64_t value, int pe);

#define SHMEMC_DECL_FETCH_INC(_size)                    \
    uint64_t shmemc_finc##_size(void *target, int pe);

SHMEMC_DECL_FETCH_ADD(32)
SHMEMC_DECL_FETCH_ADD(64)

SHMEMC_DECL_FETCH_INC(32)
SHMEMC_DECL_FETCH_INC(64)

#define SHMEMC_DECL_CSWAP(_size)                                \
    uint64_t shmemc_cswap##_size(void *target,                  \
                                 uint64_t cond, uint64_t value, \
                                 int pe);

SHMEMC_DECL_CSWAP(32)
SHMEMC_DECL_CSWAP(64)

/*
 * fetch and set
 */

#define SHMEMC_DECL_FETCH(_size)                        \
    uint64_t shmemc_fetch##_size(void *target, int pe);

SHMEMC_DECL_FETCH(32)
SHMEMC_DECL_FETCH(64)

#define SHMEMC_DECL_SET(_size)                                      \
    void shmemc_set##_size(void *target, uint64_t value, int pe);

SHMEMC_DECL_SET(32)
SHMEMC_DECL_SET(64)

/*
 * bitwise
 */

#define SHMEMC_DECL_BITWISE(_op, _size)                         \
    void shmemc_##_op##_size(void *target, uint64_t value, int pe);

SHMEMC_DECL_BITWISE(and, 32)
SHMEMC_DECL_BITWISE(and, 64)

SHMEMC_DECL_BITWISE(or, 32)
SHMEMC_DECL_BITWISE(or, 64)

SHMEMC_DECL_BITWISE(xor, 32)
SHMEMC_DECL_BITWISE(xor, 64)

/*
 * fetch-bitwise
 */

#define SHMEMC_DECL_FETCH_BITWISE(_op, _size)                           \
    uint64_t shmemc_fetch_##_op##_size(void *target, uint64_t value, int pe);

SHMEMC_DECL_FETCH_BITWISE(and, 32)
SHMEMC_DECL_FETCH_BITWISE(and, 64)

SHMEMC_DECL_FETCH_BITWISE(or, 32)
SHMEMC_DECL_FETCH_BITWISE(or, 64)

SHMEMC_DECL_FETCH_BITWISE(xor, 32)
SHMEMC_DECL_FETCH_BITWISE(xor, 64)

/*
 * locks
 */

void shmemc_set_lock(long *lock);
void shmemc_clear_lock(long *lock);
int  shmemc_test_lock(long *lock);

/*
 * routine per-type and test to avoid branching
 *
 * NEEDS PUSH-DOWN INTO COMMS IMPLEMENTATION FOR ACTUAL WAIT
 */
#define SHMEMC_WAITUNTIL_TYPE(_name, _type, _opname)                \
    void shmemc_##_name##_wait_##_opname##_until(_type *var,        \
                                                 _type cmp_value);

SHMEMC_WAITUNTIL_TYPE(short, short, eq)
SHMEMC_WAITUNTIL_TYPE(int, int, eq)
SHMEMC_WAITUNTIL_TYPE(long, long, eq)
SHMEMC_WAITUNTIL_TYPE(longlong, long long, eq)

SHMEMC_WAITUNTIL_TYPE(short, short, ne)
SHMEMC_WAITUNTIL_TYPE(int, int, ne)
SHMEMC_WAITUNTIL_TYPE(long, long, ne)
SHMEMC_WAITUNTIL_TYPE(longlong, long long, ne)

SHMEMC_WAITUNTIL_TYPE(short, short, gt)
SHMEMC_WAITUNTIL_TYPE(int, int, gt)
SHMEMC_WAITUNTIL_TYPE(long, long, gt)
SHMEMC_WAITUNTIL_TYPE(longlong, long long, gt)

SHMEMC_WAITUNTIL_TYPE(short, short, le)
SHMEMC_WAITUNTIL_TYPE(int, int, le)
SHMEMC_WAITUNTIL_TYPE(long, long, le)
SHMEMC_WAITUNTIL_TYPE(longlong, long long, le)

SHMEMC_WAITUNTIL_TYPE(short, short, lt)
SHMEMC_WAITUNTIL_TYPE(int, int, lt)
SHMEMC_WAITUNTIL_TYPE(long, long, lt)
SHMEMC_WAITUNTIL_TYPE(longlong, long long, lt)

SHMEMC_WAITUNTIL_TYPE(short, short, ge)
SHMEMC_WAITUNTIL_TYPE(int, int, ge)
SHMEMC_WAITUNTIL_TYPE(long, long, ge)
SHMEMC_WAITUNTIL_TYPE(longlong, long long, ge)

/*
 * barriers
 */
void shmemc_barrier(int start, int log_stride, int size, long *pSync);
void shmemc_barrier_all(void);

/*
 * broadcasts
 */
void shmemc_broadcast32(void *target, const void *source,
                        size_t nelems,
                        int PE_root, int PE_start,
                        int logPE_stride, int PE_size,
                        long *pSync);
void shmemc_broadcast64(void *target, const void *source,
                        size_t nelems,
                        int PE_root, int PE_start,
                        int logPE_stride, int PE_size,
                        long *pSync);

/*
 * zap the program
 */

inline static int
shmemc_global_exit(int status)
{
    _exit(status);
}

/*
 * accessibility
 */

inline static int
shmemc_pe_accessible(int pe)
{
    return 1;
}

inline static int
shmemc_addr_accessible(const void *addr, int pe)
{
    return 0;
}

#endif /* ! _SHMEMC_H */
