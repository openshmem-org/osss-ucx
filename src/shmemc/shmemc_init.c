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

    /* launch and connect me to network resources */
    shmemc_ucx_init();

    /* prep default context */
    shmemc_context_init_default();

    /* exchange default worker info */
    shmemc_pmi_publish_worker();
    shmemc_pmi_exchange_workers();

    /* exchange memory keys and (if needed) heap info */
    shmemc_pmi_publish_rkeys_and_heaps();
    shmemc_pmi_exchange_rkeys_and_heaps();

    /* populate endpoints of default context */
    shmemc_ucx_make_eps(&shmemc_default_context);
}

void
shmemc_finalize(void)
{
    shmemc_pmi_client_finalize();

    shmemc_ucx_finalize();
}
