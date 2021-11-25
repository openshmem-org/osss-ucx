/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"
#include "shmem_mutex.h"
#include "allocator/xmemalloc.h"

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

/*
 * -- API --------------------------------------------------------------------
 */

#ifdef ENABLE_PSHMEM
#pragma weak shmemx_name_to_index = pshmemx_name_to_index
#define shmemx_name_to_index pshmemx_name_to_index
#pragma weak shmemx_index_to_name = pshmemx_index_to_name
#define shmemx_index_to_name pshmemx_index_to_name
#endif /* ENABLE_PSHMEM */

shmemx_heap_index_t
shmemx_name_to_index(const char *name)
{
    return shmemxa_name_to_index(name);
}

const char *
shmemx_index_to_name(shmemx_heap_index_t index)
{
    return shmemxa_index_to_name(index);
}

/*
 * access by numerical index
 */

#ifdef ENABLE_PSHMEM
#pragma weak shmemx_malloc_by_index = pshmemx_malloc_by_index
#define shmemx_malloc_by_index pshmemx_malloc_by_index
#pragma weak shmemx_calloc_by_index = pshmemx_calloc_by_index
#define shmemx_calloc_by_index pshmemx_calloc_by_index
#pragma weak shmemx_free_by_index = pshmemx_free_by_index
#define shmemx_free_by_index pshmemx_free_by_index
#pragma weak shmemx_realloc_by_index = pshmemx_realloc_by_index
#define shmemx_realloc_by_index pshmemx_realloc_by_index
#pragma weak shmemx_align_by_index = pshmemx_align_by_index
#define shmemx_align_by_index pshmemx_align_by_index
#endif /* ENABLE_PSHMEM */

void *
shmemx_malloc_by_index(shmemx_heap_index_t index, size_t s)
{
    void *addr;

    SHMEMU_CHECK_INIT();
    SHMEMU_CHECK_HEAP_INDEX(index);

    SHMEMT_MUTEX_PROTECT(addr = shmemxa_malloc_by_index(index, s));

    shmem_barrier_all();

    return addr;
}

void *
shmemx_calloc_by_index(shmemx_heap_index_t index, size_t n, size_t s)
{
    void *addr;

    SHMEMU_CHECK_INIT();
    SHMEMU_CHECK_HEAP_INDEX(index);

    SHMEMT_MUTEX_PROTECT(addr = shmemxa_calloc_by_index(index, n, s));

    shmem_barrier_all();

    return addr;
}

void
shmemx_free_by_index(shmemx_heap_index_t index, void *p)
{
    SHMEMU_CHECK_INIT();
    SHMEMU_CHECK_HEAP_INDEX(index);
    SHMEMU_CHECK_SYMMETRIC(p, 2);

    shmem_barrier_all();

    SHMEMT_MUTEX_PROTECT(shmemxa_free_by_index(index, p));
}

/*
 * realloc can cause memory to move around, so we protect it before
 * *and* after (spec 1.4, p. 25)
 */

void *
shmemx_realloc_by_index(shmemx_heap_index_t index, void *p, size_t s)
{
    void *addr;

    SHMEMU_CHECK_INIT();
    SHMEMU_CHECK_HEAP_INDEX(index);
    SHMEMU_CHECK_SYMMETRIC(p, 2);

    shmem_barrier_all();

    SHMEMT_MUTEX_PROTECT(addr = shmemxa_realloc_by_index(index, p, s));

    shmem_barrier_all();

    return addr;
}

void *
shmemx_align_by_index(shmemx_heap_index_t index, size_t a, size_t s)
{
    void *addr;

    SHMEMU_CHECK_INIT();
    SHMEMU_CHECK_HEAP_INDEX(index);

    SHMEMT_MUTEX_PROTECT(addr = shmemxa_align_by_index(index, a, s));

    shmem_barrier_all();

    return addr;
}

/*
 * use string as name to access
 */

#ifdef ENABLE_PSHMEM
#pragma weak shmemx_malloc_by_name = pshmemx_malloc_by_name
#define shmemx_malloc_by_name pshmemx_malloc_by_name
#pragma weak shmemx_calloc_by_name = pshmemx_calloc_by_name
#define shmemx_calloc_by_name pshmemx_calloc_by_name
#pragma weak shmemx_free_by_name = pshmemx_free_by_name
#define shmemx_free_by_name pshmemx_free_by_name
#pragma weak shmemx_realloc_by_name = pshmemx_realloc_by_name
#define shmemx_realloc_by_name pshmemx_realloc_by_name
#pragma weak shmemx_align_by_name = pshmemx_align_by_name
#define shmemx_align_by_name pshmemx_align_by_name
#endif /* ENABLE_PSHMEM */

void *
shmemx_malloc_by_name(const char *name, size_t s)
{
    const shmemx_heap_index_t index = shmemxa_name_to_index(name);
    void *addr;

    SHMEMU_CHECK_INIT();
    SHMEMU_CHECK_HEAP_INDEX(index);

    SHMEMT_MUTEX_PROTECT(addr = shmemxa_malloc_by_index(index, s));

    return addr;
}

void *
shmemx_calloc_by_name(const char *name, size_t n, size_t s)
{
    const shmemx_heap_index_t index = shmemxa_name_to_index(name);
    void *addr;

    SHMEMU_CHECK_INIT();
    SHMEMU_CHECK_HEAP_INDEX(index);

    SHMEMT_MUTEX_PROTECT(addr = shmemxa_calloc_by_index(index, n, s));

    return addr;
}

void
shmemx_free_by_name(const char *name, void *p)
{
    const shmemx_heap_index_t index = shmemxa_name_to_index(name);

    SHMEMU_CHECK_INIT();
    SHMEMU_CHECK_HEAP_INDEX(index);
    SHMEMU_CHECK_SYMMETRIC(p, 2);

    shmem_barrier_all();

    SHMEMT_MUTEX_PROTECT(shmemxa_free_by_index(index, p));
}

void *
shmemx_realloc_by_name(const char *name, void *p, size_t s)
{
    const shmemx_heap_index_t index = shmemxa_name_to_index(name);
    void *addr;

    SHMEMU_CHECK_INIT();
    SHMEMU_CHECK_HEAP_INDEX(index);
    SHMEMU_CHECK_SYMMETRIC(p, 2);

    shmem_barrier_all();

    SHMEMT_MUTEX_PROTECT(addr = shmemxa_realloc_by_index(index, p, s));

    shmem_barrier_all();

    return addr;
}

void *
shmemx_align_by_name(const char *name, size_t a, size_t s)
{
    const shmemx_heap_index_t index = shmemxa_name_to_index(name);
    void *addr;

    SHMEMU_CHECK_INIT();
    SHMEMU_CHECK_HEAP_INDEX(index);

    SHMEMT_MUTEX_PROTECT(addr = shmemxa_align_by_index(index, a, s));

    shmem_barrier_all();

    return addr;
}
