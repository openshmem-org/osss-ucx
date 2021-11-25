/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"
#include "module.h"
#include "shmemu.h"
#include "shmemc.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_short_test_all = pshmem_short_test_all
#define shmem_short_test_all pshmem_short_test_all
#pragma weak shmem_int_test_all = pshmem_int_test_all
#define shmem_int_test_all pshmem_int_test_all
#pragma weak shmem_long_test_all = pshmem_long_test_all
#define shmem_long_test_all pshmem_long_test_all
#pragma weak shmem_longlong_test_all = pshmem_longlong_test_all
#define shmem_longlong_test_all pshmem_longlong_test_all
#pragma weak shmem_ushort_test_all = pshmem_ushort_test_all
#define shmem_ushort_test_all pshmem_ushort_test_all
#pragma weak shmem_uint_test_all = pshmem_uint_test_all
#define shmem_uint_test_all pshmem_uint_test_all
#pragma weak shmem_ulong_test_all = pshmem_ulong_test_all
#define shmem_ulong_test_all pshmem_ulong_test_all
#pragma weak shmem_ulonglong_test_all = pshmem_ulonglong_test_all
#define shmem_ulonglong_test_all pshmem_ulonglong_test_all
#pragma weak shmem_int32_test_all = pshmem_int32_test_all
#define shmem_int32_test_all pshmem_int32_test_all
#pragma weak shmem_int64_test_all = pshmem_int64_test_all
#define shmem_int64_test_all pshmem_int64_test_all
#pragma weak shmem_uint32_test_all = pshmem_uint32_test_all
#define shmem_uint32_test_all pshmem_uint32_test_all
#pragma weak shmem_uint64_test_all = pshmem_uint64_test_all
#define shmem_uint64_test_all pshmem_uint64_test_all
#pragma weak shmem_size_test_all = pshmem_size_test_all
#define shmem_size_test_all pshmem_size_test_all
#pragma weak shmem_ptrdiff_test_all = pshmem_ptrdiff_test_all
#define shmem_ptrdiff_test_all pshmem_ptrdiff_test_all
#endif  /* ENABLE_PSHMEM */

#define SHMEM_TYPE_TEST_ALL(_opname, _type, _size)                      \
    int                                                                 \
    shmem_##_opname##_test_all(_type *ivars, size_t nelems,             \
                               const int *status,                       \
                               int cmp, _type cmp_value)                \
    {                                                                   \
        SHMEMT_MUTEX_PROTECT                                            \
            (                                                           \
             switch (cmp) {                                             \
             case SHMEM_CMP_EQ:                                         \
                 return shmemc_ctx_test_all_eq##_size(SHMEM_CTX_DEFAULT, \
                                                      (int##_size##_t *) ivars, \
                                                      nelems,           \
                                                      status,           \
                                                      cmp_value);       \
                 break;                                                 \
             case SHMEM_CMP_NE:                                         \
                 return shmemc_ctx_test_all_ne##_size(SHMEM_CTX_DEFAULT, \
                                                      (int##_size##_t *) ivars, \
                                                      nelems,           \
                                                      status,           \
                                                      cmp_value);       \
                 break;                                                 \
             case SHMEM_CMP_GT:                                         \
                 return shmemc_ctx_test_all_gt##_size(SHMEM_CTX_DEFAULT, \
                                                      (int##_size##_t *) ivars, \
                                                      nelems,           \
                                                      status,           \
                                                      cmp_value);       \
                 break;                                                 \
             case SHMEM_CMP_LE:                                         \
                 return shmemc_ctx_test_all_le##_size(SHMEM_CTX_DEFAULT, \
                                                      (int##_size##_t *) ivars, \
                                                      nelems,           \
                                                      status,           \
                                                      cmp_value);       \
                 break;                                                 \
             case SHMEM_CMP_LT:                                         \
                 return shmemc_ctx_test_all_lt##_size(SHMEM_CTX_DEFAULT, \
                                                      (int##_size##_t *) ivars, \
                                                      nelems,           \
                                                      status,           \
                                                      cmp_value);       \
                 break;                                                 \
             case SHMEM_CMP_GE:                                         \
                 return shmemc_ctx_test_all_ge##_size(SHMEM_CTX_DEFAULT, \
                                                      (int##_size##_t *) ivars, \
                                                      nelems,           \
                                                      status,           \
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

SHMEM_TYPE_TEST_ALL(short, short, 16)
SHMEM_TYPE_TEST_ALL(int, int, 32)
SHMEM_TYPE_TEST_ALL(long, long, 64)
SHMEM_TYPE_TEST_ALL(longlong, long long, 64)
SHMEM_TYPE_TEST_ALL(ushort, unsigned short, 16)
SHMEM_TYPE_TEST_ALL(uint, unsigned int, 32)
SHMEM_TYPE_TEST_ALL(ulong, unsigned long, 64)
SHMEM_TYPE_TEST_ALL(ulonglong, unsigned long long, 64)
SHMEM_TYPE_TEST_ALL(int32, int32_t, 32)
SHMEM_TYPE_TEST_ALL(int64, int64_t, 64)
SHMEM_TYPE_TEST_ALL(uint32, uint32_t, 32)
SHMEM_TYPE_TEST_ALL(uint64, uint64_t, 64)
SHMEM_TYPE_TEST_ALL(size, size_t, 64)
SHMEM_TYPE_TEST_ALL(ptrdiff, ptrdiff_t, 64)
