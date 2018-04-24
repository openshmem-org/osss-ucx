/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"

#include "shmemu.h"
#include "shmemc.h"
#include "shmem/defs.h"

#include <bits/wordsize.h>
#include <sys/types.h>
#include <stdint.h>
#include <stddef.h>

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_float_atomic_swap = pshmem_ctx_float_atomic_swap
#define shmem_ctx_float_atomic_swap pshmem_ctx_float_atomic_swap
#pragma weak shmem_ctx_double_atomic_swap = pshmem_ctx_double_atomic_swap
#define shmem_ctx_double_atomic_swap pshmem_ctx_double_atomic_swap
#pragma weak shmem_ctx_int_atomic_swap = pshmem_ctx_int_atomic_swap
#define shmem_ctx_int_atomic_swap pshmem_ctx_int_atomic_swap
#pragma weak shmem_ctx_long_atomic_swap = pshmem_ctx_long_atomic_swap
#define shmem_ctx_long_atomic_swap pshmem_ctx_long_atomic_swap
#pragma weak shmem_ctx_longlong_atomic_swap = pshmem_ctx_longlong_atomic_swap
#define shmem_ctx_longlong_atomic_swap pshmem_ctx_longlong_atomic_swap
#pragma weak shmem_ctx_uint_atomic_swap = pshmem_ctx_uint_atomic_swap
#define shmem_ctx_uint_atomic_swap pshmem_ctx_uint_atomic_swap
#pragma weak shmem_ctx_ulong_atomic_swap = pshmem_ctx_ulong_atomic_swap
#define shmem_ctx_ulong_atomic_swap pshmem_ctx_ulong_atomic_swap
#pragma weak shmem_ctx_ulonglong_atomic_swap = pshmem_ctx_ulonglong_atomic_swap
#define shmem_ctx_ulonglong_atomic_swap pshmem_ctx_ulonglong_atomic_swap
#pragma weak shmem_ctx_int32_atomic_swap = pshmem_ctx_int32_atomic_swap
#define shmem_ctx_int32_atomic_swap pshmem_ctx_int32_atomic_swap
#pragma weak shmem_ctx_int64_atomic_swap = pshmem_ctx_int64_atomic_swap
#define shmem_ctx_int64_atomic_swap pshmem_ctx_int64_atomic_swap
#pragma weak shmem_ctx_uint32_atomic_swap = pshmem_ctx_uint32_atomic_swap
#define shmem_ctx_uint32_atomic_swap pshmem_ctx_uint32_atomic_swap
#pragma weak shmem_ctx_uint64_atomic_swap = pshmem_ctx_uint64_atomic_swap
#define shmem_ctx_uint64_atomic_swap pshmem_ctx_uint64_atomic_swap
#pragma weak shmem_ctx_size_atomic_swap = pshmem_ctx_size_atomic_swap
#define shmem_ctx_size_atomic_swap pshmem_ctx_size_atomic_swap
#pragma weak shmem_ctx_ptrdiff_atomic_swap = pshmem_ctx_ptrdiff_atomic_swap
#define shmem_ctx_ptrdiff_atomic_swap pshmem_ctx_ptrdiff_atomic_swap
#endif /* ENABLE_PSHMEM */

/*
 * swap
 */

#define SHMEM_CTX_TYPE_SWAP(_name, _type, _size)                        \
    _type                                                               \
    shmem_ctx_##_name##_atomic_swap(shmem_ctx_t ctx,                    \
                                    _type *target, _type value, int pe) \
    {                                                                   \
        _type v;                                                        \
                                                                        \
        SHMEMU_CHECK_INIT();                                            \
        SHMEMU_CHECK_SYMMETRIC(target, 2);                              \
                                                                        \
        logger(LOG_ATOMICS,                                             \
               "%s(ctx=%lu, target=%p, value=%lu, pe=%d)",              \
               __func__,                                                \
               shmemc_context_id(ctx), target, value, pe                \
               );                                                       \
                                                                        \
        SHMEMT_MUTEX_PROTECT(v = shmemc_ctx_swap##_size(ctx,            \
                                                        target, value,  \
                                                        pe));           \
        return v;                                                       \
    }
SHMEM_CTX_TYPE_SWAP(int, int, 32)
#if __WORDSIZE == 64
SHMEM_CTX_TYPE_SWAP(long, long, 64)
#else
SHMEM_CTX_TYPE_SWAP(long, long, 32)
#endif
SHMEM_CTX_TYPE_SWAP(longlong, long long, 64)
SHMEM_CTX_TYPE_SWAP(float, float, 32)
SHMEM_CTX_TYPE_SWAP(double, double, 64)
SHMEM_CTX_TYPE_SWAP(uint, unsigned int, 32)
SHMEM_CTX_TYPE_SWAP(ulong, unsigned long, 64)
SHMEM_CTX_TYPE_SWAP(ulonglong, unsigned long long, 64)
SHMEM_CTX_TYPE_SWAP(int32, int32_t, 32)
SHMEM_CTX_TYPE_SWAP(int64, int64_t, 64)
SHMEM_CTX_TYPE_SWAP(uint32, uint32_t, 32)
SHMEM_CTX_TYPE_SWAP(uint64, uint64_t, 64)
SHMEM_CTX_TYPE_SWAP(size, size_t, 64)
SHMEM_CTX_TYPE_SWAP(ptrdiff, ptrdiff_t, 64)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_int_atomic_compare_swap = pshmem_ctx_int_atomic_compare_swap
#define shmem_ctx_int_atomic_compare_swap pshmem_ctx_int_atomic_compare_swap
#pragma weak shmem_ctx_long_atomic_compare_swap = pshmem_ctx_long_atomic_compare_swap
#define shmem_ctx_long_atomic_compare_swap pshmem_ctx_long_atomic_compare_swap
#pragma weak shmem_ctx_longlong_atomic_compare_swap = pshmem_ctx_longlong_atomic_compare_swap
#define shmem_ctx_longlong_atomic_compare_swap pshmem_ctx_longlong_atomic_compare_swap
#pragma weak shmem_ctx_uint_atomic_compare_swap = pshmem_ctx_uint_atomic_compare_swap
#define shmem_ctx_uint_atomic_compare_swap pshmem_ctx_uint_atomic_compare_swap
#pragma weak shmem_ctx_ulong_atomic_compare_swap = pshmem_ctx_ulong_atomic_compare_swap
#define shmem_ctx_ulong_atomic_compare_swap pshmem_ctx_ulong_atomic_compare_swap
#pragma weak shmem_ctx_ulonglong_atomic_compare_swap = pshmem_ctx_ulonglong_atomic_compare_swap
#define shmem_ctx_ulonglong_atomic_compare_swap pshmem_ctx_ulonglong_atomic_compare_swap
#pragma weak shmem_ctx_int32_atomic_compare_swap = pshmem_ctx_int32_atomic_compare_swap
#define shmem_ctx_int32_atomic_compare_swap pshmem_ctx_int32_atomic_compare_swap
#pragma weak shmem_ctx_int64_atomic_compare_swap = pshmem_ctx_int64_atomic_compare_swap
#define shmem_ctx_int64_atomic_compare_swap pshmem_ctx_int64_atomic_compare_swap
#pragma weak shmem_ctx_uint32_atomic_compare_swap = pshmem_ctx_uint32_atomic_compare_swap
#define shmem_ctx_uint32_atomic_compare_swap pshmem_ctx_uint32_atomic_compare_swap
#pragma weak shmem_ctx_uint64_atomic_compare_swap = pshmem_ctx_uint64_atomic_compare_swap
#define shmem_ctx_uint64_atomic_compare_swap pshmem_ctx_uint64_atomic_compare_swap
#pragma weak shmem_ctx_size_atomic_compare_swap = pshmem_ctx_size_atomic_compare_swap
#define shmem_ctx_size_atomic_compare_swap pshmem_ctx_size_atomic_compare_swap
#pragma weak shmem_ctx_ptrdiff_atomic_compare_swap = pshmem_ctx_ptrdiff_atomic_compare_swap
#define shmem_ctx_ptrdiff_atomic_compare_swap pshmem_ctx_ptrdiff_atomic_compare_swap
#endif /* ENABLE_PSHMEM */

/*
 * conditional swap
 */

#define SHMEM_CTX_TYPE_CSWAP(_name, _type, _size)                       \
    _type                                                               \
    shmem_ctx_##_name##_atomic_compare_swap(shmem_ctx_t ctx,            \
                                            _type *target,              \
                                            _type cond, _type value,    \
                                            int pe)                     \
    {                                                                   \
        _type v;                                                        \
                                                                        \
        logger(LOG_ATOMICS,                                             \
               "%s(ctx=%lu, target=%p, cond=%lu, value=%lu, pe=%d)",    \
               __func__,                                                \
               shmemc_context_id(ctx), target, cond, value, pe          \
               );                                                       \
                                                                        \
        SHMEMT_MUTEX_PROTECT(v = shmemc_ctx_cswap##_size(ctx,           \
                                                         target, cond,  \
                                                         value, pe));   \
        return v;                                                       \
    }

SHMEM_CTX_TYPE_CSWAP(int, int, 32)
#if __WORDSIZE == 64
SHMEM_CTX_TYPE_CSWAP(long, long, 64)
#else
SHMEM_CTX_TYPE_CSWAP(long, long, 32)
#endif
SHMEM_CTX_TYPE_CSWAP(longlong, long long, 64)
SHMEM_CTX_TYPE_CSWAP(uint, unsigned int, 32)
SHMEM_CTX_TYPE_CSWAP(ulong, unsigned long, 64)
SHMEM_CTX_TYPE_CSWAP(ulonglong, unsigned long long, 64)
SHMEM_CTX_TYPE_CSWAP(int32, int32_t, 32)
SHMEM_CTX_TYPE_CSWAP(int64, int64_t, 64)
SHMEM_CTX_TYPE_CSWAP(uint32, uint32_t, 32)
SHMEM_CTX_TYPE_CSWAP(uint64, uint64_t, 64)
SHMEM_CTX_TYPE_CSWAP(size, size_t, 64)
SHMEM_CTX_TYPE_CSWAP(ptrdiff, ptrdiff_t, 64)

