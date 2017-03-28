#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <assert.h>

#include "shmemi.h"

heapx_t heapx = NULL;
int nheaps = 1;

void
shmemi_heapx_set_pe(int idx, int pe, void *p, size_t s)
{
    heapx[idx][pe].base = p;
    heapx[idx][pe].size = s;   /* alignment may change request size */
    heapx[idx][pe].filled = 1;
}

void
shmemi_heapx_create(int idx, size_t s)
{
    heapx[idx] = (heap_exchange_t *) calloc(p.npes, sizeof(*heapx[idx]));
    assert(heapx[idx] != NULL);

    /* TODO malloc is clearly a dummy here */

    shmemi_heapx_set_pe(idx, p.me, malloc(s), s);
}

void
shmemi_heapx_init(void)
{
    int i;

    heapx = (heapx_t) calloc(nheaps, sizeof(*heapx));
    assert(heapx != NULL);

    for (i = 0; i < nheaps; i += 1) {
        shmemi_heapx_create(i, HEAP_SIZE);
    }
    shmemi_setup_heaps();
}

int
shmemi_heapx_initialized(int idx)
{
    if (heapx != NULL) {
        return heapx[idx][p.me].filled;
    }

    return 0;
}

void
shmemi_heapx_finalize(void)
{
    if (heapx != NULL) {
        int i;

        for (i = 0; i < nheaps; i += 1) {
            free(heapx[i][p.me].base);
        }
        free(heapx);
    }
}
