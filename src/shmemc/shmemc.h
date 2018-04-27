/* For license: see LICENSE file at top-level */

#ifndef _SHMEMC_H
#define _SHMEMC_H 1

#include "thispe.h"
#include "state.h"
#include "shmemc.h"
#include "memfence.h"
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
void shmemc_global_exit(int status);

inline static int shmemc_my_pe(void) { return proc.rank; }
inline static int shmemc_n_pes(void) { return proc.nranks; }

void *shmemc_ctx_ptr(shmem_ctx_t ctx, const void *target, int pe);
int shmemc_pe_accessible(int pe);
int shmemc_addr_accessible(const void *addr, int pe);

void shmemc_print_env_vars(FILE *stream, const char *prefix);

/*
 * -- Per-context routines ---------------------------------------------------
 */

void shmemc_ctx_progress(shmem_ctx_t ctx);
void shmemc_progress(void);

void shmemc_ctx_fence(shmem_ctx_t ctx);
void shmemc_ctx_quiet(shmem_ctx_t ctx);

#ifdef ENABLE_EXPERIMENTAL

int shmemc_ctx_fence_test(shmem_ctx_t ctx);
int shmemc_ctx_quiet_test(shmem_ctx_t ctx);

#endif  /* ENABLE_EXPERIMENTAL */

void shmemc_ctx_put(shmem_ctx_t ctx,
                    void *dest, const void *src,
                    size_t nbytes, int pe);
void shmemc_ctx_get(shmem_ctx_t ctx,
                    void *dest, const void *src,
                    size_t nbytes, int pe);

void shmemc_ctx_put_nbi(shmem_ctx_t ctx,
                        void *dest, const void *src,
                        size_t nbytes, int pe);
void shmemc_ctx_get_nbi(shmem_ctx_t ctx,
                        void *dest, const void *src,
                        size_t nbytes, int pe);

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
                                           void *target,                \
                                           uint64_t value, int pe);

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
 * routine per-type and test to avoid branching
 */

#define SHMEMC_CTX_TEST(_size, _opname)                         \
    int shmemc_ctx_test_##_opname##_size(shmem_ctx_t ctx,       \
                                         int##_size##_t *var,   \
                                         int##_size##_t value);

SHMEMC_CTX_TEST(16, eq)
SHMEMC_CTX_TEST(32, eq)
SHMEMC_CTX_TEST(64, eq)

SHMEMC_CTX_TEST(16, ne)
SHMEMC_CTX_TEST(32, ne)
SHMEMC_CTX_TEST(64, ne)

SHMEMC_CTX_TEST(16, gt)
SHMEMC_CTX_TEST(32, gt)
SHMEMC_CTX_TEST(64, gt)

SHMEMC_CTX_TEST(16, le)
SHMEMC_CTX_TEST(32, le)
SHMEMC_CTX_TEST(64, le)

SHMEMC_CTX_TEST(16, lt)
SHMEMC_CTX_TEST(32, lt)
SHMEMC_CTX_TEST(64, lt)

SHMEMC_CTX_TEST(16, ge)
SHMEMC_CTX_TEST(32, ge)
SHMEMC_CTX_TEST(64, ge)

#define SHMEMC_CTX_WAIT_UNTIL(_size, _opname)                           \
    void shmemc_ctx_wait_##_opname##_until##_size(shmem_ctx_t ctx,      \
                                                  int##_size##_t *var,  \
                                                  int##_size##_t value);

SHMEMC_CTX_WAIT_UNTIL(16, eq)
SHMEMC_CTX_WAIT_UNTIL(32, eq)
SHMEMC_CTX_WAIT_UNTIL(64, eq)

SHMEMC_CTX_WAIT_UNTIL(16, ne)
SHMEMC_CTX_WAIT_UNTIL(32, ne)
SHMEMC_CTX_WAIT_UNTIL(64, ne)

SHMEMC_CTX_WAIT_UNTIL(16, gt)
SHMEMC_CTX_WAIT_UNTIL(32, gt)
SHMEMC_CTX_WAIT_UNTIL(64, gt)

SHMEMC_CTX_WAIT_UNTIL(16, le)
SHMEMC_CTX_WAIT_UNTIL(32, le)
SHMEMC_CTX_WAIT_UNTIL(64, le)

SHMEMC_CTX_WAIT_UNTIL(16, lt)
SHMEMC_CTX_WAIT_UNTIL(32, lt)
SHMEMC_CTX_WAIT_UNTIL(64, lt)

SHMEMC_CTX_WAIT_UNTIL(16, ge)
SHMEMC_CTX_WAIT_UNTIL(32, ge)
SHMEMC_CTX_WAIT_UNTIL(64, ge)

/*
 * -- Context management -----------------------------------------------------
 *
 * int functions return 0 for success, non-zero otherwise
 */

int shmemc_context_create(long options, shmem_ctx_t *ctxp);
void shmemc_context_destroy(shmem_ctx_t ctx);
unsigned long shmemc_context_id(shmem_ctx_t ctx);

extern shmemc_context_t shmemc_default_context;
int shmemc_create_default_context(void);

/*
 * -- barriers & syncs -------------------------------------------------------
 */

void shmemc_barrier_init(void);
void shmemc_barrier_finalize(void);

