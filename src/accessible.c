#include "shmemi/shmemi.h"

int
shmem_pe_accessible(int pe)
{
    return shmemi_pe_accessible(pe);
}

int
shmem_addr_accessible(const void *addr, int pe)
{
    return shmemi_addr_accessible(addr, pe);
}
