/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmem/defs.h"
#include "shmem/api.h"

#include <complex.h>

/**
 * common reduce code.  Build generalized reduction for various types.
 * Comparison operator passed as 1st parameter
 *
 */

#include <string.h>
#include <stdlib.h>

/*
 * Check the source and target areas to see if they touch each other.
 * Need to copy things if they do.
 */

#define INRANGE_CHECK(_a, _b, _n)                       \
    ( ( (_a) >= (_b) ) && ( (_a) < ( (_b) + (_n) ) ) )
#define OVERLAP_CHECK(_t, _s, _n)                               \
    ( INRANGE_CHECK(_t, _s, _n) || INRANGE_CHECK(_s, _t, _n) )

#define SHMEM_UDR_TYPE_OP(_name, _type)                                 \
    static void                                                         \
    udr_##_name##_to_all(_type (*the_op)(_type, _type),                 \
                                _type initval,                          \
                                _type *target, _type *source, int nreduce, \
                                int PE_start, int logPE_stride, int PE_size, \
                                _type *pWrk, long *pSync)               \
    {                                                                   \
        const int me = shmem_my_pe();                                   \
        const int step = 1 << logPE_stride;                             \
        const int nloops = nreduce / SHMEM_REDUCE_MIN_WRKDATA_SIZE;     \
        const int nrem = nreduce % SHMEM_REDUCE_MIN_WRKDATA_SIZE;       \
        const int snred = sizeof(_type) * nreduce;                      \
        const int overlap = OVERLAP_CHECK(target, source, snred);       \
        size_t nget;                                                    \
        int i, j;                                                       \
        int pe;                                                         \
        _type *tmptrg = NULL;                                           \
        _type *write_to;                                                \
                                                                        \
        if (overlap) {                                                  \
            /* use temp target in case source/target overlap/same */    \
            tmptrg = (_type *) malloc(snred);                           \
            if (tmptrg == (_type *) NULL) {                             \
                logger(LOG_FATAL,                                       \
                       "internal error: out of memory"                  \
                       " allocating temporary reduction buffer"         \
                       );                                               \
                return;                                                 \
                /* NOT REACHED */                                       \
            }                                                           \
            write_to = tmptrg;                                          \
            logger(LOG_REDUCTIONS,                                      \
                   "target (%p) and source (%p, size %ld)"              \
                   " overlap, using temporary target",                  \
                   target, source, snred                                \
                   );                                                   \
        }                                                               \
        else {                                                          \
            write_to = target;                                          \
            logger(LOG_REDUCTIONS,                                      \
                   "target (%p) and source (%p, size %ld)"              \
                   " do not overlap",                                   \
                   target, source, snred                                \
                   );                                                   \
        } /* end overlap check */                                       \
        /* everyone must initialize */                                  \
        for (j = 0; j < nreduce; j += 1) {                              \
            write_to[j] = source[j];                                    \
        }                                                               \
        shmem_barrier(PE_start, logPE_stride, PE_size, pSync);          \
        /* now go through other PEs and get source */                   \
        pe = PE_start;                                                  \
        for (i = 0; i < PE_size; i += 1) {                              \
            if (me != pe) {                                             \
                int k;                                                  \
                int ti = 0, si = 0; /* target & source index walk */    \
                /* pull in all the full chunks */                       \
                nget = SHMEM_REDUCE_MIN_WRKDATA_SIZE * sizeof(_type);   \
                for (k = 0; k < nloops; k += 1) {                       \
                    shmem_getmem(pWrk, & (source[si]), nget, pe);       \
                    for (j = 0; j < SHMEM_REDUCE_MIN_WRKDATA_SIZE;      \
                         j += 1) {                                      \
                        write_to[ti] =                                  \
                            (*the_op)(write_to[ti], pWrk[j]);           \
                        ti += 1;                                        \
                    }                                                   \
                    si += SHMEM_REDUCE_MIN_WRKDATA_SIZE;                \
                }                                                       \
                /* now get remaining part of source */                  \
                nget = nrem * sizeof(_type);                            \
                shmem_getmem(pWrk, & (source[si]), nget, pe);           \
                for (j = 0; j < nrem; j += 1) {                         \
                    write_to[ti] =                                      \
                        (*the_op)(write_to[ti], pWrk[j]);               \
                    ti += 1;                                            \
                }                                                       \
            }                                                           \
            pe += step;                                                 \
        }                                                               \
        /* everyone has to have finished */                             \
        shmem_barrier(PE_start, logPE_stride, PE_size, pSync);          \
        if (overlap) {                                                  \
            /* write to real local target and free temp */              \
            memcpy(target, tmptrg, snred);                              \
            free(tmptrg);                                               \
            tmptrg = NULL;                                              \
        }                                                               \
    }

