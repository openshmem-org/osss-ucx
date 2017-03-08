#include "pe.h"

/*
 * shared state
 */
pe_t p;

int
shmem_my_pe(void)
{
    return p.me;
}

int
shmem_n_pes(void)
{
    return p.npes;
}
