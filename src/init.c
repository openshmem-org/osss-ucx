/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"
#include "state.h"
#include "info.h"
#include "threading.h"
#include "progress.h"

#ifdef ENABLE_EXPERIMENTAL
#include "allocator/xmemalloc.h"
#endif  /* ENABLE_EXPERIMENTAL */

#include "shmem/api.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef ENABLE_PSHMEM
#pragma weak shmem_init_thread = pshmem_init_thread
#define shmem_init_thread pshmem_init_thread
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
    threadwrap_thread_t this;

    /* do nothing if multiple finalizes */
    if (proc.refcount < 1) {
        return;
    }

    logger(LOG_FINALIZE,
           "%s()",
           __func__
           );

    /* implicit barrier on finalize */
    shmem_barrier_all();

    this = threadwrap_thread_id();
    if (this != proc.td.invoking_thread) {
        logger(LOG_FINALIZE,
               "mis-match: thread %lu initialized, but %lu finalized",
               proc.td.invoking_thread, this);
    }

    progress_finalize();
    shmemc_finalize();
    shmemu_finalize();

#ifdef ENABLE_EXPERIMENTAL
    shmemxa_finalize();
#endif  /* ENABLE_EXPERIMENTAL */

    proc.refcount = 0;      /* finalized is finalized */
    proc.status = SHMEMC_PE_SHUTDOWN;
}

inline static int
init_thread_helper(int requested, int *provided)
{
    int s;

    /* do nothing if multiple inits */
    if (proc.refcount > 0) {
        return 0;
    }

    shmemc_init();
    shmemu_init();
    progress_init();

#ifdef ENABLE_EXPERIMENTAL
    shmemxa_init(proc.env.heaps.nheaps);
#endif  /* ENABLE_EXPERIMENTAL */

    s = atexit(finalize_helper);
    if (s != 0) {
        logger(LOG_FATAL,
               "unable to register atexit() handler: %s",
               strerror(errno)
               );
        /* NOT REACHED */
    }

    proc.status = SHMEMC_PE_RUNNING;

    proc.refcount += 1;

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
        *provided = proc.td.osh_tl;
    }

    proc.td.invoking_thread = threadwrap_thread_id();

    if (shmemc_my_pe() == 0) {
        if (proc.env.print_version) {
            info_output_package_version(stdout, 0);
        }
        if (proc.env.print_info) {
            shmemc_print_env_vars(stdout, "# ");
        }
    }

    logger(LOG_INIT,
           "%s(requested=%d, provided->%d)",
           __func__,
           requested, proc.td.osh_tl
           );

    /* make sure all symmetric memory ready */
    shmem_barrier_all();

    /* just declare success */
    return 0;
}

/*
 * initialize/finalize SHMEM portion of program with threading model
 */

void
shmem_finalize(void)
{
    finalize_helper();
}

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
 * deprecated
 */

#ifdef ENABLE_PSHMEM
#pragma weak start_pes = pstart_pes
#define start_pes pstart_pes
#endif /* ENABLE_PSHMEM */

void
start_pes(int n)
{
    NO_WARN_UNUSED(n);

    shmem_init();

    /* can't tell anyone until init has really happened */
    deprecate(__func__, 1, 2);
}
