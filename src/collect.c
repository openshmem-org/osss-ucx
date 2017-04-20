#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <sys/types.h>

#ifdef ENABLE_PSHMEM
#pragma weak shmem_collect32 = pshmem_collect32
#define shmem_collect32 pshmem_collect32

#pragma weak shmem_collect64 = pshmem_collect64
#define shmem_collect64 pshmem_collect64
#endif /* ENABLE_PSHMEM */

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
