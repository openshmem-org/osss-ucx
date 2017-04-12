#ifndef _SHMEMC_COMMS_H
#define _SHMEMC_COMMS_H 1

#include <unistd.h>
#include <sys/types.h>
#include <stddef.h>

inline static void shmemc_quiet(void)
{
}
inline static void shmemc_fence(void)
{
    shmemc_quiet();
}

/*
 * STUBS SO THINGS WILL BUILD FOR NOW
 */

#include <stdlib.h>

inline static char *
shmemc_getenv(const char *name)
{
    return getenv(name);
}

/*
 * aligned (or not) puts & gets
 */

void shmemc_put(void *dest, const void *src,
                size_t nbytes, int pe);
void shmemc_get(void *dest, const void *src,
                size_t nbytes, int pe);

/*
 * aligned (or not) non-blocking puts & gets
 */

void shmemc_put_nbi(void *dest, const void *src,
                    size_t nbytes, int pe);
void shmemc_get_nbi(void *dest, const void *src,
                    size_t nbytes, int pe);

/*
 * aligned (or not) strided puts & gets
 */

void shmemc_iput(void *dest, const void *src, size_t nbytes,
                 ptrdiff_t tst, ptrdiff_t sst, int pe);
void shmemc_iget(void *dest, const void *src, size_t nbytes,
                 ptrdiff_t tst, ptrdiff_t sst, int pe);

/*
 * locks
 */

void shmemc_set_lock(volatile long *lock);
void shmemc_clear_lock(volatile long *lock);
int  shmemc_test_lock(volatile long *lock);

/*
 * barriers
 */

inline static void
shmemc_barrier_all(void)
{
    sleep(1);
}

inline static void
shmemc_barrier(int start, int log_stride, int size, long *pSync)
{
}

#endif /* ! _SHMEMC_COMMS_H */
