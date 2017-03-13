#include <stdio.h>
#include <sys/time.h>

double
shmemx_wtime(void)
{
    struct timeval t;

    if (gettimeofday(&t, NULL) != 0) {
        return 0.0;
    }

    return ((t.tv_sec * 1.0e6) + t.tv_usec) / 1.0e6;
}
