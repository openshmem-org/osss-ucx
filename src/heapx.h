#ifndef _HEAPX_H
#define _HEAPX_H 1

#include <sys/types.h>

typedef struct heap_exchange {
    void *base;                 /* start address */
    size_t size;                /* bytes in this heap */
    short filled;               /* has this been exchanged yet? */
} heap_exchange_t;

extern heap_exchange_t *heapx;

/* just hack for now */
#define HEAP_SIZE (size_t) 4096

void heapx_set_pe(int pe, void *p, size_t s);

void heapx_create(size_t s);

int heapx_initialized(int n);

void heapx_init(void);
void heapx_finalize(void);

#endif /* ! _HEAPX_H */
