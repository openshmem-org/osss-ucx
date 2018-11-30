/* For license: see LICENSE file at top-level */

#ifndef _SHMEM_SHCOLL_SHIM_H
#define _SHMEM_SHCOLL_SHIM_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <shcoll.h>

#ifdef ENABLE_PSHMEM
#pragma weak shmem_alltoall32 = pshmem_alltoall32
#define shmem_alltoall32 pshmem_alltoall32
#pragma weak shmem_alltoall64 = pshmem_alltoall64
#define shmem_alltoall64 pshmem_alltoall64
#endif /* ENABLE_PSHMEM */

#define SHIM_ALLTOALL(_name, _size)                             \
    void                                                        \
    shmem_alltoall##_size(void *dest, const void *source,       \
                          size_t nelems,                        \
                          int PE_start,                         \
                          int logPE_stride, int PE_size,        \
                          long *pSync)                          \
    {                                                           \
        shcoll_alltoall##_size##_##_name(dest, source,          \
                                         nelems,                \
                                         PE_start,              \
                                         logPE_stride, PE_size, \
                                         pSync);                \
    }

#ifdef ENABLE_PSHMEM
#pragma weak shmem_alltoalls32 = pshmem_alltoalls32
#define shmem_alltoalls32 pshmem_alltoalls32
#pragma weak shmem_alltoalls64 = pshmem_alltoalls64
#define shmem_alltoalls64 pshmem_alltoalls64
#endif /* ENABLE_PSHMEM */

#define SHIM_ALLTOALLS(_name, _size)                                \
    void                                                            \
    shmem_alltoalls##_size(void *dest, const void *source,          \
                           ptrdiff_t tst, ptrdiff_t sst,            \
                           size_t nelems,                           \
                           int PE_start,                            \
                           int logPE_stride, int PE_size,           \
                           long *pSync)                             \
    {                                                               \
        shcoll_alltoalls##_size##_##_name(dest, source,             \
                                          tst, sst,                 \
                                          nelems,                   \
                                          PE_start,                 \
                                          logPE_stride, PE_size,    \
                                          pSync);                   \
    }

#ifdef ENABLE_PSHMEM
#pragma weak shmem_collect64 = pshmem_collect64
#define shmem_collect64 pshmem_collect64
#pragma weak shmem_collect64 = pshmem_collect64
#define shmem_collect64 pshmem_collect64
#endif /* ENABLE_PSHMEM */

#define SHIM_COLLECT(_name, _size)                              \
    void                                                        \
    shmem_collect##_size(void *dest, const void *source,        \
                         size_t nelems,                         \
                         int PE_start,                          \
                         int logPE_stride, int PE_size,         \
                         long *pSync)                           \
    {                                                           \
        shcoll_collect##_size##_##_name(dest, source,           \
                                        nelems,                 \
                                        PE_start,               \
                                        logPE_stride, PE_size,  \
                                        pSync);                 \
    }

#ifdef ENABLE_PSHMEM
#pragma weak shmem_fcollect32 = pshmem_fcollect32
#define shmem_fcollect32 pshmem_fcollect32
#pragma weak shmem_fcollect64 = pshmem_fcollect64
#define shmem_fcollect64 pshmem_fcollect64
#endif /* ENABLE_PSHMEM */

#define SHIM_FCOLLECT(_name, _size)                             \
    void                                                        \
    shmem_fcollect##_size(void *dest, const void *source,       \
                          size_t nelems,                        \
                          int PE_start,                         \
                          int logPE_stride, int PE_size,        \
                          long *pSync)                          \
    {                                                           \
        shcoll_fcollect##_size##_##_name(dest, source,          \
                                         nelems,                \
                                         PE_start,              \
                                         logPE_stride, PE_size, \
                                         pSync);                \
    }

extern long *shmemc_barrier_all_psync;

#ifdef ENABLE_PSHMEM
#pragma weak shmem_barrier_all = pshmem_barrier_all
#define shmem_barrier_all pshmem_barrier_all
#pragma weak shmem_barrier = pshmem_barrier
#define shmem_barrier pshmem_barrier
#endif /* ENABLE_PSHMEM */

