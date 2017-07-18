#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <assert.h>

#include "thispe.h"

void
heapx_init(void)
{
    int pe;
    int h;

    /* allocate heap stubs for all ranks */
    proc.heaps = (heapx_t *) calloc(proc.nranks, sizeof(heapx_t));
    assert(proc.heaps != NULL);

    for (pe = 0; pe < proc.nranks; pe += 1) {
        /* foreach rank, allocate symmetric heap stubs */
        proc.heaps[pe] =
            (symmetric_heaps_t *) calloc(proc.nheaps,
                                         sizeof(symmetric_heaps_t));
        for (h = 0; h < proc.nheaps; h += 1) {
            /* present, but uninitialized */
            proc.heaps[pe][h]->base = NULL;
        }
    }
}

void
heapx_finalize(void)
{
    if (proc.heaps != NULL) {
        int pe;

        for (pe = 0; pe < proc.nranks; pe += 1) {
            int h;

            for (h = 0; h < proc.nheaps; h += 1) {
                free(proc.heaps[pe][h]);
            }
        }

        free(proc.heaps[pe]);
    }

    free(proc.heaps);
}
