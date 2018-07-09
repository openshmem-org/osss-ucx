#ifndef _SHMEMC_SHCOLL_SHIM_H
#define _SHMEMC_SHCOLL_SHIM_H 1

#include <shcoll.h>

#define SHIM_ALLTOALL(_name, _size)                             \
    void                                                        \
    shmemc_alltoall##_size(void *dest, const void *source,      \
                           size_t nelems,                       \
                           int PE_start,                        \
                           int logPE_stride, int PE_size,       \
                           long *pSync)                         \
    {                                                           \
        shcoll_alltoall##_size##_##_name(dest, source,          \
                                         nelems,                \
                                         PE_start,              \
                                         logPE_stride, PE_size, \
                                         pSync);                \
    }

#define SHIM_ALLTOALLS(_name, _size)                                \
    void                                                            \
    shmemc_alltoalls##_size(void *dest, const void *source,         \
                            ptrdiff_t tst, ptrdiff_t sst,           \
                            size_t nelems,                          \
                            int PE_start,                           \
                            int logPE_stride, int PE_size,          \
                            long *pSync)                            \
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
    shmemc_collect##_size(void *dest, const void *source,       \
                          size_t nelems,                        \
                          int PE_start,                         \
                          int logPE_stride, int PE_size,        \
                          long *pSync)                          \
    {                                                           \
        shcoll_collect##_size##_##_name(dest, source,           \
                                        nelems,                 \
                                        PE_start,               \
                                        logPE_stride, PE_size,  \
                                        pSync);                 \
    }

#define SHIM_FCOLLECT(_name, _size)                             \
    void                                                        \
    shmemc_fcollect##_size(void *dest, const void *source,      \
                           size_t nelems,                       \
                           int PE_start,                        \
                           int logPE_stride, int PE_size,       \
                           long *pSync)                         \
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
    shmemc_barrier_all(void)                                    \
    {                                                           \
        shcoll_barrier_all_##_name(shmemc_barrier_all_psync);   \
    }                                                           \
    void                                                        \
    shmemc_barrier(int PE_start, int logPE_stride,              \
                   int PE_size, long *pSync)                    \
    {                                                           \
        shcoll_barrier_##_name(PE_start, logPE_stride,          \
                               PE_size, pSync);                 \
    }

extern long *shmemc_sync_all_psync;

#define SHIM_SYNC(_name)                                \
    void                                                \
    shmemc_sync_all(void)                               \
    {                                                   \
        shcoll_sync_all_##_name(shmemc_sync_all_psync); \
    }                                                   \
    void                                                \
    shmemc_sync(int PE_start, int logPE_stride,         \
                int PE_size, long *pSync)               \
    {                                                   \
        shcoll_sync_##_name(PE_start, logPE_stride,     \
                            PE_size, pSync);            \
    }

#define SHIM_BROADCAST(_name, _size)                                \
    void                                                            \
    shmemc_broadcast##_size(void *dest, const void *source,         \
                            size_t nelems,                          \
                            int PE_root, int PE_start,              \
                            int logPE_stride, int PE_size,          \
                            long *pSync)                            \
    {                                                               \
        shcoll_broadcast##_size##_##_name(dest, source,             \
                                          nelems,                   \
                                          PE_root, PE_start,        \
                                          logPE_stride, PE_size,    \
                                          pSync);                   \
    }

#endif /* ! _SHMEM_SHCOLL_SHIM_H */
