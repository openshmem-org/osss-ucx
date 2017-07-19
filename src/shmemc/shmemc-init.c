#include "shmemc-ucx.h"
#include "pmix-client.h"
#include "thispe.h"

void
shmemc_init(void)
{
    /* find launch info */
    pmix_client_init();

    /* create heap stubs, 1 per PE for later exchange */
    heapx_init();

    /* launch and connect my heap to network resources */
    shmemc_ucx_init();

    /* now heap registered... */
    pmix_publish_heap_info();
    pmix_exchange_heap_info();

    proc.status = SHMEM_PE_RUNNING;
 }

void
shmemc_finalize(void)
{
    shmemc_ucx_finalize();

    pmix_client_finalize();

    proc.status = SHMEM_PE_SHUTDOWN;
}
