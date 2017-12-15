/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "state.h"
#include "shmem/api.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_alltoall32 = pshmem_alltoall32
#define shmem_alltoall32 pshmem_alltoall32

#pragma weak shmem_alltoall64 = pshmem_alltoall64
#define shmem_alltoall64 pshmem_alltoall64

#pragma weak shmem_alltoalls32 = pshmem_alltoalls32
#define shmem_alltoalls32 pshmem_alltoalls32

#pragma weak shmem_alltoalls64 = pshmem_alltoalls64
#define shmem_alltoalls64 pshmem_alltoalls64
#endif /* ENABLE_PSHMEM */

#define SHMEM_ALLTOALLS_TYPE(_name, _size)                              \
    void                                                                \
    shmem_alltoalls##_name(void *target, const void *source,            \
                           ptrdiff_t dst, ptrdiff_t sst,                \
                           size_t nelems,                               \
                           int PE_start,                                \
                           int logPE_stride, int PE_size,               \
                           long *pSync)                                 \
    {                                                                   \
        const int step = 1 << logPE_stride;                             \
        int pe, i;                                                      \
                                                                        \
        for (i = 0, pe = PE_start; i < PE_size; i += 1, pe += step) {   \
            const int tidx = _size * nelems * dst * i;                  \
            const int sidx = _size * nelems * sst * proc.rank;          \
                                                                        \
            shmem_iget##_name(&((char *) target)[tidx],                 \
                              &((char *) source)[sidx],                 \
                              dst, sst, nelems, pe);                    \
        }                                                               \
    }

SHMEM_ALLTOALLS_TYPE(32, 4)
SHMEM_ALLTOALLS_TYPE(64, 8)

#define SHMEM_ALLTOALL_TYPE(_name, _size)                               \
    void                                                                \
    shmem_alltoall##_name(void *target, const void *source,             \
                          size_t nelems,                                \
                           int PE_start,                                \
                           int logPE_stride, int PE_size,               \
                           long *pSync)                                 \
    {                                                                   \
        const int step = 1 << logPE_stride;                             \
        int pe, i;                                                      \
                                                                        \
        for (i = 0, pe = PE_start; i < PE_size; i += 1, pe += step) {   \
            const int tidx = _size * nelems * i;                        \
            const int sidx = _size * nelems * proc.rank;                \
                                                                        \
            shmem_get##_name(&((char *) target)[tidx],                  \
                             &((char *) source)[sidx],                  \
                             nelems, pe);                               \
        }                                                               \
    }

SHMEM_ALLTOALL_TYPE(32, 4)
SHMEM_ALLTOALL_TYPE(64, 8)
