/* For license: see LICENSE file at top-level */

#include "shmemu.h"
#include "state.h"

#include <pthread.h>
#include <time.h>
#include <assert.h>

#include <ucp/api/ucp.h>

/*
 * progress thread and its sentinel
 */
static pthread_t thread;
static volatile short go = 1;

/*
 * not used at the moment
 */
static struct timespec ts;
static const unsigned long backoff = 1e6; /* nanoseconds */

/*
 * this is intended to help back off this thread when the mainline is
 * progressing by itself
 */
double last_call;
static const double interval = 2.0;

/*
 * how long ot back off (unused)
 */
inline static void
setup_backoff(void)
{
    ts.tv_sec = 0;
    ts.tv_nsec = backoff;
}

/*
 * progress UCX
 */
inline static void
progress(void)
{
#if 0
    unsigned count;

    do {
        nanosleep(&ts, NULL);
        count = ucp_worker_progress(proc.comms.wrkr);
    } while (count > 0);
#else
    (void) ucp_worker_progress(proc.comms.wrkr);
#endif
}

/*
 * this is what runs in the progress thread to advance UCX.  Need to
 * think about how to back it off when things are progressing
 * automatically (blocking calls/wait/test).
 */

static void *
progress_impl_simple(void *unused_arg)
{
    return NULL;                /* can't work this out for now */
    while (go) {
        const double now = shmemu_timer();

        if ((now - last_call) > interval) {
            progress();
            last_call = now;
        }
        nanosleep(&ts, NULL);
    }

    return NULL;
}

static void *
progress_impl_event(void *unused_arg)
{
    ucs_status_t s;

    s = ucp_worker_arm(proc.comms.wrkr);
    assert(s == UCS_OK);

    while (go) {
        progress();
        nanosleep(&ts, NULL);

        /* wait for something to happen */
        s = ucp_worker_wait(proc.comms.wrkr);
        assert(s == UCS_OK);
    }

    return NULL;
}

/*
 * choose the progress implementation:
 *
 * currently one of
 *
 * "simple" = unrestrained polling loop    <-- current
 * "event"  = back off until signaled
 *
 */
static void *(*progress_call)(void *) = progress_impl_simple;

/*
 * start the progress thread
 */
void
shmemc_ucx_progress_init(void)
{
    int ps;

    /* throttle polling */
    setup_backoff();
    last_call = shmemu_timer();

    /* start progress thread */
    ps = pthread_create(&thread, NULL, progress_call, NULL);
    assert(ps == 0);

    logger(LOG_INIT, "created progress thread");
}

/*
 * terminate the progress thread
 */
void
shmemc_ucx_progress_finalize(void)
{
#if 0
    ucs_status_t s;
#endif
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

    logger(LOG_FINALIZE, "finished progress thread");
}
