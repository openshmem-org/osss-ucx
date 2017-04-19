#include <sys/types.h>
#include <stddef.h>

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
