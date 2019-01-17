/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"

#include "shmemu.h"
#include "shmemc.h"
#include "shmem/defs.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmemx_short_wait_until_any = pshmemx_short_wait_until_any
#define shmemx_short_wait_until_any pshmemx_short_wait_until_any
#pragma weak shmemx_int_wait_until_any = pshmemx_int_wait_until_any
#define shmemx_int_wait_until_any pshmemx_int_wait_until_any
#pragma weak shmemx_long_wait_until_any = pshmemx_long_wait_until_any
#define shmemx_long_wait_until_any pshmemx_long_wait_until_any
#pragma weak shmemx_longlong_wait_until_any = pshmemx_longlong_wait_until_any
#define shmemx_longlong_wait_until_any pshmemx_longlong_wait_until_any
#pragma weak shmemx_ushort_wait_until_any = pshmemx_ushort_wait_until_any
#define shmemx_ushort_wait_until_any pshmemx_ushort_wait_until_any
#pragma weak shmemx_uint_wait_until_any = pshmemx_uint_wait_until_any
#define shmemx_uint_wait_until_any pshmemx_uint_wait_until_any
#pragma weak shmemx_ulong_wait_until_any = pshmemx_ulong_wait_until_any
#define shmemx_ulong_wait_until_any pshmemx_ulong_wait_until_any
#pragma weak shmemx_ulonglong_wait_until_any = pshmemx_ulonglong_wait_until_any
#define shmemx_ulonglong_wait_until_any pshmemx_ulonglong_wait_until_any
#pragma weak shmemx_int32_wait_until_any = pshmemx_int32_wait_until_any
#define shmemx_int32_wait_until_any pshmemx_int32_wait_until_any
#pragma weak shmemx_int64_wait_until_any = pshmemx_int64_wait_until_any
#define shmemx_int64_wait_until_any pshmemx_int64_wait_until_any
#pragma weak shmemx_uint32_wait_until_any = pshmemx_uint32_wait_until_any
#define shmemx_uint32_wait_until_any pshmemx_uint32_wait_until_any
#pragma weak shmemx_uint64_wait_until_any = pshmemx_uint64_wait_until_any
#define shmemx_uint64_wait_until_any pshmemx_uint64_wait_until_any
#pragma weak shmemx_size_wait_until_any = pshmemx_size_wait_until_any
#define shmemx_size_wait_until_any pshmemx_size_wait_until_any
#pragma weak shmemx_ptrdiff_wait_until_any = pshmemx_ptrdiff_wait_until_any
#define shmemx_ptrdiff_wait_until_any pshmemx_ptrdiff_wait_until_any
#endif  /* ENABLE_PSHMEM */

#define SHMEMX_TYPE_WAIT_UNTIL_ANY(_opname, _type, _size)               \
    size_t                                                              \
    shmemx_##_opname##_wait_until_any(_type * restrict ivars,           \
                                      size_t nelems,                    \
                                      int * restrict status,            \
                                      int cmp, _type cmp_value)         \
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

SHMEMX_TYPE_WAIT_UNTIL_ANY(short, short, 16)
SHMEMX_TYPE_WAIT_UNTIL_ANY(int, int, 32)
SHMEMX_TYPE_WAIT_UNTIL_ANY(long, long, 64)
SHMEMX_TYPE_WAIT_UNTIL_ANY(longlong, long long, 64)
SHMEMX_TYPE_WAIT_UNTIL_ANY(ushort, unsigned short, 16)
SHMEMX_TYPE_WAIT_UNTIL_ANY(uint, unsigned int, 32)
SHMEMX_TYPE_WAIT_UNTIL_ANY(ulong, unsigned long, 64)
SHMEMX_TYPE_WAIT_UNTIL_ANY(ulonglong, unsigned long long, 64)
SHMEMX_TYPE_WAIT_UNTIL_ANY(int32, int32_t, 32)
SHMEMX_TYPE_WAIT_UNTIL_ANY(int64, int64_t, 64)
SHMEMX_TYPE_WAIT_UNTIL_ANY(uint32, uint32_t, 32)
SHMEMX_TYPE_WAIT_UNTIL_ANY(uint64, uint64_t, 64)
SHMEMX_TYPE_WAIT_UNTIL_ANY(size, size_t, 64)
SHMEMX_TYPE_WAIT_UNTIL_ANY(ptrdiff, ptrdiff_t, 64)