#define SHIM_BARRIER(_name)                                     \
    void                                                        \
    shmem_barrier_all(void)                                     \
    {                                                           \
        shcoll_barrier_all_##_name(shmemc_barrier_all_psync);   \
    }                                                           \
    void                                                        \
    shmem_barrier(int PE_start, int logPE_stride,               \
                  int PE_size, long *pSync)                     \
    {                                                           \
        shcoll_barrier_##_name(PE_start, logPE_stride,          \
                               PE_size, pSync);                 \
    }

extern long *shmemc_sync_all_psync;

#ifdef ENABLE_PSHMEM
#pragma weak shmem_sync_all = pshmem_sync_all
#define shmem_sync_all pshmem_sync_all
#pragma weak shmem_sync = pshmem_sync
#define shmem_sync pshmem_sync
#endif /* ENABLE_PSHMEM */

#define SHIM_SYNC(_name)                                \
    void                                                \
    shmem_sync_all(void)                                \
    {                                                   \
        shcoll_sync_all_##_name(shmemc_sync_all_psync); \
    }                                                   \
    void                                                \
    shmem_sync(int PE_start, int logPE_stride,          \
               int PE_size, long *pSync)                \
    {                                                   \
        shcoll_sync_##_name(PE_start, logPE_stride,     \
                            PE_size, pSync);            \
    }

#ifdef ENABLE_PSHMEM
#pragma weak shmem_broadcast32 = pshmem_broadcast32
#define shmem_broadcast32 pshmem_broadcast32
#pragma weak shmem_broadcast64 = pshmem_broadcast64
#define shmem_broadcast64 pshmem_broadcast64
#endif /* ENABLE_PSHMEM */

#define SHIM_BROADCAST(_name, _size)                                \
    void                                                            \
    shmem_broadcast##_size(void *dest, const void *source,          \
                           size_t nelems,                           \
                           int PE_root, int PE_start,               \
                           int logPE_stride, int PE_size,           \
                           long *pSync)                             \
    {                                                               \
        shcoll_broadcast##_size##_##_name(dest, source,             \
                                          nelems,                   \
                                          PE_root, PE_start,        \
                                          logPE_stride, PE_size,    \
                                          pSync);                   \
    }

