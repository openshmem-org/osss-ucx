/* For license: see LICENSE file at top-level */

/*
 * This is the Fortran interface.  Parameter-less routines are
 * the same as C, but with the standard underscore mangling we all love.
 *
 * Also provides pass-by-reference wrappers to translate C value params
 *
 */

/*
 * stop deprecation warnings
 */
#define _SHMEM_IN_SOURCE_TREE_ 123321
#include "shmem/api.h"
#undef _SHMEM_IN_SOURCE_TREE_

#include "fortran-common.h"

#include <stdint.h>
#include <stddef.h>             /* ptrdiff_t */
#include <string.h>

/**
 *
 * init & query functions
 */

void
FORTRANIFY(start_pes)(int *npes /* unused */ )
{
    shmem_init();
}

FORTRANIFY_VOID_VOID(shmem_init)
FORTRANIFY_VOID_VOID(shmem_finalize)

void
FORTRANIFY(shmem_global_exit)(int *status)
{
    shmem_global_exit(*status);
}

#define SHMEM_FORTRAN_QUERY_PE(_fname, _cname)  \
    int                                         \
    FORTRANIFY(_fname)(void)                    \
    {                                           \
        return _cname();                        \
    }

SHMEM_FORTRAN_QUERY_PE(my_pe, shmem_my_pe)
SHMEM_FORTRAN_QUERY_PE(num_pes, shmem_n_pes)
#if 0
/* invalid Fortran symbol names */
SHMEM_FORTRAN_QUERY_PE(_my_pe, shmem_my_pe)
SHMEM_FORTRAN_QUERY_PE(_num_pes, shmem_n_pes)
#endif

SHMEM_FORTRAN_QUERY_PE(shmem_my_pe, shmem_my_pe)
SHMEM_FORTRAN_QUERY_PE(shmem_n_pes, shmem_n_pes)


/*
 * puts and gets
 */

#define SHMEM_FORTRAN_PUT(_fname, _cname, _ctype)                       \
    void                                                                \
    FORTRANIFY(shmem_##_fname##_put)(_ctype *target, const _ctype *source, \
                                     int *size, int *pe)                \
    {                                                                   \
        shmem_##_cname##_put(target, source, *size, *pe);               \
    }

#define SHMEM_FORTRAN_PUT_SIZE(Size, _cname, _ctype)                    \
    void                                                                \
    FORTRANIFY(shmem_put##Size)(_ctype *target, const _ctype *source,   \
                                int *size, int *pe)                     \
    {                                                                   \
        shmem_##_cname##_put(target, source, *size, *pe);               \
    }

SHMEM_FORTRAN_PUT(character, char, char)
SHMEM_FORTRAN_PUT(double, double, double)
SHMEM_FORTRAN_PUT(integer, int, int)
SHMEM_FORTRAN_PUT(logical, int, int)
SHMEM_FORTRAN_PUT(real, float, float)
SHMEM_FORTRAN_PUT(complex, complexf, COMPLEXIFY(float))
SHMEM_FORTRAN_PUT_SIZE(4, int, int)
SHMEM_FORTRAN_PUT_SIZE(8, long, long)
SHMEM_FORTRAN_PUT_SIZE(32, int, int)
SHMEM_FORTRAN_PUT_SIZE(64, long, long)
SHMEM_FORTRAN_PUT_SIZE(128, longdouble, long double)

void
FORTRANIFY(shmem_putmem)(void *target, const void *source, int *size, int *pe)
{
    shmem_putmem(target, source, *size, *pe);
}

void
FORTRANIFY(shmem_put)(long *target, const long *source, int *size, int *pe)
{
    shmem_long_put(target, source, *size, *pe);
}

#define SHMEM_FORTRAN_GET(_fname, _cname, _ctype)                       \
    void                                                                \
    FORTRANIFY(shmem_##_fname##_get)(_ctype *target, const _ctype *source, \
                                     int *size, int *pe)                \
    {                                                                   \
        shmem_##_cname##_get(target, source, *size, *pe);               \
    }

