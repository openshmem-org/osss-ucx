/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"

#include "shmemu.h"
#include "shmemc.h"
#include "shmem/defs.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmemx_short_test_any = pshmemx_short_test_any
#define shmemx_short_test_any pshmemx_short_test_any
#pragma weak shmemx_int_test_any = pshmemx_int_test_any
#define shmemx_int_test_any pshmemx_int_test_any
#pragma weak shmemx_long_test_any = pshmemx_long_test_any
#define shmemx_long_test_any pshmemx_long_test_any
#pragma weak shmemx_longlong_test_any = pshmemx_longlong_test_any
#define shmemx_longlong_test_any pshmemx_longlong_test_any
#pragma weak shmemx_ushort_test_any = pshmemx_ushort_test_any
#define shmemx_ushort_test_any pshmemx_ushort_test_any
#pragma weak shmemx_uint_test_any = pshmemx_uint_test_any
#define shmemx_uint_test_any pshmemx_uint_test_any
#pragma weak shmemx_ulong_test_any = pshmemx_ulong_test_any
#define shmemx_ulong_test_any pshmemx_ulong_test_any
#pragma weak shmemx_ulonglong_test_any = pshmemx_ulonglong_test_any
#define shmemx_ulonglong_test_any pshmemx_ulonglong_test_any
#pragma weak shmemx_int32_test_any = pshmemx_int32_test_any
#define shmemx_int32_test_any pshmemx_int32_test_any
#pragma weak shmemx_int64_test_any = pshmemx_int64_test_any
#define shmemx_int64_test_any pshmemx_int64_test_any
#pragma weak shmemx_uint32_test_any = pshmemx_uint32_test_any
#define shmemx_uint32_test_any pshmemx_uint32_test_any
#pragma weak shmemx_uint64_test_any = pshmemx_uint64_test_any
#define shmemx_uint64_test_any pshmemx_uint64_test_any
#pragma weak shmemx_size_test_any = pshmemx_size_test_any
#define shmemx_size_test_any pshmemx_size_test_any
#pragma weak shmemx_ptrdiff_test_any = pshmemx_ptrdiff_test_any
#define shmemx_ptrdiff_test_any pshmemx_ptrdiff_test_any
#endif  /* ENABLE_PSHMEM */

#define SHMEMX_TYPE_TEST_ANY(_opname, _type, _size)                     \
    size_t                                                              \
    shmemx_##_opname##_test_any(_type *ivars, size_t nelems,            \
                                int * restrict status,                  \
                                int cmp, _type cmp_value)               \
    {                                                                   \
        SHMEMT_MUTEX_PROTECT                                            \
            (                                                           \
             switch (cmp) {                                             \
             case SHMEM_CMP_EQ:                                         \
             return shmemc_ctx_test_any_eq##_size(SHMEM_CTX_DEFAULT,    \
                                                  (int##_size##_t *) ivars, \
                                                  nelems, status,       \
                                                  cmp_value);           \
             break;                                                     \
             case SHMEM_CMP_NE:                                         \
             return shmemc_ctx_test_any_ne##_size(SHMEM_CTX_DEFAULT,    \
                                                  (int##_size##_t *) ivars, \
                                                  nelems, status,       \
                                                  cmp_value);           \
             break;                                                     \
             case SHMEM_CMP_GT:                                         \
             return shmemc_ctx_test_any_gt##_size(SHMEM_CTX_DEFAULT,    \
                                                  (int##_size##_t *) ivars, \
                                                  nelems, status,       \
                                                  cmp_value);           \
             break;                                                     \
             case SHMEM_CMP_LE:                                         \
             return shmemc_ctx_test_any_le##_size(SHMEM_CTX_DEFAULT,    \
                                                  (int##_size##_t *) ivars, \
                                                  nelems, status,       \
                                                  cmp_value);           \
             break;                                                     \
             case SHMEM_CMP_LT:                                         \
             return shmemc_ctx_test_any_lt##_size(SHMEM_CTX_DEFAULT,    \
                                                  (int##_size##_t *) ivars, \
                                                  nelems, status,       \
                                                  cmp_value);           \
             break;                                                     \
             case SHMEM_CMP_GE:                                         \
             return shmemc_ctx_test_any_ge##_size(SHMEM_CTX_DEFAULT,    \
                                                  (int##_size##_t *) ivars, \
                                                  nelems, status,       \
                                                  cmp_value);           \
             break;                                                     \
             default:                                                   \
             logger(LOG_FATAL,                                          \
                    "unknown operator (code %d) in \"%s\"",             \
                    cmp,                                                \
                    __func__                                            \
                    );                                                  \
             return -1;                                                 \
             /* NOT REACHED */                                          \
             break;                                                     \
             }                                                          \
                                                                        ); \
    }

SHMEMX_TYPE_TEST_ANY(short, short, 16)
SHMEMX_TYPE_TEST_ANY(int, int, 32)
SHMEMX_TYPE_TEST_ANY(long, long, 64)
SHMEMX_TYPE_TEST_ANY(longlong, long long, 64)
SHMEMX_TYPE_TEST_ANY(ushort, unsigned short, 16)
SHMEMX_TYPE_TEST_ANY(uint, unsigned int, 32)
SHMEMX_TYPE_TEST_ANY(ulong, unsigned long, 64)
SHMEMX_TYPE_TEST_ANY(ulonglong, unsigned long long, 64)
SHMEMX_TYPE_TEST_ANY(int32, int32_t, 32)
SHMEMX_TYPE_TEST_ANY(int64, int64_t, 64)
SHMEMX_TYPE_TEST_ANY(uint32, uint32_t, 32)
SHMEMX_TYPE_TEST_ANY(uint64, uint64_t, 64)
SHMEMX_TYPE_TEST_ANY(size, size_t, 64)
SHMEMX_TYPE_TEST_ANY(ptrdiff, ptrdiff_t, 64)
