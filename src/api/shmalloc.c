/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"

#include "shmem_mutex.h"
#include "allocator/memalloc.h"

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

/*
 * -- API --------------------------------------------------------------------
 */

#ifdef ENABLE_PSHMEM
#pragma weak shmem_malloc = pshmem_malloc
#define shmem_malloc pshmem_malloc
#pragma weak shmem_malloc_with_hints = pshmem_malloc_with_hints
#define shmem_malloc_with_hints pshmem_malloc_with_hints
#pragma weak shmem_calloc = pshmem_calloc
#define shmem_calloc pshmem_calloc
#pragma weak shmem_free = pshmem_free
#define shmem_free pshmem_free
#pragma weak shmem_realloc = pshmem_realloc
#define shmem_realloc pshmem_realloc
#pragma weak shmem_align = pshmem_align
#define shmem_align pshmem_align
#endif /* ENABLE_PSHMEM */

inline static void *
shmem_malloc_private(size_t s)
{
    void *addr;

    if (shmemu_unlikely(s == 0)) {
        return NULL;
    }

    SHMEMT_MUTEX_PROTECT(addr = shmema_malloc(s));

    shmemc_barrier_all();

    SHMEMU_CHECK_ALLOC(addr, s);

    return addr;
}

void *
shmem_malloc(size_t s)
{
    void *addr;

    addr = shmem_malloc_private(s);

    logger(LOG_MEMORY,
           "%s(size=%lu) -> %p",
           __func__,
           (unsigned long) s, addr
           );

    return addr;
}

void *
shmem_malloc_with_hints(size_t s, long hints)
{
    void *addr;

    NO_WARN_UNUSED(hints);

    addr = shmem_malloc_private(s);

    logger(LOG_MEMORY,
           "%s(size=%lu) -> %p",
           __func__,
           (unsigned long) s, addr
           );

    return addr;
}

void *
shmem_calloc(size_t n, size_t s)
{
    void *addr;

    if (shmemu_unlikely((n == 0) || (s == 0))) {
        return NULL;
    }

    SHMEMT_MUTEX_PROTECT(addr = shmema_calloc(n, s));

    shmemc_barrier_all();

    logger(LOG_MEMORY,
           "%s(count=%lu, size=%lu) -> %p",
           __func__,
           (unsigned long) n, (unsigned long) s, addr
           );

    SHMEMU_CHECK_ALLOC(addr, s);

    return addr;
}

void
shmem_free(void *p)
{
    shmemc_barrier_all();

    SHMEMT_MUTEX_PROTECT(shmema_free(p));

    logger(LOG_MEMORY, "%s(addr=%p)", __func__, p);
}

/*
 * realloc can cause memory to move around, so we protect it before
 * *and* after (spec 1.4, p. 25)
 */

void *
shmem_realloc(void *p, size_t s)
{
    void *addr;

    if (shmemu_unlikely(s == 0)) {
        return NULL;
    }

    shmemc_barrier_all();

    SHMEMT_MUTEX_PROTECT(addr = shmema_realloc(p, s));

    shmemc_barrier_all();

    logger(LOG_MEMORY,
           "%s(addr=%p, size=%lu) -> %p",
           __func__,
           p, (unsigned long) s, addr
           );

    SHMEMU_CHECK_ALLOC(addr, s);

    return addr;
}

void *
shmem_align(size_t a, size_t s)
{
    void *addr;

    if (shmemu_unlikely(s == 0)) {
        return NULL;
    }

    SHMEMT_MUTEX_PROTECT(addr = shmema_align(a, s));

    shmemc_barrier_all();

    logger(LOG_MEMORY,
           "%s(align=%lu, size=%lu) -> %p",
           __func__,
           (unsigned long) a, (unsigned long) s, addr
           );

    SHMEMU_CHECK_ALLOC(addr, s);

    return addr;
}
