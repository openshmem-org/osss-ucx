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
#include <pthread.h>

#ifdef ENABLE_PSHMEM
#pragma weak shmem_init_thread = pshmem_init_thread
#define shmem_init_thread pshmem_init_thread
#pragma weak shmem_query_thread = pshmem_query_thread
#define shmem_query_thread pshmem_query_thread
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
 * initialize SHMEM portion of program with threading model
 */

int
shmem_init_thread(int requested, int *provided)
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
                   strerror(errno)
                   );
            /* NOT REACHED */
        }

        proc.status = SHMEM_PE_RUNNING;

        /* for now */
        switch(requested) {
        case SHMEM_THREAD_SINGLE:
            break;
        case SHMEM_THREAD_FUNNELED:
            break;
        case SHMEM_THREAD_SERIALIZED:
            break;
        case SHMEM_THREAD_MULTIPLE: /* unsupported for now */
            requested = SHMEM_THREAD_SERIALIZED;
            break;
        default:
            logger(LOG_FATAL,
                   "unknown thread level %d requested",
                   requested
                   );
            /* NOT REACHED */
            break;
        }

        /* save and return */
        proc.thread_level = requested;
        if (provided != NULL) {
            *provided = requested;
        }

        proc.invoking_thread = pthread_self();
    }

    proc.refcount += 1;

    logger(LOG_INIT,
           "leave \"%s\", refcount = %d, thread support = %d",
           __func__,
           proc.refcount,
           proc.thread_level);

    /* just declare success */
    return 0;
}

void
shmem_init(void)
{
    int throwaway = 0;

    (void) shmem_init_thread(SHMEM_THREAD_SINGLE, &throwaway);
}

/*
 * query thread level support
 *
 * TODO: should this be here or in pe-query.c ?
 */

void
shmem_query_thread(int *provided)
{
    *provided = proc.thread_level;
}

/*
 * deprecated
 */

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
