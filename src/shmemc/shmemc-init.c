#include "shmemc-ucx.h"
#include "pmix-client.h"
#include "state.h"

void
shmemc_init(void)
{
    /* find launch info */
    shmemc_pmix_client_init();

    /* launch and connect my heap to network resources */
    shmemc_ucx_init();

    /* now heap registered... */
    shmemc_pmix_publish_heap_info();
    shmemc_pmix_exchange_heap_info();

    /* exchange worker info and then create EPs */
    shmemc_pmix_publish_worker();
    shmemc_pmix_exchange_workers();

    shmemc_ucx_make_remote_endpoints();

    shmemc_pmix_publish_my_rkeys();
    shmemc_pmix_exchange_all_rkeys();

    proc.status = SHMEM_PE_RUNNING;
 }

void
shmemc_finalize(void)
{
    shmemc_ucx_finalize();

    shmemc_pmix_client_finalize();

    proc.status = SHMEM_PE_SHUTDOWN;
}
