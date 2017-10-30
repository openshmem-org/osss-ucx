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
 */

#define SHMEMC_TEST(_size, _opname)                                     \
    int shmemc_test_##_opname##_size(int##_size##_t *var,               \
                                     int##_size##_t value);

SHMEMC_TEST(16, eq)
SHMEMC_TEST(32, eq)
SHMEMC_TEST(64, eq)

SHMEMC_TEST(16, ne)
SHMEMC_TEST(32, ne)
SHMEMC_TEST(64, ne)

SHMEMC_TEST(16, gt)
SHMEMC_TEST(32, gt)
SHMEMC_TEST(64, gt)

SHMEMC_TEST(16, le)
SHMEMC_TEST(32, le)
SHMEMC_TEST(64, le)

SHMEMC_TEST(16, lt)
SHMEMC_TEST(32, lt)
SHMEMC_TEST(64, lt)

SHMEMC_TEST(16, ge)
SHMEMC_TEST(32, ge)
SHMEMC_TEST(64, ge)

#define SHMEMC_WAITUNTIL(_size, _opname)                                \
    void shmemc_wait_##_opname##_until##_size(int##_size##_t *var,      \
                                              int##_size##_t value);

SHMEMC_WAITUNTIL(16, eq)
SHMEMC_WAITUNTIL(32, eq)
SHMEMC_WAITUNTIL(64, eq)

SHMEMC_WAITUNTIL(16, ne)
SHMEMC_WAITUNTIL(32, ne)
SHMEMC_WAITUNTIL(64, ne)

SHMEMC_WAITUNTIL(16, gt)
SHMEMC_WAITUNTIL(32, gt)
SHMEMC_WAITUNTIL(64, gt)

SHMEMC_WAITUNTIL(16, le)
SHMEMC_WAITUNTIL(32, le)
SHMEMC_WAITUNTIL(64, le)

SHMEMC_WAITUNTIL(16, lt)
SHMEMC_WAITUNTIL(32, lt)
SHMEMC_WAITUNTIL(64, lt)

SHMEMC_WAITUNTIL(16, ge)
SHMEMC_WAITUNTIL(32, ge)
SHMEMC_WAITUNTIL(64, ge)

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
void shmemc_trigger_globalexit(int status);

/*
 * accessibility
 */
int shmemc_pe_accessible(int pe);
int shmemc_addr_accessible(const void *addr, int pe);

#endif /* ! _SHMEMC_H */
