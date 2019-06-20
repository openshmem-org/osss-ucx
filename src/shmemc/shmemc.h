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

void shmemc_globalexit_init(void);
void shmemc_globalexit_finalize(void);
void shmemc_global_exit(int status);

inline static int shmemc_my_pe(void) { return proc.rank; }
inline static int shmemc_n_pes(void) { return proc.nranks; }

void *shmemc_ctx_ptr(shmem_ctx_t ctx, const void *target, int pe);
int shmemc_pe_accessible(int pe);
int shmemc_addr_accessible(const void *addr, int pe);

void shmemc_env_init(void);
void shmemc_env_finalize(void);
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

void shmemc_ctx_put_signal(shmem_ctx_t ctx,
                           void *dest, const void *src, size_t nbytes,
                           uint64_t *sig_target, uint64_t sig_val,
                           int pe);
void shmemc_ctx_get_signal(shmem_ctx_t ctx,
                           void *dest, const void *src, size_t nbytes,
                           uint64_t *sig_target, uint64_t sig_val,
                           int pe);

/*
 * -- AMOs -------------------------------------------------------------------
 */

/*
 * swappity
 */

void shmemc_ctx_swap(shmem_ctx_t ctx,
                     void *target, void *value, size_t vals,
                     int pe,
                     void *retp);

void shmemc_ctx_cswap(shmem_ctx_t ctx,
                      void *target,
                      void *cond, void *value, size_t vals,
                      int pe,
                      void *retp);

/*
 * adds and incs
 */

void shmemc_ctx_add(shmem_ctx_t ctx,
                    void *target, void *value, size_t vals,
                    int pe);

void shmemc_ctx_fadd(shmem_ctx_t ctx,
                    void *target, void *value, size_t vals,
                    int pe,
                    void *retp);

/*
 * set
 */

void shmemc_ctx_set(shmem_ctx_t ctx,
                    void *target, void *vp, size_t vs,
                    int pe);

/*
 * bitwise
 */

#define SHMEMC_CTX_DECL_BITWISE(_op)                                    \
    void shmemc_ctx_##_op(shmem_ctx_t ctx,                              \
                          void *target, void *value, size_t vals,       \
                          int pe);

SHMEMC_CTX_DECL_BITWISE(and)
SHMEMC_CTX_DECL_BITWISE(or)
SHMEMC_CTX_DECL_BITWISE(xor)

/*
 * fetch-bitwise
 */

#define SHMEMC_CTX_DECL_FETCH_BITWISE(_op)                              \
    void shmemc_ctx_fetch_##_op(shmem_ctx_t ctx,                        \
                                void *target, void *value, size_t vals, \
                                int pe,                                 \
                                void *retp);

SHMEMC_CTX_DECL_FETCH_BITWISE(and)
SHMEMC_CTX_DECL_FETCH_BITWISE(or)
SHMEMC_CTX_DECL_FETCH_BITWISE(xor)

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
    void shmemc_ctx_wait_until_##_opname##_size(shmem_ctx_t ctx,        \
                                                int##_size##_t *var,    \
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

#define SHMEMC_CTX_TEST_ALL(_size, _opname)                             \
    int shmemc_ctx_test_all_##_opname##_size(shmem_ctx_t ctx,           \
                                             int##_size##_t *vars,      \
                                             size_t nelems,             \
                                             int##_size##_t value);

SHMEMC_CTX_TEST_ALL(16, eq)
SHMEMC_CTX_TEST_ALL(32, eq)
SHMEMC_CTX_TEST_ALL(64, eq)

SHMEMC_CTX_TEST_ALL(16, ne)
SHMEMC_CTX_TEST_ALL(32, ne)
SHMEMC_CTX_TEST_ALL(64, ne)

SHMEMC_CTX_TEST_ALL(16, gt)
SHMEMC_CTX_TEST_ALL(32, gt)
SHMEMC_CTX_TEST_ALL(64, gt)

SHMEMC_CTX_TEST_ALL(16, le)
SHMEMC_CTX_TEST_ALL(32, le)
SHMEMC_CTX_TEST_ALL(64, le)

