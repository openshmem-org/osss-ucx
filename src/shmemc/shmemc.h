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

#if 0
void shmemc_char_p(char *dest, char value, int pe);
void shmemc_short_p(short *dest, short value, int pe);
void shmemc_int_p(int *dest, int value, int pe);
void shmemc_long_p(long *dest, long value, int pe);
void shmemc_longlong_p(long long *dest, long long value, int pe);
void shmemc_longdouble_p(long double *dest, long double value, int pe);
void shmemc_float_p(float *dest, float value, int pe);
void shmemc_double_p(double *dest, double value, int pe);

char shmemc_char_g(char *dest, int pe);
short shmemc_short_g(short *dest, int pe);
int shmemc_int_g(int *dest, int pe);
long shmemc_long_g(long *dest, int pe);
long long shmemc_longlong_g(long long *dest, int pe);
long double shmemc_longdouble_g(long double *dest, int pe);
float shmemc_float_g(float *dest, int pe);
double shmemc_double_g(double *dest, int pe);

void shmemc_char_put(char *dest, const char *src, size_t nbytes, int pe);
void shmemc_short_put(short *dest, const short *src, size_t nbytes, int pe);
void shmemc_int_put(int *dest, const int *src, size_t nbytes, int pe);
void shmemc_long_put(long *dest, const long *src, size_t nbytes, int pe);
void shmemc_longlong_put(long long *dest, const long long *src,
                         size_t nbytes, int pe);
void shmemc_longdouble_put(long double *dest, const long double *src,
                           size_t nbytes, int pe);
void shmemc_float_put(float *dest, const float *src,
                      size_t nbytes, int pe);
void shmemc_double_put(double *dest, const double *src,
                       size_t nbytes, int pe);
void shmemc_complexf_put(COMPLEXIFY(float) *dest,
                         const COMPLEXIFY(float) *src,
                         size_t nbytes, int pe);
void shmemc_complexd_put(COMPLEXIFY(double) *dest,
                         const COMPLEXIFY(double) *src,
                         size_t nbytes, int pe);

void shmemc_char_get(char *dest, const char *src, size_t nbytes, int pe);
void shmemc_short_get(short *dest, const short *src, size_t nbytes, int pe);
void shmemc_int_get(int *dest, const int *src, size_t nbytes, int pe);
void shmemc_long_get(long *dest, const long *src, size_t nbytes, int pe);
void shmemc_longlong_get(long long *dest, const long long *src,
                         size_t nbytes, int pe);
void shmemc_longdouble_get(long double *dest, const long double *src,
                           size_t nbytes, int pe);
void shmemc_float_get(float *dest, const float *src, size_t nbytes, int pe);
void shmemc_double_get(double *dest, const double *src,
                       size_t nbytes, int pe);
void shmemc_complexf_get(COMPLEXIFY(float) *dest,
                         const COMPLEXIFY(float) *src,
                         size_t nbytes, int pe);
void shmemc_complexd_get(COMPLEXIFY(double) *dest,
                         const COMPLEXIFY(double) *src,
                         size_t nbytes, int pe);
#endif

void shmemc_put(void *dest, const void *src, size_t nbytes, int pe);
void shmemc_get(void *dest, const void *src, size_t nbytes, int pe);

void shmemc_put_nbi(void *dest, const void *src, size_t nbytes, int pe);
void shmemc_get_nbi(void *dest, const void *src, size_t nbytes, int pe);

#if 0
void shmemc_char_iput(char *dest, const char *src,
                      ptrdiff_t tst, ptrdiff_t sst,
                      size_t nbytes, int pe);
void shmemc_short_iput(short *dest, const short *src,
                       ptrdiff_t tst, ptrdiff_t sst,
                       size_t nbytes, int pe);
void shmemc_int_iput(int *dest, const int *src,
                     ptrdiff_t tst, ptrdiff_t sst,
                     size_t nbytes, int pe);
void shmemc_long_iput(long *dest, const long *src,
                      ptrdiff_t tst, ptrdiff_t sst,
                      size_t nbytes, int pe);
void shmemc_longlong_iput(long long *dest, const long long *src,
                          ptrdiff_t tst, ptrdiff_t sst,
                          size_t nbytes, int pe);
void shmemc_longdouble_iput(long double *dest, const long double *src,
                            ptrdiff_t tst, ptrdiff_t sst,
                            size_t nbytes, int pe);
void shmemc_float_iput(float *dest, const float *src,
                       ptrdiff_t tst, ptrdiff_t sst,
                       size_t nbytes, int pe);
void shmemc_double_iput(double *dest, const double *src,
                        ptrdiff_t tst, ptrdiff_t sst,
                        size_t nbytes, int pe);

void shmemc_char_iget(char *dest, const char *src,
                      ptrdiff_t tst, ptrdiff_t sst,
                      size_t nbytes, int pe);
void shmemc_short_iget(short *dest, const short *src,
                       ptrdiff_t tst, ptrdiff_t sst,
                       size_t nbytes, int pe);
void shmemc_int_iget(int *dest, const int *src,
                     ptrdiff_t tst, ptrdiff_t sst,
                     size_t nbytes, int pe);
void shmemc_long_iget(long *dest, const long *src,
                      ptrdiff_t tst, ptrdiff_t sst,
                      size_t nbytes, int pe);
void shmemc_longlong_iget(long long *dest, const long long *src,
                          ptrdiff_t tst, ptrdiff_t sst,
                          size_t nbytes, int pe);
void shmemc_longdouble_iget(long double *dest, const long double *src,
                            ptrdiff_t tst, ptrdiff_t sst,
                            size_t nbytes, int pe);
void shmemc_float_iget(float *dest, const float *src,
                       ptrdiff_t tst, ptrdiff_t sst,
                       size_t nbytes, int pe);
void shmemc_double_iget(double *dest, const double *src,
                        ptrdiff_t tst, ptrdiff_t sst,
                        size_t nbytes, int pe);
#endif

/**
 * these are per-type so we can handle per-type locks
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
