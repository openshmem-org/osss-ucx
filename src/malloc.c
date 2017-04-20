#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

#include "shmem/defs.h"
#include "shmemi/shmemi.h"

int malloc_error = SHMEM_MALLOC_OK;

void *
shmem_malloc(size_t s)
{
    return shmemi_malloc(s);
}

void
shmem_free(void *p)
{
    shmemi_free(p);
}

void *
shmem_realloc(void *p, size_t s)
{
    return shmemi_realloc(p, s);
}

void *
shmem_align(size_t a, size_t s)
{
    return shmemi_align(a, s);
}


void *
shmalloc(size_t s)
{
    deprecate(__func__);
    return shmem_malloc(s);
}

void
shfree(void *p)
{
    deprecate(__func__);
    shmem_free(p);
}

void *
shrealloc(void *p, size_t s)
{
    deprecate(__func__);
    return shmem_realloc(p, s);
}

void *
shmemalign(size_t a, size_t s)
{
    deprecate(__func__);
    return shmem_align(a, s);
}
