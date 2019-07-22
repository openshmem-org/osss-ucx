/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "internal-malloc.h"
#include "shmemu.h"
#include "shmemc.h"
#include "state.h"

/*
 * This is the handler for mspace corruption in dlmalloc.
 */

void
report_corruption(mspace m)
{
    logger(LOG_MEMORY,
           "CORRUPTION DETECTED IN SPACE %p",
           m);
    if (proc.env.memfatal) {
        shmemc_global_exit(1);
        /* NOT REACHED */
    }
}

/*
 * This is the handler for mspace bad usage, like allocation overflow.
 */

void
report_usage_error(mspace m, void *p)
{
    logger(LOG_MEMORY,
           "USAGE ERROR DETECTED IN SPACE %p, ADDRESS %p",
           m, p);
    if (proc.env.memfatal) {
        shmemc_global_exit(1);
        /* NOT REACHED */
    }
}
