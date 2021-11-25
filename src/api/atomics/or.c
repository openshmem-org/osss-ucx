/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"
#include "shmemu.h"
#include "shmemc.h"
#include "common.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_uint_atomic_or = pshmem_ctx_uint_atomic_or
#define shmem_ctx_uint_atomic_or pshmem_ctx_uint_atomic_or
#pragma weak shmem_ctx_ulong_atomic_or = pshmem_ctx_ulong_atomic_or
#define shmem_ctx_ulong_atomic_or pshmem_ctx_ulong_atomic_or
#pragma weak shmem_ctx_ulonglong_atomic_or = pshmem_ctx_ulonglong_atomic_or
#define shmem_ctx_ulonglong_atomic_or pshmem_ctx_ulonglong_atomic_or
#pragma weak shmem_ctx_int32_atomic_or = pshmem_ctx_int32_atomic_or
#define shmem_ctx_int32_atomic_or pshmem_ctx_int32_atomic_or
#pragma weak shmem_ctx_int64_atomic_or = pshmem_ctx_int64_atomic_or
#define shmem_ctx_int64_atomic_or pshmem_ctx_int64_atomic_or
#pragma weak shmem_ctx_uint32_atomic_or = pshmem_ctx_uint32_atomic_or
#define shmem_ctx_uint32_atomic_or pshmem_ctx_uint32_atomic_or
#pragma weak shmem_ctx_uint64_atomic_or = pshmem_ctx_uint64_atomic_or
#define shmem_ctx_uint64_atomic_or pshmem_ctx_uint64_atomic_or
#endif /* ENABLE_PSHMEM */

SHMEM_CTX_TYPE_BITWISE(or, uint, unsigned int)
SHMEM_CTX_TYPE_BITWISE(or, ulong, unsigned long)
SHMEM_CTX_TYPE_BITWISE(or, ulonglong, unsigned long long)
SHMEM_CTX_TYPE_BITWISE(or, int32, int32_t)
SHMEM_CTX_TYPE_BITWISE(or, int64, int64_t)
SHMEM_CTX_TYPE_BITWISE(or, uint32, uint32_t)
SHMEM_CTX_TYPE_BITWISE(or, uint64, uint64_t)

API_DEF_VOID_AMO2(or, uint, unsigned int)
API_DEF_VOID_AMO2(or, ulong, unsigned long)
API_DEF_VOID_AMO2(or, ulonglong, unsigned long long)
API_DEF_VOID_AMO2(or, int32, int32_t)
API_DEF_VOID_AMO2(or, int64, int64_t)
API_DEF_VOID_AMO2(or, uint32, uint32_t)
API_DEF_VOID_AMO2(or, uint64, uint64_t)
