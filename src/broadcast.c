#include <sys/types.h>

void
shmem_broadcast32(void *target, const void *source, size_t nelems,
                  int PE_root, int PE_start, int logPE_stride, int PE_size,
                  long *pSync)
{
}

void
shmem_broadcast64(void *target, const void *source, size_t nelems,
                  int PE_root, int PE_start, int logPE_stride, int PE_size,
                  long *pSync)
{
}
