#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <pmi2.h>

#include "shmemi.h"

/* STUB == STUB == STUB == STUB == STUB == STUB == STUB == STUB */

/*
 * if finalize called through atexit, force a barrier
 */

static
void
shmemi_finalize_handler_pmi2(bool need_barrier)
{
    if (need_barrier) {
        shmemi_logger(LOG_FINALIZE, "PE still alive, add barrier to finalize");
    }


    p.running = false;
}

static
void
shmemi_finalize_atexit_pmi2(void)
{
    shmemi_finalize_handler_pmi2(p.running);
}

void
shmemi_finalize_pmi2(void)
{
    shmemi_finalize_handler_pmi2(false);
}

void
shmemi_init_pmi2(void)
{
    int s;

    s = atexit(shmemi_finalize_atexit_pmi2);
    assert(s == 0);

    p.running = true;
}

void
shmemi_setup_heaps_pmi2(void)
{
}
