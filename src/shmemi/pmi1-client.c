#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>
#include <pmi.h>

#include "shmemi.h"
#include "shmemc.h"

/*
 * if finalize called through atexit, force a barrier
 */

static
void
shmemi_finalize_handler_pmi1(bool need_barrier)
{
    int ret;

    if (need_barrier) {
        logger(LOG_FINALIZE, "PE still alive, add barrier to finalize");
    }

#if 1
    ret = PMI_Finalize();
    assert(ret == PMI_SUCCESS);
#endif

    proc.status = PE_SHUTDOWN;
}

static
void
shmemi_finalize_atexit_pmi1(void)
{
    shmemi_finalize_handler_pmi1(proc.status == PE_RUNNING);
}

void
shmemi_finalize_pmi1(void)
{
    shmemi_finalize_handler_pmi1(false);
}

void
shmemi_init_pmi1(void)
{
    int ret;

    /* this isn't working, let's try environment instead */
#if 1
    int spawned = 0;

    ret = PMI_Init(&spawned);
    assert(ret == PMI_SUCCESS);

    ret = PMI_Get_size(&proc.nranks);
    assert(ret == PMI_SUCCESS);

    ret = PMI_Get_rank(&proc.rank);
    assert(ret == PMI_SUCCESS);
#else

    proc.nranks = atoi(shmemc_getenv("PMI_SIZE"));
    proc.rank = atoi(shmemc_getenv("PMI_RANK"));
#endif

    ret = atexit(shmemi_finalize_atexit_pmi1);
    assert(ret == 0);

    proc.status = PE_RUNNING;
}

void
shmemi_setup_heaps_pmi1(void)
{
}
