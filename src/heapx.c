#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <assert.h>

#include "heapx.h"
#include "pe.h"
#include "logger.h"
#include "shmemi_pmix.h"

heap_exchange_t *heapx = NULL;

void
heapx_set_pe(int pe, void *p, size_t s)
{
    heapx[pe].base = p;
    heapx[pe].size = s;       /* alignment may change request size */
    heapx[pe].filled = 1;
}

void
heapx_create(size_t s)
{
    heapx = (heap_exchange_t *) calloc(p.npes, sizeof(*heapx));
    assert(heapx != NULL);

    heapx_set_pe(p.me, malloc(s), s);
}

/*
 * ignore index for now
 */
int
heapx_initialized(int n)
{
    if (heapx == NULL) {
        return 0;
    }
    else {
        return heapx[p.me].filled;
    }
}

void
heapx_init(void)
{
    heapx_create(HEAP_SIZE);
    shmemi_setup_heaps_pmix();
}

void
heapx_finalize(void)
{
    if (heapx != NULL) {
        free(heapx[p.me].base);
        free(heapx);
    }
}
