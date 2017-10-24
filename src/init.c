#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"
#include "state.h"

#include <stdlib.h>
#include <assert.h>

#ifdef ENABLE_PSHMEM
#pragma weak shmem_finalize = pshmem_finalize
#define shmem_finalize pshmem_finalize
#pragma weak shmem_init = pshmem_init
#define shmem_init pshmem_init
#endif /* ENABLE_PSHMEM */

/*
 * finish SHMEM portion of program, release resources
 */

void
shmem_finalize(void)
{
    logger(LOG_FINALIZE,
           "enter \"%s\", refcount = %d",
           __func__,
           proc.refcount);

    /* do nothing if multiple finalizes */
    if (proc.refcount < 1) {
        return;
    }

    proc.refcount -= 1;

    shmemc_finalize();
    shmemu_finalize();
}

/*
 * initialize SHMEM portion of program with default threading model
 */

void
shmem_init(void)
{
    int s;

    /* do nothing if multiple inits */
    if (proc.refcount > 0) {
        return;
    }

    proc.refcount += 1;

    shmemu_init();
    shmemc_init();

    s = atexit(shmem_finalize);
    assert(s == 0);

    logger(LOG_INIT,
           "leave \"%s\", refcount = %d",
           __func__,
           proc.refcount);

    /* 'ere we go! */
}

/*
 * initialize SHMEM portion of program and request a threading model
 *
 * TODO: invert this and shmem_init(), just a stub for now
 */

void
shmem_init_thread(int requested, int *provided)
{
    *provided = SHMEM_THREAD_SINGLE;

    shmem_init();
}

/*
 * query thread level support
 *
 * TODO: should this be here or in pe-query.c ?
 */

void
shmem_query_thread(int *provided)
{
    *provided = SHMEM_THREAD_SINGLE;
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
