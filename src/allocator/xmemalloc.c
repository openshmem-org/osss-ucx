/* For license: see LICENSE file at top-level */

#include "xmemalloc.h"
#include "klib/khash.h"

#include "dlmalloc.h"

#include <stdio.h>
#include <assert.h>

static mspace *spaces;

/*
 * map named heap to its index
 */

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

/*
 * boot API
 */

static shmemx_heap_index_t nheaps; /* local copy */

void
shmemxa_init(shmemx_heap_index_t numheaps)
{
    spaces = (mspace *) malloc(numheaps * sizeof(*spaces));

    assert(spaces != NULL);

    nheaps = numheaps;
}

void
shmemxa_finalize(void)
{
    if (spaces != NULL) {
        free(spaces);
    }
}

/*
 * manage heap
 */

#define INDEX_CHECK(i)                          \
    do {                                        \
        assert( (0 <= (i)) && ((i) < nheaps) ); \
    } while (0)

void
shmemxa_init_by_index(shmemx_heap_index_t index,
                      void *base, size_t capacity)
{
    INDEX_CHECK(index);

    spaces[index] = create_mspace_with_base(base, capacity, 1);
}

void
shmemxa_finalize_by_index(shmemx_heap_index_t index)
{
    INDEX_CHECK(index);

    destroy_mspace(spaces[index]);
}

/*
 * heap allocations
 */

void *
shmemxa_base_by_index(shmemx_heap_index_t index)
{
    INDEX_CHECK(index);

    return spaces[index];
}

void *
shmemxa_malloc_by_index(shmemx_heap_index_t index,
                        size_t size)
{
    INDEX_CHECK(index);

    return mspace_malloc(spaces[index], size);
}

void *
shmemxa_calloc_by_index(shmemx_heap_index_t index,
                        size_t count, size_t size)
{
    INDEX_CHECK(index);

    return mspace_calloc(spaces[index], count, size);
}

void
shmemxa_free_by_index(shmemx_heap_index_t index,
                      void *addr)
{
    INDEX_CHECK(index);

    mspace_free(spaces[index], addr);
}

void *
shmemxa_realloc_by_index(shmemx_heap_index_t index,
                         void *addr, size_t new_size)
{
    INDEX_CHECK(index);

    return mspace_realloc(spaces[index], addr, new_size);
}

void *
shmemxa_align_by_index(shmemx_heap_index_t index,
                       size_t alignment, size_t size)
{
    INDEX_CHECK(index);

    return mspace_memalign(spaces[index], alignment, size);
}
