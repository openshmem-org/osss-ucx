#include <stdio.h>
#include <sys/time.h>

static long epoch;

static
inline
long
read_time_us(void)
{
    struct timeval t;

    gettimeofday(&t, NULL);

    return (long) ((t.tv_sec * 1e6) + t.tv_usec);
}

void
shmemi_timer_init(void)
{
    epoch = read_time_us();
}

void
shmemi_timer_finalize(void)
{
}

long
shmemi_timer_get_elapsed_us(void)
{
    return read_time_us() - epoch;
}
