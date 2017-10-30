/* For license: see LICENSE file at top-level */

#ifndef _SHMEM_MEMALLOC_H
#define _SHMEM_MEMALLOC_H 1

#include <sys/types.h>          /* size_t */

/*
 * memory allocation
 */
void shmemm_mem_init(void *base, size_t capacity);
void shmemm_mem_finalize(void);
void *shmemm_mem_base(void);
void *shmemm_mem_malloc(size_t size);
void *shmemm_mem_calloc(size_t count, size_t size);
void shmemm_mem_free(void *addr);
void *shmemm_mem_realloc(void *addr, size_t new_size);
void *shmemm_mem_align(size_t alignment, size_t size);

#endif /* ! _SHMEM_MEMALLOC_H */
