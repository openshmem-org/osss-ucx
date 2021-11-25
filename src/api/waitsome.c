/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"
#include "shmemu.h"
#include "shmemc.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_short_wait_until_some = pshmem_short_wait_until_some
#define shmem_short_wait_until_some pshmem_short_wait_until_some
#pragma weak shmem_int_wait_until_some = pshmem_int_wait_until_some
#define shmem_int_wait_until_some pshmem_int_wait_until_some
#pragma weak shmem_long_wait_until_some = pshmem_long_wait_until_some
#define shmem_long_wait_until_some pshmem_long_wait_until_some
#pragma weak shmem_longlong_wait_until_some = pshmem_longlong_wait_until_some
#define shmem_longlong_wait_until_some pshmem_longlong_wait_until_some
#pragma weak shmem_ushort_wait_until_some = pshmem_ushort_wait_until_some
#define shmem_ushort_wait_until_some pshmem_ushort_wait_until_some
#pragma weak shmem_uint_wait_until_some = pshmem_uint_wait_until_some
#define shmem_uint_wait_until_some pshmem_uint_wait_until_some
#pragma weak shmem_ulong_wait_until_some = pshmem_ulong_wait_until_some
#define shmem_ulong_wait_until_some pshmem_ulong_wait_until_some
#pragma weak shmem_ulonglong_wait_until_some = pshmem_ulonglong_wait_until_some
#define shmem_ulonglong_wait_until_some pshmem_ulonglong_wait_until_some
#pragma weak shmem_int32_wait_until_some = pshmem_int32_wait_until_some
#define shmem_int32_wait_until_some pshmem_int32_wait_until_some
#pragma weak shmem_int64_wait_until_some = pshmem_int64_wait_until_some
#define shmem_int64_wait_until_some pshmem_int64_wait_until_some
#pragma weak shmem_uint32_wait_until_some = pshmem_uint32_wait_until_some
#define shmem_uint32_wait_until_some pshmem_uint32_wait_until_some
#pragma weak shmem_uint64_wait_until_some = pshmem_uint64_wait_until_some
#define shmem_uint64_wait_until_some pshmem_uint64_wait_until_some
#pragma weak shmem_size_wait_until_some = pshmem_size_wait_until_some
#define shmem_size_wait_until_some pshmem_size_wait_until_some
#pragma weak shmem_ptrdiff_wait_until_some = pshmem_ptrdiff_wait_until_some
#define shmem_ptrdiff_wait_until_some pshmem_ptrdiff_wait_until_some
#endif  /* ENABLE_PSHMEM */

#define SHMEM_TYPE_WAIT_UNTIL_SOME(_opname, _type, _size)               \
    size_t                                                              \
    shmem_##_opname##_wait_until_some(_type *ivars, size_t nelems,      \
                                      size_t *idxs,                     \
                                      const int *status,                \
                                      int cmp, _type cmp_value)         \
    {                                                                   \
        SHMEMT_MUTEX_PROTECT                                            \
            (                                                           \
             switch (cmp) {                                             \
             case SHMEM_CMP_EQ:                                         \
             return shmemc_ctx_wait_until_some_eq##_size(SHMEM_CTX_DEFAULT, \
                                                         (int##_size##_t *) ivars, \
                                                         nelems,        \
                                                         idxs,          \
                                                         status,        \
                                                         cmp_value);    \
             break;                                                     \
             case SHMEM_CMP_NE:                                         \
             return shmemc_ctx_wait_until_some_ne##_size(SHMEM_CTX_DEFAULT, \
                                                         (int##_size##_t *) ivars, \
                                                         nelems,        \
                                                         idxs,          \
                                                         status,        \
                                                         cmp_value);    \
             break;                                                     \
             case SHMEM_CMP_GT:                                         \
             return shmemc_ctx_wait_until_some_gt##_size(SHMEM_CTX_DEFAULT, \
                                                         (int##_size##_t *) ivars, \
                                                         nelems,        \
                                                         idxs,          \
                                                         status,        \
                                                         cmp_value);    \
             break;                                                     \
             case SHMEM_CMP_LE:                                         \
             return shmemc_ctx_wait_until_some_le##_size(SHMEM_CTX_DEFAULT, \
                                                         (int##_size##_t *) ivars, \
                                                         nelems,        \
                                                         idxs,          \
                                                         status,        \
                                                         cmp_value);    \
             break;                                                     \
             case SHMEM_CMP_LT:                                         \
             return shmemc_ctx_wait_until_some_lt##_size(SHMEM_CTX_DEFAULT, \
                                                         (int##_size##_t *) ivars, \
                                                         nelems,        \
                                                         idxs,          \
                                                         status,        \
                                                         cmp_value);    \
             break;                                                     \
             case SHMEM_CMP_GE:                                         \
             return shmemc_ctx_wait_until_some_ge##_size(SHMEM_CTX_DEFAULT, \
                                                         (int##_size##_t *) ivars, \
                                                         nelems,        \
                                                         idxs,          \
                                                         status,        \
                                                         cmp_value);    \
             break;                                                     \
             default:                                                   \
             shmemu_fatal("unknown operator (code %d) in \"%s\"",       \
                          cmp,                                          \
                          __func__                                      \
                          );                                            \
             return -1;                                                 \
             /* NOT REACHED */                                          \
             break;                                                     \
             }                                                          \
                                                                        ); \
    }

SHMEM_TYPE_WAIT_UNTIL_SOME(short, short, 16)
SHMEM_TYPE_WAIT_UNTIL_SOME(int, int, 32)
SHMEM_TYPE_WAIT_UNTIL_SOME(long, long, 64)
SHMEM_TYPE_WAIT_UNTIL_SOME(longlong, long long, 64)
SHMEM_TYPE_WAIT_UNTIL_SOME(ushort, unsigned short, 16)
SHMEM_TYPE_WAIT_UNTIL_SOME(uint, unsigned int, 32)
SHMEM_TYPE_WAIT_UNTIL_SOME(ulong, unsigned long, 64)
SHMEM_TYPE_WAIT_UNTIL_SOME(ulonglong, unsigned long long, 64)
SHMEM_TYPE_WAIT_UNTIL_SOME(int32, int32_t, 32)
SHMEM_TYPE_WAIT_UNTIL_SOME(int64, int64_t, 64)
SHMEM_TYPE_WAIT_UNTIL_SOME(uint32, uint32_t, 32)
SHMEM_TYPE_WAIT_UNTIL_SOME(uint64, uint64_t, 64)
SHMEM_TYPE_WAIT_UNTIL_SOME(size, size_t, 64)
SHMEM_TYPE_WAIT_UNTIL_SOME(ptrdiff, ptrdiff_t, 64)
