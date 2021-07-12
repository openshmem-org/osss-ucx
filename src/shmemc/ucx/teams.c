/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"
#include "api.h"

#include <stdlib.h>

#include <ucp/api/ucp.h>

void
shmemc_ucx_teardown_context(shmemc_context_h ch)
{
    size_t r;
    int pe;

    shmemc_ucx_disconnect_all_eps(ch);
    /* release remote access memory */
    for (r = 0; r < proc.comms.nregions; ++r) {
        for (pe = 0; pe < proc.li.nranks; ++pe) {
            ucp_rkey_destroy(ch->racc[r].rinfo[pe].rkey);
        }
        free(ch->racc[r].rinfo);
    }
    free(ch->racc);

    shmemc_ucx_deallocate_eps_table(ch);
    ucp_worker_destroy(ch->w);
}

void
shmemc_ucx_team_world_create(void)
{
    return;
}

void
shmemc_ucx_team_world_destroy(void)
{
    return;
}
