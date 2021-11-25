/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"
#include "shmemu.h"
#include "shmemc.h"
#include "common.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_uint_atomic_xor = pshmem_ctx_uint_atomic_xor
#define shmem_ctx_uint_atomic_xor pshmem_ctx_uint_atomic_xor
#pragma weak shmem_ctx_ulong_atomic_xor = pshmem_ctx_ulong_atomic_xor
#define shmem_ctx_ulong_atomic_xor pshmem_ctx_ulong_atomic_xor
#pragma weak shmem_ctx_ulonglong_atomic_xor = pshmem_ctx_ulonglong_atomic_xor
#define shmem_ctx_ulonglong_atomic_xor pshmem_ctx_ulonglong_atomic_xor
#pragma weak shmem_ctx_int32_atomic_xor = pshmem_ctx_int32_atomic_xor
#define shmem_ctx_int32_atomic_xor pshmem_ctx_int32_atomic_xor
#pragma weak shmem_ctx_int64_atomic_xor = pshmem_ctx_int64_atomic_xor
#define shmem_ctx_int64_atomic_xor pshmem_ctx_int64_atomic_xor
#pragma weak shmem_ctx_uint32_atomic_xor = pshmem_ctx_uint32_atomic_xor
#define shmem_ctx_uint32_atomic_xor pshmem_ctx_uint32_atomic_xor
#pragma weak shmem_ctx_uint64_atomic_xor = pshmem_ctx_uint64_atomic_xor
#define shmem_ctx_uint64_atomic_xor pshmem_ctx_uint64_atomic_xor
#endif /* ENABLE_PSHMEM */

SHMEM_CTX_TYPE_BITWISE(xor, uint, unsigned int)
SHMEM_CTX_TYPE_BITWISE(xor, ulong, unsigned long)
SHMEM_CTX_TYPE_BITWISE(xor, ulonglong, unsigned long long)
SHMEM_CTX_TYPE_BITWISE(xor, int32, int32_t)
SHMEM_CTX_TYPE_BITWISE(xor, int64, int64_t)
SHMEM_CTX_TYPE_BITWISE(xor, uint32, uint32_t)
SHMEM_CTX_TYPE_BITWISE(xor, uint64, uint64_t)

API_DEF_VOID_AMO2(xor, uint, unsigned int)
API_DEF_VOID_AMO2(xor, ulong, unsigned long)
API_DEF_VOID_AMO2(xor, ulonglong, unsigned long long)
API_DEF_VOID_AMO2(xor, int32, int32_t)
API_DEF_VOID_AMO2(xor, int64, int64_t)
API_DEF_VOID_AMO2(xor, uint32, uint32_t)
API_DEF_VOID_AMO2(xor, uint64, uint64_t)
