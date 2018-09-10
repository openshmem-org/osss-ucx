/* For license: see LICENSE file at top-level */

#ifndef _SHMEM_SHCOLL_SHIM_H
#define _SHMEM_SHCOLL_SHIM_H 1

#include <shcoll.h>

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