#ifdef ENABLE_PSHMEM
#pragma weak shmem_complexd_sum_to_all = pshmem_complexd_sum_to_all
#define shmem_complexd_sum_to_all pshmem_complexd_sum_to_all
#pragma weak shmem_complexf_sum_to_all = pshmem_complexf_sum_to_all
#define shmem_complexf_sum_to_all pshmem_complexf_sum_to_all
#pragma weak shmem_double_sum_to_all = pshmem_double_sum_to_all
#define shmem_double_sum_to_all pshmem_double_sum_to_all
#pragma weak shmem_float_sum_to_all = pshmem_float_sum_to_all
#define shmem_float_sum_to_all pshmem_float_sum_to_all
#pragma weak shmem_int_sum_to_all = pshmem_int_sum_to_all
#define shmem_int_sum_to_all pshmem_int_sum_to_all
#pragma weak shmem_long_sum_to_all = pshmem_long_sum_to_all
#define shmem_long_sum_to_all pshmem_long_sum_to_all
#pragma weak shmem_longdouble_sum_to_all = pshmem_longdouble_sum_to_all
#define shmem_longdouble_sum_to_all pshmem_longdouble_sum_to_all
#pragma weak shmem_longlong_sum_to_all = pshmem_longlong_sum_to_all
#define shmem_longlong_sum_to_all pshmem_longlong_sum_to_all
#pragma weak shmem_short_sum_to_all = pshmem_short_sum_to_all
#define shmem_short_sum_to_all pshmem_short_sum_to_all
#pragma weak shmem_complexd_prod_to_all = pshmem_complexd_prod_to_all
#define shmem_complexd_prod_to_all pshmem_complexd_prod_to_all
#pragma weak shmem_complexf_prod_to_all = pshmem_complexf_prod_to_all
#define shmem_complexf_prod_to_all pshmem_complexf_prod_to_all
#pragma weak shmem_double_prod_to_all = pshmem_double_prod_to_all
#define shmem_double_prod_to_all pshmem_double_prod_to_all
#pragma weak shmem_float_prod_to_all = pshmem_float_prod_to_all
#define shmem_float_prod_to_all pshmem_float_prod_to_all
#pragma weak shmem_int_prod_to_all = pshmem_int_prod_to_all
#define shmem_int_prod_to_all pshmem_int_prod_to_all
#pragma weak shmem_long_prod_to_all = pshmem_long_prod_to_all
#define shmem_long_prod_to_all pshmem_long_prod_to_all
#pragma weak shmem_longdouble_prod_to_all = pshmem_longdouble_prod_to_all
#define shmem_longdouble_prod_to_all pshmem_longdouble_prod_to_all
#pragma weak shmem_longlong_prod_to_all = pshmem_longlong_prod_to_all
#define shmem_longlong_prod_to_all pshmem_longlong_prod_to_all
#pragma weak shmem_short_prod_to_all = pshmem_short_prod_to_all
#define shmem_short_prod_to_all pshmem_short_prod_to_all
#pragma weak shmem_int_and_to_all = pshmem_int_and_to_all
#define shmem_int_and_to_all pshmem_int_and_to_all
#pragma weak shmem_long_and_to_all = pshmem_long_and_to_all
#define shmem_long_and_to_all pshmem_long_and_to_all
#pragma weak shmem_longlong_and_to_all = pshmem_longlong_and_to_all
#define shmem_longlong_and_to_all pshmem_longlong_and_to_all
#pragma weak shmem_short_and_to_all = pshmem_short_and_to_all
#define shmem_short_and_to_all pshmem_short_and_to_all
#pragma weak shmem_int_or_to_all = pshmem_int_or_to_all
#define shmem_int_or_to_all pshmem_int_or_to_all
#pragma weak shmem_long_or_to_all = pshmem_long_or_to_all
#define shmem_long_or_to_all pshmem_long_or_to_all
#pragma weak shmem_longlong_or_to_all = pshmem_longlong_or_to_all
#define shmem_longlong_or_to_all pshmem_longlong_or_to_all
#pragma weak shmem_short_or_to_all = pshmem_short_or_to_all
#define shmem_short_or_to_all pshmem_short_or_to_all
#pragma weak shmem_int_xor_to_all = pshmem_int_xor_to_all
#define shmem_int_xor_to_all pshmem_int_xor_to_all
#pragma weak shmem_long_xor_to_all = pshmem_long_xor_to_all
#define shmem_long_xor_to_all pshmem_long_xor_to_all
#pragma weak shmem_longlong_xor_to_all = pshmem_longlong_xor_to_all
#define shmem_longlong_xor_to_all pshmem_longlong_xor_to_all
#pragma weak shmem_short_xor_to_all = pshmem_short_xor_to_all
#define shmem_short_xor_to_all pshmem_short_xor_to_all
#pragma weak shmem_int_max_to_all = pshmem_int_max_to_all
#define shmem_int_max_to_all pshmem_int_max_to_all
#pragma weak shmem_long_max_to_all = pshmem_long_max_to_all
#define shmem_long_max_to_all pshmem_long_max_to_all
#pragma weak shmem_longlong_max_to_all = pshmem_longlong_max_to_all
#define shmem_longlong_max_to_all pshmem_longlong_max_to_all
#pragma weak shmem_short_max_to_all = pshmem_short_max_to_all
#define shmem_short_max_to_all pshmem_short_max_to_all
#pragma weak shmem_longdouble_max_to_all = pshmem_longdouble_max_to_all
#define shmem_longdouble_max_to_all pshmem_longdouble_max_to_all
#pragma weak shmem_float_max_to_all = pshmem_float_max_to_all
#define shmem_float_max_to_all pshmem_float_max_to_all
#pragma weak shmem_double_max_to_all = pshmem_double_max_to_all
#define shmem_double_max_to_all pshmem_double_max_to_all
#pragma weak shmem_int_min_to_all = pshmem_int_min_to_all
#define shmem_int_min_to_all pshmem_int_min_to_all
#pragma weak shmem_long_min_to_all = pshmem_long_min_to_all
#define shmem_long_min_to_all pshmem_long_min_to_all
#pragma weak shmem_longlong_min_to_all = pshmem_longlong_min_to_all
#define shmem_longlong_min_to_all pshmem_longlong_min_to_all
#pragma weak shmem_short_min_to_all = pshmem_short_min_to_all
#define shmem_short_min_to_all pshmem_short_min_to_all
#pragma weak shmem_longdouble_min_to_all = pshmem_longdouble_min_to_all
#define shmem_longdouble_min_to_all pshmem_longdouble_min_to_all
#pragma weak shmem_float_min_to_all = pshmem_float_min_to_all
#define shmem_float_min_to_all pshmem_float_min_to_all
#pragma weak shmem_double_min_to_all = pshmem_double_min_to_all
#define shmem_double_min_to_all pshmem_double_min_to_all
#endif /* ENABLE_PSHMEM */

