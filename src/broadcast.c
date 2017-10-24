#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"

#include <sys/types.h>

#ifdef ENABLE_PSHMEM
#pragma weak shmem_broadcast32 = pshmem_broadcast32
#define shmem_broadcast32 pshmem_broadcast32

#pragma weak shmem_broadcast64 = pshmem_broadcast64
#define shmem_broadcast64 pshmem_broadcast64
#endif /* ENABLE_PSHMEM */

/*
 * stupid linear broadcast
 */

#define SHMEM_BROADCAST_TYPE(_name)                                     \
    void                                                                \
    shmem_broadcast##_name(void *target, const void *source,            \
                           size_t nelems,                               \
                           int PE_root, int PE_start,                   \
                           int logPE_stride, int PE_size,               \
                           long *pSync)                                 \
    {                                                                   \
        shmemc_broadcast##_name(target, source,                         \
                                nelems,                                 \
                                PE_root, PE_start,                      \
                                logPE_stride, PE_size,                  \
                                pSync);                                 \
    }

SHMEM_BROADCAST_TYPE(32)
SHMEM_BROADCAST_TYPE(64)
