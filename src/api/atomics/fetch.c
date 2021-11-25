/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"
#include "shmemu.h"
#include "shmemc.h"
#include "common.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_int_atomic_fetch = pshmem_ctx_int_atomic_fetch
#define shmem_ctx_int_atomic_fetch pshmem_ctx_int_atomic_fetch
#pragma weak shmem_ctx_long_atomic_fetch = pshmem_ctx_long_atomic_fetch
#define shmem_ctx_long_atomic_fetch pshmem_ctx_long_atomic_fetch
#pragma weak shmem_ctx_longlong_atomic_fetch = pshmem_ctx_longlong_atomic_fetch
#define shmem_ctx_longlong_atomic_fetch pshmem_ctx_longlong_atomic_fetch
#pragma weak shmem_ctx_float_atomic_fetch = pshmem_ctx_float_atomic_fetch
#define shmem_ctx_float_atomic_fetch pshmem_ctx_float_atomic_fetch
#pragma weak shmem_ctx_double_atomic_fetch = pshmem_ctx_double_atomic_fetch
#define shmem_ctx_double_atomic_fetch pshmem_ctx_double_atomic_fetch
#pragma weak shmem_ctx_uint_atomic_fetch = pshmem_ctx_uint_atomic_fetch
#define shmem_ctx_uint_atomic_fetch pshmem_ctx_uint_atomic_fetch
#pragma weak shmem_ctx_ulong_atomic_fetch = pshmem_ctx_ulong_atomic_fetch
#define shmem_ctx_ulong_atomic_fetch pshmem_ctx_ulong_atomic_fetch
#pragma weak shmem_ctx_ulonglong_atomic_fetch = pshmem_ctx_ulonglong_atomic_fetch
#define shmem_ctx_ulonglong_atomic_fetch pshmem_ctx_ulonglong_atomic_fetch
#pragma weak shmem_ctx_int32_atomic_fetch = pshmem_ctx_int32_atomic_fetch
#define shmem_ctx_int32_atomic_fetch pshmem_ctx_int32_atomic_fetch
#pragma weak shmem_ctx_int64_atomic_fetch = pshmem_ctx_int64_atomic_fetch
#define shmem_ctx_int64_atomic_fetch pshmem_ctx_int64_atomic_fetch
#pragma weak shmem_ctx_uint32_atomic_fetch = pshmem_ctx_uint32_atomic_fetch
#define shmem_ctx_uint32_atomic_fetch pshmem_ctx_uint32_atomic_fetch
#pragma weak shmem_ctx_uint64_atomic_fetch = pshmem_ctx_uint64_atomic_fetch
#define shmem_ctx_uint64_atomic_fetch pshmem_ctx_uint64_atomic_fetch
#pragma weak shmem_ctx_size_atomic_fetch = pshmem_ctx_size_atomic_fetch
#define shmem_ctx_size_atomic_fetch pshmem_ctx_size_atomic_fetch
#pragma weak shmem_ctx_ptrdiff_atomic_fetch = pshmem_ctx_ptrdiff_atomic_fetch
#define shmem_ctx_ptrdiff_atomic_fetch pshmem_ctx_ptrdiff_atomic_fetch
#endif /* ENABLE_PSHMEM */

#define SHMEM_CTX_TYPE_FETCH(_name, _type)                              \
    _type                                                               \
    shmem_ctx_##_name##_atomic_fetch(shmem_ctx_t ctx,                   \
                                     const _type *target, int pe)       \
    {                                                                   \
        _type v;                                                        \
                                                                        \
        SHMEMT_MUTEX_NOPROTECT(shmemc_ctx_fetch(ctx,                    \
                                                (_type *) target,       \
                                                sizeof(*target),        \
                                                pe,                     \
                                                &v));                   \
        return v;                                                       \
    }

SHMEM_CTX_TYPE_FETCH(float, float)
SHMEM_CTX_TYPE_FETCH(double, double)
SHMEM_CTX_TYPE_FETCH(int, int)
SHMEM_CTX_TYPE_FETCH(long, long)
SHMEM_CTX_TYPE_FETCH(longlong, long long)
SHMEM_CTX_TYPE_FETCH(uint, unsigned int)
SHMEM_CTX_TYPE_FETCH(ulong, unsigned long)
SHMEM_CTX_TYPE_FETCH(ulonglong, unsigned long long)
SHMEM_CTX_TYPE_FETCH(int32, int32_t)
SHMEM_CTX_TYPE_FETCH(int64, int64_t)
SHMEM_CTX_TYPE_FETCH(uint32, uint32_t)
SHMEM_CTX_TYPE_FETCH(uint64, uint64_t)
SHMEM_CTX_TYPE_FETCH(size, size_t)
SHMEM_CTX_TYPE_FETCH(ptrdiff, ptrdiff_t)

API_DEF_CONST_AMO1(fetch, float, float)
API_DEF_CONST_AMO1(fetch, double, double)
API_DEF_CONST_AMO1(fetch, int, int)
API_DEF_CONST_AMO1(fetch, long, long)
API_DEF_CONST_AMO1(fetch, longlong, long long)
API_DEF_CONST_AMO1(fetch, uint, unsigned int)
API_DEF_CONST_AMO1(fetch, ulong, unsigned long)
API_DEF_CONST_AMO1(fetch, ulonglong, unsigned long long)
API_DEF_CONST_AMO1(fetch, int32, int32_t)
API_DEF_CONST_AMO1(fetch, int64, int64_t)
API_DEF_CONST_AMO1(fetch, uint32, uint32_t)
API_DEF_CONST_AMO1(fetch, uint64, uint64_t)
API_DEF_CONST_AMO1(fetch, size, size_t)
API_DEF_CONST_AMO1(fetch, ptrdiff, ptrdiff_t)
