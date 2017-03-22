#include "shmemi/shmemi.h"

int
shmem_my_pe(void)
{
    return shmemi_my_pe();
}

int
shmem_n_pes(void)
{
    return shmemi_n_pes();
}

int
_my_pe(void)
{
    return shmemi_my_pe();
}

int
_num_pes(void)
{
    return shmemi_n_pes();
}
