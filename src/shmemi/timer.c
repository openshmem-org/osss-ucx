#include <stdio.h>
#include <sys/time.h>

static double epoch;

static
inline
double
read_time(void)
{
    struct timeval t;

    gettimeofday(&t, NULL);

    return (double) (t.tv_sec + (t.tv_usec / 1.0e6));
}

void
shmemi_timer_init(void)
{
    epoch = read_time();
}

void
shmemi_timer_finalize(void)
{
}

double
shmemi_timer_get_elapsed(void)
{
    return read_time() - epoch;
}
