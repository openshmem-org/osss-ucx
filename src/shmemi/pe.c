#include "shmemi.h"

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

/*
 * deprecated
 */

int
_my_pe(void)
{
    return p.me;
}

int
_num_pes(void)
{
    return p.npes;
}