#define SHMEMC_DECL_BARRIER_SYNC(_op)                                   \
    void shmemc_##_op(int start, int log_stride, int size, long *pSync); \
    void shmemc_##_op##_all(void);

SHMEMC_DECL_BARRIER_SYNC(sync)
SHMEMC_DECL_BARRIER_SYNC(barrier)

/*
 * -- broadcasts -------------------------------------------------------------
 */

void shmemc_broadcast_init(void);
void shmemc_broadcast_finalize(void);

#define SHMEMC_DECL_BROADCAST_SIZE(_size)                           \
    void shmemc_broadcast##_size(void *target, const void *source,  \
                                 size_t nelems,                     \
                                 int PE_root, int PE_start,         \
                                 int logPE_stride, int PE_size,     \
                                 long *pSync);

SHMEMC_DECL_BROADCAST_SIZE(32)
SHMEMC_DECL_BROADCAST_SIZE(64)

/*
 * -- collects ---------------------------------------------------------------
 */

#define SHMEMC_DECL_FCOLLECT_SIZE(_size)                             \
    void shmemc_fcollect##_size(void *target, const void *source,    \
                                size_t nelems,                       \
                                int PE_start,                        \
                                int logPE_stride, int PE_size,       \
                                long *pSync);

SHMEMC_DECL_FCOLLECT_SIZE(32)
SHMEMC_DECL_FCOLLECT_SIZE(64)

#define SHMEMC_DECL_COLLECT_SIZE(_size)                             \
    void shmemc_collect##_size(void *target, const void *source,    \
                               size_t nelems,                       \
                               int PE_start,                        \
                               int logPE_stride, int PE_size,       \
                               long *pSync);

SHMEMC_DECL_COLLECT_SIZE(32)
SHMEMC_DECL_COLLECT_SIZE(64)

/*
 * TODO: reductions should get moved here in case comms-layer has
 * hardware assist or similar
 */

/*
 * -- Routines that now operate on default context ---------------------------
 */

#define shmemc_fence()                          \
    shmemc_ctx_fence(SHMEM_CTX_DEFAULT)
#define shmemc_quiet()                          \
    shmemc_ctx_quiet(SHMEM_CTX_DEFAULT)

#define shmemc_ptr(...)                             \
    shmemc_ctx_ptr(SHMEM_CTX_DEFAULT, __VA_ARGS__)

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

#define shmemc_test_eq16(...)                         \
    shmemc_ctx_test_eq16(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_test_eq32(...)                         \
    shmemc_ctx_test_eq32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_test_eq64(...)                         \
    shmemc_ctx_test_eq64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_test_ne16(...)                         \
    shmemc_ctx_test_ne16(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_test_ne32(...)                         \
    shmemc_ctx_test_ne32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_test_ne64(...)                         \
    shmemc_ctx_test_ne64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_test_gt16(...)                         \
    shmemc_ctx_test_gt16(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_test_gt32(...)                         \
    shmemc_ctx_test_gt32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_test_gt64(...)                         \
    shmemc_ctx_test_gt64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_test_le16(...)                         \
    shmemc_ctx_test_le16(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_test_le32(...)                         \
    shmemc_ctx_test_le32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_test_le64(...)                         \
    shmemc_ctx_test_le64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_test_lt16(...)                         \
    shmemc_ctx_test_lt16(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_test_lt32(...)                         \
    shmemc_ctx_test_lt32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_test_lt64(...)                         \
    shmemc_ctx_test_lt64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_test_ge16(...)                         \
    shmemc_ctx_test_ge16(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_test_ge32(...)                         \
    shmemc_ctx_test_ge32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_test_ge64(...)                         \
    shmemc_ctx_test_ge64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_wait_eq_until16(...)                         \
    shmemc_ctx_wait_eq_until16(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_wait_eq_until32(...)                         \
    shmemc_ctx_wait_eq_until32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_wait_eq_until64(...)                         \
    shmemc_ctx_wait_eq_until64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_wait_ne_until16(...)                         \
    shmemc_ctx_wait_ne_until16(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_wait_ne_until32(...)                         \
    shmemc_ctx_wait_ne_until32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_wait_ne_until64(...)                         \
    shmemc_ctx_wait_ne_until64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_wait_gt_until16(...)                         \
    shmemc_ctx_wait_gt_until16(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_wait_gt_until32(...)                         \
    shmemc_ctx_wait_gt_until32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_wait_gt_until64(...)                         \
    shmemc_ctx_wait_gt_until64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_wait_le_until16(...)                         \
    shmemc_ctx_wait_le_until16(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_wait_le_until32(...)                         \
    shmemc_ctx_wait_le_until32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_wait_le_until64(...)                         \
    shmemc_ctx_wait_le_until64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_wait_lt_until16(...)                         \
    shmemc_ctx_wait_lt_until16(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_wait_lt_until32(...)                         \
    shmemc_ctx_wait_lt_until32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_wait_lt_until64(...)                         \
    shmemc_ctx_wait_lt_until64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_wait_ge_until16(...)                         \
    shmemc_ctx_wait_ge_until16(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_wait_ge_until32(...)                         \
    shmemc_ctx_wait_ge_until32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_wait_ge_until64(...)                         \
    shmemc_ctx_wait_ge_until64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#endif /* ! _SHMEMC_H */
