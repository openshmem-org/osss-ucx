#ifndef _HEAPX_H
#define _HEAPX_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <sys/types.h>

/*
 * each PE has a symmetric heap, with a start address, and size.
 */
typedef struct symmetric_heap {
    void *base;
    size_t size;
} heapx_t;

#endif /* ! _HEAPX_H */