#define SHMEM_FORTRAN_GET_SIZE(Size, _cname, _ctype)                    \
    void                                                                \
    FORTRANIFY(shmem_get##Size)(_ctype *target, const _ctype *source,   \
                                int *size, int *pe)                     \
    {                                                                   \
        shmem_##_cname##_get(target, source, *size, *pe);               \
    }

SHMEM_FORTRAN_GET(character, char, char)
SHMEM_FORTRAN_GET(double, double, double)
SHMEM_FORTRAN_GET(integer, int, int)
SHMEM_FORTRAN_GET(logical, int, int)
SHMEM_FORTRAN_GET(real, float, float)
SHMEM_FORTRAN_GET(complex, complexf, COMPLEXIFY(float))
SHMEM_FORTRAN_GET_SIZE(4, int, int)
SHMEM_FORTRAN_GET_SIZE(8, long, long)
SHMEM_FORTRAN_GET_SIZE(32, int, int)
SHMEM_FORTRAN_GET_SIZE(64, long, long)
SHMEM_FORTRAN_GET_SIZE(128, longdouble, long double)

void
FORTRANIFY(shmem_getmem)(void *target, const void *source, int *size, int *pe)
{
    shmem_getmem(target, source, *size, *pe);
}

void
FORTRANIFY(shmem_get)(long *target, const long *source, int *size, int *pe)
{
    shmem_long_get(target, source, *size, *pe);
}

/*
 * non-blocking implicit put/get
 */

#define SHMEM_FORTRAN_PUT_NBI(_fname, _cname, _ctype)           \
    void                                                        \
    FORTRANIFY(shmem_##_fname##_put_nbi)(_ctype *target,        \
                                         const _ctype *source,  \
                                         int *size, int *pe)    \
    {                                                           \
        shmem_##_cname##_put_nbi(target, source, *size, *pe);   \
    }

#define SHMEM_FORTRAN_PUT_SIZE_NBI(Size, _cname, _ctype)        \
    void                                                        \
    FORTRANIFY(shmem_put##Size##_nbi)(_ctype *target,           \
                                      const _ctype *source,     \
                                      int *size, int *pe)       \
    {                                                           \
        shmem_##_cname##_put_nbi(target, source, *size, *pe);   \
    }

SHMEM_FORTRAN_PUT_NBI(character, char, char)
SHMEM_FORTRAN_PUT_NBI(double, double, double)
SHMEM_FORTRAN_PUT_NBI(integer, int, int)
SHMEM_FORTRAN_PUT_NBI(logical, int, int)
SHMEM_FORTRAN_PUT_NBI(real, int, int)
SHMEM_FORTRAN_PUT_NBI(complex, complexf, COMPLEXIFY(float))
SHMEM_FORTRAN_PUT_SIZE_NBI(4, int, int)
SHMEM_FORTRAN_PUT_SIZE_NBI(8, long, long)
SHMEM_FORTRAN_PUT_SIZE_NBI(32, int, int)
SHMEM_FORTRAN_PUT_SIZE_NBI(64, long, long)
SHMEM_FORTRAN_PUT_SIZE_NBI(128, longlong, long long)


void
FORTRANIFY(shmem_putmem_nbi)(void *target, const void *source,
                             int *size, int *pe)
{
    shmem_putmem_nbi(target, source, *size, *pe);
}

#define SHMEM_FORTRAN_GET_NBI(_fname, _cname, _ctype)           \
    void                                                        \
    FORTRANIFY(shmem_##_fname##_get_nbi)(_ctype *target,        \
                                         const _ctype *source,  \
                                         int *size, int *pe)    \
    {                                                           \
        shmem_##_cname##_get_nbi(target, source, *size, *pe);   \
    }

#define SHMEM_FORTRAN_GET_SIZE_NBI(Size, _cname, _ctype)        \
    void                                                        \
    FORTRANIFY(shmem_get##Size##_nbi)(_ctype *target,           \
                                      const _ctype *source,     \
                                      int *size, int *pe)       \
    {                                                           \
        shmem_##_cname##_get_nbi(target, source, *size, *pe);   \
    }

