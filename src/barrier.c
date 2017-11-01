/* For license: see LICENSE file at top-level */

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
shmem_barrier(int start, int log2stride, int size, long *pSync)
{
    shmemc_barrier(start, log2stride, size, pSync);
}

void
shmem_barrier_all(void)
{
    shmemc_barrier_all();
}


#ifdef ENABLE_PSHMEM
#pragma weak shmem_sync_all = pshmem_sync_all
#define shmem_sync_all pshmem_sync_all

#pragma weak shmem_sync = pshmem_sync
#define shmem_sync pshmem_sync
#endif /* ENABLE_PSHMEM */

void
shmem_sync(int start, int log2stride, int size, long *pSync)
{
    shmemc_sync(start, log2stride, size, pSync);
}

void
shmem_sync_all(void)
{
    shmemc_sync_all();
}
