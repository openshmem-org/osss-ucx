/**
 * Do network service.  When code is not engaged in shmem calls,
 * something needs to provide communication access so that operations
 * where "this" PE is a passive target can continue
 */

#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#include "gasnet-common.h"

#include "shmemi.h"

/**
 * choose thread implementation
 */

#undef SHMEM_USE_QTHREADS

#if defined(SHMEM_USE_QTHREADS)

#include <qthread.h>

typedef aligned_t shmem_thread_return_t;
typedef qthread_f shmem_thread_t;

static shmem_thread_return_t thr_ret;

#else

/* defaulting to pthreads */

#define SHMEM_USE_PTHREADS 1

#include <pthread.h>

typedef void *shmem_thread_return_t;
typedef pthread_t shmem_thread_t;

/**
 * new thread for progress-o-matic
 */

static shmem_thread_t thr;

#endif /* threading model */

/**
 * for hi-res timer
 */

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309
#endif /* _POSIX_C_SOURCE */
#include <time.h>

/**
 * for refractory back-off
 */

static long delay = 1000L;      /* ns */
static struct timespec delayspec;

/**
 * polling sentinel
 */

static volatile bool done = false;

/**
 * Does comms. service until told not to
 */

static shmem_thread_return_t
start_service(void *unused)
{
    do {
        gasnet_AMPoll();
        pthread_yield();
        nanosleep(&delayspec, NULL);   /* back off */
    }
    while (!done);

    return (shmem_thread_return_t) 0;
}

/**
 * assume initially we need to manage progress ourselves
 */
static bool use_conduit_thread = false;

/**
 * tell a PE how to contend for updates
 *
 */
static void
waitmode_init(void)
{
    /*
     * this gives best performance in all cases observed by the author
     * (@ UH).  Could make this programmable.
     * @ Intel - Found GASNET_WAIT_SPIN gives the best PSM performance.
     */
#ifdef GASNET_CONDUIT_PSM
    gasnet_set_waitmode(GASNET_WAIT_SPIN);
#else
    gasnet_set_waitmode(GASNET_WAIT_SPINBLOCK);
#endif
}

/**
 * only start progress thread if I am first PE on a host
 */

static bool thread_starter;

static bool
check_if_thread_starter(void)
{
    const int *where = p.locp;

    /* PE 0 always starts a thread */
    if (p.me == 0) {
        return true;
    }

    if (where[p.me - 1] < where[p.me]) {
        return true;
    }

    return false;
}

/**
 * start the servicer
 */

void
shmemc_service_init(void)
{
    /*
     * Zap this code for now.  Problems with IBV conduit thread if all
     * PEs on one physical node.
     *
     */
#if 0
#if defined(GASNETC_IBV_RCV_THREAD) &&                          \
  (defined(GASNET_CONDUIT_IBV) || defined(GASNET_CONDUIT_VAPI))
    /*
     * if we have an IBV progress thread configured, then check env for
     * GASNET_RCV_THREAD.
     *
     * With no env var, let ibv conduit handle things...
     *
     * If set to [0nN] (false), we start our own progress thread
     * If set to [1yY] (true), the conduit handles progress
     *
     * Any other value, assume true but make a note.  NB with 1.20.2,
     * GASNet itself traps other values and aborts.
     *
     */

    const char *grt_str = "GASNET_RCV_THREAD";
    char *rtv = shmemu_getenv(grt_str);
    if (rtv == NULL) {
        use_conduit_thread = true;
    }
    else {
        switch (*rtv) {
        case '0':
        case 'n':
        case 'N':
            use_conduit_thread = false;
            break;
        case '1':
        case 'y':
        case 'Y':
            use_conduit_thread = true;
            break;
        default:
            use_conduit_thread = true;
            break;
        }
    }
#endif /* defined(GASNETC_IBV_RCV_THREAD) && (defined(GASNET_CONDUIT_IBV) ||
          defined(GASNET_CONDUIT_VAPI)) */
#endif /* commented out */

    if (!use_conduit_thread) {
        delayspec.tv_sec = (time_t) 0;
        delayspec.tv_nsec = delay;

#if defined(GASNET_CONDUIT_MPI)
        thread_starter = true;
#else
        thread_starter = check_if_thread_starter();
#endif /* GASNET_CONDUIT_MPI */

        if (thread_starter) {
#if defined(SHMEM_USE_PTHREADS)
            const int s = pthread_create(&thr, NULL,
                                         start_service, (void *) 0);
#elif defined(SHMEM_USE_QTHREADS)
            qthread_initialize();

            const int s = qthread_fork(start_service, (void *) 0, &thr_ret);
#endif

            if (s != 0) {
                shmemc_bailout
                    ("internal error: progress thread creation failed (%s)",
                     strerror(s)
                     );
                /* NOT REACHED */
            }
            waitmode_init();
        }
    }
}

/**
 * stop the servicer
 */

void
shmemc_service_finalize(void)
{
    if (!use_conduit_thread) {
        done = true;

        if (thread_starter) {
#if defined(SHMEM_USE_PTHREADS)
            const int s = pthread_join(thr, NULL);

            if (s != 0) {
                shmemc_bailout
                    ("internal error: progress thread termination failed (%s)",
                     strerror(s)
                     );
                /* NOT REACHED */
            }
#elif defined(SHMEM_USE_QTHREADS)
            /**
             * not sure if need readFF() here
             */
            qthread_finalize();
#endif
        }
    }
}
