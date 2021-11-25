/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"
#include "shmemu.h"
#include "shmemc.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_short_wait_until_any_vector = pshmem_short_wait_until_any_vector
#define shmem_short_wait_until_any_vector pshmem_short_wait_until_any_vector
#pragma weak shmem_int_wait_until_any_vector = pshmem_int_wait_until_any_vector
#define shmem_int_wait_until_any_vector pshmem_int_wait_until_any_vector
#pragma weak shmem_long_wait_until_any_vector = pshmem_long_wait_until_any_vector
#define shmem_long_wait_until_any_vector pshmem_long_wait_until_any_vector
#pragma weak shmem_longlong_wait_until_any_vector = pshmem_longlong_wait_until_any_vector
#define shmem_longlong_wait_until_any_vector pshmem_longlong_wait_until_any_vector
#pragma weak shmem_ushort_wait_until_any_vector = pshmem_ushort_wait_until_any_vector
#define shmem_ushort_wait_until_any_vector pshmem_ushort_wait_until_any_vector
#pragma weak shmem_uint_wait_until_any_vector = pshmem_uint_wait_until_any_vector
#define shmem_uint_wait_until_any_vector pshmem_uint_wait_until_any_vector
#pragma weak shmem_ulong_wait_until_any_vector = pshmem_ulong_wait_until_any_vector
#define shmem_ulong_wait_until_any_vector pshmem_ulong_wait_until_any_vector
#pragma weak shmem_ulonglong_wait_until_any_vector = pshmem_ulonglong_wait_until_any_vector
#define shmem_ulonglong_wait_until_any_vector pshmem_ulonglong_wait_until_any_vector
#pragma weak shmem_int32_wait_until_any_vector = pshmem_int32_wait_until_any_vector
#define shmem_int32_wait_until_any_vector pshmem_int32_wait_until_any_vector
#pragma weak shmem_int64_wait_until_any_vector = pshmem_int64_wait_until_any_vector
#define shmem_int64_wait_until_any_vector pshmem_int64_wait_until_any_vector
#pragma weak shmem_uint32_wait_until_any_vector = pshmem_uint32_wait_until_any_vector
#define shmem_uint32_wait_until_any_vector pshmem_uint32_wait_until_any_vector
#pragma weak shmem_uint64_wait_until_any_vector = pshmem_uint64_wait_until_any_vector
#define shmem_uint64_wait_until_any_vector pshmem_uint64_wait_until_any_vector
#pragma weak shmem_size_wait_until_any_vector = pshmem_size_wait_until_any_vector
#define shmem_size_wait_until_any_vector pshmem_size_wait_until_any_vector
#pragma weak shmem_ptrdiff_wait_until_any_vector = pshmem_ptrdiff_wait_until_any_vector
#define shmem_ptrdiff_wait_until_any_vector pshmem_ptrdiff_wait_until_any_vector
#endif  /* ENABLE_PSHMEM */

#define SHMEM_TYPE_WAIT_UNTIL_ANY_VECTOR(_opname, _type, _size)         \
    size_t                                                              \
    shmem_##_opname##_wait_until_any_vector(_type *ivars,               \
                                            size_t nelems,              \
                                            const int *status,          \
                                            int cmp, _type *cmp_values) \
    {                                                                   \
        SHMEMT_MUTEX_PROTECT                                            \
            (                                                           \
             switch (cmp) {                                             \
             case SHMEM_CMP_EQ:                                         \
                 return shmemc_ctx_wait_until_any_vector_eq##_size(SHMEM_CTX_DEFAULT, \
                                                                   (int##_size##_t *) ivars, \
                                                                   nelems, \
                                                                   status, \
                                                                   cmp_values); \
                 break;                                                 \
             case SHMEM_CMP_NE:                                         \
                 return shmemc_ctx_wait_until_any_vector_ne##_size(SHMEM_CTX_DEFAULT, \
                                                                   (int##_size##_t *) ivars, \
                                                                   nelems, \
                                                                   status, \
                                                                   cmp_values); \
                 break;                                                 \
             case SHMEM_CMP_GT:                                         \
                 return shmemc_ctx_wait_until_any_vector_gt##_size(SHMEM_CTX_DEFAULT, \
                                                                   (int##_size##_t *) ivars, \
                                                                   nelems, \
                                                                   status, \
                                                                   cmp_values); \
                 break;                                                 \
             case SHMEM_CMP_LE:                                         \
                 return shmemc_ctx_wait_until_any_vector_le##_size(SHMEM_CTX_DEFAULT, \
                                                                   (int##_size##_t *) ivars, \
                                                                   nelems, \
                                                                   status, \
                                                                   cmp_values); \
                 break;                                                 \
             case SHMEM_CMP_LT:                                         \
                 return shmemc_ctx_wait_until_any_vector_lt##_size(SHMEM_CTX_DEFAULT, \
                                                                   (int##_size##_t *) ivars, \
                                                                   nelems, \
                                                                   status, \
                                                                   cmp_values); \
                 break;                                                 \
             case SHMEM_CMP_GE:                                         \
                 return shmemc_ctx_wait_until_any_vector_ge##_size(SHMEM_CTX_DEFAULT, \
                                                                   (int##_size##_t *) ivars, \
                                                                   nelems, \
                                                                   status, \
                                                                   cmp_values); \
                 break;                                                 \
             default:                                                   \
                 shmemu_fatal("unknown operator (code %d) in \"%s\"",   \
                              cmp,                                      \
                              __func__                                  \
                              );                                        \
                 return -1;                                             \
                 /* NOT REACHED */                                      \
                 break;                                                 \
             }                                                          \
                                                                        ); \
    }

SHMEM_TYPE_WAIT_UNTIL_ANY_VECTOR(short, short, 16)
SHMEM_TYPE_WAIT_UNTIL_ANY_VECTOR(int, int, 32)
SHMEM_TYPE_WAIT_UNTIL_ANY_VECTOR(long, long, 64)
SHMEM_TYPE_WAIT_UNTIL_ANY_VECTOR(longlong, long long, 64)
SHMEM_TYPE_WAIT_UNTIL_ANY_VECTOR(ushort, unsigned short, 16)
SHMEM_TYPE_WAIT_UNTIL_ANY_VECTOR(uint, unsigned int, 32)
SHMEM_TYPE_WAIT_UNTIL_ANY_VECTOR(ulong, unsigned long, 64)
SHMEM_TYPE_WAIT_UNTIL_ANY_VECTOR(ulonglong, unsigned long long, 64)
SHMEM_TYPE_WAIT_UNTIL_ANY_VECTOR(int32, int32_t, 32)
SHMEM_TYPE_WAIT_UNTIL_ANY_VECTOR(int64, int64_t, 64)
SHMEM_TYPE_WAIT_UNTIL_ANY_VECTOR(uint32, uint32_t, 32)
SHMEM_TYPE_WAIT_UNTIL_ANY_VECTOR(uint64, uint64_t, 64)
SHMEM_TYPE_WAIT_UNTIL_ANY_VECTOR(size, size_t, 64)
SHMEM_TYPE_WAIT_UNTIL_ANY_VECTOR(ptrdiff, ptrdiff_t, 64)
