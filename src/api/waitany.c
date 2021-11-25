/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"
#include "shmemu.h"
#include "shmemc.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_short_wait_until_any = pshmem_short_wait_until_any
#define shmem_short_wait_until_any pshmem_short_wait_until_any
#pragma weak shmem_int_wait_until_any = pshmem_int_wait_until_any
#define shmem_int_wait_until_any pshmem_int_wait_until_any
#pragma weak shmem_long_wait_until_any = pshmem_long_wait_until_any
#define shmem_long_wait_until_any pshmem_long_wait_until_any
#pragma weak shmem_longlong_wait_until_any = pshmem_longlong_wait_until_any
#define shmem_longlong_wait_until_any pshmem_longlong_wait_until_any
#pragma weak shmem_ushort_wait_until_any = pshmem_ushort_wait_until_any
#define shmem_ushort_wait_until_any pshmem_ushort_wait_until_any
#pragma weak shmem_uint_wait_until_any = pshmem_uint_wait_until_any
#define shmem_uint_wait_until_any pshmem_uint_wait_until_any
#pragma weak shmem_ulong_wait_until_any = pshmem_ulong_wait_until_any
#define shmem_ulong_wait_until_any pshmem_ulong_wait_until_any
#pragma weak shmem_ulonglong_wait_until_any = pshmem_ulonglong_wait_until_any
#define shmem_ulonglong_wait_until_any pshmem_ulonglong_wait_until_any
#pragma weak shmem_int32_wait_until_any = pshmem_int32_wait_until_any
#define shmem_int32_wait_until_any pshmem_int32_wait_until_any
#pragma weak shmem_int64_wait_until_any = pshmem_int64_wait_until_any
#define shmem_int64_wait_until_any pshmem_int64_wait_until_any
#pragma weak shmem_uint32_wait_until_any = pshmem_uint32_wait_until_any
#define shmem_uint32_wait_until_any pshmem_uint32_wait_until_any
#pragma weak shmem_uint64_wait_until_any = pshmem_uint64_wait_until_any
#define shmem_uint64_wait_until_any pshmem_uint64_wait_until_any
#pragma weak shmem_size_wait_until_any = pshmem_size_wait_until_any
#define shmem_size_wait_until_any pshmem_size_wait_until_any
#pragma weak shmem_ptrdiff_wait_until_any = pshmem_ptrdiff_wait_until_any
#define shmem_ptrdiff_wait_until_any pshmem_ptrdiff_wait_until_any
#endif  /* ENABLE_PSHMEM */

#define SHMEM_TYPE_WAIT_UNTIL_ANY(_opname, _type, _size)                \
    size_t                                                              \
    shmem_##_opname##_wait_until_any(_type *ivars,                      \
                                     size_t nelems,                     \
                                     const int *status,                 \
                                     int cmp, _type cmp_value)          \
    {                                                                   \
        SHMEMT_MUTEX_PROTECT                                            \
            (                                                           \
             switch (cmp) {                                             \
             case SHMEM_CMP_EQ:                                         \
             return shmemc_ctx_wait_until_any_eq##_size(SHMEM_CTX_DEFAULT, \
                                                        (int##_size##_t *) ivars, \
                                                        nelems,         \
                                                        status,         \
                                                        cmp_value);     \
             break;                                                     \
             case SHMEM_CMP_NE:                                         \
             return shmemc_ctx_wait_until_any_ne##_size(SHMEM_CTX_DEFAULT, \
                                                        (int##_size##_t *) ivars, \
                                                        nelems,         \
                                                        status,         \
                                                        cmp_value);     \
             break;                                                     \
             case SHMEM_CMP_GT:                                         \
             return shmemc_ctx_wait_until_any_gt##_size(SHMEM_CTX_DEFAULT, \
                                                        (int##_size##_t *) ivars, \
                                                        nelems,         \
                                                        status,         \
                                                        cmp_value);     \
             break;                                                     \
             case SHMEM_CMP_LE:                                         \
             return shmemc_ctx_wait_until_any_le##_size(SHMEM_CTX_DEFAULT, \
                                                        (int##_size##_t *) ivars, \
                                                        nelems,         \
                                                        status,         \
                                                        cmp_value);     \
             break;                                                     \
             case SHMEM_CMP_LT:                                         \
             return shmemc_ctx_wait_until_any_lt##_size(SHMEM_CTX_DEFAULT, \
                                                        (int##_size##_t *) ivars, \
                                                        nelems,         \
                                                        status,         \
                                                        cmp_value);     \
             break;                                                     \
             case SHMEM_CMP_GE:                                         \
             return shmemc_ctx_wait_until_any_ge##_size(SHMEM_CTX_DEFAULT, \
                                                        (int##_size##_t *) ivars, \
                                                        nelems,         \
                                                        status,         \
                                                        cmp_value);     \
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

SHMEM_TYPE_WAIT_UNTIL_ANY(short, short, 16)
SHMEM_TYPE_WAIT_UNTIL_ANY(int, int, 32)
SHMEM_TYPE_WAIT_UNTIL_ANY(long, long, 64)
SHMEM_TYPE_WAIT_UNTIL_ANY(longlong, long long, 64)
SHMEM_TYPE_WAIT_UNTIL_ANY(ushort, unsigned short, 16)
SHMEM_TYPE_WAIT_UNTIL_ANY(uint, unsigned int, 32)
SHMEM_TYPE_WAIT_UNTIL_ANY(ulong, unsigned long, 64)
SHMEM_TYPE_WAIT_UNTIL_ANY(ulonglong, unsigned long long, 64)
SHMEM_TYPE_WAIT_UNTIL_ANY(int32, int32_t, 32)
SHMEM_TYPE_WAIT_UNTIL_ANY(int64, int64_t, 64)
SHMEM_TYPE_WAIT_UNTIL_ANY(uint32, uint32_t, 32)
SHMEM_TYPE_WAIT_UNTIL_ANY(uint64, uint64_t, 64)
SHMEM_TYPE_WAIT_UNTIL_ANY(size, size_t, 64)
SHMEM_TYPE_WAIT_UNTIL_ANY(ptrdiff, ptrdiff_t, 64)
