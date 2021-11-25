/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"
#include "shmemu.h"
#include "shmemc.h"
#include "common.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_int_atomic_fetch_nbi = pshmem_ctx_int_atomic_fetch_nbi
#define shmem_ctx_int_atomic_fetch_nbi pshmem_ctx_int_atomic_fetch_nbi
#pragma weak shmem_ctx_long_atomic_fetch_nbi = pshmem_ctx_long_atomic_fetch_nbi
#define shmem_ctx_long_atomic_fetch_nbi pshmem_ctx_long_atomic_fetch_nbi
#pragma weak shmem_ctx_longlong_atomic_fetch_nbi = pshmem_ctx_longlong_atomic_fetch_nbi
#define shmem_ctx_longlong_atomic_fetch_nbi pshmem_ctx_longlong_atomic_fetch_nbi
#pragma weak shmem_ctx_float_atomic_fetch_nbi = pshmem_ctx_float_atomic_fetch_nbi
#define shmem_ctx_float_atomic_fetch_nbi pshmem_ctx_float_atomic_fetch_nbi
#pragma weak shmem_ctx_double_atomic_fetch_nbi = pshmem_ctx_double_atomic_fetch_nbi
#define shmem_ctx_double_atomic_fetch_nbi pshmem_ctx_double_atomic_fetch_nbi
#pragma weak shmem_ctx_uint_atomic_fetch_nbi = pshmem_ctx_uint_atomic_fetch_nbi
#define shmem_ctx_uint_atomic_fetch_nbi pshmem_ctx_uint_atomic_fetch_nbi
#pragma weak shmem_ctx_ulong_atomic_fetch_nbi = pshmem_ctx_ulong_atomic_fetch_nbi
#define shmem_ctx_ulong_atomic_fetch_nbi pshmem_ctx_ulong_atomic_fetch_nbi
#pragma weak shmem_ctx_ulonglong_atomic_fetch_nbi = pshmem_ctx_ulonglong_atomic_fetch_nbi
#define shmem_ctx_ulonglong_atomic_fetch_nbi pshmem_ctx_ulonglong_atomic_fetch_nbi
#pragma weak shmem_ctx_int32_atomic_fetch_nbi = pshmem_ctx_int32_atomic_fetch_nbi
#define shmem_ctx_int32_atomic_fetch_nbi pshmem_ctx_int32_atomic_fetch_nbi
#pragma weak shmem_ctx_int64_atomic_fetch_nbi = pshmem_ctx_int64_atomic_fetch_nbi
#define shmem_ctx_int64_atomic_fetch_nbi pshmem_ctx_int64_atomic_fetch_nbi
#pragma weak shmem_ctx_uint32_atomic_fetch_nbi = pshmem_ctx_uint32_atomic_fetch_nbi
#define shmem_ctx_uint32_atomic_fetch_nbi pshmem_ctx_uint32_atomic_fetch_nbi
#pragma weak shmem_ctx_uint64_atomic_fetch_nbi = pshmem_ctx_uint64_atomic_fetch_nbi
#define shmem_ctx_uint64_atomic_fetch_nbi pshmem_ctx_uint64_atomic_fetch_nbi
#pragma weak shmem_ctx_size_atomic_fetch_nbi = pshmem_ctx_size_atomic_fetch_nbi
#define shmem_ctx_size_atomic_fetch_nbi pshmem_ctx_size_atomic_fetch_nbi
#pragma weak shmem_ctx_ptrdiff_atomic_fetch_nbi = pshmem_ctx_ptrdiff_atomic_fetch_nbi
#define shmem_ctx_ptrdiff_atomic_fetch_nbi pshmem_ctx_ptrdiff_atomic_fetch_nbi
#endif /* ENABLE_PSHMEM */

/*
 * NB currently using blocking as first hack
 */

#define SHMEM_CTX_TYPE_FETCH_NBI(_name, _type)                          \
    void                                                                \
    shmem_ctx_##_name##_atomic_fetch_nbi(shmem_ctx_t ctx,               \
                                         _type *fetch,                  \
                                         const _type *target, int pe)   \
    {                                                                   \
        SHMEMT_MUTEX_NOPROTECT(shmemc_ctx_fetch(ctx,                    \
                                                (_type *) target,       \
                                                sizeof(*target),        \
                                                pe,                     \
                                                fetch));                \
    }

SHMEM_CTX_TYPE_FETCH_NBI(float, float)
SHMEM_CTX_TYPE_FETCH_NBI(double, double)
SHMEM_CTX_TYPE_FETCH_NBI(int, int)
SHMEM_CTX_TYPE_FETCH_NBI(long, long)
SHMEM_CTX_TYPE_FETCH_NBI(longlong, long long)
SHMEM_CTX_TYPE_FETCH_NBI(uint, unsigned int)
SHMEM_CTX_TYPE_FETCH_NBI(ulong, unsigned long)
SHMEM_CTX_TYPE_FETCH_NBI(ulonglong, unsigned long long)
SHMEM_CTX_TYPE_FETCH_NBI(int32, int32_t)
SHMEM_CTX_TYPE_FETCH_NBI(int64, int64_t)
SHMEM_CTX_TYPE_FETCH_NBI(uint32, uint32_t)
SHMEM_CTX_TYPE_FETCH_NBI(uint64, uint64_t)
SHMEM_CTX_TYPE_FETCH_NBI(size, size_t)
SHMEM_CTX_TYPE_FETCH_NBI(ptrdiff, ptrdiff_t)

API_DEF_CONST_AMO1_NBI(fetch, float, float)
API_DEF_CONST_AMO1_NBI(fetch, double, double)
API_DEF_CONST_AMO1_NBI(fetch, int, int)
API_DEF_CONST_AMO1_NBI(fetch, long, long)
API_DEF_CONST_AMO1_NBI(fetch, longlong, long long)
API_DEF_CONST_AMO1_NBI(fetch, uint, unsigned int)
API_DEF_CONST_AMO1_NBI(fetch, ulong, unsigned long)
API_DEF_CONST_AMO1_NBI(fetch, ulonglong, unsigned long long)
API_DEF_CONST_AMO1_NBI(fetch, int32, int32_t)
API_DEF_CONST_AMO1_NBI(fetch, int64, int64_t)
API_DEF_CONST_AMO1_NBI(fetch, uint32, uint32_t)
API_DEF_CONST_AMO1_NBI(fetch, uint64, uint64_t)
API_DEF_CONST_AMO1_NBI(fetch, size, size_t)
API_DEF_CONST_AMO1_NBI(fetch, ptrdiff, ptrdiff_t)
