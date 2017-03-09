#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

#include "shmemi/shmemi.h"

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
