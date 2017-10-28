/* For license: see LICENSE file at top-level */

#include "shmemu.h"
#include "shmemc.h"
#include "state.h"
#include "shmem/defs.h"

#include <pthread.h>
#include <assert.h>
#include <unistd.h>

/*
 * monitor thread and its sentinel
 */
static pthread_t thread;

enum sentinel_values {
    SENTINEL_ARMED = 0,
    SENTINEL_DONE,
    SENTINEL_ZAPPED
};

static long shmemc_globalexit_sentinel = SENTINEL_ARMED;
static int shmemc_globalexit_status = 0;

static void *
progress(void *unused)
{
    shmemc_long_wait_ne_until(&shmemc_globalexit_sentinel, SENTINEL_ARMED);

    if (shmemc_globalexit_sentinel == SENTINEL_ZAPPED) {
        _exit(shmemc_globalexit_status);
        /* NOT REACHED */
    }

    return NULL;
}

/*
 * start the monitor thread
 */
void
shmemc_globalexit_init(void)
{
    int ps;

    ps = pthread_create(&thread, NULL, progress, NULL);
    assert(ps == 0);

    logger(LOG_INIT, "created globalexit thread");
}

/*
 * terminate the monitor thread
 */
void
shmemc_globalexit_finalize(void)
{
    int ps;

    shmemc_globalexit_sentinel = SENTINEL_DONE;

    ps = pthread_join(thread, NULL);
    assert(ps == 0);

    logger(LOG_FINALIZE, "terminated globalexit thread");
}

static long shemmc_globalexit_sync = SHMEM_SYNC_VALUE;

void shmemc_trigger_globalexit(int status)
{
    logger(LOG_FINALIZE,
           "global_exit trigger (status = %d)",
           status);

    shmemc_globalexit_status = status;

    shmemc_globalexit_sentinel = SENTINEL_ZAPPED;

    shmemc_broadcast64(&shmemc_globalexit_sentinel,
                       &shmemc_globalexit_sentinel,
                       1,
                       0, proc.rank, 0, proc.nranks - 1,
                       &shemmc_globalexit_sync);
}
