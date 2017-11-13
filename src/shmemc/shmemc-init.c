/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc-ucx.h"
#include "pmix-client.h"
#include "shmemu.h"
#include "shmemc.h"

void
shmemc_init(void)
{
    /* find launch info */
    shmemc_pmix_client_init();

    /* launch and connect my heap to network resources */
    shmemc_ucx_init();

    /* now heap registered... */
    /* TODO: don't do this if all addresses fixed */
    shmemc_pmix_publish_heap_info();
    shmemc_pmix_publish_worker();
    shmemc_pmix_barrier_all();

    shmemc_pmix_exchange_heap_info();
    /* exchange worker info and then create EPs */
    shmemc_pmix_exchange_workers();
    shmemc_ucx_make_remote_endpoints();

    shmemc_pmix_publish_my_rkeys();
    shmemc_pmix_barrier_all();
    shmemc_pmix_exchange_all_rkeys();

#if 0
    shmemc_barrier_all();
#endif
}

void
shmemc_finalize(void)
{
    shmemc_barrier_all();       /* finalize has implicit global barrier */

    /* shmemc_pmix_barrier_all(); */
    shmemc_pmix_client_finalize();

    shmemc_ucx_finalize();
}
