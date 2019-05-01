/* For license: see LICENSE file at top-level */

#ifndef _SHMEMC_YIELDER_H
#define _SHMEMC_YIELDER_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_SCHED_H
# include <sched.h>
#endif  /* HAVE_SCHED_H */
#ifdef HAVE_TIME_H
# include <time.h>
#endif  /* HAVE_SCHED_H */

inline static void
yielder(void)
{
#ifdef HAVE_SCHED_YIELD
    sched_yield();
#elif HAVE_NANOSLEEP
    const struct timespec req = { 0, 100 };

    (void) nanosleep(&req, NULL);
#endif  /* HAVE_SCHED_YIELDER || NANOSLEEP */
}

#endif /* ! _SHMEMC_YIELDER_H */
