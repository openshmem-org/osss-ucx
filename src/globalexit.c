#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"

/*
 * These are compatibility routines for older SGI architectures.  They
 * are now defined in OpenSHMEM to do nothing.
 */

#ifdef ENABLE_PSHMEM
#pragma weak shmem_global_exit = pshmem_global_exit
#define shmem_global_exit pshmem_global_exit
#endif /* ENABLE_PSHMEM */

void
shmem_global_exit(int status)
{
    shmemc_global_exit(status);
}