#define SHIM_REDUCE_DECLARE(_name, _type, _op, _algo)                   \
    void                                                                \
    shmem_##_name##_##_op##_to_all(_type *dest,                         \
                                   const _type *source,                 \
                                   int nreduce,                         \
                                   int PE_start,                        \
                                   int logPE_stride,                    \
                                   int PE_size,                         \
                                   _type *pWrk,                         \
                                   long *pSync)                         \
    {                                                                   \
        shcoll_##_name##_##_op##_to_all_##_algo(dest,                   \
                                                source,                 \
                                                nreduce,                \
                                                PE_start,               \
                                                logPE_stride,           \
                                                PE_size,                \
                                                pWrk,                   \
                                                pSync);                 \
    }

#define SHIM_REDUCE_BITWISE_TYPES(_op, _algo)                       \
    SHIM_REDUCE_DECLARE(short,      short,       _op, _algo)        \
        SHIM_REDUCE_DECLARE(int,        int,         _op, _algo)    \
        SHIM_REDUCE_DECLARE(long,       long,        _op, _algo)    \
        SHIM_REDUCE_DECLARE(longlong,   long long,   _op, _algo)

#define SHIM_REDUCE_MINMAX_TYPES(_op, _algo)                        \
    SHIM_REDUCE_BITWISE_TYPES(_op, _algo)                           \
        SHIM_REDUCE_DECLARE(double,     double,      _op, _algo)    \
        SHIM_REDUCE_DECLARE(float,      float,       _op, _algo)    \
        SHIM_REDUCE_DECLARE(longdouble, long double, _op, _algo)

#define SHIM_REDUCE_ARITH_TYPES(_op, _algo)                         \
    SHIM_REDUCE_MINMAX_TYPES(_op, _algo)                            \
        SHIM_REDUCE_DECLARE(complexd,  double _Complex, _op, _algo) \
        SHIM_REDUCE_DECLARE(complexf,   float _Complex, _op, _algo)

#define SHIM_REDUCE_BITWISE_ALL(_algo)          \
    SHIM_REDUCE_BITWISE_TYPES(or, _algo)        \
        SHIM_REDUCE_BITWISE_TYPES(xor, _algo)   \
        SHIM_REDUCE_BITWISE_TYPES(and, _algo)

#define SHIM_REDUCE_MINMAX_ALL(_algo)           \
    SHIM_REDUCE_MINMAX_TYPES(min, _algo)        \
        SHIM_REDUCE_MINMAX_TYPES(max, _algo)

#define SHIM_REDUCE_ARITH_ALL(_algo)            \
    SHIM_REDUCE_ARITH_TYPES(sum, _algo)         \
        SHIM_REDUCE_ARITH_TYPES(prod, _algo)

#define SHIM_REDUCE_ALL(_algo)                  \
    SHIM_REDUCE_BITWISE_ALL(_algo)              \
        SHIM_REDUCE_MINMAX_ALL(_algo)           \
        SHIM_REDUCE_ARITH_ALL(_algo)

#endif /* ! _SHIM_SHCOLL_SHIM_H */