SHMEM_UDR_TYPE_OP(short, short)
SHMEM_UDR_TYPE_OP(int, int)
SHMEM_UDR_TYPE_OP(long, long)
SHMEM_UDR_TYPE_OP(longlong, long long)
SHMEM_UDR_TYPE_OP(float, float)
SHMEM_UDR_TYPE_OP(double, double)
SHMEM_UDR_TYPE_OP(longdouble, long double)
SHMEM_UDR_TYPE_OP(complexf, float complex)
SHMEM_UDR_TYPE_OP(complexd, double complex)


/**
 * Pass in type/operation, macro builds the reduction function as
 * defined above
 *
 */

#define SHMEM_REDUCE_TYPE_OP(_opcall, _initval, _name, _type)           \
    void                                                                \
    shmem_##_name##_##_opcall##_to_all(_type *target, _type *source,    \
                                       int nreduce,                     \
                                       int PE_start, int logPE_stride,  \
                                       int PE_size,                     \
                                       _type *pWrk, long *pSync)        \
    {                                                                   \
        udr_##_name##_to_all(shmemu_##_opcall##_##_name##_func,         \
                             _initval,                                  \
                             target, source,                            \
                             nreduce,                                   \
                             PE_start, logPE_stride,                    \
                             PE_size,                                   \
                             pWrk, pSync);                              \
    }

