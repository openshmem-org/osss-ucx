#ifndef _SHMEMC_H
#define _SHMEMC_H 1

#include <sys/types.h>          /* size_t */
#include <stddef.h>             /* ptrdiff_t */
#include <stdint.h>

/**
 * API
 *
 **/

void shmemc_init(void);
void shmemc_finalize(void);

void shmemc_quiet(void);

inline static void shmemc_fence(void)
{
    shmemc_quiet();
}

char *shmemc_getenv(const char *);

void shmemc_put(void *dest, const void *src,
                size_t nbytes, int pe);
void shmemc_get(void *dest, const void *src,
                size_t nbytes, int pe);

void shmemc_add_int(int *t, int v, int pe);
void shmemc_add_long(long *t, long v, int pe);
void shmemc_add_longlong(long long *t, long long v, int pe);
void shmemc_inc_int(int *t, int pe);
void shmemc_inc_long(long *t, int pe);
void shmemc_inc_longlong(long long *t, int pe);
int shmemc_fadd_int(int *t, int v, int pe);
long shmemc_fadd_long(long *t, long v, int pe);
long long shmemc_fadd_longlong(long long *t, long long v, int pe);
int shmemc_finc_int(int *t, int pe);
long shmemc_finc_long(long *t, int pe);
long long shmemc_finc_longlong(long *t, int pe);
int shmemc_swap_int(int *t, int v, int pe);
long shmemc_swap_long(long *t, long v, int pe);
long long shmemc_swap_longlong(long long *t, long long v, int pe);
int shmemc_cswap_int(int *t, int c, int v, int pe);
long shmemc_cswap_long(long *t, long c, long v, int pe);
long long shmemc_cswap_longlong(long long *t, long long c, long long v, int pe);
int shmemc_fetch_int(int *t, int pe);
long shmemc_fetch_long(long *t, int pe);
long long shmemc_fetch_longlong(long long *t, int pe);
void shmemc_set_int(int *t, int v, int pe);
void shmemc_set_long(long *t, long v, int pe);
void shmemc_set_longlong(long long *t, long long v, int pe);

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

#include <unistd.h>             /* temp for sleep */

inline static void
shmemc_barrier_all(void)
{
    sleep(1);
}

inline static void
shmemc_barrier(int start, int log_stride, int size, long *pSync)
{
}

/*
 * zap the program
 */

inline static int
shmemc_global_exit(int status)
{
    return 0;
}

/*
 * accessibility
 */

inline static int
shmemc_pe_accessible(int pe)
{
    return 1;
}

inline static int
shmemc_addr_accessible(const void *addr, int pe)
{
    return 0;
}

#endif /* ! _SHMEMC_H */
