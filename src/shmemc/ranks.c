/* For license: see LICENSE file at top-level */

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
