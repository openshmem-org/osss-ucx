/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <sys/time.h>

static double epoch;

inline static double
read_time(void)
{
    struct timeval t;

    gettimeofday(&t, NULL);

    return (double) (t.tv_sec + (t.tv_usec / 1.0e6));
}

void
shmemu_timer_init(void)
{
    epoch = read_time();
}

void
shmemu_timer_finalize(void)
{
    return;
}

double
shmemu_timer(void)
{
    return read_time() - epoch;
}