SHMEMC_CTX_TEST_ALL(16, lt)
SHMEMC_CTX_TEST_ALL(32, lt)
SHMEMC_CTX_TEST_ALL(64, lt)

SHMEMC_CTX_TEST_ALL(16, ge)
SHMEMC_CTX_TEST_ALL(32, ge)
SHMEMC_CTX_TEST_ALL(64, ge)

#define SHMEMC_CTX_TEST_SOME(_size, _opname)                            \
    size_t shmemc_ctx_test_some_##_opname##_size(shmem_ctx_t ctx,       \
                                                 int##_size##_t * restrict vars,  \
                                                 size_t nelems,         \
                                                 size_t * restrict idxs, \
                                                 int * restrict status, \
                                                 int##_size##_t value);

SHMEMC_CTX_TEST_SOME(16, eq)
SHMEMC_CTX_TEST_SOME(32, eq)
SHMEMC_CTX_TEST_SOME(64, eq)

SHMEMC_CTX_TEST_SOME(16, ne)
SHMEMC_CTX_TEST_SOME(32, ne)
SHMEMC_CTX_TEST_SOME(64, ne)

SHMEMC_CTX_TEST_SOME(16, gt)
SHMEMC_CTX_TEST_SOME(32, gt)
SHMEMC_CTX_TEST_SOME(64, gt)

SHMEMC_CTX_TEST_SOME(16, le)
SHMEMC_CTX_TEST_SOME(32, le)
SHMEMC_CTX_TEST_SOME(64, le)

SHMEMC_CTX_TEST_SOME(16, lt)
SHMEMC_CTX_TEST_SOME(32, lt)
SHMEMC_CTX_TEST_SOME(64, lt)

SHMEMC_CTX_TEST_SOME(16, ge)
SHMEMC_CTX_TEST_SOME(32, ge)
SHMEMC_CTX_TEST_SOME(64, ge)

#define SHMEMC_CTX_TEST_ANY(_size, _opname)                             \
    size_t shmemc_ctx_test_any_##_opname##_size(shmem_ctx_t ctx,        \
                                                int##_size##_t * restrict vars,   \
                                                size_t nelems,          \
                                                int * restrict status,  \
                                                int##_size##_t value);

SHMEMC_CTX_TEST_ANY(16, eq)
SHMEMC_CTX_TEST_ANY(32, eq)
SHMEMC_CTX_TEST_ANY(64, eq)

SHMEMC_CTX_TEST_ANY(16, ne)
SHMEMC_CTX_TEST_ANY(32, ne)
SHMEMC_CTX_TEST_ANY(64, ne)

SHMEMC_CTX_TEST_ANY(16, gt)
SHMEMC_CTX_TEST_ANY(32, gt)
SHMEMC_CTX_TEST_ANY(64, gt)

SHMEMC_CTX_TEST_ANY(16, le)
SHMEMC_CTX_TEST_ANY(32, le)
SHMEMC_CTX_TEST_ANY(64, le)

SHMEMC_CTX_TEST_ANY(16, lt)
SHMEMC_CTX_TEST_ANY(32, lt)
SHMEMC_CTX_TEST_ANY(64, lt)

SHMEMC_CTX_TEST_ANY(16, ge)
SHMEMC_CTX_TEST_ANY(32, ge)
SHMEMC_CTX_TEST_ANY(64, ge)

#define SHMEMC_CTX_WAIT_UNTIL_ALL(_size, _opname)                       \
    void shmemc_ctx_wait_until_all_##_opname##_size(shmem_ctx_t ctx,    \
                                                    int##_size##_t *vars, \
                                                    size_t nelems,      \
                                                    int##_size##_t value);

SHMEMC_CTX_WAIT_UNTIL_ALL(16, eq)
SHMEMC_CTX_WAIT_UNTIL_ALL(32, eq)
SHMEMC_CTX_WAIT_UNTIL_ALL(64, eq)

