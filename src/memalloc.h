/* For license: see LICENSE file at top-level */

#ifndef _SHMEM_MEMALLOC_H
#define _SHMEM_MEMALLOC_H 1

#include <sys/types.h>          /* size_t */

/*
 * memory allocation
 */
void shmemc_mem_init(void *base, size_t capacity);
void shmemc_mem_finalize(void);
void *shmemc_mem_base(void);
void *shmemc_mem_malloc(size_t size);
void *shmemc_mem_calloc(size_t count, size_t size);
void shmemc_mem_free(void *addr);
void *shmemc_mem_realloc(void *addr, size_t new_size);
void *shmemc_mem_align(size_t alignment, size_t size);

#endif /* ! _SHMEM_MEMALLOC_H */
