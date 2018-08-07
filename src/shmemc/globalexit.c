/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <unistd.h>

/*
 * start up the exit monitor
 */
void
shmemc_globalexit_init(void)
{
}

/*
 * shut down the exit monitor
 */
void
shmemc_globalexit_finalize(void)
{
}

#ifdef HAVE__EXIT
# define EXIT _exit
#elif defined(HAVE_EXIT)
# define EXIT exit
#else
# error "no exit() routine found"
#endif

void
shmemc_global_exit(int status)
{
    /* just forcing an exit locally should abort the PMIx launcher */
    EXIT(status);
}
