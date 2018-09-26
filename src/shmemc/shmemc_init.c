/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "ucx/api.h"
#include "shmemc.h"

#include "pmi_client.h"

void
shmemc_init(void)
{
    /* find launch info */
    shmemc_pmi_client_init();

    /* launch and connect my heap to network resources */
    shmemc_ucx_init();

    /* now heap registered... */

    shmemc_pmi_publish_worker();

    /* exchange worker info and then create EPs */
    shmemc_pmi_exchange_workers();

    shmemc_ucx_make_remote_endpoints();

    shmemc_pmi_publish_rkeys_and_heaps();

    shmemc_pmi_exchange_rkeys_and_heaps();

    /*
     * TODO: 2018-04-27: newest UCX has revamped AMOs, getting hangs
     * without flush here
     */
    /* shmemc_quiet(); */
}

void
shmemc_finalize(void)
{
    shmemc_pmi_client_finalize();

    shmemc_ucx_finalize();
}
