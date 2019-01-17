/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"

#include "shmemu.h"
#include "shmemc.h"
#include "shmem/defs.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmemx_short_test_all = pshmemx_short_test_all
#define shmemx_short_test_all pshmemx_short_test_all
#pragma weak shmemx_int_test_all = pshmemx_int_test_all
#define shmemx_int_test_all pshmemx_int_test_all
#pragma weak shmemx_long_test_all = pshmemx_long_test_all
#define shmemx_long_test_all pshmemx_long_test_all
#pragma weak shmemx_longlong_test_all = pshmemx_longlong_test_all
#define shmemx_longlong_test_all pshmemx_longlong_test_all
#pragma weak shmemx_ushort_test_all = pshmemx_ushort_test_all
#define shmemx_ushort_test_all pshmemx_ushort_test_all
#pragma weak shmemx_uint_test_all = pshmemx_uint_test_all
#define shmemx_uint_test_all pshmemx_uint_test_all
#pragma weak shmemx_ulong_test_all = pshmemx_ulong_test_all
#define shmemx_ulong_test_all pshmemx_ulong_test_all
#pragma weak shmemx_ulonglong_test_all = pshmemx_ulonglong_test_all
#define shmemx_ulonglong_test_all pshmemx_ulonglong_test_all
#pragma weak shmemx_int32_test_all = pshmemx_int32_test_all
#define shmemx_int32_test_all pshmemx_int32_test_all
#pragma weak shmemx_int64_test_all = pshmemx_int64_test_all
#define shmemx_int64_test_all pshmemx_int64_test_all
#pragma weak shmemx_uint32_test_all = pshmemx_uint32_test_all
#define shmemx_uint32_test_all pshmemx_uint32_test_all
#pragma weak shmemx_uint64_test_all = pshmemx_uint64_test_all
#define shmemx_uint64_test_all pshmemx_uint64_test_all
#pragma weak shmemx_size_test_all = pshmemx_size_test_all
#define shmemx_size_test_all pshmemx_size_test_all
#pragma weak shmemx_ptrdiff_test_all = pshmemx_ptrdiff_test_all
#define shmemx_ptrdiff_test_all pshmemx_ptrdiff_test_all
#endif  /* ENABLE_PSHMEM */

#define SHMEMX_TYPE_TEST_ALL(_opname, _type, _size)                     \
    int                                                                 \
    shmemx_##_opname##_test_all(_type *ivars, size_t nelems,            \
                                int cmp, _type cmp_value)               \
    {                                                                   \
        SHMEMT_MUTEX_PROTECT                                            \
            (                                                           \
             switch (cmp) {                                             \
             case SHMEM_CMP_EQ:                                         \
             return shmemc_ctx_test_all_eq##_size(SHMEM_CTX_DEFAULT,    \
                                                  (int##_size##_t *) ivars, \
                                                  nelems,               \
                                                  cmp_value);           \
             break;                                                     \
             case SHMEM_CMP_NE:                                         \
             return shmemc_ctx_test_all_ne##_size(SHMEM_CTX_DEFAULT,    \
                                                  (int##_size##_t *) ivars, \
                                                  nelems,               \
                                                  cmp_value);           \
             break;                                                     \
             case SHMEM_CMP_GT:                                         \
             return shmemc_ctx_test_all_gt##_size(SHMEM_CTX_DEFAULT,    \
                                                  (int##_size##_t *) ivars, \
                                                  nelems,               \
                                                  cmp_value);           \
             break;                                                     \
             case SHMEM_CMP_LE:                                         \
             return shmemc_ctx_test_all_le##_size(SHMEM_CTX_DEFAULT,    \
                                                  (int##_size##_t *) ivars, \
                                                  nelems,               \
                                                  cmp_value);           \
             break;                                                     \
             case SHMEM_CMP_LT:                                         \
             return shmemc_ctx_test_all_lt##_size(SHMEM_CTX_DEFAULT,    \
                                                  (int##_size##_t *) ivars, \
                                                  nelems,               \
                                                  cmp_value);           \
             break;                                                     \
             case SHMEM_CMP_GE:                                         \
             return shmemc_ctx_test_all_ge##_size(SHMEM_CTX_DEFAULT,    \
                                                  (int##_size##_t *) ivars, \
                                                  nelems,               \
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

SHMEMX_TYPE_TEST_ALL(short, short, 16)
    SHMEMX_TYPE_TEST_ALL(int, int, 32)
    SHMEMX_TYPE_TEST_ALL(long, long, 64)
    SHMEMX_TYPE_TEST_ALL(longlong, long long, 64)
    SHMEMX_TYPE_TEST_ALL(ushort, unsigned short, 16)
    SHMEMX_TYPE_TEST_ALL(uint, unsigned int, 32)
    SHMEMX_TYPE_TEST_ALL(ulong, unsigned long, 64)
    SHMEMX_TYPE_TEST_ALL(ulonglong, unsigned long long, 64)
    SHMEMX_TYPE_TEST_ALL(int32, int32_t, 32)
    SHMEMX_TYPE_TEST_ALL(int64, int64_t, 64)
    SHMEMX_TYPE_TEST_ALL(uint32, uint32_t, 32)
    SHMEMX_TYPE_TEST_ALL(uint64, uint64_t, 64)
    SHMEMX_TYPE_TEST_ALL(size, size_t, 64)
    SHMEMX_TYPE_TEST_ALL(ptrdiff, ptrdiff_t, 64)