#ifdef ENABLE_PSHMEM
#pragma weak shmem_complexd_sum_to_all = pshmem_complexd_sum_to_all
#define shmem_complexd_sum_to_all pshmem_complexd_sum_to_all
#pragma weak shmem_complexf_sum_to_all = pshmem_complexf_sum_to_all
#define shmem_complexf_sum_to_all pshmem_complexf_sum_to_all
#pragma weak shmem_double_sum_to_all = pshmem_double_sum_to_all
#define shmem_double_sum_to_all pshmem_double_sum_to_all
#pragma weak shmem_float_sum_to_all = pshmem_float_sum_to_all
#define shmem_float_sum_to_all pshmem_float_sum_to_all
#pragma weak shmem_int_sum_to_all = pshmem_int_sum_to_all
#define shmem_int_sum_to_all pshmem_int_sum_to_all
#pragma weak shmem_long_sum_to_all = pshmem_long_sum_to_all
#define shmem_long_sum_to_all pshmem_long_sum_to_all
#pragma weak shmem_longdouble_sum_to_all = pshmem_longdouble_sum_to_all
#define shmem_longdouble_sum_to_all pshmem_longdouble_sum_to_all
#pragma weak shmem_longlong_sum_to_all = pshmem_longlong_sum_to_all
#define shmem_longlong_sum_to_all pshmem_longlong_sum_to_all
#pragma weak shmem_short_sum_to_all = pshmem_short_sum_to_all
#define shmem_short_sum_to_all pshmem_short_sum_to_all
#pragma weak shmem_complexd_prod_to_all = pshmem_complexd_prod_to_all
#define shmem_complexd_prod_to_all pshmem_complexd_prod_to_all
#pragma weak shmem_complexf_prod_to_all = pshmem_complexf_prod_to_all
#define shmem_complexf_prod_to_all pshmem_complexf_prod_to_all
#pragma weak shmem_double_prod_to_all = pshmem_double_prod_to_all
#define shmem_double_prod_to_all pshmem_double_prod_to_all
#pragma weak shmem_float_prod_to_all = pshmem_float_prod_to_all
#define shmem_float_prod_to_all pshmem_float_prod_to_all
#pragma weak shmem_int_prod_to_all = pshmem_int_prod_to_all
#define shmem_int_prod_to_all pshmem_int_prod_to_all
#pragma weak shmem_long_prod_to_all = pshmem_long_prod_to_all
#define shmem_long_prod_to_all pshmem_long_prod_to_all
#pragma weak shmem_longdouble_prod_to_all = pshmem_longdouble_prod_to_all
#define shmem_longdouble_prod_to_all pshmem_longdouble_prod_to_all
#pragma weak shmem_longlong_prod_to_all = pshmem_longlong_prod_to_all
#define shmem_longlong_prod_to_all pshmem_longlong_prod_to_all
#pragma weak shmem_short_prod_to_all = pshmem_short_prod_to_all
#define shmem_short_prod_to_all pshmem_short_prod_to_all
#pragma weak shmem_int_and_to_all = pshmem_int_and_to_all
#define shmem_int_and_to_all pshmem_int_and_to_all
#pragma weak shmem_long_and_to_all = pshmem_long_and_to_all
#define shmem_long_and_to_all pshmem_long_and_to_all
#pragma weak shmem_longlong_and_to_all = pshmem_longlong_and_to_all
#define shmem_longlong_and_to_all pshmem_longlong_and_to_all
#pragma weak shmem_short_and_to_all = pshmem_short_and_to_all
#define shmem_short_and_to_all pshmem_short_and_to_all
#pragma weak shmem_int_or_to_all = pshmem_int_or_to_all
#define shmem_int_or_to_all pshmem_int_or_to_all
#pragma weak shmem_long_or_to_all = pshmem_long_or_to_all
#define shmem_long_or_to_all pshmem_long_or_to_all
#pragma weak shmem_longlong_or_to_all = pshmem_longlong_or_to_all
#define shmem_longlong_or_to_all pshmem_longlong_or_to_all
#pragma weak shmem_short_or_to_all = pshmem_short_or_to_all
#define shmem_short_or_to_all pshmem_short_or_to_all
#pragma weak shmem_int_xor_to_all = pshmem_int_xor_to_all
#define shmem_int_xor_to_all pshmem_int_xor_to_all
#pragma weak shmem_long_xor_to_all = pshmem_long_xor_to_all
#define shmem_long_xor_to_all pshmem_long_xor_to_all
#pragma weak shmem_longlong_xor_to_all = pshmem_longlong_xor_to_all
#define shmem_longlong_xor_to_all pshmem_longlong_xor_to_all
#pragma weak shmem_short_xor_to_all = pshmem_short_xor_to_all
#define shmem_short_xor_to_all pshmem_short_xor_to_all
#pragma weak shmem_int_max_to_all = pshmem_int_max_to_all
#define shmem_int_max_to_all pshmem_int_max_to_all
#pragma weak shmem_long_max_to_all = pshmem_long_max_to_all
#define shmem_long_max_to_all pshmem_long_max_to_all
#pragma weak shmem_longlong_max_to_all = pshmem_longlong_max_to_all
#define shmem_longlong_max_to_all pshmem_longlong_max_to_all
#pragma weak shmem_short_max_to_all = pshmem_short_max_to_all
#define shmem_short_max_to_all pshmem_short_max_to_all
#pragma weak shmem_longdouble_max_to_all = pshmem_longdouble_max_to_all
#define shmem_longdouble_max_to_all pshmem_longdouble_max_to_all
#pragma weak shmem_float_max_to_all = pshmem_float_max_to_all
#define shmem_float_max_to_all pshmem_float_max_to_all
#pragma weak shmem_double_max_to_all = pshmem_double_max_to_all
#define shmem_double_max_to_all pshmem_double_max_to_all
#pragma weak shmem_int_min_to_all = pshmem_int_min_to_all
#define shmem_int_min_to_all pshmem_int_min_to_all
#pragma weak shmem_long_min_to_all = pshmem_long_min_to_all
#define shmem_long_min_to_all pshmem_long_min_to_all
#pragma weak shmem_longlong_min_to_all = pshmem_longlong_min_to_all
#define shmem_longlong_min_to_all pshmem_longlong_min_to_all
#pragma weak shmem_short_min_to_all = pshmem_short_min_to_all
#define shmem_short_min_to_all pshmem_short_min_to_all
#pragma weak shmem_longdouble_min_to_all = pshmem_longdouble_min_to_all
#define shmem_longdouble_min_to_all pshmem_longdouble_min_to_all
#pragma weak shmem_float_min_to_all = pshmem_float_min_to_all
#define shmem_float_min_to_all pshmem_float_min_to_all
#pragma weak shmem_double_min_to_all = pshmem_double_min_to_all
#define shmem_double_min_to_all pshmem_double_min_to_all
#endif /* ENABLE_PSHMEM */


