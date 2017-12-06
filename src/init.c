/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"
#include "state.h"
#include "info.h"

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

inline static char *
humanize(int v)
{
    return (v == 0) ? "no" : "yes";
}

static const int var_width = 20;
static const int val_width = 12;

inline static void
print_env_vars(void)
{
    printf("Environment Variable Information\n");
    printf("\n");
    printf("  %s\n\n",
           "From specification:");
    printf("  %-*s = %-*s %s\n",
           var_width,
           "SHMEM_VERSION",
           val_width,
           humanize(proc.env.print_version),
           "print library version at start-up");
    printf("  %-*s = %-*s %s\n",
           var_width,
           "SHMEM_INFO",
           val_width,
           humanize(proc.env.print_info),
           "print this information");
    printf("  %-*s = %-*lu %s\n",
           var_width,
           "SHMEM_SYMMETRIC_SIZE",
           val_width,
           proc.env.def_heap_size,
           "set the size of the symmetric heap");
    printf("  %-*s = %-*s %s\n",
           var_width,
           "SHMEM_DEBUG",
           val_width,
           humanize(proc.env.debug),
           "enable run debugging (if configured)");

    printf("\n");
    printf("  %s\n\n",
           "Specific to this implementation:");
    printf("  %-*s = %-*s %s\n",
           var_width,
           "SHMEM_DEBUG_FILE",
           val_width,
           (proc.env.debug_file != NULL) ? proc.env.debug_file : "none",
           "file to receive debugging information\n");

    printf("\n");
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

        if (proc.rank == 0) {
            if (proc.env.print_version) {
                osh_info.package_version(0);
            }
            if (proc.env.print_info) {
                print_env_vars();
            }
        }
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
