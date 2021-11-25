/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"
#include "module.h"
#include "shmemu.h"
#include "shmemc.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_short_test_any = pshmem_short_test_any
#define shmem_short_test_any pshmem_short_test_any
#pragma weak shmem_int_test_any = pshmem_int_test_any
#define shmem_int_test_any pshmem_int_test_any
#pragma weak shmem_long_test_any = pshmem_long_test_any
#define shmem_long_test_any pshmem_long_test_any
#pragma weak shmem_longlong_test_any = pshmem_longlong_test_any
#define shmem_longlong_test_any pshmem_longlong_test_any
#pragma weak shmem_ushort_test_any = pshmem_ushort_test_any
#define shmem_ushort_test_any pshmem_ushort_test_any
#pragma weak shmem_uint_test_any = pshmem_uint_test_any
#define shmem_uint_test_any pshmem_uint_test_any
#pragma weak shmem_ulong_test_any = pshmem_ulong_test_any
#define shmem_ulong_test_any pshmem_ulong_test_any
#pragma weak shmem_ulonglong_test_any = pshmem_ulonglong_test_any
#define shmem_ulonglong_test_any pshmem_ulonglong_test_any
#pragma weak shmem_int32_test_any = pshmem_int32_test_any
#define shmem_int32_test_any pshmem_int32_test_any
#pragma weak shmem_int64_test_any = pshmem_int64_test_any
#define shmem_int64_test_any pshmem_int64_test_any
#pragma weak shmem_uint32_test_any = pshmem_uint32_test_any
#define shmem_uint32_test_any pshmem_uint32_test_any
#pragma weak shmem_uint64_test_any = pshmem_uint64_test_any
#define shmem_uint64_test_any pshmem_uint64_test_any
#pragma weak shmem_size_test_any = pshmem_size_test_any
#define shmem_size_test_any pshmem_size_test_any
#pragma weak shmem_ptrdiff_test_any = pshmem_ptrdiff_test_any
#define shmem_ptrdiff_test_any pshmem_ptrdiff_test_any
#endif  /* ENABLE_PSHMEM */

#define SHMEM_TYPE_TEST_ANY(_opname, _type, _size)                      \
    size_t                                                              \
    shmem_##_opname##_test_any(_type *ivars, size_t nelems,             \
                               const int *status,                       \
                               int cmp, _type cmp_value)                \
    {                                                                   \
        SHMEMT_MUTEX_PROTECT                                            \
            (                                                           \
             switch (cmp) {                                             \
             case SHMEM_CMP_EQ:                                         \
                 return shmemc_ctx_test_any_eq##_size(SHMEM_CTX_DEFAULT, \
                                                      (int##_size##_t *) ivars, \
                                                      nelems, status,   \
                                                      cmp_value);       \
                 break;                                                 \
             case SHMEM_CMP_NE:                                         \
                 return shmemc_ctx_test_any_ne##_size(SHMEM_CTX_DEFAULT, \
                                                      (int##_size##_t *) ivars, \
                                                      nelems, status,   \
                                                      cmp_value);       \
                 break;                                                 \
             case SHMEM_CMP_GT:                                         \
                 return shmemc_ctx_test_any_gt##_size(SHMEM_CTX_DEFAULT, \
                                                      (int##_size##_t *) ivars, \
                                                      nelems, status,   \
                                                      cmp_value);       \
                 break;                                                 \
             case SHMEM_CMP_LE:                                         \
                 return shmemc_ctx_test_any_le##_size(SHMEM_CTX_DEFAULT, \
                                                      (int##_size##_t *) ivars, \
                                                      nelems, status,   \
                                                      cmp_value);       \
                 break;                                                 \
             case SHMEM_CMP_LT:                                         \
                 return shmemc_ctx_test_any_lt##_size(SHMEM_CTX_DEFAULT, \
                                                      (int##_size##_t *) ivars, \
                                                      nelems, status,   \
                                                      cmp_value);       \
                 break;                                                 \
             case SHMEM_CMP_GE:                                         \
                 return shmemc_ctx_test_any_ge##_size(SHMEM_CTX_DEFAULT, \
                                                      (int##_size##_t *) ivars, \
                                                      nelems, status,   \
                                                      cmp_value);       \
                 break;                                                 \
             default:                                                   \
                 shmemu_fatal(MODULE ": unknown operator (code %d) in \"%s\"", \
                              cmp,                                      \
                              __func__                                  \
                              );                                        \
                 return -1;                                             \
                 /* NOT REACHED */                                      \
                 break;                                                 \
             }                                                          \
                                                                        ); \
    }

SHMEM_TYPE_TEST_ANY(short, short, 16)
SHMEM_TYPE_TEST_ANY(int, int, 32)
SHMEM_TYPE_TEST_ANY(long, long, 64)
SHMEM_TYPE_TEST_ANY(longlong, long long, 64)
SHMEM_TYPE_TEST_ANY(ushort, unsigned short, 16)
SHMEM_TYPE_TEST_ANY(uint, unsigned int, 32)
SHMEM_TYPE_TEST_ANY(ulong, unsigned long, 64)
SHMEM_TYPE_TEST_ANY(ulonglong, unsigned long long, 64)
SHMEM_TYPE_TEST_ANY(int32, int32_t, 32)
SHMEM_TYPE_TEST_ANY(int64, int64_t, 64)
SHMEM_TYPE_TEST_ANY(uint32, uint32_t, 32)
SHMEM_TYPE_TEST_ANY(uint64, uint64_t, 64)
SHMEM_TYPE_TEST_ANY(size, size_t, 64)
SHMEM_TYPE_TEST_ANY(ptrdiff, ptrdiff_t, 64)
