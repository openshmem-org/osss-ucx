#ifndef _HEAPX_H
#define _HEAPX_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <sys/types.h>

#include <ucp/api/ucp.h>

/*
 * each PE has a symmetric heap, with a start address, and size.
 */
typedef struct heapx {
    void *base;                 /* start of this region */
    size_t length;              /* its size (b) */
} heapx_t;

void shmemc_heapx_init(void);
void shmemc_heapx_finalize(void);

#endif /* ! _HEAPX_H */
