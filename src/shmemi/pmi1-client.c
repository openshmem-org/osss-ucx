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

    p.status = PE_SHUTDOWN;
}

static
void
shmemi_finalize_atexit_pmi1(void)
{
    shmemi_finalize_handler_pmi1(p.status == PE_RUNNING);
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

    ret = PMI_Get_size(&p.npes);
    assert(ret == PMI_SUCCESS);

    ret = PMI_Get_rank(&p.me);
    assert(ret == PMI_SUCCESS);
#else

    p.npes = atoi(shmemc_getenv("PMI_SIZE"));
    p.me = atoi(shmemc_getenv("PMI_RANK"));
#endif

    ret = atexit(shmemi_finalize_atexit_pmi1);
    assert(ret == 0);

    p.status = PE_RUNNING;
}

void
shmemi_setup_heaps_pmi1(void)
{
}