#undef SHMEM_CTX_TYPE_CSWAP

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

#define SHMEM_CTX_TYPE_FADD(_name, _type, _size)                        \
    _type                                                               \
    shmem_ctx_##_name##_atomic_fetch_add(shmem_ctx_t ctx,               \
                                         _type *target,                 \
                                         _type value, int pe)           \
    {                                                                   \
        _type v;                                                        \
                                                                        \
        logger(LOG_ATOMICS,                                             \
               "%s(ctx=%lu, target=%p, value=%lu, pe=%d)",              \
               __func__,                                                \
               shmemc_context_id(ctx), target, value, pe                \
               );                                                       \
                                                                        \
        SHMEMT_MUTEX_PROTECT(v = shmemc_ctx_fadd##_size(ctx,            \
                                                        target, value,  \
                                                        pe));           \
        return v;                                                       \
    }

SHMEM_CTX_TYPE_FADD(int, int, 32)
#if __WORDSIZE == 64
SHMEM_CTX_TYPE_FADD(long, long, 64)
#else
SHMEM_CTX_TYPE_FADD(long, long, 32)
#endif
SHMEM_CTX_TYPE_FADD(longlong, long long, 64)
SHMEM_CTX_TYPE_FADD(uint, unsigned int, 32)
SHMEM_CTX_TYPE_FADD(ulong, unsigned long, 64)
SHMEM_CTX_TYPE_FADD(ulonglong, unsigned long long, 64)
SHMEM_CTX_TYPE_FADD(int32, int32_t, 32)
SHMEM_CTX_TYPE_FADD(int64, int64_t, 64)
SHMEM_CTX_TYPE_FADD(uint32, uint32_t, 32)
SHMEM_CTX_TYPE_FADD(uint64, uint64_t, 64)
SHMEM_CTX_TYPE_FADD(size, size_t, 64)
SHMEM_CTX_TYPE_FADD(ptrdiff, ptrdiff_t, 64)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_int_atomic_fetch_inc = pshmem_ctx_int_atomic_fetch_inc
#define shmem_ctx_int_atomic_fetch_inc pshmem_ctx_int_atomic_fetch_inc
#pragma weak shmem_ctx_long_atomic_fetch_inc = pshmem_ctx_long_atomic_fetch_inc
#define shmem_ctx_long_atomic_fetch_inc pshmem_ctx_long_atomic_fetch_inc
#pragma weak shmem_ctx_longlong_atomic_fetch_inc = pshmem_ctx_longlong_atomic_fetch_inc
#define shmem_ctx_longlong_atomic_fetch_inc pshmem_ctx_longlong_atomic_fetch_inc
#pragma weak shmem_ctx_uint_atomic_fetch_inc = pshmem_ctx_uint_atomic_fetch_inc
#define shmem_ctx_uint_atomic_fetch_inc pshmem_ctx_uint_atomic_fetch_inc
#pragma weak shmem_ctx_ulong_atomic_fetch_inc = pshmem_ctx_ulong_atomic_fetch_inc
#define shmem_ctx_ulong_atomic_fetch_inc pshmem_ctx_ulong_atomic_fetch_inc
#pragma weak shmem_ctx_ulonglong_atomic_fetch_inc = pshmem_ctx_ulonglong_atomic_fetch_inc
#define shmem_ctx_ulonglong_atomic_fetch_inc pshmem_ctx_ulonglong_atomic_fetch_inc
#pragma weak shmem_ctx_int32_atomic_fetch_inc = pshmem_ctx_int32_atomic_fetch_inc
#define shmem_ctx_int32_atomic_fetch_inc pshmem_ctx_int32_atomic_fetch_inc
#pragma weak shmem_ctx_int64_atomic_fetch_inc = pshmem_ctx_int64_atomic_fetch_inc
#define shmem_ctx_int64_atomic_fetch_inc pshmem_ctx_int64_atomic_fetch_inc
#pragma weak shmem_ctx_uint32_atomic_fetch_inc = pshmem_ctx_uint32_atomic_fetch_inc
#define shmem_ctx_uint32_atomic_fetch_inc pshmem_ctx_uint32_atomic_fetch_inc
#pragma weak shmem_ctx_uint64_atomic_fetch_inc = pshmem_ctx_uint64_atomic_fetch_inc
#define shmem_ctx_uint64_atomic_fetch_inc pshmem_ctx_uint64_atomic_fetch_inc
#pragma weak shmem_ctx_size_atomic_fetch_inc = pshmem_ctx_size_atomic_fetch_inc
#define shmem_ctx_size_atomic_fetch_inc pshmem_ctx_size_atomic_fetch_inc
#pragma weak shmem_ctx_ptrdiff_atomic_fetch_inc = pshmem_ctx_ptrdiff_atomic_fetch_inc
#define shmem_ctx_ptrdiff_atomic_fetch_inc pshmem_ctx_ptrdiff_atomic_fetch_inc
#endif /* ENABLE_PSHMEM */

/*
 * fetch-and-increment
 */

#define SHMEM_CTX_TYPE_FINC(_name, _type, _size)                        \
    _type                                                               \
    shmem_ctx_##_name##_atomic_fetch_inc(shmem_ctx_t ctx,               \
                                         _type *target, int pe)         \
    {                                                                   \
        _type v;                                                        \
                                                                        \
        logger(LOG_ATOMICS,                                             \
               "%s(ctx=%lu, target=%p, pe=%d)",                         \
               __func__,                                                \
               shmemc_context_id(ctx), target, pe                       \
               );                                                       \
                                                                        \
        SHMEMT_MUTEX_PROTECT(v = shmemc_ctx_finc##_size(ctx,            \
                                                        target, pe));   \
                                                                        \
        return v;                                                       \
    }

SHMEM_CTX_TYPE_FINC(int, int, 32)
#if __WORDSIZE == 64
SHMEM_CTX_TYPE_FINC(long, long, 64)
#else
SHMEM_CTX_TYPE_FINC(long, long, 32)
#endif
SHMEM_CTX_TYPE_FINC(longlong, long long, 64)
SHMEM_CTX_TYPE_FINC(uint, unsigned int, 32)
SHMEM_CTX_TYPE_FINC(ulong, unsigned long, 64)
SHMEM_CTX_TYPE_FINC(ulonglong, unsigned long long, 64)
SHMEM_CTX_TYPE_FINC(int32, int32_t, 32)
SHMEM_CTX_TYPE_FINC(int64, int64_t, 64)
SHMEM_CTX_TYPE_FINC(uint32, uint32_t, 32)
SHMEM_CTX_TYPE_FINC(uint64, uint64_t, 64)
SHMEM_CTX_TYPE_FINC(size, size_t, 64)
SHMEM_CTX_TYPE_FINC(ptrdiff, ptrdiff_t, 64)

#undef SHMEM_CTX_TYPE_FINC

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_int_atomic_add = pshmem_ctx_int_atomic_add
#define shmem_ctx_int_atomic_add pshmem_ctx_int_atomic_add
#pragma weak shmem_ctx_long_atomic_add = pshmem_ctx_long_atomic_add
#define shmem_ctx_long_atomic_add pshmem_ctx_long_atomic_add
#pragma weak shmem_ctx_longlong_atomic_add = pshmem_ctx_longlong_atomic_add
#define shmem_ctx_longlong_atomic_add pshmem_ctx_longlong_atomic_add
#pragma weak shmem_ctx_uint_atomic_add = pshmem_ctx_uint_atomic_add
#define shmem_ctx_uint_atomic_add pshmem_ctx_uint_atomic_add
#pragma weak shmem_ctx_ulong_atomic_add = pshmem_ctx_ulong_atomic_add
#define shmem_ctx_ulong_atomic_add pshmem_ctx_ulong_atomic_add
#pragma weak shmem_ctx_ulonglong_atomic_add = pshmem_ctx_ulonglong_atomic_add
#define shmem_ctx_ulonglong_atomic_add pshmem_ctx_ulonglong_atomic_add
#pragma weak shmem_ctx_int32_atomic_add = pshmem_ctx_int32_atomic_add
#define shmem_ctx_int32_atomic_add pshmem_ctx_int32_atomic_add
#pragma weak shmem_ctx_int64_atomic_add = pshmem_ctx_int64_atomic_add
#define shmem_ctx_int64_atomic_add pshmem_ctx_int64_atomic_add
#pragma weak shmem_ctx_uint32_atomic_add = pshmem_ctx_uint32_atomic_add
#define shmem_ctx_uint32_atomic_add pshmem_ctx_uint32_atomic_add
#pragma weak shmem_ctx_uint64_atomic_add = pshmem_ctx_uint64_atomic_add
#define shmem_ctx_uint64_atomic_add pshmem_ctx_uint64_atomic_add
#pragma weak shmem_ctx_size_atomic_add = pshmem_ctx_size_atomic_add
#define shmem_ctx_size_atomic_add pshmem_ctx_size_atomic_add
#pragma weak shmem_ctx_ptrdiff_atomic_add = pshmem_ctx_ptrdiff_atomic_add
#define shmem_ctx_ptrdiff_atomic_add pshmem_ctx_ptrdiff_atomic_add
#endif /* ENABLE_PSHMEM */

/*
 * add
 */

