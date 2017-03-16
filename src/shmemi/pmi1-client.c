#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>
#include <pmi.h>

#include "shmemi.h"

/*
 * if finalize called through atexit, force a barrier
 */

static
void
shmemi_finalize_handler_pmi1(bool need_barrier)
{
    int s;

    if (need_barrier) {
        logger(LOG_FINALIZE, "PE still alive, add barrier to finalize");
    }

#if 0
    s = PMI_Finalize();
    assert(s == PMI_SUCCESS);
#endif

    p.running = false;
}

static
void
shmemi_finalize_atexit_pmi1(void)
{
    shmemi_finalize_handler_pmi1(p.running);
}

void
shmemi_finalize_pmi1(void)
{
    shmemi_finalize_handler_pmi1(false);
}

void
shmemi_init_pmi1(void)
{
    int s;

    /* this isnn't working, let's try environment instead */
#if 0
    int spawned = 0;

    s = PMI_Init(&spawned);
    assert(s == PMI_SUCCESS);

    s = PMI_Get_size(&p.npes);
    assert(s == PMI_SUCCESS);

    s = PMI_Get_rank(&p.me);
    assert(s == PMI_SUCCESS);
#endif

    p.npes = atoi(getenv("PMI_SIZE"));
    p.me = atoi(getenv("PMI_RANK"));

    s = atexit(shmemi_finalize_atexit_pmi1);
    assert(s == 0);

    p.running = true;
}

void
shmemi_setup_heaps_pmi1(void)
{
}
