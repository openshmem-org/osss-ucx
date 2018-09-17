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

static pthread_t thr;

/*
 * for refractory back-off
 */

static long delay = 1000;      /* ns */
static struct timespec delayspec;

/*
 * polling sentinel
 */

static volatile bool done = false;

/*
 * Does comms. service until told not to
 */

static void *
start_progress(void *unused)
{
    NO_WARN_UNUSED(unused);

    do {
        shmemc_progress();
        nanosleep(&delayspec, NULL);   /* back off */
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
        delayspec.tv_sec = (time_t) 0;
        delayspec.tv_nsec = delay;

        const int s = pthread_create(&thr, NULL,
                                     start_progress, (void *) 0);
        NO_WARN_UNUSED(s);

    }
}

/*
 * stop the servicer
 */

void
progress_finalize(void)
{
    if (proc.env.progress_thread) {
        done = true;

        const int s = pthread_join(thr, NULL);
        NO_WARN_UNUSED(s);

    }
}
