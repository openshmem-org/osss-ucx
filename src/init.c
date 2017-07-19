#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"

#include <stdlib.h>
#include <assert.h>

#ifdef ENABLE_PSHMEM
#pragma weak shmem_finalize = pshmem_finalize
#define shmem_finalize pshmem_finalize
#pragma weak shmem_init = pshmem_init
#define shmem_init pshmem_init
#endif /* ENABLE_PSHMEM */

void
shmem_finalize(void)
{
    shmemc_finalize();
    shmemu_finalize();
}

void
shmem_init(void)
{
    int s;

    shmemu_init();
    shmemc_init();

    s = atexit(shmem_finalize);
    assert(s == 0);

    /* urgh! */
}

#ifdef ENABLE_PSHMEM
#pragma weak start_pes = pstart_pes
#define start_pes pstart_pes
#endif /* ENABLE_PSHMEM */

void
start_pes(int n /* unused */)
{
    deprecate(__func__);
    shmem_init();
}
