#include "heapx.h"
#include "state.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

heapx_t *global_segment = & proc.comms.heaps[0];
heapx_t *symm_segment = & proc.comms.heaps[1];

void
shmemc_heapx_init(void)
{
    if (proc.heaps == NULL) {
        /* allocate heap stubs for all ranks */
        proc.heaps = (heapx_t *) calloc(proc.nranks,
                                      sizeof(*proc.heaps));
        assert(proc.heaps != NULL);
    }
}

void
shmemc_heapx_finalize(void)
{
    if (proc.heaps != NULL) {
        free(proc.heaps);
    }
}
