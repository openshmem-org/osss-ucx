#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <sys/types.h>

#ifdef ENABLE_PSHMEM
#pragma weak shmem_broadcast32 = pshmem_broadcast32
#define shmem_broadcast32 pshmem_broadcast32

#pragma weak shmem_broadcast64 = pshmem_broadcast64
#define shmem_broadcast64 pshmem_broadcast64
#endif /* ENABLE_PSHMEM */


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
