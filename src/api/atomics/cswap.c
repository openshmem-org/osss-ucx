/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"
#include "shmemu.h"
#include "shmemc.h"
#include "common.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_atomic_compare_swap = pshmem_int_atomic_compare_swap
#define shmem_int_atomic_compare_swap pshmem_int_atomic_compare_swap
#pragma weak shmem_long_atomic_compare_swap = pshmem_long_atomic_compare_swap
#define shmem_long_atomic_compare_swap pshmem_long_atomic_compare_swap
#pragma weak shmem_longlong_atomic_compare_swap = pshmem_longlong_atomic_compare_swap
#define shmem_longlong_atomic_compare_swap pshmem_longlong_atomic_compare_swap
#pragma weak shmem_uint_atomic_compare_swap = pshmem_uint_atomic_compare_swap
#define shmem_uint_atomic_compare_swap pshmem_uint_atomic_compare_swap
#pragma weak shmem_ulong_atomic_compare_swap = pshmem_ulong_atomic_compare_swap
#define shmem_ulong_atomic_compare_swap pshmem_ulong_atomic_compare_swap
#pragma weak shmem_ulonglong_atomic_compare_swap = pshmem_ulonglong_atomic_compare_swap
#define shmem_ulonglong_atomic_compare_swap pshmem_ulonglong_atomic_compare_swap
#pragma weak shmem_int32_atomic_compare_swap = pshmem_int32_atomic_compare_swap
#define shmem_int32_atomic_compare_swap pshmem_int32_atomic_compare_swap
#pragma weak shmem_int64_atomic_compare_swap = pshmem_int64_atomic_compare_swap
#define shmem_int64_atomic_compare_swap pshmem_int64_atomic_compare_swap
#pragma weak shmem_uint32_atomic_compare_swap = pshmem_uint32_atomic_compare_swap
#define shmem_uint32_atomic_compare_swap pshmem_uint32_atomic_compare_swap
#pragma weak shmem_uint64_atomic_compare_swap = pshmem_uint64_atomic_compare_swap
#define shmem_uint64_atomic_compare_swap pshmem_uint64_atomic_compare_swap
#pragma weak shmem_size_atomic_compare_swap = pshmem_size_atomic_compare_swap
#define shmem_size_atomic_compare_swap pshmem_size_atomic_compare_swap
#pragma weak shmem_ptrdiff_atomic_compare_swap = pshmem_ptrdiff_atomic_compare_swap
#define shmem_ptrdiff_atomic_compare_swap pshmem_ptrdiff_atomic_compare_swap
#endif /* ENABLE_PSHMEM */

#define SHMEM_CTX_TYPE_CSWAP(_name, _type)                              \
    _type                                                               \
    shmem_ctx_##_name##_atomic_compare_swap(shmem_ctx_t ctx,            \
                                            _type *target,              \
                                            _type cond, _type value,    \
                                            int pe)                     \
    {                                                                   \
        _type v;                                                        \
                                                                        \
        SHMEMT_MUTEX_NOPROTECT(shmemc_ctx_cswap(ctx,                    \
                                                target,                 \
                                                &cond,                  \
                                                &value, sizeof(value),  \
                                                pe, &v));               \
        return v;                                                       \
    }

SHMEM_CTX_TYPE_CSWAP(int, int)
SHMEM_CTX_TYPE_CSWAP(long, long)
SHMEM_CTX_TYPE_CSWAP(longlong, long long)
SHMEM_CTX_TYPE_CSWAP(uint, unsigned int)
SHMEM_CTX_TYPE_CSWAP(ulong, unsigned long)
SHMEM_CTX_TYPE_CSWAP(ulonglong, unsigned long long)
SHMEM_CTX_TYPE_CSWAP(int32, int32_t)
SHMEM_CTX_TYPE_CSWAP(int64, int64_t)
SHMEM_CTX_TYPE_CSWAP(uint32, uint32_t)
SHMEM_CTX_TYPE_CSWAP(uint64, uint64_t)
SHMEM_CTX_TYPE_CSWAP(size, size_t)
SHMEM_CTX_TYPE_CSWAP(ptrdiff, ptrdiff_t)

API_DEF_AMO3(compare_swap, int, int)
API_DEF_AMO3(compare_swap, long, long)
API_DEF_AMO3(compare_swap, longlong, long long)
API_DEF_AMO3(compare_swap, uint, unsigned int)
API_DEF_AMO3(compare_swap, ulong, unsigned long)
API_DEF_AMO3(compare_swap, ulonglong, unsigned long long)
API_DEF_AMO3(compare_swap, int32, int32_t)
API_DEF_AMO3(compare_swap, int64, int64_t)
API_DEF_AMO3(compare_swap, uint32, uint32_t)
API_DEF_AMO3(compare_swap, uint64, uint64_t)
API_DEF_AMO3(compare_swap, size, size_t)
API_DEF_AMO3(compare_swap, ptrdiff, ptrdiff_t)
