#include "heapx.h"
#include "thispe.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void
heapx_init(void)
{
    if (proc.heaps == NULL) {
        fprintf(stderr, "%d: ranks = %d\n", proc.rank, proc.nranks);

        /* allocate heap stubs for all ranks */
        proc.heaps = (heapx_t *) calloc(proc.nranks,
                                      sizeof(heapx_t));
        assert(proc.heaps != NULL);
    }
}

void
heapx_finalize(void)
{
    if (proc.heaps != NULL) {
        free(proc.heaps);
    }
}