SHMEMC_CTX_WAIT_UNTIL_ALL(16, ne)
SHMEMC_CTX_WAIT_UNTIL_ALL(32, ne)
SHMEMC_CTX_WAIT_UNTIL_ALL(64, ne)

SHMEMC_CTX_WAIT_UNTIL_ALL(16, gt)
SHMEMC_CTX_WAIT_UNTIL_ALL(32, gt)
SHMEMC_CTX_WAIT_UNTIL_ALL(64, gt)

SHMEMC_CTX_WAIT_UNTIL_ALL(16, le)
SHMEMC_CTX_WAIT_UNTIL_ALL(32, le)
SHMEMC_CTX_WAIT_UNTIL_ALL(64, le)

SHMEMC_CTX_WAIT_UNTIL_ALL(16, lt)
SHMEMC_CTX_WAIT_UNTIL_ALL(32, lt)
SHMEMC_CTX_WAIT_UNTIL_ALL(64, lt)

SHMEMC_CTX_WAIT_UNTIL_ALL(16, ge)
SHMEMC_CTX_WAIT_UNTIL_ALL(32, ge)
SHMEMC_CTX_WAIT_UNTIL_ALL(64, ge)

#define SHMEMC_CTX_WAIT_UNTIL_SOME(_size, _opname)                      \
    size_t shmemc_ctx_wait_until_some_##_opname##_size(shmem_ctx_t ctx, \
                                                       int##_size##_t * restrict vars, \
                                                       size_t nelems,   \
                                                       size_t * restrict idxs, \
                                                       int * restrict status, \
                                                       int##_size##_t value);

SHMEMC_CTX_WAIT_UNTIL_SOME(16, eq)
SHMEMC_CTX_WAIT_UNTIL_SOME(32, eq)
SHMEMC_CTX_WAIT_UNTIL_SOME(64, eq)

SHMEMC_CTX_WAIT_UNTIL_SOME(16, ne)
SHMEMC_CTX_WAIT_UNTIL_SOME(32, ne)
SHMEMC_CTX_WAIT_UNTIL_SOME(64, ne)

SHMEMC_CTX_WAIT_UNTIL_SOME(16, gt)
SHMEMC_CTX_WAIT_UNTIL_SOME(32, gt)
SHMEMC_CTX_WAIT_UNTIL_SOME(64, gt)

SHMEMC_CTX_WAIT_UNTIL_SOME(16, le)
SHMEMC_CTX_WAIT_UNTIL_SOME(32, le)
SHMEMC_CTX_WAIT_UNTIL_SOME(64, le)

SHMEMC_CTX_WAIT_UNTIL_SOME(16, lt)
SHMEMC_CTX_WAIT_UNTIL_SOME(32, lt)
SHMEMC_CTX_WAIT_UNTIL_SOME(64, lt)

SHMEMC_CTX_WAIT_UNTIL_SOME(16, ge)
SHMEMC_CTX_WAIT_UNTIL_SOME(32, ge)
SHMEMC_CTX_WAIT_UNTIL_SOME(64, ge)

#define SHMEMC_CTX_WAIT_UNTIL_ANY(_size, _opname)                       \
    size_t shmemc_ctx_wait_until_any_##_opname##_size(shmem_ctx_t ctx,  \
                                                      int##_size##_t * restrict vars, \
                                                      size_t nelems,    \
                                                      int * restrict status, \
                                                      int##_size##_t value);

SHMEMC_CTX_WAIT_UNTIL_ANY(16, eq)
SHMEMC_CTX_WAIT_UNTIL_ANY(32, eq)
SHMEMC_CTX_WAIT_UNTIL_ANY(64, eq)

SHMEMC_CTX_WAIT_UNTIL_ANY(16, ne)
SHMEMC_CTX_WAIT_UNTIL_ANY(32, ne)
SHMEMC_CTX_WAIT_UNTIL_ANY(64, ne)

SHMEMC_CTX_WAIT_UNTIL_ANY(16, gt)
SHMEMC_CTX_WAIT_UNTIL_ANY(32, gt)
SHMEMC_CTX_WAIT_UNTIL_ANY(64, gt)

