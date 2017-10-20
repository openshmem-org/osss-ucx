#include "state.h"

#include <pthread.h>
#include <time.h>
#include <assert.h>

static pthread_t thread;

static volatile short go = 1;

static struct timespec ts;
static unsigned long backoff = 10000; /* nanoseconds */

inline static void
setup_backoff(void)
{
    ts.tv_sec = 0;
    ts.tv_nsec = backoff;
}

/*
 * this is what runs in the progress thread to advance UCX.  Need to
 * think about how to back it off when things are progressing
 * automatically (blocking calls/wait/test).
 */

static void *
progress_impl_simple(void *unused_arg)
{
    return NULL;                /* TODO: no progress thread while testing */

    while (go) {
        unsigned s = ucp_worker_progress(proc.comms.wrkr);

        nanosleep(&ts, NULL);
    };

    return NULL;
}

static void *
progress_impl_event(void *unused_arg)
{
    ucs_status_t s;

    s = ucp_worker_arm(proc.comms.wrkr);
    assert(s == UCS_OK);

    do {
        unsigned ps;

        /* clean up */
        do {
            ps = ucp_worker_progress(proc.comms.wrkr);
            nanosleep(&ts, NULL);
        } while (ps != 0);

        /* wait for something to happen */
        s = ucp_worker_wait(proc.comms.wrkr);
        assert(s == UCS_OK);

    } while (go);

    return NULL;
}

/*
 * choose the progress implementation:
 *
 * currently one of
 *
 * "simple" = unrestrained polling loop
 * "event"  = back off until signaled
 *
 */
static void *(*progress)(void *) = progress_impl_simple;

/*
 * start the progress thread
 */
void
shmemc_ucx_progress_init(void)
{
    int ps;

    /* throttle polling */
    setup_backoff();

    /* start progress thread */
    ps = pthread_create(&thread, NULL, progress, NULL);
    assert(ps == 0);
}

/*
 * terminate the progress thread
 */
void
shmemc_ucx_progress_finalize(void)
{
    ucs_status_t s;
    int ps;

    /* (eventually) tell thread poll to shut down */
    go = 0;

#if 0
    /* poke the worker */
    s = ucp_worker_signal(proc.comms.wrkr);
    assert(s == UCS_OK);
#endif

    /* clean up terminated thread */
    ps = pthread_join(thread, NULL);
    assert(ps == 0);
}