SHMEM_FORTRAN_GET_NBI(character, char, char)
SHMEM_FORTRAN_GET_NBI(double, double, double)
SHMEM_FORTRAN_GET_NBI(integer, int, int)
SHMEM_FORTRAN_GET_NBI(logical, int, int)
SHMEM_FORTRAN_GET_NBI(real, int, int)
SHMEM_FORTRAN_GET_NBI(complex, complexf, COMPLEXIFY(float))
SHMEM_FORTRAN_GET_SIZE_NBI(4, int, int)
SHMEM_FORTRAN_GET_SIZE_NBI(8, long, long)
SHMEM_FORTRAN_GET_SIZE_NBI(32, int, int)
SHMEM_FORTRAN_GET_SIZE_NBI(64, long, long)
SHMEM_FORTRAN_GET_SIZE_NBI(128, longlong, long long)

void
FORTRANIFY(shmem_getmem_nbi)(void *target, const void *source,
                             int *size, int *pe)
{
    shmem_getmem_nbi(target, source, *size, *pe);
}

/*
 * strided puts
 */

#define SHMEM_FORTRAN_IPUT(Name, _cname, _ctype)                        \
    void                                                                \
    FORTRANIFY(shmem_##Name##_iput)(_ctype *target, const _ctype *source, \
                                    int *tst, int *sst,                 \
                                    int *size, int *pe)                 \
    {                                                                   \
        shmem_##_cname##_iput(target, source, *tst, *sst, *size, *pe);  \
    }

#include <stdio.h>

#define SHMEM_FORTRAN_IPUT_SIZE(Size, _cname, _ctype)                   \
    void                                                                \
    FORTRANIFY(shmem_iput##Size)(_ctype *target, const _ctype *source,  \
                                 int *tst, int *sst,                    \
                                 int *size, int *pe)                    \
    {                                                                   \
        shmem_##_cname##_iput(target, source, *tst, *sst, *size, *pe);  \
    }

SHMEM_FORTRAN_IPUT(character, char, char)
SHMEM_FORTRAN_IPUT(double, double, double)
SHMEM_FORTRAN_IPUT(integer, int, int)
SHMEM_FORTRAN_IPUT(logical, int, int)
SHMEM_FORTRAN_IPUT(real, int, int)
SHMEM_FORTRAN_IPUT_SIZE(4, int, int)
SHMEM_FORTRAN_IPUT_SIZE(8, long, long)
SHMEM_FORTRAN_IPUT_SIZE(32, int, int)
SHMEM_FORTRAN_IPUT_SIZE(64, long, long)
SHMEM_FORTRAN_IPUT_SIZE(128, longdouble, long double)

SHMEM_FORTRAN_IPUT(complex, complexf, COMPLEXIFY(float))

/*
 * strided gets
 */

#define SHMEM_FORTRAN_IGET(Name, _cname, _ctype)                        \
    void                                                                \
    FORTRANIFY(shmem_##Name##_iget)(_ctype *target, const _ctype *source, \
                                    int *tst, int *sst,                 \
                                    int *size, int *pe)                 \
    {                                                                   \
        shmem_##_cname##_iget(target, source, *tst, *sst, *size, *pe);  \
    }

#define SHMEM_FORTRAN_IGET_SIZE(Size, _cname, _ctype)                   \
    void                                                                \
    FORTRANIFY(shmem_iget##Size)(_ctype *target, const _ctype *source,  \
                                 int *tst, int *sst,                    \
                                 int *size, int *pe)                    \
    {                                                                   \
        shmem_##_cname##_iget(target, source, *tst, *sst, *size, *pe);  \
    }

SHMEM_FORTRAN_IGET(character, char, char)
SHMEM_FORTRAN_IGET(double, double, double)
SHMEM_FORTRAN_IGET(integer, int, int)
SHMEM_FORTRAN_IGET(logical, int, int)
SHMEM_FORTRAN_IGET(real, int, int)
SHMEM_FORTRAN_IGET_SIZE(4, int, int)
SHMEM_FORTRAN_IGET_SIZE(8, long, long)
SHMEM_FORTRAN_IGET_SIZE(32, int, int)
SHMEM_FORTRAN_IGET_SIZE(64, long, long)
SHMEM_FORTRAN_IGET_SIZE(128, longdouble, long double)

