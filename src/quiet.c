/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_quiet = pshmem_quiet
#define shmem_quiet pshmem_quiet

#pragma weak shmem_fence = pshmem_fence
#define shmem_fence pshmem_fence
#endif /* ENABLE_PSHMEM */

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