#define SHMEM_CTX_TYPE_ADD(_name, _type, _size)                         \
    void                                                                \
    shmem_ctx_##_name##_atomic_add(shmem_ctx_t ctx,                     \
                                   _type *target, _type value, int pe)  \
    {                                                                   \
        logger(LOG_ATOMICS,                                             \
               "%s(ctx=%lu, target=%p, value=%lu, pe=%d)",              \
               __func__,                                                \
               shmemc_context_id(ctx), target, value, pe                \
               );                                                       \
                                                                        \
        SHMEMT_MUTEX_PROTECT(shmemc_ctx_add##_size(ctx,                 \
                                                   target, value,       \
                                                   pe));                \
    }

SHMEM_CTX_TYPE_ADD(int, int, 32)
#if __WORDSIZE == 64
SHMEM_CTX_TYPE_ADD(long, long, 64)
#else
SHMEM_CTX_TYPE_ADD(long, long, 32)
#endif
SHMEM_CTX_TYPE_ADD(longlong, long long, 64)
SHMEM_CTX_TYPE_ADD(uint, unsigned int, 32)
SHMEM_CTX_TYPE_ADD(ulong, unsigned long, 64)
SHMEM_CTX_TYPE_ADD(ulonglong, unsigned long long, 64)
SHMEM_CTX_TYPE_ADD(int32, int32_t, 32)
SHMEM_CTX_TYPE_ADD(int64, int64_t, 64)
SHMEM_CTX_TYPE_ADD(uint32, uint32_t, 32)
SHMEM_CTX_TYPE_ADD(uint64, uint64_t, 64)
SHMEM_CTX_TYPE_ADD(size, size_t, 64)
SHMEM_CTX_TYPE_ADD(ptrdiff, ptrdiff_t, 64)

#undef SHMEM_CTX_TYPE_ADD

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

/*
 * increment
 */

#define SHMEM_CTX_TYPE_INC(_name, _type, _size)                         \
    void                                                                \
    shmem_ctx_##_name##_atomic_inc(shmem_ctx_t ctx,                     \
                                   _type *target, int pe)               \
    {                                                                   \
        logger(LOG_ATOMICS,                                             \
               "%s(ctx=%lu, target=%p, pe=%d)",                         \
               __func__,                                                \
               shmemc_context_id(ctx), target, pe                       \
               );                                                       \
                                                                        \
        SHMEMT_MUTEX_PROTECT(shmemc_ctx_inc##_size(ctx, target, pe));   \
    }

SHMEM_CTX_TYPE_INC(int, int, 32)
#if __WORDSIZE == 64
SHMEM_CTX_TYPE_INC(long, long, 64)
#else
SHMEM_CTX_TYPE_INC(long, long, 32)
#endif
SHMEM_CTX_TYPE_INC(longlong, long long, 64)
SHMEM_CTX_TYPE_INC(uint, unsigned int, 32)
SHMEM_CTX_TYPE_INC(ulong, unsigned long, 64)
SHMEM_CTX_TYPE_INC(ulonglong, unsigned long long, 64)
SHMEM_CTX_TYPE_INC(int32, int32_t, 32)
SHMEM_CTX_TYPE_INC(int64, int64_t, 64)
SHMEM_CTX_TYPE_INC(uint32, uint32_t, 32)
SHMEM_CTX_TYPE_INC(uint64, uint64_t, 64)
SHMEM_CTX_TYPE_INC(size, size_t, 64)
SHMEM_CTX_TYPE_INC(ptrdiff, ptrdiff_t, 64)

#undef SHMEM_CTX_TYPE_INC

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
#pragma weak shmem_ctx_size_atomic_fetch = pshmem_ctx_size_atomic_fetch
#define shmem_ctx_size_atomic_fetch pshmem_ctx_size_atomic_fetch
#pragma weak shmem_ctx_ptrdiff_atomic_fetch = pshmem_ctx_ptrdiff_atomic_fetch
#define shmem_ctx_ptrdiff_atomic_fetch pshmem_ctx_ptrdiff_atomic_fetch
#endif /* ENABLE_PSHMEM */

/*
 * fetch
 */

#define SHMEM_CTX_TYPE_FETCH(_name, _type, _size)                       \
    _type                                                               \
    shmem_ctx_##_name##_atomic_fetch(shmem_ctx_t ctx,                   \
                                     _type *target, int pe)             \
    {                                                                   \
        _type v;                                                        \
                                                                        \
        logger(LOG_ATOMICS,                                             \
               "%s(ctx=%lu, target=%p, pe=%d)",                         \
               __func__,                                                \
               shmemc_context_id(ctx), target, pe                       \
               );                                                       \
                                                                        \
        SHMEMT_MUTEX_PROTECT(v = shmemc_ctx_fetch##_size(ctx, target, pe)); \
        return v;                                                       \
    }

SHMEM_CTX_TYPE_FETCH(float, float, 32)
SHMEM_CTX_TYPE_FETCH(double, double, 64)
SHMEM_CTX_TYPE_FETCH(int, int, 32)
#if __WORDSIZE == 64
SHMEM_CTX_TYPE_FETCH(long, long, 64)
#else
SHMEM_CTX_TYPE_FETCH(long, long, 32)
#endif
SHMEM_CTX_TYPE_FETCH(longlong, long long, 64)
SHMEM_CTX_TYPE_FETCH(uint, unsigned int, 32)
SHMEM_CTX_TYPE_FETCH(ulong, unsigned long, 64)
SHMEM_CTX_TYPE_FETCH(ulonglong, unsigned long long, 64)
SHMEM_CTX_TYPE_FETCH(int32, int32_t, 32)
SHMEM_CTX_TYPE_FETCH(int64, int64_t, 64)
SHMEM_CTX_TYPE_FETCH(uint32, uint32_t, 32)
SHMEM_CTX_TYPE_FETCH(uint64, uint64_t, 64)
SHMEM_CTX_TYPE_FETCH(size, size_t, 64)
SHMEM_CTX_TYPE_FETCH(ptrdiff, ptrdiff_t, 64)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_int_atomic_set = pshmem_ctx_int_atomic_set
#define shmem_ctx_int_atomic_set pshmem_ctx_int_atomic_set
#pragma weak shmem_ctx_long_atomic_set = pshmem_ctx_long_atomic_set
#define shmem_ctx_long_atomic_set pshmem_ctx_long_atomic_set
#pragma weak shmem_ctx_longlong_atomic_set = pshmem_ctx_longlong_atomic_set
#define shmem_ctx_longlong_atomic_set pshmem_ctx_longlong_atomic_set
#pragma weak shmem_ctx_float_atomic_set = pshmem_ctx_float_atomic_set
#define shmem_ctx_float_atomic_set pshmem_ctx_float_atomic_set
#pragma weak shmem_ctx_double_atomic_set = pshmem_ctx_double_atomic_set
#define shmem_ctx_double_atomic_set pshmem_ctx_double_atomic_set
#pragma weak shmem_ctx_uint_atomic_set = pshmem_ctx_uint_atomic_set
#define shmem_ctx_uint_atomic_set pshmem_ctx_uint_atomic_set
#pragma weak shmem_ctx_ulong_atomic_set = pshmem_ctx_ulong_atomic_set
#define shmem_ctx_ulong_atomic_set pshmem_ctx_ulong_atomic_set
#pragma weak shmem_ctx_ulonglong_atomic_set = pshmem_ctx_ulonglong_atomic_set
#define shmem_ctx_ulonglong_atomic_set pshmem_ctx_ulonglong_atomic_set
#pragma weak shmem_ctx_int32_atomic_set = pshmem_ctx_int32_atomic_set
#define shmem_ctx_int32_atomic_set pshmem_ctx_int32_atomic_set
#pragma weak shmem_ctx_int64_atomic_set = pshmem_ctx_int64_atomic_set
#define shmem_ctx_int64_atomic_set pshmem_ctx_int64_atomic_set
#pragma weak shmem_ctx_size_atomic_set = pshmem_ctx_size_atomic_set
#define shmem_ctx_size_atomic_set pshmem_ctx_size_atomic_set
#pragma weak shmem_ctx_ptrdiff_atomic_set = pshmem_ctx_ptrdiff_atomic_set
#define shmem_ctx_ptrdiff_atomic_set pshmem_ctx_ptrdiff_atomic_set
#endif /* ENABLE_PSHMEM */

/*
 * set
 */

#define SHMEM_CTX_TYPE_SET(_name, _type, _size)                         \
    void                                                                \
    shmem_ctx_##_name##_atomic_set(shmem_ctx_t ctx,                     \
                                   _type *target, _type value, int pe)  \
    {                                                                   \
        logger(LOG_ATOMICS,                                             \
               "%s(ctx=%lu, target=%p, value=%lu, pe=%d)",              \
               __func__,                                                \
               shmemc_context_id(ctx), target, value, pe                \
               );                                                       \
                                                                        \
        SHMEMT_MUTEX_PROTECT(shmemc_ctx_set##_size(ctx, target, value, pe)); \
    }

SHMEM_CTX_TYPE_SET(float, float, 32)
SHMEM_CTX_TYPE_SET(double, double, 64)
SHMEM_CTX_TYPE_SET(int, int, 32)
#if __WORDSIZE == 64
SHMEM_CTX_TYPE_SET(long, long, 64)
#else
SHMEM_CTX_TYPE_SET(long, long, 32)
#endif
SHMEM_CTX_TYPE_SET(longlong, long long, 64)
SHMEM_CTX_TYPE_SET(uint, unsigned int, 32)
SHMEM_CTX_TYPE_SET(ulong, unsigned long, 64)
SHMEM_CTX_TYPE_SET(ulonglong, unsigned long long, 64)
SHMEM_CTX_TYPE_SET(int32, int32_t, 32)
SHMEM_CTX_TYPE_SET(int64, int64_t, 64)
SHMEM_CTX_TYPE_SET(uint32, uint32_t, 32)
SHMEM_CTX_TYPE_SET(uint64, uint64_t, 64)
SHMEM_CTX_TYPE_SET(size, size_t, 64)
SHMEM_CTX_TYPE_SET(ptrdiff, ptrdiff_t, 64)

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

#pragma weak shmem_ctx_uint_atomic_and = pshmem_ctx_uint_atomic_and
#define shmem_ctx_uint_atomic_and pshmem_ctx_uint_atomic_and
#pragma weak shmem_ctx_ulong_atomic_and = pshmem_ctx_ulong_atomic_and
#define shmem_ctx_ulong_atomic_and pshmem_ctx_ulong_atomic_and
#pragma weak shmem_ctx_ulonglong_atomic_and = pshmem_ctx_ulonglong_atomic_and
#define shmem_ctx_ulonglong_atomic_and pshmem_ctx_ulonglong_atomic_and
#pragma weak shmem_ctx_int32_atomic_xor = pshmem_ctx_int32_atomic_xor
#define shmem_ctx_int32_atomic_and pshmem_ctx_int32_atomic_and
#pragma weak shmem_ctx_int64_atomic_and = pshmem_ctx_int64_atomic_and
#define shmem_ctx_int64_atomic_and pshmem_ctx_int64_atomic_and
#pragma weak shmem_ctx_uint32_atomic_and = pshmem_ctx_uint32_atomic_and
#define shmem_ctx_uint32_atomic_and pshmem_ctx_uint32_atomic_and
#pragma weak shmem_ctx_uint64_atomic_and = pshmem_ctx_uint64_atomic_and
#define shmem_ctx_uint64_atomic_and pshmem_ctx_uint64_atomic_and
#endif /* ENABLE_PSHMEM */

/*
 * bitwise
 */

#define SHMEM_CTX_TYPE_BITWISE(_opname, _name, _type, _size)            \
    void                                                                \
    shmem_ctx_##_name##_atomic_##_opname(shmem_ctx_t ctx,               \
                                         _type *target,                 \
                                         _type value, int pe)           \
    {                                                                   \
        logger(LOG_ATOMICS,                                             \
               "%s(ctx=%lu, target=%p, value=%lu, pe=%d)",              \
               __func__,                                                \
               shmemc_context_id(ctx), target, value, pe                \
               );                                                       \
                                                                        \
        SHMEMT_MUTEX_PROTECT(shmemc_ctx_##_opname##_size(ctx, target,   \
                                                         value, pe));   \
    }

