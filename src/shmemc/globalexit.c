/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <unistd.h>

/*
 * start the monitor thread
 */
void
shmemc_globalexit_init(void)
{
}

/*
 * terminate the monitor thread
 */
void
shmemc_globalexit_finalize(void)
{
}

void
shmemc_global_exit(int status)
{
    /* just forcing an exit locally should abort the PMIx launcher */
    _exit(status);
}
