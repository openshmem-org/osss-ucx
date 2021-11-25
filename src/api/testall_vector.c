/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"
#include "module.h"
#include "shmemu.h"
#include "shmemc.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_short_test_all_vector = pshmem_short_test_all_vector
#define shmem_short_test_all_vector pshmem_short_test_all_vector
#pragma weak shmem_int_test_all_vector = pshmem_int_test_all_vector
#define shmem_int_test_all_vector pshmem_int_test_all_vector
#pragma weak shmem_long_test_all_vector = pshmem_long_test_all_vector
#define shmem_long_test_all_vector pshmem_long_test_all_vector
#pragma weak shmem_longlong_test_all_vector = pshmem_longlong_test_all_vector
#define shmem_longlong_test_all_vector pshmem_longlong_test_all_vector
#pragma weak shmem_ushort_test_all_vector = pshmem_ushort_test_all_vector
#define shmem_ushort_test_all_vector pshmem_ushort_test_all_vector
#pragma weak shmem_uint_test_all_vector = pshmem_uint_test_all_vector
#define shmem_uint_test_all_vector pshmem_uint_test_all_vector
#pragma weak shmem_ulong_test_all_vector = pshmem_ulong_test_all_vector
#define shmem_ulong_test_all_vector pshmem_ulong_test_all_vector
#pragma weak shmem_ulonglong_test_all_vector = pshmem_ulonglong_test_all_vector
#define shmem_ulonglong_test_all_vector pshmem_ulonglong_test_all_vector
#pragma weak shmem_int32_test_all_vector = pshmem_int32_test_all_vector
#define shmem_int32_test_all_vector pshmem_int32_test_all_vector
#pragma weak shmem_int64_test_all_vector = pshmem_int64_test_all_vector
#define shmem_int64_test_all_vector pshmem_int64_test_all_vector
#pragma weak shmem_uint32_test_all_vector = pshmem_uint32_test_all_vector
#define shmem_uint32_test_all_vector pshmem_uint32_test_all_vector
#pragma weak shmem_uint64_test_all_vector = pshmem_uint64_test_all_vector
#define shmem_uint64_test_all_vector pshmem_uint64_test_all_vector
#pragma weak shmem_size_test_all_vector = pshmem_size_test_all_vector
#define shmem_size_test_all_vector pshmem_size_test_all_vector
#pragma weak shmem_ptrdiff_test_all_vector = pshmem_ptrdiff_test_all_vector
#define shmem_ptrdiff_test_all_vector pshmem_ptrdiff_test_all_vector
#endif  /* ENABLE_PSHMEM */

#define SHMEM_TYPE_TEST_ALL_VECTOR(_opname, _type, _size)               \
    int                                                                 \
    shmem_##_opname##_test_all_vector(_type *ivars, size_t nelems,      \
                                      const int *status,                \
                                      int cmp, _type *cmp_values)       \
    {                                                                   \
        SHMEMT_MUTEX_PROTECT                                            \
            (                                                           \
             switch (cmp) {                                             \
             case SHMEM_CMP_EQ:                                         \
                 return shmemc_ctx_test_all_vector_eq##_size(SHMEM_CTX_DEFAULT, \
                                                             (int##_size##_t *) ivars, \
                                                             nelems,    \
                                                             status,    \
                                                             cmp_values); \
                 break;                                                 \
             case SHMEM_CMP_NE:                                         \
                 return shmemc_ctx_test_all_vector_ne##_size(SHMEM_CTX_DEFAULT, \
                                                             (int##_size##_t *) ivars, \
                                                             nelems,    \
                                                             status,    \
                                                             cmp_values); \
                 break;                                                 \
             case SHMEM_CMP_GT:                                         \
                 return shmemc_ctx_test_all_vector_gt##_size(SHMEM_CTX_DEFAULT, \
                                                             (int##_size##_t *) ivars, \
                                                             nelems,    \
                                                             status,    \
                                                             cmp_values); \
                 break;                                                 \
             case SHMEM_CMP_LE:                                         \
                 return shmemc_ctx_test_all_vector_le##_size(SHMEM_CTX_DEFAULT, \
                                                             (int##_size##_t *) ivars, \
                                                             nelems,    \
                                                             status,    \
                                                             cmp_values); \
                 break;                                                 \
             case SHMEM_CMP_LT:                                         \
                 return shmemc_ctx_test_all_vector_lt##_size(SHMEM_CTX_DEFAULT, \
                                                             (int##_size##_t *) ivars, \
                                                             nelems,    \
                                                             status,    \
                                                             cmp_values); \
                 break;                                                 \
             case SHMEM_CMP_GE:                                         \
                 return shmemc_ctx_test_all_vector_ge##_size(SHMEM_CTX_DEFAULT, \
                                                             (int##_size##_t *) ivars, \
                                                             nelems,    \
                                                             status,    \
                                                             cmp_values); \
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

SHMEM_TYPE_TEST_ALL_VECTOR(short, short, 16)
SHMEM_TYPE_TEST_ALL_VECTOR(int, int, 32)
SHMEM_TYPE_TEST_ALL_VECTOR(long, long, 64)
SHMEM_TYPE_TEST_ALL_VECTOR(longlong, long long, 64)
SHMEM_TYPE_TEST_ALL_VECTOR(ushort, unsigned short, 16)
SHMEM_TYPE_TEST_ALL_VECTOR(uint, unsigned int, 32)
SHMEM_TYPE_TEST_ALL_VECTOR(ulong, unsigned long, 64)
SHMEM_TYPE_TEST_ALL_VECTOR(ulonglong, unsigned long long, 64)
SHMEM_TYPE_TEST_ALL_VECTOR(int32, int32_t, 32)
SHMEM_TYPE_TEST_ALL_VECTOR(int64, int64_t, 64)
SHMEM_TYPE_TEST_ALL_VECTOR(uint32, uint32_t, 32)
SHMEM_TYPE_TEST_ALL_VECTOR(uint64, uint64_t, 64)
SHMEM_TYPE_TEST_ALL_VECTOR(size, size_t, 64)
SHMEM_TYPE_TEST_ALL_VECTOR(ptrdiff, ptrdiff_t, 64)
