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

inline static void
terminate_thread(void)
{
    int ps;

    ps = pthread_join(thread, NULL);
    assert(ps == 0);
}

static void *
progress(void *unused)
{
    shmemc_wait_ne_until64(&shmemc_globalexit_sentinel, SENTINEL_ARMED);

    if (shmemc_globalexit_sentinel == SENTINEL_ZAPPED) {
        _exit(shmemc_globalexit_status);
        /* NOT REACHED */
    }

    return NULL;
}

inline static void
start_thread(void)
{
    int ps;

    ps = pthread_create(&thread, NULL, progress, NULL);
    assert(ps == 0);
}

/*
 * start the monitor thread
 */
void
shmemc_globalexit_init(void)
{
    start_thread();

    logger(LOG_INIT, "created globalexit thread");
}

/*
 * terminate the monitor thread
 */
void
shmemc_globalexit_finalize(void)
{
    shmemc_globalexit_sentinel = SENTINEL_DONE;
    terminate_thread();

    logger(LOG_FINALIZE, "terminated globalexit thread");
}

/*
 * Signalling shutdown with raw puts to avoid e.g. a broadcast getting
 * stuck as PEs fall off the map...
 *
 * (also doing a redundant but harmless put-to-self to avoid loop
 * conditional/split)
 */
inline static void
tell_pes(void)
{
    int i;

    for (i = 0; i < proc.nranks; i += 1) {
        shmemc_put(&shmemc_globalexit_sentinel,
                   &shmemc_globalexit_sentinel,
                   1,
                   i);
    }
}

void
shmemc_trigger_globalexit(int status)
{

    shmemc_globalexit_status = status;

    logger(LOG_FINALIZE,
           "global_exit trigger (status = %d)",
           status);

    shmemc_globalexit_sentinel = SENTINEL_ZAPPED;

    tell_pes();

    terminate_thread();
}