SHMEM_CTX_TYPE_BITWISE(xor, uint, unsigned int, 32)
SHMEM_CTX_TYPE_BITWISE(xor, ulong, unsigned long, 64)
SHMEM_CTX_TYPE_BITWISE(xor, ulonglong, unsigned long long, 64)
SHMEM_CTX_TYPE_BITWISE(xor, int32, int32_t, 32)
SHMEM_CTX_TYPE_BITWISE(xor, int64, int64_t, 64)
SHMEM_CTX_TYPE_BITWISE(xor, uint32, uint32_t, 32)
SHMEM_CTX_TYPE_BITWISE(xor, uint64, uint64_t, 64)

SHMEM_CTX_TYPE_BITWISE(or, uint, unsigned int, 32)
SHMEM_CTX_TYPE_BITWISE(or, ulong, unsigned long, 64)
SHMEM_CTX_TYPE_BITWISE(or, ulonglong, unsigned long long, 64)
SHMEM_CTX_TYPE_BITWISE(or, int32, int32_t, 32)
SHMEM_CTX_TYPE_BITWISE(or, int64, int64_t, 64)
SHMEM_CTX_TYPE_BITWISE(or, uint32, uint32_t, 32)
SHMEM_CTX_TYPE_BITWISE(or, uint64, uint64_t, 64)

SHMEM_CTX_TYPE_BITWISE(and, uint, unsigned int, 32)
SHMEM_CTX_TYPE_BITWISE(and, ulong, unsigned long, 64)
SHMEM_CTX_TYPE_BITWISE(and, ulonglong, unsigned long long, 64)
SHMEM_CTX_TYPE_BITWISE(and, int32, int32_t, 32)
SHMEM_CTX_TYPE_BITWISE(and, int64, int64_t, 64)
SHMEM_CTX_TYPE_BITWISE(and, uint32, uint32_t, 32)
SHMEM_CTX_TYPE_BITWISE(and, uint64, uint64_t, 64)


#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_uint_atomic_fetch_xor = pshmem_ctx_uint_atomic_fetch_xor
#define shmem_ctx_uint_atomic_fetch_xor pshmem_ctx_uint_atomic_fetch_xor
#pragma weak shmem_ctx_ulong_atomic_fetch_xor = pshmem_ctx_ulong_atomic_fetch_xor
#define shmem_ctx_ulong_atomic_fetch_xor pshmem_ctx_ulong_atomic_fetch_xor
#pragma weak shmem_ctx_ulonglong_atomic_fetch_xor = pshmem_ctx_ulonglong_atomic_fetch_xor
#define shmem_ctx_ulonglong_atomic_fetch_xor pshmem_ctx_ulonglong_atomic_fetch_xor
#pragma weak shmem_ctx_int32_atomic_fetch_xor = pshmem_ctx_int32_atomic_fetch_xor
#define shmem_ctx_int32_atomic_fetch_xor pshmem_ctx_int32_atomic_fetch_xor
#pragma weak shmem_ctx_int64_atomic_fetch_xor = pshmem_ctx_int64_atomic_fetch_xor
#define shmem_ctx_int64_atomic_fetch_xor pshmem_ctx_int64_atomic_fetch_xor
#pragma weak shmem_ctx_uint32_atomic_fetch_xor = pshmem_ctx_uint32_atomic_fetch_xor
#define shmem_ctx_uint32_atomic_fetch_xor pshmem_ctx_uint32_atomic_fetch_xor
#pragma weak shmem_ctx_uint64_atomic_fetch_xor = pshmem_ctx_uint64_atomic_fetch_xor
#define shmem_ctx_uint64_atomic_fetch_xor pshmem_ctx_uint64_atomic_fetch_xor

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

/*
 * fetch-bitwise
 */

#define SHMEM_CTX_TYPE_FETCH_BITWISE(_opname, _name, _type, _size)      \
    _type                                                               \
    shmem_ctx_##_name##_atomic_fetch_##_opname(shmem_ctx_t ctx,         \
                                               _type *target,           \
                                               _type value, int pe)     \
    {                                                                   \
        _type v;                                                        \
                                                                        \
        logger(LOG_ATOMICS,                                             \
               "%s(ctx=%lu, target=%p, value=%lu, pe=%d)",              \
               __func__,                                                \
               shmemc_context_id(ctx), target, value, pe                \
               );                                                       \
                                                                        \
        SHMEMT_MUTEX_PROTECT(v = shmemc_ctx_fetch_##_opname##_size(ctx, \
                                                                   target, \
                                                                   value, \
                                                                   pe)); \
        return v;                                                       \
    }

SHMEM_CTX_TYPE_FETCH_BITWISE(xor, uint, unsigned int, 32)
SHMEM_CTX_TYPE_FETCH_BITWISE(xor, ulong, unsigned long, 64)
SHMEM_CTX_TYPE_FETCH_BITWISE(xor, ulonglong, unsigned long long, 64)
SHMEM_CTX_TYPE_FETCH_BITWISE(xor, int32, int32_t, 32)
SHMEM_CTX_TYPE_FETCH_BITWISE(xor, int64, int64_t, 64)
SHMEM_CTX_TYPE_FETCH_BITWISE(xor, uint32, uint32_t, 32)
SHMEM_CTX_TYPE_FETCH_BITWISE(xor, uint64, uint64_t, 64)

SHMEM_CTX_TYPE_FETCH_BITWISE(or, uint, unsigned int, 32)
SHMEM_CTX_TYPE_FETCH_BITWISE(or, ulong, unsigned long, 64)
SHMEM_CTX_TYPE_FETCH_BITWISE(or, ulonglong, unsigned long long, 64)
SHMEM_CTX_TYPE_FETCH_BITWISE(or, int32, int32_t, 32)
SHMEM_CTX_TYPE_FETCH_BITWISE(or, int64, int64_t, 64)
SHMEM_CTX_TYPE_FETCH_BITWISE(or, uint32, uint32_t, 32)
SHMEM_CTX_TYPE_FETCH_BITWISE(or, uint64, uint64_t, 64)

SHMEM_CTX_TYPE_FETCH_BITWISE(and, uint, unsigned int, 32)
SHMEM_CTX_TYPE_FETCH_BITWISE(and, ulong, unsigned long, 64)
SHMEM_CTX_TYPE_FETCH_BITWISE(and, ulonglong, unsigned long long, 64)
SHMEM_CTX_TYPE_FETCH_BITWISE(and, int32, int32_t, 32)
SHMEM_CTX_TYPE_FETCH_BITWISE(and, int64, int64_t, 64)
SHMEM_CTX_TYPE_FETCH_BITWISE(and, uint32, uint32_t, 32)
SHMEM_CTX_TYPE_FETCH_BITWISE(and, uint64, uint64_t, 64)


/* ------------------------------------------------------------------------ */

#define API_DEF_AMO1(_op, _name, _type)                                 \
    _type shmem_##_name##_atomic_##_op(_type *target, int pe)           \
    {                                                                   \
        return shmem_ctx_##_name##_atomic_##_op(SHMEM_CTX_DEFAULT,      \
                                                target, pe);            \
    }

#define API_DEF_AMO2(_op, _name, _type)                                 \
    _type shmem_##_name##_atomic_##_op(_type *target, _type value,      \
                                       int pe)                          \
    {                                                                   \
        return shmem_ctx_##_name##_atomic_##_op(SHMEM_CTX_DEFAULT,      \
                                                target, value, pe);     \
    }

