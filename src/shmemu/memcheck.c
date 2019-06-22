/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "dlmalloc.h"
#include "shmemu.h"
#include "state.h"
#include "shmem/api.h"

#include <stdio.h>

int
report_corruption(mspace m)
{
    logger(LOG_MEMORY,
           "CORRUPTION DETECTED IN SPACE %p",
           m);
    if (proc.env.memfatal) {
        shmem_global_exit(1);
        /* NOT REACHED */
    }
    return 1;
}

int
report_usage_error(mspace m, void *p)
{
    logger(LOG_MEMORY,
           "USAGE ERROR DETECTED IN SPACE %p, ADDRESS %p",
           m, p);
    if (proc.env.memfatal) {
        shmem_global_exit(1);
        /* NOT REACHED */
    }
    return 1;
}
