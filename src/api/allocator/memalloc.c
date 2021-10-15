/* For license: see LICENSE file at top-level */

#include "internal-malloc.h"

#include "memalloc.h"

/**
 * the memory area we manage in this unit.
 *
 * Not visible to anyone else
 */
static mspace myspace;

/**
 * initialize the memory pool
 */
void
shmema_init(void *base, size_t capacity)
{
    myspace = create_mspace_with_base(base, capacity, 1);
}

/**
 * clean up memory pool
 */
void
shmema_finalize(void)
{
    destroy_mspace(myspace);
}

/**
 * return start of pool
 */
void *
shmema_base(void)
{
    return myspace;
}

/**
 * allocate SIZE bytes from the pool
 */

void *
shmema_malloc(size_t size)
{
    return mspace_malloc(myspace, size);
}

/**
 * allocate COUNT * SIZE bytes, zero out
 */

void *
shmema_calloc(size_t count, size_t size)
{
    return = mspace_calloc(myspace, count, size);
}

/**
 * release memory previously allocated at ADDR
 */
void
shmema_free(void *addr)
{
    mspace_free(myspace, addr);
}

/**
 * resize ADDR to NEW_SIZE bytes
 */
void *
shmema_realloc(void *addr, size_t new_size)
{
    return mspace_realloc(myspace, addr, new_size);
}

/**
 * allocate memory of SIZE bytes, aligning to ALIGNMENT
 */
void *
shmema_align(size_t alignment, size_t size)
{
    return mspace_memalign(myspace, alignment, size);
}
