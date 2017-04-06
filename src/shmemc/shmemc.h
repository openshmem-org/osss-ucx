#ifndef _SHMEMC_COMMS_H
#define _SHMEMC_COMMS_H 1

#include <sys/types.h>

static inline void shmemc_quiet(void)
{
}
static inline void shmemc_fence(void)
{
    shmemc_quiet();
}

/*
 * STUBS SO THINGS WILL BUILD FOR NOW
 */

#include <stdlib.h>

static inline char *
shmemc_getenv(const char *name)
{
    return getenv(name);
}

/*
 * aligned (or not) puts & gets
 */

static inline void
shmemc_put(void *dest, const void *src, size_t nelems, int pe)
{
    return;
}

static inline void
shmemc_get(void *dest, const void *src, size_t nelems, int pe)
{
    return;
}

/*
 * locks
 */

static inline void
shmemc_set_lock(volatile long *lock)
{
}

static inline void
shmemc_clear_lock(volatile long *lock)
{
}

static inline int
shmemc_test_lock(volatile long *lock)
{
    return 0;
}

/*
 * barriers
 */

static inline void
shmemc_barrier_all(void)
{
}

static inline void
shmemc_barrier(int start, int log_stride, int size, long *pSync)
{
}

#endif /* ! _SHMEMC_COMMS_H */
