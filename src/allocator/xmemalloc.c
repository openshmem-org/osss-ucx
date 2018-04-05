/* For license: see LICENSE file at top-level */

#include "xmemalloc.h"
#include "klib/khash.h"

#include <stdio.h>

KHASH_MAP_INIT_STR(heapnames, shmemx_heap_index_t)

static khash_t(heapnames) *names;

static shmemx_heap_index_t idx = 0;

/*
 * translate between name and index (create if needed)
 */

inline static shmemx_heap_index_t
lookup_name(const char *name)
{
    const khiter_t k = kh_get(heapnames, names, name);

    if (k != kh_end(names)) {
        return kh_value(names, k);
    }
    else {
        return -1;
    }
}

inline static shmemx_heap_index_t
record_name(const char *name)
{
    int there;
    khiter_t k;
    const shmemx_heap_index_t mine = idx;

    k = kh_put(heapnames, names, name, &there);
    if (there != 1) {
        return -1;
    }

    kh_value(names, k) = mine;

    idx += 1;

    return mine;
}

shmemx_heap_index_t
shmemxa_name_to_index(const char *name)
{
    const shmemx_heap_index_t i = lookup_name(name);

    if (i == -1) {
        return record_name(name);
    }
    else {
        return i;
    }
}

char *
shmemxa_index_to_name(shmemx_heap_index_t index)
{
    khiter_t k;

    for (k = kh_begin(names); k != kh_end(names); k += 1) {
		if (kh_exist(names, k)) {
            if (kh_value(names, k) == index) {
                return (char *) kh_key(names, k);
                /* NOT REACHED */
            }
        }
    }

    return NULL;
}

void
shmemxa_init_by_index(shmemx_heap_index_t index,
                      void *base, size_t capacity)
{
    return; /* myspace = create_mspace_with_base(base, capacity, 1); */
}

void
shmemxa_finalize_by_index(shmemx_heap_index_t index)
{
    return; /* destroy_mspace(myspace); */
}

void *
shmemxa_base_by_index(shmemx_heap_index_t index)
{
    return NULL;
}

void
shmemxa_init_by_name(const char *name,
                     void *base, size_t capacity)
{
    return; /* myspace = create_mspace_with_base(base, capacity, 1); */
}



void *
shmemxa_malloc_by_index(shmemx_heap_index_t index,
                        size_t size)
{
    return NULL;
}

void *
shmemxa_calloc_by_index(shmemx_heap_index_t index,
                        size_t count, size_t size)
{
    return NULL;
}

void
shmemxa_free_by_index(shmemx_heap_index_t index,
                      void *addr)
{
    return;
}

void *
shmemxa_realloc_by_index(shmemx_heap_index_t index,
                         void *addr, size_t new_size)
{
    return NULL;
}

void *
shmemxa_align_by_index(shmemx_heap_index_t index,
                       size_t alignment, size_t size)
{
    return NULL;
}
