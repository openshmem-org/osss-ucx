/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"
#include "shmemu.h"
#include "shmemc.h"
#include "common.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_atomic_fetch_add_nbi = pshmem_int_atomic_fetch_add_nbi
#define shmem_int_atomic_fetch_add_nbi pshmem_int_atomic_fetch_add_nbi
#pragma weak shmem_long_atomic_fetch_add_nbi = pshmem_long_atomic_fetch_add_nbi
#define shmem_long_atomic_fetch_add_nbi pshmem_long_atomic_fetch_add_nbi
#pragma weak shmem_longlong_atomic_fetch_add_nbi = pshmem_longlong_atomic_fetch_add_nbi
#define shmem_longlong_atomic_fetch_add_nbi pshmem_longlong_atomic_fetch_add_nbi
#pragma weak shmem_uint_atomic_fetch_add_nbi = pshmem_uint_atomic_fetch_add_nbi
#define shmem_uint_atomic_fetch_add_nbi pshmem_uint_atomic_fetch_add_nbi
#pragma weak shmem_ulong_atomic_fetch_add_nbi = pshmem_ulong_atomic_fetch_add_nbi
#define shmem_ulong_atomic_fetch_add_nbi pshmem_ulong_atomic_fetch_add_nbi
#pragma weak shmem_ulonglong_atomic_fetch_add_nbi = pshmem_ulonglong_atomic_fetch_add_nbi
#define shmem_ulonglong_atomic_fetch_add_nbi pshmem_ulonglong_atomic_fetch_add_nbi
#pragma weak shmem_int32_atomic_fetch_add_nbi = pshmem_int32_atomic_fetch_add_nbi
#define shmem_int32_atomic_fetch_add_nbi pshmem_int32_atomic_fetch_add_nbi
#pragma weak shmem_int64_atomic_fetch_add_nbi = pshmem_int64_atomic_fetch_add_nbi
#define shmem_int64_atomic_fetch_add_nbi pshmem_int64_atomic_fetch_add_nbi
#pragma weak shmem_uint32_atomic_fetch_add_nbi = pshmem_uint32_atomic_fetch_add_nbi
#define shmem_uint32_atomic_fetch_add_nbi pshmem_uint32_atomic_fetch_add_nbi
#pragma weak shmem_uint64_atomic_fetch_add_nbi = pshmem_uint64_atomic_fetch_add_nbi
#define shmem_uint64_atomic_fetch_add_nbi pshmem_uint64_atomic_fetch_add_nbi
#pragma weak shmem_size_atomic_fetch_add_nbi = pshmem_size_atomic_fetch_add_nbi
#define shmem_size_atomic_fetch_add_nbi pshmem_size_atomic_fetch_add_nbi
#pragma weak shmem_ptrdiff_atomic_fetch_add_nbi = pshmem_ptrdiff_atomic_fetch_add_nbi
#define shmem_ptrdiff_atomic_fetch_add_nbi pshmem_ptrdiff_atomic_fetch_add_nbi
#endif /* ENABLE_PSHMEM */

#define SHMEM_CTX_TYPE_FADD_NBI(_name, _type)                           \
    void                                                                \
    shmem_ctx_##_name##_atomic_fetch_add_nbi(shmem_ctx_t ctx,           \
                                             _type *fetch,              \
                                             _type *target,             \
                                             _type value, int pe)       \
    {                                                                   \
        SHMEMT_MUTEX_NOPROTECT(shmemc_ctx_fadd_nbi(ctx,                 \
                                                   target,              \
                                                   &value, sizeof(value), \
                                                   pe, fetch));         \
    }

SHMEM_CTX_TYPE_FADD_NBI(int, int)
SHMEM_CTX_TYPE_FADD_NBI(long, long)
SHMEM_CTX_TYPE_FADD_NBI(longlong, long long)
SHMEM_CTX_TYPE_FADD_NBI(uint, unsigned int)
SHMEM_CTX_TYPE_FADD_NBI(ulong, unsigned long)
SHMEM_CTX_TYPE_FADD_NBI(ulonglong, unsigned long long)
SHMEM_CTX_TYPE_FADD_NBI(int32, int32_t)
SHMEM_CTX_TYPE_FADD_NBI(int64, int64_t)
SHMEM_CTX_TYPE_FADD_NBI(uint32, uint32_t)
SHMEM_CTX_TYPE_FADD_NBI(uint64, uint64_t)
SHMEM_CTX_TYPE_FADD_NBI(size, size_t)
SHMEM_CTX_TYPE_FADD_NBI(ptrdiff, ptrdiff_t)

API_DEF_AMO2_NBI(fetch_add, int, int)
API_DEF_AMO2_NBI(fetch_add, long, long)
API_DEF_AMO2_NBI(fetch_add, longlong, long long)
API_DEF_AMO2_NBI(fetch_add, uint, unsigned int)
API_DEF_AMO2_NBI(fetch_add, ulong, unsigned long)
API_DEF_AMO2_NBI(fetch_add, ulonglong, unsigned long long)
API_DEF_AMO2_NBI(fetch_add, int32, int32_t)
API_DEF_AMO2_NBI(fetch_add, int64, int64_t)
API_DEF_AMO2_NBI(fetch_add, uint32, uint32_t)
API_DEF_AMO2_NBI(fetch_add, uint64, uint64_t)
API_DEF_AMO2_NBI(fetch_add, size, size_t)
API_DEF_AMO2_NBI(fetch_add, ptrdiff, ptrdiff_t)
