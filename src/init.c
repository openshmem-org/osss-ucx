/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"
#include "state.h"
#include "info.h"

#include <stdio.h>
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

static void
finalize_helper(void)
{
    /* do nothing if multiple finalizes */
    if (proc.refcount > 0) {
        const pthread_t this = pthread_self();

        logger(LOG_FINALIZE,
               "refcount = %d",
               proc.refcount);

        if (this != proc.td.invoking_thread) {
            logger(LOG_FINALIZE,
                   "mis-match: thread %lu initialized, but %lu finalized",
                   proc.td.invoking_thread, this);
        }

        shmemu_finalize();
        shmemc_finalize();

        proc.refcount = 0;      /* finalized is finalized */
        proc.status = SHMEMC_PE_SHUTDOWN;
    }
}

inline static int
init_thread_helper(int requested, int *provided)
{
    /* do nothing if multiple inits */
    if (proc.refcount == 0) {
        int s;

        shmemc_init();
        shmemu_init();

        s = atexit(finalize_helper);
        if (s != 0) {
            logger(LOG_FATAL,
                   "unable to register atexit() handler: %s",
                   strerror(errno)
                   );
            /* NOT REACHED */
        }

        proc.status = SHMEMC_PE_RUNNING;

        /* for now */
        switch(requested) {
        case SHMEM_THREAD_SINGLE:
            break;
        case SHMEM_THREAD_FUNNELED:
            break;
        case SHMEM_THREAD_SERIALIZED:
            break;
        case SHMEM_THREAD_MULTIPLE:
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
        proc.td.osh_tl = requested;
        if (provided != NULL) {
            *provided = requested;
        }

        proc.td.invoking_thread = pthread_self();

        if (proc.rank == 0) {
            if (proc.env.print_version) {
                osh_info.package_version(0);
            }
            if (proc.env.print_info) {
                shmemc_print_env_vars(stdout, "# ");
            }
        }
    }
    proc.refcount += 1;

    logger(LOG_INIT,
           "refcount = %d, thread support = %d",
           proc.refcount,
           proc.td.osh_tl);

    /* just declare success */
    return 0;
}

/*
 * -- API --------------------------------------------------------------------
 */

/*
 * initialize SHMEM portion of program with threading model
 */

int
shmem_init_thread(int requested, int *provided)
{
    return init_thread_helper(requested, provided);
}

void
shmem_init(void)
{
    (void) init_thread_helper(SHMEM_THREAD_SINGLE, NULL);
}

/*
 * finish SHMEM portion of program, release resources
 */

void
shmem_finalize(void)
{
    finalize_helper();
}

/*
 * query thread level support
 *
 * TODO: should this be here or in pe-query.c ?
 */

void
shmem_query_thread(int *provided)
{
    *provided = proc.td.osh_tl;
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
