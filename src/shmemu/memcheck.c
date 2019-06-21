#include "dlmalloc.h"
#include "shmemu.h"
#include "shmem/api.h"

#include <stdio.h>

int
report_corruption(mspace m)
{
    logger(LOG_MEMORY,
           "CORRUPTION DETECTED IN SPACE %p",
           m);
    shmem_global_exit(1);
    /* NOT REACHED */
    return 1;
}

int
report_usage_error(mspace m, void *p)
{
    logger(LOG_MEMORY,
           "USAGE ERROR DETECTED IN SPACE %p, ADDRESS %p",
           m, p);
    shmem_global_exit(1);
    /* NOT REACHED */
    return 1;
}