SHMEM_FORTRAN_IGET(complex, complexf, COMPLEXIFY(float))

/*
 * accessibility
 */

int
FORTRANIFY(shmem_addr_accessible)(void *addr, int *pe)
{
    return shmem_addr_accessible(addr, *pe);
}

int
FORTRANIFY(shmem_pe_accessible)(int *pe)
{
    return shmem_pe_accessible(*pe);
}

void *
FORTRANIFY(shmem_ptr)(void *target, int *pe)
{
    return shmem_ptr(target, *pe);
}

/*
 * barriers & fences
 */

void
FORTRANIFY(shmem_barrier)(int *PE_start, int *logPE_stride, int *PE_size,
                          int *pSync)
{
    shmem_barrier(*PE_start, *logPE_stride, *PE_size,(long *) pSync);
}

FORTRANIFY_VOID_VOID(shmem_barrier_all)
FORTRANIFY_VOID_VOID(shmem_fence)
FORTRANIFY_VOID_VOID(shmem_quiet)

/*
 * wait operations
 */

#define FORTRANIFY_WAIT_UNTIL(Name, Type)                       \
    void                                                        \
    FORTRANIFY(shmem_##Name##_wait_until)(Type *ivar, int *cmp, \
                                          Type *cmp_value)      \
    {                                                           \
        shmem_##Type##_wait_until(ivar, *cmp, *cmp_value);      \
    }

