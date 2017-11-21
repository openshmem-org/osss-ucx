/* For license: see LICENSE file at top-level */

#ifndef _SHMEMC_H
#define _SHMEMC_H 1

#include "thispe.h"
#include "shmemc.h"
#include "shmem/defs.h"

#include <sys/types.h>          /* size_t */
#include <stddef.h>             /* ptrdiff_t */
#include <stdint.h>
#include <unistd.h>

/**
 * -- API --------------------------------------------------------------------
 *
 **/

/*
 * setup and query
 */

void shmemc_init(void);
void shmemc_finalize(void);
void shmemc_trigger_globalexit(int status);

char *shmemc_getenv(const char *name);

int shmemc_my_pe(void);
int shmemc_n_pes(void);

void *shmemc_ptr(const void *target, int pe);
int shmemc_pe_accessible(int pe);
int shmemc_addr_accessible(const void *addr, int pe);

/*
 * -- Per-context routines ---------------------------------------------------
 */

void shmemc_ctx_fence(shmem_ctx_t ctx);
void shmemc_ctx_quiet(shmem_ctx_t ctx);

void shmemc_ctx_put(shmem_ctx_t ctx,
                    void *dest, const void *src, size_t nbytes, int pe);
void shmemc_ctx_get(shmem_ctx_t ctx,
                    void *dest, const void *src, size_t nbytes, int pe);

void shmemc_ctx_put_nbi(shmem_ctx_t ctx,
                        void *dest, const void *src, size_t nbytes, int pe);
void shmemc_ctx_get_nbi(shmem_ctx_t ctx,
                        void *dest, const void *src, size_t nbytes, int pe);

/*
 * -- AMOs -------------------------------------------------------------------
 */

/*
 * swappity
 */

#define SHMEMC_CTX_DECL_SWAP(_size)                                     \
    uint64_t shmemc_ctx_swap##_size(shmem_ctx_t ctx,                    \
                                    void *target, uint64_t value, int pe);

SHMEMC_CTX_DECL_SWAP(32)
SHMEMC_CTX_DECL_SWAP(64)

#define SHMEMC_CTX_DECL_CSWAP(_size)                                \
    uint64_t shmemc_ctx_cswap##_size(shmem_ctx_t ctx,               \
                                     void *target,                  \
                                     uint64_t cond, uint64_t value, \
                                     int pe);

SHMEMC_CTX_DECL_CSWAP(32)
SHMEMC_CTX_DECL_CSWAP(64)

/*
 * adds and incs
 */

#define SHMEMC_CTX_DECL_ADD(_size)                                      \
    void shmemc_ctx_add##_size(shmem_ctx_t ctx,                         \
                               void *target, uint64_t value, int pe);

#define SHMEMC_CTX_DECL_INC(_size)                      \
    void shmemc_ctx_inc##_size(shmem_ctx_t ctx,         \
                               void *target, int pe);

SHMEMC_CTX_DECL_ADD(32)
SHMEMC_CTX_DECL_ADD(64)

SHMEMC_CTX_DECL_INC(32)
SHMEMC_CTX_DECL_INC(64)

#define SHMEMC_CTX_DECL_FETCH_ADD(_size)                                \
    uint64_t shmemc_ctx_fadd##_size(shmem_ctx_t ctx,                    \
                                    void *target, uint64_t value, int pe);

#define SHMEMC_CTX_DECL_FETCH_INC(_size)                    \
    uint64_t shmemc_ctx_finc##_size(shmem_ctx_t ctx,        \
                                    void *target, int pe);

SHMEMC_CTX_DECL_FETCH_ADD(32)
SHMEMC_CTX_DECL_FETCH_ADD(64)

SHMEMC_CTX_DECL_FETCH_INC(32)
SHMEMC_CTX_DECL_FETCH_INC(64)

/*
 * fetch and set
 */

#define SHMEMC_CTX_DECL_FETCH(_size)                        \
    uint64_t shmemc_ctx_fetch##_size(shmem_ctx_t ctx,       \
                                     void *target, int pe);

SHMEMC_CTX_DECL_FETCH(32)
SHMEMC_CTX_DECL_FETCH(64)

#define SHMEMC_CTX_DECL_SET(_size)                                      \
    void shmemc_ctx_set##_size(shmem_ctx_t ctx,                         \
                               void *target, uint64_t value, int pe);

SHMEMC_CTX_DECL_SET(32)
SHMEMC_CTX_DECL_SET(64)

/*
 * bitwise
 */

#define SHMEMC_CTX_DECL_BITWISE(_op, _size)                             \
    void shmemc_ctx_##_op##_size(shmem_ctx_t ctx,                       \
                                 void *target, uint64_t value, int pe);

SHMEMC_CTX_DECL_BITWISE(and, 32)
SHMEMC_CTX_DECL_BITWISE(and, 64)

SHMEMC_CTX_DECL_BITWISE(or, 32)
SHMEMC_CTX_DECL_BITWISE(or, 64)

