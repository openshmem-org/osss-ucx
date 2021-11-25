/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"
#include "shmemu.h"
#include "shmemc.h"
#include "common.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_uint_atomic_fetch_or_nbi = pshmem_uint_atomic_fetch_or_nbi
#define shmem_uint_atomic_fetch_or_nbi pshmem_uint_atomic_fetch_or_nbi
#pragma weak shmem_ulong_atomic_fetch_or_nbi = pshmem_ulong_atomic_fetch_or_nbi
#define shmem_ulong_atomic_fetch_or_nbi pshmem_ulong_atomic_fetch_or_nbi
#pragma weak shmem_ulonglong_atomic_fetch_or_nbi = pshmem_ulonglong_atomic_fetch_or_nbi
#define shmem_ulonglong_atomic_fetch_or_nbi pshmem_ulonglong_atomic_fetch_or_nbi
#pragma weak shmem_int32_atomic_fetch_or_nbi = pshmem_int32_atomic_fetch_or_nbi
#define shmem_int32_atomic_fetch_or_nbi pshmem_int32_atomic_fetch_or_nbi
#pragma weak shmem_int64_atomic_fetch_or_nbi = pshmem_int64_atomic_fetch_or_nbi
#define shmem_int64_atomic_fetch_or_nbi pshmem_int64_atomic_fetch_or_nbi
#pragma weak shmem_uint32_atomic_fetch_or_nbi = pshmem_uint32_atomic_fetch_or_nbi
#define shmem_uint32_atomic_fetch_or_nbi pshmem_uint32_atomic_fetch_or_nbi
#pragma weak shmem_uint64_atomic_fetch_or_nbi = pshmem_uint64_atomic_fetch_or_nbi
#define shmem_uint64_atomic_fetch_or_nbi pshmem_uint64_atomic_fetch_or_nbi
#endif /* ENABLE_PSHMEM */

SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(or, uint, unsigned int)
SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(or, ulong, unsigned long)
SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(or, ulonglong, unsigned long long)
SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(or, int32, int32_t)
SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(or, int64, int64_t)
SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(or, uint32, uint32_t)
SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(or, uint64, uint64_t)

API_DEF_AMO2_NBI(fetch_or, uint, unsigned int)
API_DEF_AMO2_NBI(fetch_or, ulong, unsigned long)
API_DEF_AMO2_NBI(fetch_or, ulonglong, unsigned long long)
API_DEF_AMO2_NBI(fetch_or, int32, int32_t)
API_DEF_AMO2_NBI(fetch_or, int64, int64_t)
API_DEF_AMO2_NBI(fetch_or, uint32, uint32_t)
API_DEF_AMO2_NBI(fetch_or, uint64, uint64_t)
