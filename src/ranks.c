/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "state.h"

#include "shmemc.h"
#include "shmemu.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_my_pe = pshmem_my_pe
#define shmem_my_pe pshmem_my_pe
#pragma weak shmem_n_pes = pshmem_n_pes
#define shmem_n_pes pshmem_n_pes
#endif /* ENABLE_PSHMEM */

int
shmem_my_pe(void)
{
    return proc.rank;
}

int
shmem_n_pes(void)
{
    return proc.nranks;
}

#ifdef ENABLE_PSHMEM
#pragma weak _my_pe = p_my_pe
#define _my_pe p_my_pe
#pragma weak _num_pes = p_num_pes
#define _num_pes p_num_pes
#endif /* ENABLE_PSHMEM */

int
_my_pe(void)
{
    deprecate(__func__);
    return proc.rank;
}

int
_num_pes(void)
{
    deprecate(__func__);
    return proc.nranks;
}