#define API_DEF_AMO3(_op, _name, _type)                                 \
    _type shmem_##_name##_atomic_##_op(_type *target,                   \
                                       _type cond, _type value,         \
                                       int pe)                          \
    {                                                                   \
        return shmem_ctx_##_name##_atomic_##_op(SHMEM_CTX_DEFAULT,      \
                                                target, cond, value, pe); \
    }

#define API_DEF_VOID_AMO1(_op, _name, _type)                            \
    void shmem_##_name##_atomic_##_op(_type *target, int pe)            \
    {                                                                   \
        shmem_ctx_##_name##_atomic_##_op(SHMEM_CTX_DEFAULT,             \
                                         target, pe);                   \
    }

#define API_DEF_VOID_AMO2(_op, _name, _type)                            \
    void shmem_##_name##_atomic_##_op(_type *target, _type value,       \
                                      int pe)                           \
    {                                                                   \
        shmem_ctx_##_name##_atomic_##_op(SHMEM_CTX_DEFAULT,             \
                                         target, value, pe);            \
    }

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_atomic_set = pshmem_int_atomic_set
#define shmem_int_atomic_set pshmem_int_atomic_set
#pragma weak shmem_long_atomic_set = pshmem_long_atomic_set
#define shmem_long_atomic_set pshmem_long_atomic_set
#pragma weak shmem_longlong_atomic_set = pshmem_longlong_atomic_set
#define shmem_longlong_atomic_set pshmem_longlong_atomic_set
#pragma weak shmem_float_atomic_set = pshmem_float_atomic_set
#define shmem_float_atomic_set pshmem_float_atomic_set
#pragma weak shmem_double_atomic_set = pshmem_double_atomic_set
#define shmem_double_atomic_set pshmem_double_atomic_set
#pragma weak shmem_uint_atomic_set = pshmem_uint_atomic_set
#define shmem_uint_atomic_set pshmem_uint_atomic_set
#pragma weak shmem_ulong_atomic_set = pshmem_ulong_atomic_set
#define shmem_ulong_atomic_set pshmem_ulong_atomic_set
#pragma weak shmem_ulonglong_atomic_set = pshmem_ulonglong_atomic_set
#define shmem_ulonglong_atomic_set pshmem_ulonglong_atomic_set
#pragma weak shmem_int32_atomic_set = pshmem_int32_atomic_set
#define shmem_int32_atomic_set pshmem_int32_atomic_set
#pragma weak shmem_int64_atomic_set = pshmem_int64_atomic_set
#define shmem_int64_atomic_set pshmem_int64_atomic_set
#pragma weak shmem_size_atomic_set = pshmem_size_atomic_set
#define shmem_size_atomic_set pshmem_size_atomic_set
#pragma weak shmem_ptrdiff_atomic_set = pshmem_ptrdiff_atomic_set
#define shmem_ptrdiff_atomic_set pshmem_ptrdiff_atomic_set
#endif /* ENABLE_PSHMEM */

API_DEF_VOID_AMO2(set, float, float)
API_DEF_VOID_AMO2(set, double, double)
API_DEF_VOID_AMO2(set, int, int)
API_DEF_VOID_AMO2(set, long, long)
API_DEF_VOID_AMO2(set, longlong, long long)
API_DEF_VOID_AMO2(set, uint, unsigned int)
API_DEF_VOID_AMO2(set, ulong, unsigned long)
API_DEF_VOID_AMO2(set, ulonglong, unsigned long long)
API_DEF_VOID_AMO2(set, int32, int32_t)
API_DEF_VOID_AMO2(set, int64, int64_t)
API_DEF_VOID_AMO2(set, uint32, uint32_t)
API_DEF_VOID_AMO2(set, uint64, uint64_t)
API_DEF_VOID_AMO2(set, size, size_t)
API_DEF_VOID_AMO2(set, ptrdiff, ptrdiff_t)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_atomic_inc = pshmem_int_atomic_inc
#define shmem_int_atomic_inc pshmem_int_atomic_inc
#pragma weak shmem_long_atomic_inc = pshmem_long_atomic_inc
#define shmem_long_atomic_inc pshmem_long_atomic_inc
#pragma weak shmem_longlong_atomic_inc = pshmem_longlong_atomic_inc
#define shmem_longlong_atomic_inc pshmem_longlong_atomic_inc
#pragma weak shmem_uint_atomic_inc = pshmem_uint_atomic_inc
#define shmem_uint_atomic_inc pshmem_uint_atomic_inc
#pragma weak shmem_ulong_atomic_inc = pshmem_ulong_atomic_inc
#define shmem_ulong_atomic_inc pshmem_ulong_atomic_inc
#pragma weak shmem_ulonglong_atomic_inc = pshmem_ulonglong_atomic_inc
#define shmem_ulonglong_atomic_inc pshmem_ulonglong_atomic_inc
#pragma weak shmem_int32_atomic_inc = pshmem_int32_atomic_inc
#define shmem_int32_atomic_inc pshmem_int32_atomic_inc
#pragma weak shmem_int64_atomic_inc = pshmem_int64_atomic_inc
#define shmem_int64_atomic_inc pshmem_int64_atomic_inc
#pragma weak shmem_uint32_atomic_inc = pshmem_uint32_atomic_inc
#define shmem_uint32_atomic_inc pshmem_uint32_atomic_inc
#pragma weak shmem_uint64_atomic_inc = pshmem_uint64_atomic_inc
#define shmem_uint64_atomic_inc pshmem_uint64_atomic_inc
#pragma weak shmem_size_atomic_inc = pshmem_size_atomic_inc
#define shmem_size_atomic_inc pshmem_size_atomic_inc
#pragma weak shmem_ptrdiff_atomic_inc = pshmem_ptrdiff_atomic_inc
#define shmem_ptrdiff_atomic_inc pshmem_ptrdiff_atomic_inc
#endif /* ENABLE_PSHMEM */

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

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_atomic_add = pshmem_int_atomic_add
#define shmem_int_atomic_add pshmem_int_atomic_add
#pragma weak shmem_long_atomic_add = pshmem_long_atomic_add
#define shmem_long_atomic_add pshmem_long_atomic_add
#pragma weak shmem_longlong_atomic_add = pshmem_longlong_atomic_add
#define shmem_longlong_atomic_add pshmem_longlong_atomic_add
#pragma weak shmem_uint_atomic_add = pshmem_uint_atomic_add
#define shmem_uint_atomic_add pshmem_uint_atomic_add
#pragma weak shmem_ulong_atomic_add = pshmem_ulong_atomic_add
#define shmem_ulong_atomic_add pshmem_ulong_atomic_add
#pragma weak shmem_ulonglong_atomic_add = pshmem_ulonglong_atomic_add
#define shmem_ulonglong_atomic_add pshmem_ulonglong_atomic_add
#pragma weak shmem_int32_atomic_add = pshmem_int32_atomic_add
#define shmem_int32_atomic_add pshmem_int32_atomic_add
#pragma weak shmem_int64_atomic_add = pshmem_int64_atomic_add
#define shmem_int64_atomic_add pshmem_int64_atomic_add
#pragma weak shmem_uint32_atomic_add = pshmem_uint32_atomic_add
#define shmem_uint32_atomic_add pshmem_uint32_atomic_add
#pragma weak shmem_uint64_atomic_add = pshmem_uint64_atomic_add
#define shmem_uint64_atomic_add pshmem_uint64_atomic_add
#pragma weak shmem_size_atomic_add = pshmem_size_atomic_add
#define shmem_size_atomic_add pshmem_size_atomic_add
#pragma weak shmem_ptrdiff_atomic_add = pshmem_ptrdiff_atomic_add
#define shmem_ptrdiff_atomic_add pshmem_ptrdiff_atomic_add
#endif /* ENABLE_PSHMEM */

API_DEF_VOID_AMO2(add, int, int)
API_DEF_VOID_AMO2(add, long, long)
API_DEF_VOID_AMO2(add, longlong, long long)
API_DEF_VOID_AMO2(add, uint, unsigned int)
API_DEF_VOID_AMO2(add, ulong, unsigned long)
API_DEF_VOID_AMO2(add, ulonglong, unsigned long long)
API_DEF_VOID_AMO2(add, int32, int32_t)
API_DEF_VOID_AMO2(add, int64, int64_t)
API_DEF_VOID_AMO2(add, uint32, uint32_t)
API_DEF_VOID_AMO2(add, uint64, uint64_t)
API_DEF_VOID_AMO2(add, size, size_t)
API_DEF_VOID_AMO2(add, ptrdiff, ptrdiff_t)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_atomic_fetch = pshmem_int_atomic_fetch
#define shmem_int_atomic_fetch pshmem_int_atomic_fetch
#pragma weak shmem_long_atomic_fetch = pshmem_long_atomic_fetch
#define shmem_long_atomic_fetch pshmem_long_atomic_fetch
#pragma weak shmem_longlong_atomic_fetch = pshmem_longlong_atomic_fetch
#define shmem_longlong_atomic_fetch pshmem_longlong_atomic_fetch
#pragma weak shmem_float_atomic_fetch = pshmem_float_atomic_fetch
#define shmem_float_atomic_fetch pshmem_float_atomic_fetch
#pragma weak shmem_double_atomic_fetch = pshmem_double_atomic_fetch
#define shmem_double_atomic_fetch pshmem_double_atomic_fetch
#pragma weak shmem_uint_atomic_fetch = pshmem_uint_atomic_fetch
#define shmem_uint_atomic_fetch pshmem_uint_atomic_fetch
#pragma weak shmem_ulong_atomic_fetch = pshmem_ulong_atomic_fetch
#define shmem_ulong_atomic_fetch pshmem_ulong_atomic_fetch
#pragma weak shmem_ulonglong_atomic_fetch = pshmem_ulonglong_atomic_fetch
#define shmem_ulonglong_atomic_fetch pshmem_ulonglong_atomic_fetch
#pragma weak shmem_int32_atomic_fetch = pshmem_int32_atomic_fetch
#define shmem_int32_atomic_fetch pshmem_int32_atomic_fetch
#pragma weak shmem_int64_atomic_fetch = pshmem_int64_atomic_fetch
#define shmem_int64_atomic_fetch pshmem_int64_atomic_fetch
#pragma weak shmem_size_atomic_fetch = pshmem_size_atomic_fetch
#define shmem_size_atomic_fetch pshmem_size_atomic_fetch
#pragma weak shmem_ptrdiff_atomic_fetch = pshmem_ptrdiff_atomic_fetch
#define shmem_ptrdiff_atomic_fetch pshmem_ptrdiff_atomic_fetch
#endif /* ENABLE_PSHMEM */

