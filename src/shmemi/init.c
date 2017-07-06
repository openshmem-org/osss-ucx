#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "shmemi.h"
#include "shmemu.h"

void
shmemi_setup_heaps(void)
{
    // ??? api.heap_setup_fn();
}

static int ref_count = 0;

void
shmemi_finalize(void)
{
    logger(LOG_FINALIZE,
           "entering finalize shmemi (ref #%d)",
           ref_count);

    if (ref_count == 1) {
        logger(LOG_FINALIZE, "finalizing shmemi");
    }

    ref_count -= 1;
}

void
shmemi_init(void)
{
    logger(LOG_INIT,
           "entering shmemi (ref #%d)",
           ref_count);

    if (ref_count == 0) {
        const int s = atexit(shmemi_finalize);
        assert(s == 0);
    }

    ref_count += 1;
}