SHMEMC_CTX_WAIT_UNTIL_ANY(16, le)
SHMEMC_CTX_WAIT_UNTIL_ANY(32, le)
SHMEMC_CTX_WAIT_UNTIL_ANY(64, le)

SHMEMC_CTX_WAIT_UNTIL_ANY(16, lt)
SHMEMC_CTX_WAIT_UNTIL_ANY(32, lt)
SHMEMC_CTX_WAIT_UNTIL_ANY(64, lt)

SHMEMC_CTX_WAIT_UNTIL_ANY(16, ge)
SHMEMC_CTX_WAIT_UNTIL_ANY(32, ge)
SHMEMC_CTX_WAIT_UNTIL_ANY(64, ge)

/*
 * -- Context management -----------------------------------------------------
 *
 * int functions return 0 for success, non-zero for failure
 */

int shmemc_context_create(long options, shmem_ctx_t *ctxp);
void shmemc_context_destroy(shmem_ctx_t ctx);
unsigned long shmemc_context_id(shmem_ctx_t ctx);

/*
 * Special handling for default context
 */
extern shmemc_context_t shmemc_default_context;
extern shmemc_context_h defcp;

int shmemc_context_init_default(void);

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

#define shmemc_swap(...)                              \
    shmemc_ctx_swap(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_cswap(...)                             \
    shmemc_ctx_cswap(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_add(...)                               \
    shmemc_ctx_add(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_inc(...)                               \
    shmemc_ctx_inc(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_fadd(...)                              \
    shmemc_ctx_fadd(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_finc(...)                              \
    shmemc_ctx_finc(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_fetch(...)                             \
    shmemc_ctx_fetch(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_set(...)                               \
    shmemc_ctx_set(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_and(...)                               \
    shmemc_ctx_and(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_or(...)                            \
    shmemc_ctx_or(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_xor(...)                               \
    shmemc_ctx_xor(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_fetch_and(...)                             \
    shmemc_ctx_fetch_and(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_fetch_or(...)                              \
    shmemc_ctx_fetch_or(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_fetch_xor(...)                             \
    shmemc_ctx_fetch_xor(SHMEM_CTX_DEFAULT, __VA_ARGS__)

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

#define shmemc_wait_until_eq16(...)                         \
    shmemc_ctx_wait_until_eq16(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_wait_until_eq32(...)                         \
    shmemc_ctx_wait_until_eq32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_wait_until_eq64(...)                         \
    shmemc_ctx_wait_until_eq64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_wait_until_ne16(...)                         \
    shmemc_ctx_wait_until_ne16(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_wait_until_ne32(...)                         \
    shmemc_ctx_wait_until_ne32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_wait_until_ne64(...)                         \
    shmemc_ctx_wait_until_ne64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_wait_until_gt16(...)                         \
    shmemc_ctx_wait_until_gt16(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_wait_until_gt32(...)                         \
    shmemc_ctx_wait_until_gt32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_wait_until_gt64(...)                         \
    shmemc_ctx_wait_until_gt64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_wait_until_le16(...)                         \
    shmemc_ctx_wait_until_le16(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_wait_until_le32(...)                         \
    shmemc_ctx_wait_until_le32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_wait_until_le64(...)                         \
    shmemc_ctx_wait_until_le64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_wait_until_lt16(...)                         \
    shmemc_ctx_wait_until_lt16(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_wait_until_lt32(...)                         \
    shmemc_ctx_wait_until_lt32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_wait_until_lt64(...)                         \
    shmemc_ctx_wait_until_lt64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#define shmemc_wait_until_ge16(...)                         \
    shmemc_ctx_wait_until_ge16(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_wait_until_ge32(...)                         \
    shmemc_ctx_wait_until_ge32(SHMEM_CTX_DEFAULT, __VA_ARGS__)
#define shmemc_wait_until_ge64(...)                         \
    shmemc_ctx_wait_until_ge64(SHMEM_CTX_DEFAULT, __VA_ARGS__)

#endif /* ! _SHMEMC_H */