API_DEF_AMO1(fetch, float, float)
API_DEF_AMO1(fetch, double, double)
API_DEF_AMO1(fetch, int, int)
API_DEF_AMO1(fetch, long, long)
API_DEF_AMO1(fetch, longlong, long long)
API_DEF_AMO1(fetch, uint, unsigned int)
API_DEF_AMO1(fetch, ulong, unsigned long)
API_DEF_AMO1(fetch, ulonglong, unsigned long long)
API_DEF_AMO1(fetch, int32, int32_t)
API_DEF_AMO1(fetch, int64, int64_t)
API_DEF_AMO1(fetch, uint32, uint32_t)
API_DEF_AMO1(fetch, uint64, uint64_t)
API_DEF_AMO1(fetch, size, size_t)
API_DEF_AMO1(fetch, ptrdiff, ptrdiff_t)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_atomic_fetch_inc = pshmem_int_atomic_fetch_inc
#define shmem_int_atomic_fetch_inc pshmem_int_atomic_fetch_inc
#pragma weak shmem_long_atomic_fetch_inc = pshmem_long_atomic_fetch_inc
#define shmem_long_atomic_fetch_inc pshmem_long_atomic_fetch_inc
#pragma weak shmem_longlong_atomic_fetch_inc = pshmem_longlong_atomic_fetch_inc
#define shmem_longlong_atomic_fetch_inc pshmem_longlong_atomic_fetch_inc
#pragma weak shmem_uint_atomic_fetch_inc = pshmem_uint_atomic_fetch_inc
#define shmem_uint_atomic_fetch_inc pshmem_uint_atomic_fetch_inc
#pragma weak shmem_ulong_atomic_fetch_inc = pshmem_ulong_atomic_fetch_inc
#define shmem_ulong_atomic_fetch_inc pshmem_ulong_atomic_fetch_inc
#pragma weak shmem_ulonglong_atomic_fetch_inc = pshmem_ulonglong_atomic_fetch_inc
#define shmem_ulonglong_atomic_fetch_inc pshmem_ulonglong_atomic_fetch_inc
#pragma weak shmem_int32_atomic_fetch_inc = pshmem_int32_atomic_fetch_inc
#define shmem_int32_atomic_fetch_inc pshmem_int32_atomic_fetch_inc
#pragma weak shmem_int64_atomic_fetch_inc = pshmem_int64_atomic_fetch_inc
#define shmem_int64_atomic_fetch_inc pshmem_int64_atomic_fetch_inc
#pragma weak shmem_uint32_atomic_fetch_inc = pshmem_uint32_atomic_fetch_inc
#define shmem_uint32_atomic_fetch_inc pshmem_uint32_atomic_fetch_inc
#pragma weak shmem_uint64_atomic_fetch_inc = pshmem_uint64_atomic_fetch_inc
#define shmem_uint64_atomic_fetch_inc pshmem_uint64_atomic_fetch_inc
#pragma weak shmem_size_atomic_fetch_inc = pshmem_size_atomic_fetch_inc
#define shmem_size_atomic_fetch_inc pshmem_size_atomic_fetch_inc
#pragma weak shmem_ptrdiff_atomic_fetch_inc = pshmem_ptrdiff_atomic_fetch_inc
#define shmem_ptrdiff_atomic_fetch_inc pshmem_ptrdiff_atomic_fetch_inc
#endif /* ENABLE_PSHMEM */

API_DEF_AMO1(fetch_inc, int, int)
API_DEF_AMO1(fetch_inc, long, long)
API_DEF_AMO1(fetch_inc, longlong, long long)
API_DEF_AMO1(fetch_inc, uint, unsigned int)
API_DEF_AMO1(fetch_inc, ulong, unsigned long)
API_DEF_AMO1(fetch_inc, ulonglong, unsigned long long)
API_DEF_AMO1(fetch_inc, int32, int32_t)
API_DEF_AMO1(fetch_inc, int64, int64_t)
API_DEF_AMO1(fetch_inc, uint32, uint32_t)
API_DEF_AMO1(fetch_inc, uint64, uint64_t)
API_DEF_AMO1(fetch_inc, size, size_t)
API_DEF_AMO1(fetch_inc, ptrdiff, ptrdiff_t)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_atomic_fetch_add = pshmem_int_atomic_fetch_add
#define shmem_int_atomic_fetch_add pshmem_int_atomic_fetch_add
#pragma weak shmem_long_atomic_fetch_add = pshmem_long_atomic_fetch_add
#define shmem_long_atomic_fetch_add pshmem_long_atomic_fetch_add
#pragma weak shmem_longlong_atomic_fetch_add = pshmem_longlong_atomic_fetch_add
#define shmem_longlong_atomic_fetch_add pshmem_longlong_atomic_fetch_add
#pragma weak shmem_uint_atomic_fetch_add = pshmem_uint_atomic_fetch_add
#define shmem_uint_atomic_fetch_add pshmem_uint_atomic_fetch_add
#pragma weak shmem_ulong_atomic_fetch_add = pshmem_ulong_atomic_fetch_add
#define shmem_ulong_atomic_fetch_add pshmem_ulong_atomic_fetch_add
#pragma weak shmem_ulonglong_atomic_fetch_add = pshmem_ulonglong_atomic_fetch_add
#define shmem_ulonglong_atomic_fetch_add pshmem_ulonglong_atomic_fetch_add
#pragma weak shmem_int32_atomic_fetch_add = pshmem_int32_atomic_fetch_add
#define shmem_int32_atomic_fetch_add pshmem_int32_atomic_fetch_add
#pragma weak shmem_int64_atomic_fetch_add = pshmem_int64_atomic_fetch_add
#define shmem_int64_atomic_fetch_add pshmem_int64_atomic_fetch_add
#pragma weak shmem_uint32_atomic_fetch_add = pshmem_uint32_atomic_fetch_add
#define shmem_uint32_atomic_fetch_add pshmem_uint32_atomic_fetch_add
#pragma weak shmem_uint64_atomic_fetch_add = pshmem_uint64_atomic_fetch_add
#define shmem_uint64_atomic_fetch_add pshmem_uint64_atomic_fetch_add
#pragma weak shmem_size_atomic_fetch_add = pshmem_size_atomic_fetch_add
#define shmem_size_atomic_fetch_add pshmem_size_atomic_fetch_add
#pragma weak shmem_ptrdiff_atomic_fetch_add = pshmem_ptrdiff_atomic_fetch_add
#define shmem_ptrdiff_atomic_fetch_add pshmem_ptrdiff_atomic_fetch_add
#endif /* ENABLE_PSHMEM */

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

#ifdef ENABLE_PSHMEM
#pragma weak shmem_float_atomic_swap = pshmem_float_atomic_swap
#define shmem_float_atomic_swap pshmem_float_atomic_swap
#pragma weak shmem_double_atomic_swap = pshmem_double_atomic_swap
#define shmem_double_atomic_swap pshmem_double_atomic_swap
#pragma weak shmem_int_atomic_swap = pshmem_int_atomic_swap
#define shmem_int_atomic_swap pshmem_int_atomic_swap
#pragma weak shmem_long_atomic_swap = pshmem_long_atomic_swap
#define shmem_long_atomic_swap pshmem_long_atomic_swap
#pragma weak shmem_longlong_atomic_swap = pshmem_longlong_atomic_swap
#define shmem_longlong_atomic_swap pshmem_longlong_atomic_swap
#pragma weak shmem_uint_atomic_swap = pshmem_uint_atomic_swap
#define shmem_uint_atomic_swap pshmem_uint_atomic_swap
#pragma weak shmem_ulong_atomic_swap = pshmem_ulong_atomic_swap
#define shmem_ulong_atomic_swap pshmem_ulong_atomic_swap
#pragma weak shmem_ulonglong_atomic_swap = pshmem_ulonglong_atomic_swap
#define shmem_ulonglong_atomic_swap pshmem_ulonglong_atomic_swap
#pragma weak shmem_int32_atomic_swap = pshmem_int32_atomic_swap
#define shmem_int32_atomic_swap pshmem_int32_atomic_swap
#pragma weak shmem_int64_atomic_swap = pshmem_int64_atomic_swap
#define shmem_int64_atomic_swap pshmem_int64_atomic_swap
#pragma weak shmem_uint32_atomic_swap = pshmem_uint32_atomic_swap
#define shmem_uint32_atomic_swap pshmem_uint32_atomic_swap
#pragma weak shmem_uint64_atomic_swap = pshmem_uint64_atomic_swap
#define shmem_uint64_atomic_swap pshmem_uint64_atomic_swap
#pragma weak shmem_size_atomic_swap = pshmem_size_atomic_swap
#define shmem_size_atomic_swap pshmem_size_atomic_swap
#pragma weak shmem_ptrdiff_atomic_swap = pshmem_ptrdiff_atomic_swap
#define shmem_ptrdiff_atomic_swap pshmem_ptrdiff_atomic_swap
#endif /* ENABLE_PSHMEM */

