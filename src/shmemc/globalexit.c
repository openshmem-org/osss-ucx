/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"
#include "pmi_client.h"

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

void
shmemc_global_exit(int status)
{
    shmemc_pmi_client_abort("global_exit", status);
}
