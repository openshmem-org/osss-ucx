/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc-ucx.h"
#include "shmemu.h"
#include "shmemc.h"

#include "pmi-client.h"

void
shmemc_init(void)
{
    /* find launch info */
    shmemc_pmi_client_init();

    /* launch and connect my heap to network resources */
    shmemc_ucx_init();

    /* now heap registered... */

#ifndef ENABLE_ALIGNED_ADDRESSES
    shmemc_pmi_publish_heap_info();
#endif /* ! ENABLE_ALIGNED_ADDRESSES */

    shmemc_pmi_publish_worker();
    shmemc_pmi_barrier_all();

#ifndef ENABLE_ALIGNED_ADDRESSES
    shmemc_pmi_exchange_heap_info();
#endif /* ! ENABLE_ALIGNED_ADDRESSES */

    /* exchange worker info and then create EPs */
    shmemc_pmi_exchange_workers();
    shmemc_ucx_make_remote_endpoints();

    shmemc_pmi_publish_my_rkeys();
    shmemc_pmi_barrier_all();
    shmemc_pmi_exchange_all_rkeys();

    /*
     * TODO: 2018-04-27: newest UCX has revamped AMOs, getting hangs
     * without flush here
     */
    shmemc_quiet();
}

void
shmemc_finalize(void)
{
    /* finalize has implicit global barrier */
    shmemc_barrier_all();

    shmemc_pmi_client_finalize();

    shmemc_ucx_finalize();
}
