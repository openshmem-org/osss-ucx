/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"
#include "shmemu.h"
#include "shmemc.h"
#include "common.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_float_atomic_swap_nbi = pshmem_ctx_float_atomic_swap_nbi
#define shmem_ctx_float_atomic_swap_nbi pshmem_ctx_float_atomic_swap_nbi
#pragma weak shmem_ctx_double_atomic_swap_nbi = pshmem_ctx_double_atomic_swap_nbi
#define shmem_ctx_double_atomic_swap_nbi pshmem_ctx_double_atomic_swap_nbi
#pragma weak shmem_ctx_int_atomic_swap_nbi = pshmem_ctx_int_atomic_swap_nbi
#define shmem_ctx_int_atomic_swap_nbi pshmem_ctx_int_atomic_swap_nbi
#pragma weak shmem_ctx_long_atomic_swap_nbi = pshmem_ctx_long_atomic_swap_nbi
#define shmem_ctx_long_atomic_swap_nbi pshmem_ctx_long_atomic_swap_nbi
#pragma weak shmem_ctx_longlong_atomic_swap_nbi = pshmem_ctx_longlong_atomic_swap_nbi
#define shmem_ctx_longlong_atomic_swap_nbi pshmem_ctx_longlong_atomic_swap_nbi
#pragma weak shmem_ctx_uint_atomic_swap_nbi = pshmem_ctx_uint_atomic_swap_nbi
#define shmem_ctx_uint_atomic_swap_nbi pshmem_ctx_uint_atomic_swap_nbi
#pragma weak shmem_ctx_ulong_atomic_swap_nbi = pshmem_ctx_ulong_atomic_swap_nbi
#define shmem_ctx_ulong_atomic_swap_nbi pshmem_ctx_ulong_atomic_swap_nbi
#pragma weak shmem_ctx_ulonglong_atomic_swap_nbi = pshmem_ctx_ulonglong_atomic_swap_nbi
#define shmem_ctx_ulonglong_atomic_swap_nbi pshmem_ctx_ulonglong_atomic_swap_nbi
#pragma weak shmem_ctx_int32_atomic_swap_nbi = pshmem_ctx_int32_atomic_swap_nbi
#define shmem_ctx_int32_atomic_swap_nbi pshmem_ctx_int32_atomic_swap_nbi
#pragma weak shmem_ctx_int64_atomic_swap_nbi = pshmem_ctx_int64_atomic_swap_nbi
#define shmem_ctx_int64_atomic_swap_nbi pshmem_ctx_int64_atomic_swap_nbi
#pragma weak shmem_ctx_uint32_atomic_swap_nbi = pshmem_ctx_uint32_atomic_swap_nbi
#define shmem_ctx_uint32_atomic_swap_nbi pshmem_ctx_uint32_atomic_swap_nbi
#pragma weak shmem_ctx_uint64_atomic_swap_nbi = pshmem_ctx_uint64_atomic_swap_nbi
#define shmem_ctx_uint64_atomic_swap_nbi pshmem_ctx_uint64_atomic_swap_nbi
#pragma weak shmem_ctx_size_atomic_swap_nbi = pshmem_ctx_size_atomic_swap_nbi
#define shmem_ctx_size_atomic_swap_nbi pshmem_ctx_size_atomic_swap_nbi
#pragma weak shmem_ctx_ptrdiff_atomic_swap_nbi = pshmem_ctx_ptrdiff_atomic_swap_nbi
#define shmem_ctx_ptrdiff_atomic_swap_nbi pshmem_ctx_ptrdiff_atomic_swap_nbi
#endif /* ENABLE_PSHMEM */

#define SHMEM_CTX_TYPE_SWAP_NBI(_name, _type)                           \
    void                                                                \
    shmem_ctx_##_name##_atomic_swap_nbi(shmem_ctx_t ctx,                \
                                        _type *fetch,                   \
                                        _type *target,                  \
                                        _type value,                    \
                                        int pe)                         \
    {                                                                   \
        SHMEMU_CHECK_INIT();                                            \
        SHMEMU_CHECK_SYMMETRIC(target, 3);                              \
                                                                        \
        SHMEMT_MUTEX_NOPROTECT(shmemc_ctx_swap(ctx,                     \
                                               target,                  \
                                               &value, sizeof(value),   \
                                               pe, fetch));             \
    }

SHMEM_CTX_TYPE_SWAP_NBI(int, int)
SHMEM_CTX_TYPE_SWAP_NBI(long, long)
SHMEM_CTX_TYPE_SWAP_NBI(longlong, long long)
SHMEM_CTX_TYPE_SWAP_NBI(float, float)
SHMEM_CTX_TYPE_SWAP_NBI(double, double)
SHMEM_CTX_TYPE_SWAP_NBI(uint, unsigned int)
SHMEM_CTX_TYPE_SWAP_NBI(ulong, unsigned long)
SHMEM_CTX_TYPE_SWAP_NBI(ulonglong, unsigned long long)
SHMEM_CTX_TYPE_SWAP_NBI(int32, int32_t)
SHMEM_CTX_TYPE_SWAP_NBI(int64, int64_t)
SHMEM_CTX_TYPE_SWAP_NBI(uint32, uint32_t)
SHMEM_CTX_TYPE_SWAP_NBI(uint64, uint64_t)
SHMEM_CTX_TYPE_SWAP_NBI(size, size_t)
SHMEM_CTX_TYPE_SWAP_NBI(ptrdiff, ptrdiff_t)

API_DEF_AMO2_NBI(swap, float, float)
API_DEF_AMO2_NBI(swap, double, double)
API_DEF_AMO2_NBI(swap, int, int)
API_DEF_AMO2_NBI(swap, long, long)
API_DEF_AMO2_NBI(swap, longlong, long long)
API_DEF_AMO2_NBI(swap, uint, unsigned int)
API_DEF_AMO2_NBI(swap, ulong, unsigned long)
API_DEF_AMO2_NBI(swap, ulonglong, unsigned long long)
API_DEF_AMO2_NBI(swap, int32, int32_t)
API_DEF_AMO2_NBI(swap, int64, int64_t)
API_DEF_AMO2_NBI(swap, uint32, uint32_t)
API_DEF_AMO2_NBI(swap, uint64, uint64_t)
API_DEF_AMO2_NBI(swap, size, size_t)
API_DEF_AMO2_NBI(swap, ptrdiff, ptrdiff_t)