SHMEM_REDUCE_TYPE_OP(sum, 0, short, short)
SHMEM_REDUCE_TYPE_OP(sum, 0, int, int)
SHMEM_REDUCE_TYPE_OP(sum, 0, long, long)
SHMEM_REDUCE_TYPE_OP(sum, 0, longlong, long long)
SHMEM_REDUCE_TYPE_OP(sum, 0.0, float, float)
SHMEM_REDUCE_TYPE_OP(sum, 0.0, double, double)
SHMEM_REDUCE_TYPE_OP(sum, 0.0, longdouble, long double)
SHMEM_REDUCE_TYPE_OP(sum, 0.0, complexf, float complex)
SHMEM_REDUCE_TYPE_OP(sum, 0.0, complexd, double complex)

SHMEM_REDUCE_TYPE_OP(prod, 1, short, short)
SHMEM_REDUCE_TYPE_OP(prod, 1, int, int)
SHMEM_REDUCE_TYPE_OP(prod, 1, long, long)
SHMEM_REDUCE_TYPE_OP(prod, 1, longlong, long long)
SHMEM_REDUCE_TYPE_OP(prod, 1.0, float, float)
SHMEM_REDUCE_TYPE_OP(prod, 1.0, double, double)
SHMEM_REDUCE_TYPE_OP(prod, 1.0, longdouble, long double)
SHMEM_REDUCE_TYPE_OP(prod, 1.0, complexf, float complex)
SHMEM_REDUCE_TYPE_OP(prod, 1.0, complexd, double complex)

SHMEM_REDUCE_TYPE_OP(and, 1, short, short)
SHMEM_REDUCE_TYPE_OP(and, 1, int, int)
SHMEM_REDUCE_TYPE_OP(and, 1, long, long)
SHMEM_REDUCE_TYPE_OP(and, 1, longlong, long long)

SHMEM_REDUCE_TYPE_OP(or, 0, short, short)
SHMEM_REDUCE_TYPE_OP(or, 0, int, int)
SHMEM_REDUCE_TYPE_OP(or, 0, long, long)
SHMEM_REDUCE_TYPE_OP(or, 0, longlong, long long)

SHMEM_REDUCE_TYPE_OP(xor, 0, short, short)
SHMEM_REDUCE_TYPE_OP(xor, 0, int, int)
SHMEM_REDUCE_TYPE_OP(xor, 0, long, long)
SHMEM_REDUCE_TYPE_OP(xor, 0, longlong, long long)

SHMEM_REDUCE_TYPE_OP(max, 0, short, short)
SHMEM_REDUCE_TYPE_OP(max, 0, int, int)
SHMEM_REDUCE_TYPE_OP(max, 0, long, long)
SHMEM_REDUCE_TYPE_OP(max, 0, longlong, long long)
SHMEM_REDUCE_TYPE_OP(max, 0.0, float, float)
SHMEM_REDUCE_TYPE_OP(max, 0.0, double, double)
SHMEM_REDUCE_TYPE_OP(max, 0.0, longdouble, long double)

SHMEM_REDUCE_TYPE_OP(min, 0, short, short)
SHMEM_REDUCE_TYPE_OP(min, 0, int, int)
SHMEM_REDUCE_TYPE_OP(min, 0, long, long)
SHMEM_REDUCE_TYPE_OP(min, 0, longlong, long long)
SHMEM_REDUCE_TYPE_OP(min, 0.0, float, float)
SHMEM_REDUCE_TYPE_OP(min, 0.0, double, double)
SHMEM_REDUCE_TYPE_OP(min, 0.0, longdouble, long double)