#define FORTRANIFY_WAIT(Name, Type)                                 \
    void                                                            \
    FORTRANIFY(shmem_##Name##_wait)(Type *ivar, Type *cmp_value)    \
    {                                                               \
        shmem_##Type##_wait(ivar, *cmp_value);                      \
    }

FORTRANIFY_WAIT_UNTIL(int4, int)
FORTRANIFY_WAIT_UNTIL(int8, long)
FORTRANIFY_WAIT(int4, int)
FORTRANIFY_WAIT(int8, long)

void
FORTRANIFY(shmem_wait_until)(int *ivar, int *cmp, int *cmp_value)
{
    shmem_int_wait_until(ivar, *cmp, *cmp_value);
}

void
FORTRANIFY(shmem_wait)(int *ivar, int *cmp_value)
{
    shmem_int_wait(ivar, *cmp_value);
}

/*
 * atomics
 *
 */

/*
 * incs
 */

void
FORTRANIFY(shmem_int4_inc)(int *target, int *pe)
{
    shmem_int_atomic_inc(target, *pe);
}

void
FORTRANIFY(shmem_int8_inc)(long *target, int *pe)
{
    shmem_long_atomic_inc(target, *pe);
}

/*
 * fincs
 */

int
FORTRANIFY(shmem_int4_finc)(int *target, int *pe)
{
    return shmem_int_atomic_fetch_inc(target, *pe);
}

long
FORTRANIFY(shmem_int8_finc)(long *target, int *pe)
{
    return shmem_long_atomic_fetch_inc(target, *pe);
}

/*
 * adds
 */

void
FORTRANIFY(shmem_int4_add)(int *target, int *value, int *pe)
{
    shmem_int_atomic_add(target, *value, *pe);
}

void
FORTRANIFY(shmem_int8_add)(long *target, long *value, int *pe)
{
    shmem_long_atomic_add(target, *value, *pe);
}

/*
 * fadds
 */

int
FORTRANIFY(shmem_int4_fadd)(int *target, int *value, int *pe)
{
    return shmem_int_atomic_fetch_add(target, *value, *pe);
}

long
FORTRANIFY(shmem_int8_fadd)(long *target, long *value, int *pe)
{
    return shmem_long_atomic_fetch_add(target, *value, *pe);
}


/*
 * swaps
 */

int
FORTRANIFY(shmem_int4_swap)(int *target, int *value, int *pe)
{
    return shmem_int_atomic_swap(target, *value, *pe);
}

long
FORTRANIFY(shmem_int8_swap)(long *target, long *value, int *pe)
{
    return shmem_long_atomic_swap(target, *value, *pe);
}

float
FORTRANIFY(shmem_real4_swap)(float *target, float *value, int *pe)
{
    return shmem_float_atomic_swap(target, *value, *pe);
}

double
FORTRANIFY(shmem_real8_swap)(double *target, double *value, int *pe)
{
    return shmem_double_atomic_swap(target, *value, *pe);
}

int
FORTRANIFY(shmem_swap)(int *target, int *value, int *pe)
{
    return shmem_int_atomic_swap(target, *value, *pe);
}

/*
 * cswaps
 */

int
FORTRANIFY(shmem_int4_cswap)(int *target, int *cond, int *value, int *pe)
{
    return shmem_int_atomic_compare_swap(target, *cond, *value, *pe);
}

long
FORTRANIFY(shmem_int8_cswap)(long *target, long *cond, long *value, int *pe)
{
    return shmem_long_atomic_compare_swap(target, *cond, *value, *pe);
}

/*
 * fetch/set
 */

int
FORTRANIFY(shmem_int4_fetch)(int *target, int *pe)
{
    return shmem_int_atomic_fetch(target, *pe);
}

long
FORTRANIFY(shmem_int8_fetch)(long *target, int *pe)
{
    return shmem_long_atomic_fetch(target, *pe);
}

float
FORTRANIFY(shmem_real4_fetch)(float *target, int *pe)
{
    return shmem_float_atomic_fetch(target, *pe);
}

double
FORTRANIFY(shmem_real8_fetch)(double *target, int *pe)
{
    return shmem_double_atomic_fetch(target, *pe);
}

void
FORTRANIFY(shmem_int4_set)(int *target, int *val, int *pe)
{
    shmem_int_atomic_set(target, *val, *pe);
}

void
FORTRANIFY(shmem_int8_set)(long *target, long *val, int *pe)
{
    shmem_long_atomic_set(target, *val, *pe);
}

void
FORTRANIFY(shmem_real4_set)(float *target, float *val, int *pe)
{
    shmem_float_atomic_set(target, *val, *pe);
}

void
FORTRANIFY(shmem_real8_set)(double *target, double *val, int *pe)
{
    shmem_double_atomic_set(target, *val, *pe);
}

/*
 * broadcasts
 */

void
FORTRANIFY(shmem_broadcast4)(void *target, const void *source, int *nelems,
                             int *PE_root, int *PE_start,
                             int *logPE_stride, int *PE_size, int *pSync)
{
    shmem_broadcast32(target, source,
                      *nelems, *PE_root, *PE_start, *logPE_stride, *PE_size,
                      (long *) pSync);
}

void
FORTRANIFY(shmem_broadcast8)(void *target, const void *source, int *nelems,
                             int *PE_root, int *PE_start,
                             int *logPE_stride, int *PE_size, int *pSync)
{
    shmem_broadcast64(target, source,
                      *nelems, *PE_root, *PE_start, *logPE_stride, *PE_size,
                      (long *) pSync);
}

void
FORTRANIFY(shmem_broadcast32)(void *target, const void *source,
                              int *nelems, int *PE_root, int *PE_start,
                              int *logPE_stride, int *PE_size, int *pSync)
{
    shmem_broadcast32(target, source,
                      *nelems, *PE_root, *PE_start, *logPE_stride, *PE_size,
                      (long *) pSync);
}

void
FORTRANIFY(shmem_broadcast64)(void *target, const void *source,
                              int *nelems, int *PE_root, int *PE_start,
                              int *logPE_stride, int *PE_size, int *pSync)
{
    shmem_broadcast64(target, source,
                      *nelems, *PE_root, *PE_start, *logPE_stride, *PE_size,
                      (long *) pSync);
}



/*
 * fixed collects
 */

void
FORTRANIFY(shmem_fcollect32)(void *target, const void *source, int *nelems,
                             int *PE_start, int *logPE_stride, int *PE_size,
                             int *pSync)
{
    shmem_fcollect32(target, source,
                     *nelems, *PE_start, *logPE_stride, *PE_size,
                     (long *) pSync);
}

void
FORTRANIFY(shmem_fcollect4)(void *target, const void *source, int *nelems,
                            int *PE_start, int *logPE_stride, int *PE_size,
                            int *pSync)
{
    shmem_fcollect32(target, source,
                     *nelems, *PE_start, *logPE_stride, *PE_size,
                     (long *) pSync);
}

void
FORTRANIFY(shmem_fcollect64)(void *target, const void *source, int *nelems,
                             int *PE_start, int *logPE_stride, int *PE_size,
                             int *pSync)
{
    shmem_fcollect64(target, source,
                     *nelems, *PE_start, *logPE_stride, *PE_size,
                     (long *) pSync);
}

void
FORTRANIFY(shmem_fcollect8)(void *target, const void *source, int *nelems,
                            int *PE_start, int *logPE_stride, int *PE_size,
                            int *pSync)
{
    shmem_fcollect64(target, source,
                     *nelems, *PE_start, *logPE_stride, *PE_size,
                     (long *) pSync);
}


/*
 * generalized collects
 */

void
FORTRANIFY(shmem_collect32)(void *target, const void *source, int *nelems,
                            int *PE_start, int *logPE_stride, int *PE_size,
                            int *pSync)
{
    shmem_collect32(target, source, *nelems,
                    *PE_start, *logPE_stride, *PE_size,(long *) pSync);
}

void
FORTRANIFY(shmem_collect64)(void *target, const void *source, int *nelems,
                            int *PE_start, int *logPE_stride, int *PE_size,
                            int *pSync)
{
    shmem_collect64(target, source, *nelems,
                    *PE_start, *logPE_stride, *PE_size,(long *) pSync);
}

void
FORTRANIFY(shmem_collect4)(void *target, const void *source, int *nelems,
                           int *PE_start, int *logPE_stride, int *PE_size,
                           int *pSync)
{
    shmem_collect32(target, source, *nelems,
                    *PE_start, *logPE_stride, *PE_size,(long *) pSync);
}

void
FORTRANIFY(shmem_collect8)(void *target, const void *source, int *nelems,
                           int *PE_start, int *logPE_stride, int *PE_size,
                           int *pSync)
{
    shmem_collect64(target, source, *nelems,
                    *PE_start, *logPE_stride, *PE_size,(long *) pSync);
}


/*
 * reductions
 *
 */

#define REDUCIFY(Op, Fname, Cname, Ctype)                       \
    void                                                        \
    FORTRANIFY(shmem_##Fname##_##Op##_to_all)                   \
        (Ctype *target, Ctype *source, int *nreduce,            \
         int *PE_start, int *logPE_stride, int *PE_size,        \
         Ctype *pWrk,                                           \
         int *pSync)                                            \
    {                                                           \
        shmem_##Cname##_##Op##_to_all(target, source,           \
                                      *nreduce, *PE_start,      \
                                      *logPE_stride, *PE_size,  \
                                      pWrk,                     \
                                      (long *) pSync);          \
    }

REDUCIFY(sum, int2, short, short)
REDUCIFY(sum, int4, int, int)
REDUCIFY(sum, int8, long, long)
REDUCIFY(sum, real4, float, float)
REDUCIFY(sum, real8, double, double)
REDUCIFY(sum, real16, longdouble, long double)
REDUCIFY(prod, int2, short, short)
REDUCIFY(prod, int4, int, int)
REDUCIFY(prod, int8, long, long)
REDUCIFY(prod, real4, float, float)
REDUCIFY(prod, real8, double, double)
REDUCIFY(prod, real16, longdouble, long double)
REDUCIFY(max, int2, short, short)
REDUCIFY(max, int4, int, int)
REDUCIFY(max, int8, long, long)
REDUCIFY(max, real4, float, float)
REDUCIFY(max, real8, double, double)
REDUCIFY(max, real16, longdouble, long double)
REDUCIFY(min, int2, short, short)
REDUCIFY(min, int4, int, int)
REDUCIFY(min, int8, long, long)
REDUCIFY(min, real4, float, float)
REDUCIFY(min, real8, double, double)
REDUCIFY(min, real16, longdouble, long double)
REDUCIFY(and, int2, short, short)
REDUCIFY(and, int4, int, int)
REDUCIFY(and, int8, long, long)
REDUCIFY(or, int2, short, short)
REDUCIFY(or, int4, int, int)
REDUCIFY(or, int8, long, long)
REDUCIFY(xor, int2, short, short)
REDUCIFY(xor, int4, int, int)
REDUCIFY(xor, int8, long, long)
REDUCIFY(sum, comp4, complexf, COMPLEXIFY(float))
REDUCIFY(sum, comp8, complexd, COMPLEXIFY(double))
REDUCIFY(prod, comp4, complexf, COMPLEXIFY(float))
REDUCIFY(prod, comp8, complexd, COMPLEXIFY(double))

/*
 * alltoall
 */

void
FORTRANIFY(shmem_alltoall32)(void *target, const void *source, int *nelems,
                             int *PE_start, int *logPE_stride, int *PE_size,
                             int *pSync)
{
    shmem_alltoall32(target, source, *nelems,
                     *PE_start, *logPE_stride, *PE_size,
                     (long *) pSync);
}

void
FORTRANIFY(shmem_alltoall64)(void *target, const void *source, int *nelems,
                             int *PE_start, int *logPE_stride, int *PE_size,
                             int *pSync)
{
    shmem_alltoall64(target, source, *nelems,
                     *PE_start, *logPE_stride, *PE_size,
                     (long *) pSync);
}

void
FORTRANIFY(shmem_alltoalls32)(void *target, const void *source,
                              ptrdiff_t *dst, ptrdiff_t *sst,
                              int *nelems,
                              int *PE_start, int *logPE_stride, int *PE_size,
                              int *pSync)
{
    shmem_alltoalls32(target, source,
                      *dst, *sst,
                      *nelems,
                      *PE_start, *logPE_stride, *PE_size,
                      (long *) pSync);
}

void
FORTRANIFY(shmem_alltoalls64)(void *target, const void *source,
                              ptrdiff_t *dst, ptrdiff_t *sst,
                              int *nelems,
                              int *PE_start, int *logPE_stride, int *PE_size,
                              int *pSync)
{
    shmem_alltoalls64(target, source,
                      *dst, *sst,
                      *nelems,
                      *PE_start, *logPE_stride, *PE_size,
                      (long *) pSync);
}

/*
 * locks
 *
 */

void
FORTRANIFY(shmem_clear_lock)(long *lock)
{
    shmem_clear_lock(lock);
}

void
FORTRANIFY(shmem_set_lock)(long *lock)
{
    shmem_set_lock(lock);
}

int
FORTRANIFY(shmem_test_lock)(long *lock)
{
    return shmem_test_lock(lock);
}

#if defined(ENABLE_PSHMEM)

/*
 * proposed profiling interface(note: no pshmem equiv)
 *
 */

extern void shmem_pcontrol(int level);

void
FORTRANIFY(shmem_pcontrol)(int *level)
{
    shmem_pcontrol(*level);
}

#endif /* ENABLE_PSHMEM */

void
FORTRANIFY(shmem_info_get_version)(int *major, int *minor)
{
    shmem_info_get_version(major, minor);
}

void
FORTRANIFY(shmem_info_get_name)(char *name)
{
    int len;
    const int max_f_len = SHMEM_MAX_NAME_LEN - 1;

    /* get the C string */
    shmem_info_get_name(name);

    /* pad with spaces for Fortran(TODO: memset better?) */
    len = strlen(name);
    while(len < max_f_len) {
        name[len] = ' ';
        len += 1;
    }
}

/**
 *
 * DEPRECATED API
 *
 * cache flushing
 */

FORTRANIFY_VOID_VOID(shmem_clear_cache_inv)
FORTRANIFY_VOID_VOID(shmem_set_cache_inv)
FORTRANIFY_VOID_VOID(shmem_udcflush)

FORTRANIFY_CACHE(shmem_set_cache_line_inv)
FORTRANIFY_CACHE(shmem_clear_cache_line_inv)
FORTRANIFY_CACHE(shmem_udcflush_line)
