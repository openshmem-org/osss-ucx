#include "shmemc-ucx.h"
#include "pmix-client.h"
#include "state.h"
#include "shmemc.h"
#include "shmem/defs.h"

long *shmemc_all_sync;

inline static void
barrier_all_init(void)
{
    int i;

    shmemc_all_sync =
        (long *) shmemc_mem_alloc(SHMEM_BARRIER_SYNC_SIZE *
                                  sizeof(*shmemc_all_sync));
    for (i = 0; i < SHMEM_BARRIER_SYNC_SIZE; i += 1) {
        shmemc_all_sync[i] = SHMEM_SYNC_VALUE;
    }
}

inline static void
barrier_all_finalize(void)
{
    shmemc_mem_free(shmemc_all_sync);
}

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

    barrier_all_init();

    proc.status = SHMEM_PE_RUNNING;
}

void
shmemc_finalize(void)
{
    shmemc_ucx_finalize();

    shmemc_pmix_client_finalize();

    barrier_all_finalize();

    proc.status = SHMEM_PE_SHUTDOWN;
}
