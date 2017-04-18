#ifndef _SHMEMC_H
#define _SHMEMC_H 1

#include <sys/types.h>

inline static void shmemc_quiet(void)
{
}
inline static void shmemc_fence(void)
{
    shmemc_quiet();
}

/*
 * query job environment (some can't use system getenv() directly
 */

char *shmemc_getenv(const char *name);

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

#include <stddef.h>

void shmemc_iput(void *dest, const void *src, size_t nbytes,
                 ptrdiff_t tst, ptrdiff_t sst, int pe);
void shmemc_iget(void *dest, const void *src, size_t nbytes,
                 ptrdiff_t tst, ptrdiff_t sst, int pe);

/*
 * atomics and locks
 */

void shmemc_set_lock(volatile long *lock);
void shmemc_clear_lock(volatile long *lock);
int  shmemc_test_lock(volatile long *lock);

/*
 * to be zapped
 */
#if 0
#define VOLATILIZE(_type, _var) (* ( volatile _type *) (_var))
#else
#define VOLATILIZE(_type, _var) (_var)
#endif

/*
 * routine per-type and test to avoid branching
 *
 * NEEDS PUSH-DOWN INTO COMMS IMPLEMENTATION FOR ACTUAL WAIT
 */
#define SHMEMC_WAITUNTIL_TYPE(_name, _type, _opname, _op)               \
    inline static void                                                  \
    shmemc_##_name##_wait_##_opname##_until(volatile _type *var,        \
                                            int cmp,                    \
                                            _type cmp_value)            \
    {                                                                   \
        do { } while ( *var _op cmp_value );                            \
    }

SHMEMC_WAITUNTIL_TYPE(short, short, eq, ==)
SHMEMC_WAITUNTIL_TYPE(int, int, eq, ==)
SHMEMC_WAITUNTIL_TYPE(long, long, eq, ==)
SHMEMC_WAITUNTIL_TYPE(longlong, long long, eq, ==)

SHMEMC_WAITUNTIL_TYPE(short, short, ne, !=)
SHMEMC_WAITUNTIL_TYPE(int, int, ne, !=)
SHMEMC_WAITUNTIL_TYPE(long, long, ne, !=)
SHMEMC_WAITUNTIL_TYPE(longlong, long long, ne, !=)

SHMEMC_WAITUNTIL_TYPE(short, short, gt, >)
SHMEMC_WAITUNTIL_TYPE(int, int, gt, >)
SHMEMC_WAITUNTIL_TYPE(long, long, gt, >)
SHMEMC_WAITUNTIL_TYPE(longlong, long long, gt, >)

SHMEMC_WAITUNTIL_TYPE(short, short, le, <=)
SHMEMC_WAITUNTIL_TYPE(int, int, le, <=)
SHMEMC_WAITUNTIL_TYPE(long, long, le, <=)
SHMEMC_WAITUNTIL_TYPE(longlong, long long, le, <=)

SHMEMC_WAITUNTIL_TYPE(short, short, lt, <)
SHMEMC_WAITUNTIL_TYPE(int, int, lt, <)
SHMEMC_WAITUNTIL_TYPE(long, long, lt, <)
SHMEMC_WAITUNTIL_TYPE(longlong, long long, lt, <)

SHMEMC_WAITUNTIL_TYPE(short, short, ge, >=)
SHMEMC_WAITUNTIL_TYPE(int, int, ge, >=)
SHMEMC_WAITUNTIL_TYPE(long, long, ge, >=)
SHMEMC_WAITUNTIL_TYPE(longlong, long long, ge, >=)

/*
 * barriers
 */

#include <unistd.h>

inline static void
shmemc_barrier_all(void)
{
    sleep(1);
}

inline static void
shmemc_barrier(int start, int log_stride, int size, long *pSync)
{
}

#endif /* ! _SHMEMC__H */
