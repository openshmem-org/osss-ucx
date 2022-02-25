/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmem/api.h"

/*
 * layer these through top-level API in case we want to insert error
 * checking above
 */

#ifdef ENABLE_PSHMEM
#pragma weak shmalloc = pshmalloc
#define shmalloc pshmalloc
#pragma weak shfree = pshfree
#define shfree pshfree
#pragma weak shrealloc = pshrealloc
#define shrealloc pshrealloc
#pragma weak shmemalign = pshmemalign
#define shmemalign pshmemalign
#endif /* ENABLE_PSHMEM */

#define DEPR_MAJOR 1
#define DEPR_MINOR 2

void *
shmalloc(size_t s)
{
    deprecate(__func__, DEPR_MAJOR, DEPR_MINOR);
    return shmem_malloc(s);
}

void
shfree(void *p)
{
    deprecate(__func__, DEPR_MAJOR, DEPR_MINOR);
    shmem_free(p);
}

void *
shrealloc(void *p, size_t s)
{
    deprecate(__func__, DEPR_MAJOR, DEPR_MINOR);
    return shmem_realloc(p, s);
}

void *
shmemalign(size_t a, size_t s)
{
    deprecate(__func__, DEPR_MAJOR, DEPR_MINOR);
    return shmem_align(a, s);
}
