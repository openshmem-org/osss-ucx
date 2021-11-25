/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"
#include "shmemu.h"
#include "shmemc.h"
#include "common.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_uint_atomic_fetch_and = pshmem_ctx_uint_atomic_fetch_and
#define shmem_ctx_uint_atomic_fetch_and pshmem_ctx_uint_atomic_fetch_and
#pragma weak shmem_ctx_ulong_atomic_fetch_and = pshmem_ctx_ulong_atomic_fetch_and
#define shmem_ctx_ulong_atomic_fetch_and pshmem_ctx_ulong_atomic_fetch_and
#pragma weak shmem_ctx_ulonglong_atomic_fetch_and = pshmem_ctx_ulonglong_atomic_fetch_and
#define shmem_ctx_ulonglong_atomic_fetch_and pshmem_ctx_ulonglong_atomic_fetch_and
#pragma weak shmem_ctx_int32_atomic_fetch_and = pshmem_ctx_int32_atomic_fetch_and
#define shmem_ctx_int32_atomic_fetch_and pshmem_ctx_int32_atomic_fetch_and
#pragma weak shmem_ctx_int64_atomic_fetch_and = pshmem_ctx_int64_atomic_fetch_and
#define shmem_ctx_int64_atomic_fetch_and pshmem_ctx_int64_atomic_fetch_and
#pragma weak shmem_ctx_uint32_atomic_fetch_and = pshmem_ctx_uint32_atomic_fetch_and
#define shmem_ctx_uint32_atomic_fetch_and pshmem_ctx_uint32_atomic_fetch_and
#pragma weak shmem_ctx_uint64_atomic_fetch_and = pshmem_ctx_uint64_atomic_fetch_and
#define shmem_ctx_uint64_atomic_fetch_and pshmem_ctx_uint64_atomic_fetch_and
#endif /* ENABLE_PSHMEM */

SHMEM_CTX_TYPE_FETCH_BITWISE(and, uint, unsigned int)
SHMEM_CTX_TYPE_FETCH_BITWISE(and, ulong, unsigned long)
SHMEM_CTX_TYPE_FETCH_BITWISE(and, ulonglong, unsigned long long)
SHMEM_CTX_TYPE_FETCH_BITWISE(and, int32, int32_t)
SHMEM_CTX_TYPE_FETCH_BITWISE(and, int64, int64_t)
SHMEM_CTX_TYPE_FETCH_BITWISE(and, uint32, uint32_t)
SHMEM_CTX_TYPE_FETCH_BITWISE(and, uint64, uint64_t)

API_DEF_AMO2(fetch_and, uint, unsigned int)
API_DEF_AMO2(fetch_and, ulong, unsigned long)
API_DEF_AMO2(fetch_and, ulonglong, unsigned long long)
API_DEF_AMO2(fetch_and, int32, int32_t)
API_DEF_AMO2(fetch_and, int64, int64_t)
API_DEF_AMO2(fetch_and, uint32, uint32_t)
API_DEF_AMO2(fetch_and, uint64, uint64_t)
