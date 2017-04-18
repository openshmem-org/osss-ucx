#include <sys/types.h>

/**
 * Collective concatenation of 32-bit data from participating PEs
 * into a target array on all those PEs
 */

void
shmem_collect32(void *target, const void *source, size_t nelems,
                int PE_start, int logPE_stride, int PE_size,
                long *pSync)
{
}


/**
 * Collective concatenation of 64-bit data from participating PEs
 * into a target array on all those PEs
 */

void
shmem_collect64(void *target, const void *source, size_t nelems,
                int PE_start, int logPE_stride, int PE_size,
                long *pSync)
{
}
