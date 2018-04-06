/* For license: see LICENSE file at top-level */

#ifndef _SHMEMXA_MEMALLOC_H
#define _SHMEMXA_MEMALLOC_H 1

#include <sys/types.h>          /* size_t */

/*
 * translate between heap names and indices
 */

typedef int shmemx_heap_index_t;

shmemx_heap_index_t shmemxa_name_to_index(const char *name);
char *shmemxa_index_to_name(shmemx_heap_index_t index);

/*
 * memory allocation
 */
void shmemxa_init(shmemx_heap_index_t numheaps);
void shmemxa_finalize(void);

void shmemxa_init_by_index(shmemx_heap_index_t index,
                           void *base, size_t capacity);
void shmemxa_finalize_by_index(shmemx_heap_index_t index);

void *shmemxa_base_by_index(shmemx_heap_index_t index);
void *shmemxa_malloc_by_index(shmemx_heap_index_t index,
                              size_t size);
void *shmemxa_calloc_by_index(shmemx_heap_index_t index,
                              size_t count, size_t size);
void  shmemxa_free_by_index(shmemx_heap_index_t index,
                            void *addr);
void *shmemxa_realloc_by_index(shmemx_heap_index_t index,
                               void *addr, size_t new_size);
void *shmemxa_align_by_index(shmemx_heap_index_t index,
                             size_t alignment, size_t size);

#endif /* ! _SHMEMXA_MEMALLOC_H */
