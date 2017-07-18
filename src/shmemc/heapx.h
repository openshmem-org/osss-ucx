#ifndef _HEAPX_H
#define _HEAPX_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <sys/types.h>

typedef struct symmetric_heap {
    void *base;                 /* start address */
    size_t size;                /* bytes in this heap */
} symmetric_heap_t;

/*
 * each PE has nheaps symmetric heaps
 */
typedef symmetric_heap_t *symmetric_heaps_t;

/*
 * each PE records nranks-1 exchange info of all PEs
 */
typedef symmetric_heaps_t *heapx_t;

#endif /* ! _HEAPX_H */
