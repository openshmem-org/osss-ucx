/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"
#include "module.h"
#include "shmemu.h"
#include "shmemc.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_short_test = pshmem_short_test
#define shmem_short_test pshmem_short_test
#pragma weak shmem_int_test = pshmem_int_test
#define shmem_int_test pshmem_int_test
#pragma weak shmem_long_test = pshmem_long_test
#define shmem_long_test pshmem_long_test
#pragma weak shmem_longlong_test = pshmem_longlong_test
#define shmem_longlong_test pshmem_longlong_test
#pragma weak shmem_ushort_test = pshmem_ushort_test
#define shmem_ushort_test pshmem_ushort_test
#pragma weak shmem_uint_test = pshmem_uint_test
#define shmem_uint_test pshmem_uint_test
#pragma weak shmem_ulong_test = pshmem_ulong_test
#define shmem_ulong_test pshmem_ulong_test
#pragma weak shmem_ulonglong_test = pshmem_ulonglong_test
#define shmem_ulonglong_test pshmem_ulonglong_test
#pragma weak shmem_int32_test = pshmem_int32_test
#define shmem_int32_test pshmem_int32_test
#pragma weak shmem_int64_test = pshmem_int64_test
#define shmem_int64_test pshmem_int64_test
#pragma weak shmem_uint32_test = pshmem_uint32_test
#define shmem_uint32_test pshmem_uint32_test
#pragma weak shmem_uint64_test = pshmem_uint64_test
#define shmem_uint64_test pshmem_uint64_test
#pragma weak shmem_size_test = pshmem_size_test
#define shmem_size_test pshmem_size_test
#pragma weak shmem_ptrdiff_test = pshmem_ptrdiff_test
#define shmem_ptrdiff_test pshmem_ptrdiff_test
#endif  /* ENABLE_PSHMEM */

#define SHMEM_TYPE_TEST(_opname, _type, _size)                          \
    int                                                                 \
    shmem_##_opname##_test(_type *ivar, int cmp, _type cmp_value)       \
    {                                                                   \
        SHMEMT_MUTEX_NOPROTECT                                          \
            (                                                           \
             switch (cmp) {                                             \
             case SHMEM_CMP_EQ:                                         \
                 return shmemc_ctx_test_eq##_size(SHMEM_CTX_DEFAULT,    \
                                                  (int##_size##_t *) ivar, \
                                                  cmp_value);           \
                 break;                                                 \
             case SHMEM_CMP_NE:                                         \
                 return shmemc_ctx_test_ne##_size(SHMEM_CTX_DEFAULT,    \
                                                  (int##_size##_t *) ivar, \
                                                  cmp_value);           \
                 break;                                                 \
             case SHMEM_CMP_GT:                                         \
                 return shmemc_ctx_test_gt##_size(SHMEM_CTX_DEFAULT,    \
                                                  (int##_size##_t *) ivar, \
                                                  cmp_value);           \
                 break;                                                 \
             case SHMEM_CMP_LE:                                         \
                 return shmemc_ctx_test_le##_size(SHMEM_CTX_DEFAULT,    \
                                                  (int##_size##_t *) ivar, \
                                                  cmp_value);           \
                 break;                                                 \
             case SHMEM_CMP_LT:                                         \
                 return shmemc_ctx_test_lt##_size(SHMEM_CTX_DEFAULT,    \
                                                  (int##_size##_t *) ivar, \
                                                  cmp_value);           \
                 break;                                                 \
             case SHMEM_CMP_GE:                                         \
                 return shmemc_ctx_test_ge##_size(SHMEM_CTX_DEFAULT,    \
                                                  (int##_size##_t *) ivar, \
                                                  cmp_value);           \
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

SHMEM_TYPE_TEST(short, short, 16)
SHMEM_TYPE_TEST(int, int, 32)
SHMEM_TYPE_TEST(long, long, 64)
SHMEM_TYPE_TEST(longlong, long long, 64)
SHMEM_TYPE_TEST(ushort, unsigned short, 16)
SHMEM_TYPE_TEST(uint, unsigned int, 32)
SHMEM_TYPE_TEST(ulong, unsigned long, 64)
SHMEM_TYPE_TEST(ulonglong, unsigned long long, 64)
SHMEM_TYPE_TEST(int32, int32_t, 32)
SHMEM_TYPE_TEST(int64, int64_t, 64)
SHMEM_TYPE_TEST(uint32, uint32_t, 32)
SHMEM_TYPE_TEST(uint64, uint64_t, 64)
SHMEM_TYPE_TEST(size, size_t, 64)
SHMEM_TYPE_TEST(ptrdiff, ptrdiff_t, 64)
