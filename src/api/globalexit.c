/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_global_exit = pshmem_global_exit
#define shmem_global_exit pshmem_global_exit
#endif /* ENABLE_PSHMEM */

void
shmem_global_exit(int status)
{
    logger(LOG_FINALIZE, "%s(status=%d)", __func__, status);

    shmemc_global_exit(status);
}
