#include "thispe.h"

#include <pthread.h>
#include <time.h>
#include <assert.h>

static pthread_t thread;

static short go = 1;

static struct timespec ts;
static unsigned long backoff = 10000; /* nanoseconds */

static void
setup_backoff(void)
{
    ts.tv_sec = 0;
    ts.tv_nsec = backoff;
}

static void *
progress(void *unused_arg)
{
    do {
        ucp_worker_progress(proc.comms.wrkr);
        //  nanosleep(&ts, NULL);
        sleep(1);
    } while (go);

    return NULL;
}

void
shmemc_ucx_progress_init(void)
{
    int s;

    setup_backoff();

    s = pthread_create(&thread, NULL, progress, NULL);
    assert(s == 0);
}

void
shmemc_ucx_progress_finalize(void)
{
    int s;

    go = 0;

    s = pthread_join(thread, NULL);
    assert(s == 0);
}
