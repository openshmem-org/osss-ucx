#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu/shmemu.h"
#include "shmemi/shmemi.h"

#ifdef ENABLE_PSHMEM
#pragma weak _my_pe = p_my_pe
#define _my_pe p_my_pe
#pragma weak shmem_my_pe = pshmem_my_pe
#define shmem_my_pe pshmem_my_pe

#pragma weak _num_pes = p_num_pes
#define _num_pes p_num_pes
#pragma weak shmem_n_pes = pshmem_n_pes
#define shmem_n_pes pshmem_n_pes
#endif /* ENABLE_PSHMEM */

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
    deprecate(__func__);
    return shmemi_my_pe();
}

int
_num_pes(void)
{
    deprecate(__func__);
    return shmemi_n_pes();
}
