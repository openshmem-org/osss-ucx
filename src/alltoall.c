/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <sys/types.h>
#include <stddef.h>

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

void
shmem_alltoall32(void *target, const void* source, size_t nelems,
                 int PE_start, int logPE_stride, int PE_size,
                 long *pSync)
{
}

void
shmem_alltoall64(void *target, const void* source, size_t nelems,
                 int PE_start, int logPE_stride, int PE_size,
                 long *pSync)
{
}

void
shmem_alltoalls32(void *target, const void* source,
                  ptrdiff_t dst, ptrdiff_t sst, size_t nelems,
                  int PE_start, int logPE_stride, int PE_size,
                  long *pSync)
{
}

void
shmem_alltoalls64(void *target, const void* source,
                  ptrdiff_t dst, ptrdiff_t sst, size_t nelems,
                  int PE_start, int logPE_stride, int PE_size,
                  long *pSync)
{
}
