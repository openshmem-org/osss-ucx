/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "boolean.h"
#include "shmemc.h"
#include "shmemu.h"

#include <stdio.h>

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309
#endif /* _POSIX_C_SOURCE */
#include <time.h>

/*
 * new thread for progress-o-matic
 */

static pthread_t progress_thread;

/*
 * for refractory back-off (nanoseconds)
 *
 * N.B. mutable in case we want to look at adaptive polling
 */

static long delay_ns = 1000;

/*
 * polling sentinel
 */

static volatile bool done = false;

/*
 * Does comms. service until told not to
 */

static void *
start_progress(void *args)
{
    const struct timespec *tsp = (const struct timespec *) args;

    do {
        shmemc_progress();
        nanosleep(tsp, NULL);   /* back off */
    }
    while (! done);

    return NULL;
}

/*
 * start the servicer
 */

void
progress_init(void)
{
    if (proc.env.progress_thread) {
        struct timespec ts;
        int s;

        ts.tv_sec  = (time_t) 0;
        ts.tv_nsec = delay_ns;

        s = pthread_create(&progress_thread, NULL,
                           start_progress, (void *) &ts);
        shmemu_assert(s == 0,
                      "Could not create progress thread (status = %d)",
                      s);
    }
}

/*
 * stop the servicer
 */

void
progress_finalize(void)
{
    if (proc.env.progress_thread) {
        int s;

        done = true;

        s = pthread_join(progress_thread, NULL);
        shmemu_assert(s == 0,
                      "Could not terminate progress thread (status = %d)",
                      s);
    }
}
