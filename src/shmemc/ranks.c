/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "state.h"

int
shmemc_my_pe(void)
{
    return proc.rank;
}

int
shmemc_n_pes(void)
{
    return proc.nranks;
}
