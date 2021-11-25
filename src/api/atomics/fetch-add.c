/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"
#include "shmemu.h"
#include "shmemc.h"
#include "common.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_int_atomic_fetch_add = pshmem_ctx_int_atomic_fetch_add
#define shmem_ctx_int_atomic_fetch_add pshmem_ctx_int_atomic_fetch_add
#pragma weak shmem_ctx_long_atomic_fetch_add = pshmem_ctx_long_atomic_fetch_add
#define shmem_ctx_long_atomic_fetch_add pshmem_ctx_long_atomic_fetch_add
#pragma weak shmem_ctx_longlong_atomic_fetch_add = pshmem_ctx_longlong_atomic_fetch_add
#define shmem_ctx_longlong_atomic_fetch_add pshmem_ctx_longlong_atomic_fetch_add
#pragma weak shmem_ctx_uint_atomic_fetch_add = pshmem_ctx_uint_atomic_fetch_add
#define shmem_ctx_uint_atomic_fetch_add pshmem_ctx_uint_atomic_fetch_add
#pragma weak shmem_ctx_ulong_atomic_fetch_add = pshmem_ctx_ulong_atomic_fetch_add
#define shmem_ctx_ulong_atomic_fetch_add pshmem_ctx_ulong_atomic_fetch_add
#pragma weak shmem_ctx_ulonglong_atomic_fetch_add = pshmem_ctx_ulonglong_atomic_fetch_add
#define shmem_ctx_ulonglong_atomic_fetch_add pshmem_ctx_ulonglong_atomic_fetch_add
#pragma weak shmem_ctx_int32_atomic_fetch_add = pshmem_ctx_int32_atomic_fetch_add
#define shmem_ctx_int32_atomic_fetch_add pshmem_ctx_int32_atomic_fetch_add
#pragma weak shmem_ctx_int64_atomic_fetch_add = pshmem_ctx_int64_atomic_fetch_add
#define shmem_ctx_int64_atomic_fetch_add pshmem_ctx_int64_atomic_fetch_add
#pragma weak shmem_ctx_uint32_atomic_fetch_add = pshmem_ctx_uint32_atomic_fetch_add
#define shmem_ctx_uint32_atomic_fetch_add pshmem_ctx_uint32_atomic_fetch_add
#pragma weak shmem_ctx_uint64_atomic_fetch_add = pshmem_ctx_uint64_atomic_fetch_add
#define shmem_ctx_uint64_atomic_fetch_add pshmem_ctx_uint64_atomic_fetch_add
#pragma weak shmem_ctx_size_atomic_fetch_add = pshmem_ctx_size_atomic_fetch_add
#define shmem_ctx_size_atomic_fetch_add pshmem_ctx_size_atomic_fetch_add
#pragma weak shmem_ctx_ptrdiff_atomic_fetch_add = pshmem_ctx_ptrdiff_atomic_fetch_add
#define shmem_ctx_ptrdiff_atomic_fetch_add pshmem_ctx_ptrdiff_atomic_fetch_add
#endif /* ENABLE_PSHMEM */

/*
 * fetch-and-add
 */

#define SHMEM_CTX_TYPE_FADD(_name, _type)                               \
    _type                                                               \
    shmem_ctx_##_name##_atomic_fetch_add(shmem_ctx_t ctx,               \
                                         _type *target,                 \
                                         _type value, int pe)           \
    {                                                                   \
        _type v;                                                        \
                                                                        \
        SHMEMT_MUTEX_NOPROTECT(shmemc_ctx_fadd(ctx,                     \
                                               target,                  \
                                               &value, sizeof(value),   \
                                               pe, &v));                \
        return v;                                                       \
    }

SHMEM_CTX_TYPE_FADD(int, int)
SHMEM_CTX_TYPE_FADD(long, long)
SHMEM_CTX_TYPE_FADD(longlong, long long)
SHMEM_CTX_TYPE_FADD(uint, unsigned int)
SHMEM_CTX_TYPE_FADD(ulong, unsigned long)
SHMEM_CTX_TYPE_FADD(ulonglong, unsigned long long)
SHMEM_CTX_TYPE_FADD(int32, int32_t)
SHMEM_CTX_TYPE_FADD(int64, int64_t)
SHMEM_CTX_TYPE_FADD(uint32, uint32_t)
SHMEM_CTX_TYPE_FADD(uint64, uint64_t)
SHMEM_CTX_TYPE_FADD(size, size_t)
SHMEM_CTX_TYPE_FADD(ptrdiff, ptrdiff_t)

API_DEF_AMO2(fetch_add, int, int)
API_DEF_AMO2(fetch_add, long, long)
API_DEF_AMO2(fetch_add, longlong, long long)
API_DEF_AMO2(fetch_add, uint, unsigned int)
API_DEF_AMO2(fetch_add, ulong, unsigned long)
API_DEF_AMO2(fetch_add, ulonglong, unsigned long long)
API_DEF_AMO2(fetch_add, int32, int32_t)
API_DEF_AMO2(fetch_add, int64, int64_t)
API_DEF_AMO2(fetch_add, uint32, uint32_t)
API_DEF_AMO2(fetch_add, uint64, uint64_t)
API_DEF_AMO2(fetch_add, size, size_t)
API_DEF_AMO2(fetch_add, ptrdiff, ptrdiff_t)