API_DEF_AMO2(swap, float, float)
API_DEF_AMO2(swap, double, double)
API_DEF_AMO2(swap, int, int)
API_DEF_AMO2(swap, long, long)
API_DEF_AMO2(swap, longlong, long long)
API_DEF_AMO2(swap, uint, unsigned int)
API_DEF_AMO2(swap, ulong, unsigned long)
API_DEF_AMO2(swap, ulonglong, unsigned long long)
API_DEF_AMO2(swap, int32, int32_t)
API_DEF_AMO2(swap, int64, int64_t)
API_DEF_AMO2(swap, uint32, uint32_t)
API_DEF_AMO2(swap, uint64, uint64_t)
API_DEF_AMO2(swap, size, size_t)
API_DEF_AMO2(swap, ptrdiff, ptrdiff_t)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_atomic_compare_swap = pshmem_int_atomic_compare_swap
#define shmem_int_atomic_compare_swap pshmem_int_atomic_compare_swap
#pragma weak shmem_long_atomic_compare_swap = pshmem_long_atomic_compare_swap
#define shmem_long_atomic_compare_swap pshmem_long_atomic_compare_swap
#pragma weak shmem_longlong_atomic_compare_swap = pshmem_longlong_atomic_compare_swap
#define shmem_longlong_atomic_compare_swap pshmem_longlong_atomic_compare_swap
#pragma weak shmem_uint_atomic_compare_swap = pshmem_uint_atomic_compare_swap
#define shmem_uint_atomic_compare_swap pshmem_uint_atomic_compare_swap
#pragma weak shmem_ulong_atomic_compare_swap = pshmem_ulong_atomic_compare_swap
#define shmem_ulong_atomic_compare_swap pshmem_ulong_atomic_compare_swap
#pragma weak shmem_ulonglong_atomic_compare_swap = pshmem_ulonglong_atomic_compare_swap
#define shmem_ulonglong_atomic_compare_swap pshmem_ulonglong_atomic_compare_swap
#pragma weak shmem_int32_atomic_compare_swap = pshmem_int32_atomic_compare_swap
#define shmem_int32_atomic_compare_swap pshmem_int32_atomic_compare_swap
#pragma weak shmem_int64_atomic_compare_swap = pshmem_int64_atomic_compare_swap
#define shmem_int64_atomic_compare_swap pshmem_int64_atomic_compare_swap
#pragma weak shmem_uint32_atomic_compare_swap = pshmem_uint32_atomic_compare_swap
#define shmem_uint32_atomic_compare_swap pshmem_uint32_atomic_compare_swap
#pragma weak shmem_uint64_atomic_compare_swap = pshmem_uint64_atomic_compare_swap
#define shmem_uint64_atomic_compare_swap pshmem_uint64_atomic_compare_swap
#pragma weak shmem_size_atomic_compare_swap = pshmem_size_atomic_compare_swap
#define shmem_size_atomic_compare_swap pshmem_size_atomic_compare_swap
#pragma weak shmem_ptrdiff_atomic_compare_swap = pshmem_ptrdiff_atomic_compare_swap
#define shmem_ptrdiff_atomic_compare_swap pshmem_ptrdiff_atomic_compare_swap
#endif /* ENABLE_PSHMEM */

API_DEF_AMO3(compare_swap, int, int)
API_DEF_AMO3(compare_swap, long, long)
API_DEF_AMO3(compare_swap, longlong, long long)
API_DEF_AMO3(compare_swap, uint, unsigned int)
API_DEF_AMO3(compare_swap, ulong, unsigned long)
API_DEF_AMO3(compare_swap, ulonglong, unsigned long long)
API_DEF_AMO3(compare_swap, int32, int32_t)
API_DEF_AMO3(compare_swap, int64, int64_t)
API_DEF_AMO3(compare_swap, uint32, uint32_t)
API_DEF_AMO3(compare_swap, uint64, uint64_t)
API_DEF_AMO3(compare_swap, size, size_t)
API_DEF_AMO3(compare_swap, ptrdiff, ptrdiff_t)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_uint_atomic_fetch_xor = pshmem_uint_atomic_fetch_xor
#define shmem_uint_atomic_fetch_xor pshmem_uint_atomic_fetch_xor
#pragma weak shmem_ulong_atomic_fetch_xor = pshmem_ulong_atomic_fetch_xor
#define shmem_ulong_atomic_fetch_xor pshmem_ulong_atomic_fetch_xor
#pragma weak shmem_ulonglong_atomic_fetch_xor = pshmem_ulonglong_atomic_fetch_xor
#define shmem_ulonglong_atomic_fetch_xor pshmem_ulonglong_atomic_fetch_xor
#pragma weak shmem_int32_atomic_fetch_xor = pshmem_int32_atomic_fetch_xor
#define shmem_int32_atomic_fetch_xor pshmem_int32_atomic_fetch_xor
#pragma weak shmem_int64_atomic_fetch_xor = pshmem_int64_atomic_fetch_xor
#define shmem_int64_atomic_fetch_xor pshmem_int64_atomic_fetch_xor
#pragma weak shmem_uint32_atomic_fetch_xor = pshmem_uint32_atomic_fetch_xor
#define shmem_uint32_atomic_fetch_xor pshmem_uint32_atomic_fetch_xor
#pragma weak shmem_uint64_atomic_fetch_xor = pshmem_uint64_atomic_fetch_xor
#define shmem_uint64_atomic_fetch_xor pshmem_uint64_atomic_fetch_xor

#pragma weak shmem_uint_atomic_fetch_or = pshmem_uint_atomic_fetch_or
#define shmem_uint_atomic_fetch_or pshmem_uint_atomic_fetch_or
#pragma weak shmem_ulong_atomic_fetch_or = pshmem_ulong_atomic_fetch_or
#define shmem_ulong_atomic_fetch_or pshmem_ulong_atomic_fetch_or
#pragma weak shmem_ulonglong_atomic_fetch_or = pshmem_ulonglong_atomic_fetch_or
#define shmem_ulonglong_atomic_fetch_or pshmem_ulonglong_atomic_fetch_or
#pragma weak shmem_int32_atomic_fetch_or = pshmem_int32_atomic_fetch_or
#define shmem_int32_atomic_fetch_or pshmem_int32_atomic_fetch_or
#pragma weak shmem_int64_atomic_fetch_or = pshmem_int64_atomic_fetch_or
#define shmem_int64_atomic_fetch_or pshmem_int64_atomic_fetch_or
#pragma weak shmem_uint32_atomic_fetch_or = pshmem_uint32_atomic_fetch_or
#define shmem_uint32_atomic_fetch_or pshmem_uint32_atomic_fetch_or
#pragma weak shmem_uint64_atomic_fetch_or = pshmem_uint64_atomic_fetch_or
#define shmem_uint64_atomic_fetch_or pshmem_uint64_atomic_fetch_or

#pragma weak shmem_uint_atomic_fetch_and = pshmem_uint_atomic_fetch_and
#define shmem_uint_atomic_fetch_and pshmem_uint_atomic_fetch_and
#pragma weak shmem_ulong_atomic_fetch_and = pshmem_ulong_atomic_fetch_and
#define shmem_ulong_atomic_fetch_and pshmem_ulong_atomic_fetch_and
#pragma weak shmem_ulonglong_atomic_fetch_and = pshmem_ulonglong_atomic_fetch_and
#define shmem_ulonglong_atomic_fetch_and pshmem_ulonglong_atomic_fetch_and
#pragma weak shmem_int32_atomic_fetch_xor = pshmem_int32_atomic_fetch_xor
#define shmem_int32_atomic_fetch_and pshmem_int32_atomic_fetch_and
#pragma weak shmem_int64_atomic_fetch_and = pshmem_int64_atomic_fetch_and
#define shmem_int64_atomic_fetch_and pshmem_int64_atomic_fetch_and
#pragma weak shmem_uint32_atomic_fetch_and = pshmem_uint32_atomic_fetch_and
#define shmem_uint32_atomic_fetch_and pshmem_uint32_atomic_fetch_and
#pragma weak shmem_uint64_atomic_fetch_and = pshmem_uint64_atomic_fetch_and
#define shmem_uint64_atomic_fetch_and pshmem_uint64_atomic_fetch_and
#endif /* ENABLE_PSHMEM */

API_DEF_AMO2(fetch_xor, uint, unsigned int)
API_DEF_AMO2(fetch_xor, ulong, unsigned long)
API_DEF_AMO2(fetch_xor, ulonglong, unsigned long long)
API_DEF_AMO2(fetch_xor, int32, int32_t)
API_DEF_AMO2(fetch_xor, int64, int64_t)
API_DEF_AMO2(fetch_xor, uint32, uint32_t)
API_DEF_AMO2(fetch_xor, uint64, uint64_t)

API_DEF_AMO2(fetch_or, uint, unsigned int)
API_DEF_AMO2(fetch_or, ulong, unsigned long)
API_DEF_AMO2(fetch_or, ulonglong, unsigned long long)
API_DEF_AMO2(fetch_or, int32, int32_t)
API_DEF_AMO2(fetch_or, int64, int64_t)
API_DEF_AMO2(fetch_or, uint32, uint32_t)
API_DEF_AMO2(fetch_or, uint64, uint64_t)

