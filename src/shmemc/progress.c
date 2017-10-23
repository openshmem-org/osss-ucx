#include "shmemu.h"
#include "state.h"

#include <pthread.h>
#include <time.h>
#include <assert.h>

#include <ucp/api/ucp.h>

static pthread_t thread;

static volatile short go = 1;

static struct timespec ts;
static unsigned long backoff = 1e6; /* nanoseconds */

static double last_call;

inline static void
setup_backoff(void)
{
    ts.tv_sec = 0;
    ts.tv_nsec = backoff;
}

inline static void
progress(void)
{
    (void) ucp_worker_progress(proc.comms.wrkr);
}

void
shmemc_fence(void)
{
    ucs_status_t s;

    s = ucp_worker_fence(proc.comms.wrkr);
    assert(s == UCS_OK);
}

void
shmemc_quiet(void)
{
    ucs_status_t s;

    s = ucp_worker_flush(proc.comms.wrkr);
    assert(s == UCS_OK);
}

/*
 * this is what runs in the progress thread to advance UCX.  Need to
 * think about how to back it off when things are progressing
 * automatically (blocking calls/wait/test).
 */

static void *
progress_impl_simple(void *unused_arg)
{
    while (go) {
        const double now = shmemu_timer();

        if ((now - last_call) > 0.5) {
            progress();
            last_call = now;
        }
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
 * "simple" = unrestrained polling loop
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

    /* start progress thread */
    ps = pthread_create(&thread, NULL, progress_call, NULL);
    assert(ps == 0);

    last_call = shmemu_timer();

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
