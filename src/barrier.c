#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_barrier_all = pshmem_barrier_all
#define shmem_barrier_all pshmem_barrier_all

#pragma weak shmem_barrier = pshmem_barrier
#define shmem_barrier pshmem_barrier
#endif /* ENABLE_PSHMEM */

void
shmem_barrier_all(void)
{
    shmemc_barrier_all();
}

void
shmem_barrier(int start, int log_stride, int size, long *pSync)
{
    shmemc_barrier(start, log_stride, size, pSync);
}
