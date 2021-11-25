/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"
#include "shmemu.h"
#include "shmemc.h"
#include "common.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_int_atomic_inc = pshmem_ctx_int_atomic_inc
#define shmem_ctx_int_atomic_inc pshmem_ctx_int_atomic_inc
#pragma weak shmem_ctx_long_atomic_inc = pshmem_ctx_long_atomic_inc
#define shmem_ctx_long_atomic_inc pshmem_ctx_long_atomic_inc
#pragma weak shmem_ctx_longlong_atomic_inc = pshmem_ctx_longlong_atomic_inc
#define shmem_ctx_longlong_atomic_inc pshmem_ctx_longlong_atomic_inc
#pragma weak shmem_ctx_uint_atomic_inc = pshmem_ctx_uint_atomic_inc
#define shmem_ctx_uint_atomic_inc pshmem_ctx_uint_atomic_inc
#pragma weak shmem_ctx_ulong_atomic_inc = pshmem_ctx_ulong_atomic_inc
#define shmem_ctx_ulong_atomic_inc pshmem_ctx_ulong_atomic_inc
#pragma weak shmem_ctx_ulonglong_atomic_inc = pshmem_ctx_ulonglong_atomic_inc
#define shmem_ctx_ulonglong_atomic_inc pshmem_ctx_ulonglong_atomic_inc
#pragma weak shmem_ctx_int32_atomic_inc = pshmem_ctx_int32_atomic_inc
#define shmem_ctx_int32_atomic_inc pshmem_ctx_int32_atomic_inc
#pragma weak shmem_ctx_int64_atomic_inc = pshmem_ctx_int64_atomic_inc
#define shmem_ctx_int64_atomic_inc pshmem_ctx_int64_atomic_inc
#pragma weak shmem_ctx_uint32_atomic_inc = pshmem_ctx_uint32_atomic_inc
#define shmem_ctx_uint32_atomic_inc pshmem_ctx_uint32_atomic_inc
#pragma weak shmem_ctx_uint64_atomic_inc = pshmem_ctx_uint64_atomic_inc
#define shmem_ctx_uint64_atomic_inc pshmem_ctx_uint64_atomic_inc
#pragma weak shmem_ctx_size_atomic_inc = pshmem_ctx_size_atomic_inc
#define shmem_ctx_size_atomic_inc pshmem_ctx_size_atomic_inc
#pragma weak shmem_ctx_ptrdiff_atomic_inc = pshmem_ctx_ptrdiff_atomic_inc
#define shmem_ctx_ptrdiff_atomic_inc pshmem_ctx_ptrdiff_atomic_inc
#endif /* ENABLE_PSHMEM */

#define SHMEM_CTX_TYPE_INC(_name, _type)                                \
    void                                                                \
    shmem_ctx_##_name##_atomic_inc(shmem_ctx_t ctx,                     \
                                   _type *target,                       \
                                   int pe)                              \
    {                                                                   \
        _type one = 1;                                                  \
                                                                        \
        SHMEMT_MUTEX_NOPROTECT(shmemc_ctx_add(ctx,                      \
                                              target,                   \
                                              &one, sizeof(one),        \
                                              pe));                     \
    }

SHMEM_CTX_TYPE_INC(int, int)
SHMEM_CTX_TYPE_INC(long, long)
SHMEM_CTX_TYPE_INC(longlong, long long)
SHMEM_CTX_TYPE_INC(uint, unsigned int)
SHMEM_CTX_TYPE_INC(ulong, unsigned long)
SHMEM_CTX_TYPE_INC(ulonglong, unsigned long long)
SHMEM_CTX_TYPE_INC(int32, int32_t)
SHMEM_CTX_TYPE_INC(int64, int64_t)
SHMEM_CTX_TYPE_INC(uint32, uint32_t)
SHMEM_CTX_TYPE_INC(uint64, uint64_t)
SHMEM_CTX_TYPE_INC(size, size_t)
SHMEM_CTX_TYPE_INC(ptrdiff, ptrdiff_t)

API_DEF_VOID_AMO1(inc, int, int)
API_DEF_VOID_AMO1(inc, long, long)
API_DEF_VOID_AMO1(inc, longlong, long long)
API_DEF_VOID_AMO1(inc, uint, unsigned int)
API_DEF_VOID_AMO1(inc, ulong, unsigned long)
API_DEF_VOID_AMO1(inc, ulonglong, unsigned long long)
API_DEF_VOID_AMO1(inc, int32, int32_t)
API_DEF_VOID_AMO1(inc, int64, int64_t)
API_DEF_VOID_AMO1(inc, uint32, uint32_t)
API_DEF_VOID_AMO1(inc, uint64, uint64_t)
API_DEF_VOID_AMO1(inc, size, size_t)
API_DEF_VOID_AMO1(inc, ptrdiff, ptrdiff_t)
