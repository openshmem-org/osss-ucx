#include "comms/comms.h"

void
shmem_quiet(void)
{
    shmemc_quiet();
}

void
shmem_fence(void)
{
    shmemc_fence();
}
