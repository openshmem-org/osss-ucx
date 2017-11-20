/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"
#include "state.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef ENABLE_PSHMEM
#pragma weak shmem_init = pshmem_init
#define shmem_init pshmem_init
#pragma weak shmem_finalize = pshmem_finalize
#define shmem_finalize pshmem_finalize
#endif /* ENABLE_PSHMEM */

/*
 * finish SHMEM portion of program, release resources
 */

void
shmem_finalize(void)
{
    /* do nothing if multiple finalizes */
    if (proc.refcount > 0) {
        logger(LOG_FINALIZE,
               "enter \"%s\", refcount = %d",
               __func__,
               proc.refcount);

        shmemu_finalize();
        shmemc_finalize();

        proc.refcount = 0;      /* finalized is finalized */
        proc.status = SHMEM_PE_SHUTDOWN;
    }
}

/*
 * initialize SHMEM portion of program with default threading model
 */

void
shmem_init(void)
{
    /* do nothing if multiple inits */
    if (proc.refcount == 0) {
        int s;

        shmemc_init();
        shmemu_init();

        s = atexit(shmem_finalize);
        if (s != 0) {
            logger(LOG_FATAL,
                   "unable to register atexit() handler: %s",
                   strerror(errno));
            /* NOT REACHED */
        }

        proc.status = SHMEM_PE_RUNNING;
        /* 'ere we go! */
    }

    proc.refcount += 1;

    logger(LOG_INIT,
           "leave \"%s\", refcount = %d",
           __func__,
           proc.refcount);
}

/*
 * initialize SHMEM portion of program and request a threading model
 *
 * TODO: invert this and shmem_init(), just a stub for now
 */

#ifdef ENABLE_PSHMEM
#pragma weak shmem_init_thread = pshmem_init_thread
#define shmem_init_thread pshmem_init_thread
#pragma weak shmem_query_thread = pshmem_query_thread
#define shmem_query_thread pshmem_query_thread
#endif /* ENABLE_PSHMEM */

int
shmem_init_thread(int requested, int *provided)
{
    *provided = SHMEM_THREAD_SINGLE;

    shmem_init();

    return 0;
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
