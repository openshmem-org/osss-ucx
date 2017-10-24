#ifndef _SHMEMC_H
#define _SHMEMC_H 1

#include <sys/types.h>          /* size_t */
#include <stddef.h>             /* ptrdiff_t */
#include <stdint.h>

#include "shmem/defs.h"

/**
 * API
 *
 **/

void shmemc_init(void);
void shmemc_finalize(void);

char *shmemc_getenv(const char *name);

int shmemc_my_pe(void);
int shmemc_n_pes(void);

void shmemc_quiet(void);
void shmemc_fence(void);

void shmemc_put(void *dest, const void *src, size_t nbytes, int pe);
void shmemc_get(void *dest, const void *src, size_t nbytes, int pe);

void shmemc_put_nbi(void *dest, const void *src, size_t nbytes, int pe);
void shmemc_get_nbi(void *dest, const void *src, size_t nbytes, int pe);

/**
 * these are per-type so we can handle per-type locks
 *
 * TODO: parameterize with macros for new types
 */

void      shmemc_int_add(int *t, int v, int pe);
void      shmemc_long_add(long *t, long v, int pe);
void      shmemc_longlong_add(long long *t, long long v, int pe);
void      shmemc_int_inc(int *t, int pe);
void      shmemc_long_inc(long *t, int pe);
void      shmemc_longlong_inc(long long *t, int pe);
int       shmemc_int_finc(int *t, int pe);
long      shmemc_long_finc(long *t, int pe);
long long shmemc_longlong_finc(long long *t, int pe);
int       shmemc_int_fadd(int *t, int v, int pe);
long      shmemc_long_fadd(long *t, long v, int pe);
long long shmemc_longlong_fadd(long long *t, long long v, int pe);
int       shmemc_int_swap(int *t, int v, int pe);
long      shmemc_long_swap(long *t, long v, int pe);
long long shmemc_longlong_swap(long long *t, long long v, int pe);
float     shmemc_float_swap(float *t, float v, int pe);
double    shmemc_double_swap(double *t, double v, int pe);
int       shmemc_int_cswap(int *t, int c, int v, int pe);
long      shmemc_long_cswap(long *t, long c, long v, int pe);
long long shmemc_longlong_cswap(long long *t, long long c, long long v, int pe);
int       shmemc_int_fetch(int *t, int pe);
long      shmemc_long_fetch(long *t, int pe);
long long shmemc_longlong_fetch(long long *t, int pe);
float     shmemc_float_fetch(float *t, int pe);
double    shmemc_double_fetch(double *t, int pe);
int       shmemc_int_set(int *t, int v, int pe);
long      shmemc_long_set(long *t, long v, int pe);
long long shmemc_longlong_set(long long *t, long long v, int pe);
float     shmemc_float_set(float *t, float v, int pe);
double    shmemc_double_set(double *t, double v, int pe);

void shmemc_set_lock(long *lock);
void shmemc_clear_lock(long *lock);
int  shmemc_test_lock(long *lock);

/*
 * routine per-type and test to avoid branching
 *
 * NEEDS PUSH-DOWN INTO COMMS IMPLEMENTATION FOR ACTUAL WAIT
 */
#define SHMEMC_WAITUNTIL_TYPE(_name, _type, _opname)                    \
    void shmemc_##_name##_wait_##_opname##_until(_type *var,            \
                                                 _type cmp_value);

SHMEMC_WAITUNTIL_TYPE(short, short, eq)
SHMEMC_WAITUNTIL_TYPE(int, int, eq)
SHMEMC_WAITUNTIL_TYPE(long, long, eq)
SHMEMC_WAITUNTIL_TYPE(longlong, long long, eq)

SHMEMC_WAITUNTIL_TYPE(short, short, ne)
SHMEMC_WAITUNTIL_TYPE(int, int, ne)
SHMEMC_WAITUNTIL_TYPE(long, long, ne)
SHMEMC_WAITUNTIL_TYPE(longlong, long long, ne)

SHMEMC_WAITUNTIL_TYPE(short, short, gt)
SHMEMC_WAITUNTIL_TYPE(int, int, gt)
SHMEMC_WAITUNTIL_TYPE(long, long, gt)
SHMEMC_WAITUNTIL_TYPE(longlong, long long, gt)

SHMEMC_WAITUNTIL_TYPE(short, short, le)
SHMEMC_WAITUNTIL_TYPE(int, int, le)
SHMEMC_WAITUNTIL_TYPE(long, long, le)
SHMEMC_WAITUNTIL_TYPE(longlong, long long, le)

SHMEMC_WAITUNTIL_TYPE(short, short, lt)
SHMEMC_WAITUNTIL_TYPE(int, int, lt)
SHMEMC_WAITUNTIL_TYPE(long, long, lt)
SHMEMC_WAITUNTIL_TYPE(longlong, long long, lt)

SHMEMC_WAITUNTIL_TYPE(short, short, ge)
SHMEMC_WAITUNTIL_TYPE(int, int, ge)
SHMEMC_WAITUNTIL_TYPE(long, long, ge)
SHMEMC_WAITUNTIL_TYPE(longlong, long long, ge)

/*
 * barriers
 */
void shmemc_barrier_all(void);
void shmemc_barrier(int start, int log_stride, int size, long *pSync);

/*
 * broadcasts
 */
void shmemc_broadcast32(void *target, const void *source,
                        size_t nelems,
                        int PE_root, int PE_start,
                        int logPE_stride, int PE_size,
                        long *pSync);
void shmemc_broadcast64(void *target, const void *source,
                        size_t nelems,
                        int PE_root, int PE_start,
                        int logPE_stride, int PE_size,
                        long *pSync);

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

/*
 * memory allocation
 */
void shmemc_mem_init(void *base, size_t capacity);
void shmemc_mem_finalize(void);
void *shmemc_mem_base(void);
void *shmemc_mem_alloc(size_t size);
void shmemc_mem_free(void *addr);
void *shmemc_mem_realloc(void *addr, size_t new_size);
void *shmemc_mem_align(size_t alignment, size_t size);

#endif /* ! _SHMEMC_H */
