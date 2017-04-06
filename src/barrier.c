#include "shmemc/shmemc.h"

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