SHMEMC_CTX_DECL_BITWISE(xor, 32)
SHMEMC_CTX_DECL_BITWISE(xor, 64)

/*
 * fetch-bitwise
 */

#define SHMEMC_CTX_DECL_FETCH_BITWISE(_op, _size)                       \
    uint64_t shmemc_ctx_fetch_##_op##_size(shmem_ctx_t ctx,             \
                                           void *target, uint64_t value, int pe);

SHMEMC_CTX_DECL_FETCH_BITWISE(and, 32)
SHMEMC_CTX_DECL_FETCH_BITWISE(and, 64)

SHMEMC_CTX_DECL_FETCH_BITWISE(or, 32)
SHMEMC_CTX_DECL_FETCH_BITWISE(or, 64)

SHMEMC_CTX_DECL_FETCH_BITWISE(xor, 32)
SHMEMC_CTX_DECL_FETCH_BITWISE(xor, 64)

/*
 * locks
 */

void shmemc_set_lock(long *lock);
void shmemc_clear_lock(long *lock);
int  shmemc_test_lock(long *lock);

/*
 * -- Routines that now operate on default context ---------------------------
 */

#define shmemc_fence()                          \
    shmemc_ctx_fence(SHMEM_CTX_DEFAULT)
#define shmemc_quiet()                          \
    shmemc_ctx_quiet(SHMEM_CTX_DEFAULT)

#define shmemc_put(...)                             \
    shmemc_ctx_put(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_get(...)                             \
    shmemc_ctx_get(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_put_nbi(...)                             \
    shmemc_ctx_put_nbi(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_get_nbi(...)                             \
    shmemc_ctx_get_nbi(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_swap32(...)                              \
    shmemc_ctx_swap32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_swap64(...)                              \
    shmemc_ctx_swap64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_cswap32(...)                             \
    shmemc_ctx_cswap32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_cswap64(...)                             \
    shmemc_ctx_cswap64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_add32(...)                               \
    shmemc_ctx_add32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_add64(...)                               \
    shmemc_ctx_add64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_inc32(...)                               \
    shmemc_ctx_inc32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_inc64(...)                               \
    shmemc_ctx_inc64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_fadd32(...)                              \
    shmemc_ctx_fadd32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_fadd64(...)                              \
    shmemc_ctx_fadd64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_finc32(...)                              \
    shmemc_ctx_finc32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_finc64(...)                              \
    shmemc_ctx_finc64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_fetch32(...)                             \
    shmemc_ctx_fetch32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_fetch64(...)                             \
    shmemc_ctx_fetch64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_set32(...)                               \
    shmemc_ctx_set32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_set64(...)                               \
    shmemc_ctx_set64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_and32(...)                               \
    shmemc_ctx_and32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_and64(...)                               \
    shmemc_ctx_and64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_or32(...)                            \
    shmemc_ctx_or32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_or64(...)                            \
    shmemc_ctx_or64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_xor32(...)                               \
    shmemc_ctx_xor32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_xor64(...)                               \
    shmemc_ctx_xor64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_fetch_and32(...)                             \
    shmemc_ctx_fetch_and32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_fetch_and64(...)                             \
    shmemc_ctx_fetch_and64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_fetch_or32(...)                              \
    shmemc_ctx_fetch_or32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_fetch_or64(...)                              \
    shmemc_ctx_fetch_or64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_fetch_xor32(...)                             \
    shmemc_ctx_fetch_xor32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_fetch_xor64(...)                             \
    shmemc_ctx_fetch_xor64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

/*
 * routine per-type and test to avoid branching
 */

#define SHMEMC_TEST(_size, _opname)                         \
    int shmemc_test_##_opname##_size(int##_size##_t *var,   \
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
 * -- Contexts ---------------------------------------------------------------
 */

int shmemc_context_create(long options, shmemc_context_h *ctxp);
void shmemc_context_destroy(shmemc_context_h ctx);

/*
 * -- barriers & syncs -------------------------------------------------------
 */

#define SHMEMC_DECL_BARRIER_SYNC(_op)                                   \
    void shmemc_##_op(int start, int log_stride, int size, long *pSync); \
    void shmemc_##_op##_all(void);

SHMEMC_DECL_BARRIER_SYNC(sync)
SHMEMC_DECL_BARRIER_SYNC(barrier)

/*
 * -- broadcasts -------------------------------------------------------------
 */

#define SHMEMC_DECL_BROADCAST_SIZE(_size)                           \
    void shmemc_broadcast##_size(void *target, const void *source,  \
                                 size_t nelems,                     \
                                 int PE_root, int PE_start,         \
                                 int logPE_stride, int PE_size,     \
                                 long *pSync);

SHMEMC_DECL_BROADCAST_SIZE(32)
SHMEMC_DECL_BROADCAST_SIZE(64)

/*
 * TODO: reductions should get moved here in case comms-layer has
 * hardware assist or similar
 */

#endif /* ! _SHMEMC_H */
