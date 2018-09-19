/* For license: see LICENSE file at top-level */

/*
 * This code only gets activated if UCX's emulation mode is being
 * used: where we're running on legacy hardware, or where we don't
 * have direct transport-supported RDMA and AMO.
 *
 * Further, this is only enabled if asked for by the end-user.
 *
 * Hope in the future to fine-tune this if UCX can tell us whether its
 * enmulation mode is being used.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "boolean.h"
#include "shmemc.h"
#include "shmemu.h"
#include "threading.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309
#endif /* _POSIX_C_SOURCE */
#include <time.h>

/*
 * new thread for progress-o-matic
 */

static threadwrap_thread_t thr;

/*
 * for refractory back-off (nanoseconds)
 *
 * N.B. mutable in case we want to look at adaptive polling
 */

static volatile long delay_ns = 1000;

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
    NO_WARN_UNUSED(args);

    do {
        const struct timespec ts = {
            .tv_sec  = (time_t) 0,
            .tv_nsec = delay_ns
        };

        shmemc_progress();

        nanosleep(&ts, NULL);   /* back off */
    }
    while (! done);

    return NULL;
}

/*
 * return 1 if progress required, 0 otherwise
 */
static int
check_if_progress_required(void)
{
    int *res = NULL;
    size_t nres;
    int s;
    int ret = 0;
    char *copy;

    if (proc.env.progress_threads == NULL) {
        goto out;
        /* NOT REACHED */
    }

    if (strncasecmp(proc.env.progress_threads, "all", 3) == 0) {
        ret = 1;
        goto out;
        /* NOT REACHED */
    }

    /* shmemu_parse_csv zaps the input string */
    copy = strdup(proc.env.progress_threads);
    if (copy == NULL) {
        logger(LOG_FATAL,
               "Unable to allocate memory during progress thread check: %s",
               strerror(errno)
               );
        /* NOT REACHED */
    }

    s = shmemu_parse_csv(copy, &res, &nres);
    if (s > 0) {
        size_t i;

        for (i = 0; i < nres; i += 1) {
            if (res[i] == proc.rank) {
                ret = 1;
                goto out_free;
                /* NOT REACHED */
            }
        }
    }

 out_free:
    free(res);
    free(copy);
 out:
    return ret;
}

/*
 * start the servicer
 */

void
progress_init(void)
{
    proc.progress_thread = check_if_progress_required();

    if (proc.progress_thread) {
        int s;

        s = threadwrap_thread_create(&thr, start_progress, NULL);
        shmemu_assert(s == 0,
                      "Could not create progress thread (%s)",
                      strerror(s));
    }
}

/*
 * stop the servicer
 */

void
progress_finalize(void)
{
    if (proc.progress_thread) {
        int s;

        done = true;

        s = threadwrap_thread_join(thr, NULL);
        shmemu_assert(s == 0,
                      "Could not terminate progress thread (%s)",
                      strerror(s));

    }
}

/*
 * allow changing the delay
 */

void
progress_set_delay(long newdelay)
{
    delay_ns = newdelay;
}
