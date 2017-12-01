/* For license: see LICENSE file at top-level */

#ifndef _SHMEM_MEMALLOC_H
#define _SHMEM_MEMALLOC_H 1

#include <sys/types.h>          /* size_t */

/*
 * memory allocation
 */
void shmema_mem_init(void *base, size_t capacity);
void shmema_mem_finalize(void);
void *shmema_mem_base(void);
void *shmema_mem_malloc(size_t size);
void *shmema_mem_calloc(size_t count, size_t size);
void shmema_mem_free(void *addr);
void *shmema_mem_realloc(void *addr, size_t new_size);
void *shmema_mem_align(size_t alignment, size_t size);

#endif /* ! _SHMEM_MEMALLOC_H */
