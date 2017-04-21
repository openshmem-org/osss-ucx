#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <sys/time.h>

#ifdef ENABLE_PSHMEM
#pragma weak shmemx_wtime = pshmemx_wtime
#define shmemx_wtime pshmemx_wtime
#endif /* ENABLE_PSHMEM */

double
shmemx_wtime(void)
{
    struct timeval t;

    if (gettimeofday(&t, NULL) != 0) {
        return 0.0;
    }

    return ((t.tv_sec * 1.0e6) + t.tv_usec) / 1.0e6;
}
