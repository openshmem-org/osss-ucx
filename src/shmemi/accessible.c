#include "shmemi.h"

/*
 * stupid stubs
 */

int
shmemi_pe_accessible(int pe)
{
    return 1;
}

int
shmemi_addr_accessible(const void *addr, int pe)
{
    return 0;
}