API_DEF_AMO2(fetch_and, uint, unsigned int)
API_DEF_AMO2(fetch_and, ulong, unsigned long)
API_DEF_AMO2(fetch_and, ulonglong, unsigned long long)
API_DEF_AMO2(fetch_and, int32, int32_t)
API_DEF_AMO2(fetch_and, int64, int64_t)
API_DEF_AMO2(fetch_and, uint32, uint32_t)
API_DEF_AMO2(fetch_and, uint64, uint64_t)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_uint_atomic_xor = pshmem_uint_atomic_xor
#define shmem_uint_atomic_xor pshmem_uint_atomic_xor
#pragma weak shmem_ulong_atomic_xor = pshmem_ulong_atomic_xor
#define shmem_ulong_atomic_xor pshmem_ulong_atomic_xor
#pragma weak shmem_ulonglong_atomic_xor = pshmem_ulonglong_atomic_xor
#define shmem_ulonglong_atomic_xor pshmem_ulonglong_atomic_xor
#pragma weak shmem_int32_atomic_xor = pshmem_int32_atomic_xor
#define shmem_int32_atomic_xor pshmem_int32_atomic_xor
#pragma weak shmem_int64_atomic_xor = pshmem_int64_atomic_xor
#define shmem_int64_atomic_xor pshmem_int64_atomic_xor
#pragma weak shmem_uint32_atomic_xor = pshmem_uint32_atomic_xor
#define shmem_uint32_atomic_xor pshmem_uint32_atomic_xor
#pragma weak shmem_uint64_atomic_xor = pshmem_uint64_atomic_xor
#define shmem_uint64_atomic_xor pshmem_uint64_atomic_xor

#pragma weak shmem_uint_atomic_or = pshmem_uint_atomic_or
#define shmem_uint_atomic_or pshmem_uint_atomic_or
#pragma weak shmem_ulong_atomic_or = pshmem_ulong_atomic_or
#define shmem_ulong_atomic_or pshmem_ulong_atomic_or
#pragma weak shmem_ulonglong_atomic_or = pshmem_ulonglong_atomic_or
#define shmem_ulonglong_atomic_or pshmem_ulonglong_atomic_or
#pragma weak shmem_int32_atomic_or = pshmem_int32_atomic_or
#define shmem_int32_atomic_or pshmem_int32_atomic_or
#pragma weak shmem_int64_atomic_or = pshmem_int64_atomic_or
#define shmem_int64_atomic_or pshmem_int64_atomic_or
#pragma weak shmem_uint32_atomic_or = pshmem_uint32_atomic_or
#define shmem_uint32_atomic_or pshmem_uint32_atomic_or
#pragma weak shmem_uint64_atomic_or = pshmem_uint64_atomic_or
#define shmem_uint64_atomic_or pshmem_uint64_atomic_or

#pragma weak shmem_uint_atomic_and = pshmem_uint_atomic_and
#define shmem_uint_atomic_and pshmem_uint_atomic_and
#pragma weak shmem_ulong_atomic_and = pshmem_ulong_atomic_and
#define shmem_ulong_atomic_and pshmem_ulong_atomic_and
#pragma weak shmem_ulonglong_atomic_and = pshmem_ulonglong_atomic_and
#define shmem_ulonglong_atomic_and pshmem_ulonglong_atomic_and
#pragma weak shmem_int32_atomic_xor = pshmem_int32_atomic_xor
#define shmem_int32_atomic_and pshmem_int32_atomic_and
#pragma weak shmem_int64_atomic_and = pshmem_int64_atomic_and
#define shmem_int64_atomic_and pshmem_int64_atomic_and
#pragma weak shmem_uint32_atomic_and = pshmem_uint32_atomic_and
#define shmem_uint32_atomic_and pshmem_uint32_atomic_and
#pragma weak shmem_uint64_atomic_and = pshmem_uint64_atomic_and
#define shmem_uint64_atomic_and pshmem_uint64_atomic_and
#endif /* ENABLE_PSHMEM */

API_DEF_VOID_AMO2(xor, uint, unsigned int)
API_DEF_VOID_AMO2(xor, ulong, unsigned long)
API_DEF_VOID_AMO2(xor, ulonglong, unsigned long long)
API_DEF_VOID_AMO2(xor, int32, int32_t)
API_DEF_VOID_AMO2(xor, int64, int64_t)
API_DEF_VOID_AMO2(xor, uint32, uint32_t)
API_DEF_VOID_AMO2(xor, uint64, uint64_t)

API_DEF_VOID_AMO2(or, uint, unsigned int)
API_DEF_VOID_AMO2(or, ulong, unsigned long)
API_DEF_VOID_AMO2(or, ulonglong, unsigned long long)
API_DEF_VOID_AMO2(or, int32, int32_t)
API_DEF_VOID_AMO2(or, int64, int64_t)
API_DEF_VOID_AMO2(or, uint32, uint32_t)
API_DEF_VOID_AMO2(or, uint64, uint64_t)

API_DEF_VOID_AMO2(and, uint, unsigned int)
API_DEF_VOID_AMO2(and, ulong, unsigned long)
API_DEF_VOID_AMO2(and, ulonglong, unsigned long long)
API_DEF_VOID_AMO2(and, int32, int32_t)
API_DEF_VOID_AMO2(and, int64, int64_t)
API_DEF_VOID_AMO2(and, uint32, uint32_t)
API_DEF_VOID_AMO2(and, uint64, uint64_t)

/*
 * ---------------------------------------------------------------------
 *
 * Deprecations as of 1.4.  3 different macros for different parameter
 * counts.  Provide variants for return types.  This needs to match
 * deprecation information in the header file.
 *
 */

#define SHMEM_DEPRECATE_VOID_AMO1(_op, _name, _type, _size)             \
    void                                                                \
    shmem_##_name##_##_op(_type *target, int pe)                        \
    {                                                                   \
        deprecate(__func__, 1, 4);                                      \
        shmemc_##_op##_size(target, pe);                                \
    }

#define SHMEM_DEPRECATE_VOID_AMO2(_op, _name, _type, _size)             \
    void                                                                \
    shmem_##_name##_##_op(_type *target, _type value, int pe)           \
    {                                                                   \
        deprecate(__func__, 1, 4);                                      \
        shmemc_##_op##_size(target, value, pe);                         \
    }

#define SHMEM_DEPRECATE_VOID_AMO3(_op, _name, _type, _soze)             \
    void                                                                \
    shmem_##_name##_##_op(_type *target,                                \
                          _type cond, _type value,                      \
                          int pe)                                       \
    {                                                                   \
        deprecate(__func__, 1, 4);                                      \
        shmemc_##_op##_size(target, cond, value, pe);                   \
    }

#define SHMEM_DEPRECATE_AMO1(_op, _name, _type, _size)                  \
    _type                                                               \
    shmem_##_name##_##_op(_type *target, int pe)                        \
    {                                                                   \
        deprecate(__func__, 1, 4);                                      \
        return shmemc_##_op##_size(target, pe);                         \
    }

#define SHMEM_DEPRECATE_AMO2(_op, _name, _type, _size)                  \
    _type                                                               \
    shmem_##_name##_##_op(_type *target, _type value, int pe)           \
    {                                                                   \
        deprecate(__func__, 1, 4);                                      \
        return shmemc_##_op##_size(target, value, pe);                  \
    }

#define SHMEM_DEPRECATE_AMO3(_op, _name, _type, _size)                  \
    _type                                                               \
    shmem_##_name##_##_op(_type *target,                                \
                          _type cond, _type value,                      \
                          int pe)                                       \
    {                                                                   \
        deprecate(__func__, 1, 4);                                      \
        return shmemc_##_op##_size(target, cond, value, pe);            \
    }

SHMEM_DEPRECATE_VOID_AMO2(set, int, int, 32)
SHMEM_DEPRECATE_VOID_AMO2(set, long, long, 64)
SHMEM_DEPRECATE_VOID_AMO2(set, longlong, long long, 64)
SHMEM_DEPRECATE_VOID_AMO2(set, float, float, 32)
SHMEM_DEPRECATE_VOID_AMO2(set, double, double, 64)

SHMEM_DEPRECATE_VOID_AMO1(inc, int, int, 32)
SHMEM_DEPRECATE_VOID_AMO1(inc, long, long, 64)
SHMEM_DEPRECATE_VOID_AMO1(inc, longlong, long long, 64)

SHMEM_DEPRECATE_VOID_AMO2(add, int, int, 32)
SHMEM_DEPRECATE_VOID_AMO2(add, long, long, 64)
SHMEM_DEPRECATE_VOID_AMO2(add, longlong, long long, 64)

SHMEM_DEPRECATE_AMO1(fetch, int, int, 32)
SHMEM_DEPRECATE_AMO1(fetch, long, long, 64)
SHMEM_DEPRECATE_AMO1(fetch, longlong, long long, 64)
SHMEM_DEPRECATE_AMO1(fetch, float, float, 32)
SHMEM_DEPRECATE_AMO1(fetch, double, double, 64)

SHMEM_DEPRECATE_AMO1(finc, int, int, 32)
SHMEM_DEPRECATE_AMO1(finc, long, long, 64)
SHMEM_DEPRECATE_AMO1(finc, longlong, long long, 64)

SHMEM_DEPRECATE_AMO2(fadd, int, int, 32)
SHMEM_DEPRECATE_AMO2(fadd, long, long, 64)
SHMEM_DEPRECATE_AMO2(fadd, longlong, long long, 64)

SHMEM_DEPRECATE_AMO2(swap, int, int, 32)
SHMEM_DEPRECATE_AMO2(swap, long, long, 64)
SHMEM_DEPRECATE_AMO2(swap, longlong, long long, 64)
SHMEM_DEPRECATE_AMO2(swap, float, float, 32)
SHMEM_DEPRECATE_AMO2(swap, double, double, 64)

SHMEM_DEPRECATE_AMO3(cswap, int, int, 32)
SHMEM_DEPRECATE_AMO3(cswap, long, long, 64)
SHMEM_DEPRECATE_AMO3(cswap, longlong, long long, 64)
