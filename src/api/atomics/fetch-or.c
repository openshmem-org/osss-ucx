/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"
#include "shmemu.h"
#include "shmemc.h"
#include "common.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_uint_atomic_fetch_or = pshmem_ctx_uint_atomic_fetch_or
#define shmem_ctx_uint_atomic_fetch_or pshmem_ctx_uint_atomic_fetch_or
#pragma weak shmem_ctx_ulong_atomic_fetch_or = pshmem_ctx_ulong_atomic_fetch_or
#define shmem_ctx_ulong_atomic_fetch_or pshmem_ctx_ulong_atomic_fetch_or
#pragma weak shmem_ctx_ulonglong_atomic_fetch_or = pshmem_ctx_ulonglong_atomic_fetch_or
#define shmem_ctx_ulonglong_atomic_fetch_or pshmem_ctx_ulonglong_atomic_fetch_or
#pragma weak shmem_ctx_int32_atomic_fetch_or = pshmem_ctx_int32_atomic_fetch_or
#define shmem_ctx_int32_atomic_fetch_or pshmem_ctx_int32_atomic_fetch_or
#pragma weak shmem_ctx_int64_atomic_fetch_or = pshmem_ctx_int64_atomic_fetch_or
#define shmem_ctx_int64_atomic_fetch_or pshmem_ctx_int64_atomic_fetch_or
#pragma weak shmem_ctx_uint32_atomic_fetch_or = pshmem_ctx_uint32_atomic_fetch_or
#define shmem_ctx_uint32_atomic_fetch_or pshmem_ctx_uint32_atomic_fetch_or
#pragma weak shmem_ctx_uint64_atomic_fetch_or = pshmem_ctx_uint64_atomic_fetch_or
#define shmem_ctx_uint64_atomic_fetch_or pshmem_ctx_uint64_atomic_fetch_or
#endif /* ENABLE_PSHMEM */

SHMEM_CTX_TYPE_FETCH_BITWISE(or, uint, unsigned int)
SHMEM_CTX_TYPE_FETCH_BITWISE(or, ulong, unsigned long)
SHMEM_CTX_TYPE_FETCH_BITWISE(or, ulonglong, unsigned long long)
SHMEM_CTX_TYPE_FETCH_BITWISE(or, int32, int32_t)
SHMEM_CTX_TYPE_FETCH_BITWISE(or, int64, int64_t)
SHMEM_CTX_TYPE_FETCH_BITWISE(or, uint32, uint32_t)
SHMEM_CTX_TYPE_FETCH_BITWISE(or, uint64, uint64_t)

API_DEF_AMO2(fetch_or, uint, unsigned int)
API_DEF_AMO2(fetch_or, ulong, unsigned long)
API_DEF_AMO2(fetch_or, ulonglong, unsigned long long)
API_DEF_AMO2(fetch_or, int32, int32_t)
API_DEF_AMO2(fetch_or, int64, int64_t)
API_DEF_AMO2(fetch_or, uint32, uint32_t)
API_DEF_AMO2(fetch_or, uint64, uint64_t)
