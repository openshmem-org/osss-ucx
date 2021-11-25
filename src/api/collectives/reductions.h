/* For license: see LICENSE file at top-level */

#ifndef _REDUCTIONS_H
#define _REDUCTIONS_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <shcoll.h>

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

#define SHIM_REDUCE_DECLARE(_name, _type, _op, _algo)                   \
    void                                                                \
    shmem_##_name##_##_op##_to_all(_type *dest,                         \
                                   const _type *source,                 \
                                   int nreduce,                         \
                                   int PE_start,                        \
                                   int logPE_stride,                    \
                                   int PE_size,                         \
                                   _type *pWrk,                         \
                                   long *pSync)                         \
    {                                                                   \
        shcoll_##_name##_##_op##_to_all_##_algo(dest,                   \
                                                source,                 \
                                                nreduce,                \
                                                PE_start,               \
                                                logPE_stride,           \
                                                PE_size,                \
                                                pWrk,                   \
                                                pSync);                 \
    }

#define SHIM_REDUCE_BITWISE_TYPES(_op, _algo)                       \
    SHIM_REDUCE_DECLARE(short,      short,       _op, _algo)        \
        SHIM_REDUCE_DECLARE(int,        int,         _op, _algo)    \
        SHIM_REDUCE_DECLARE(long,       long,        _op, _algo)    \
        SHIM_REDUCE_DECLARE(longlong,   long long,   _op, _algo)

#define SHIM_REDUCE_MINMAX_TYPES(_op, _algo)                        \
    SHIM_REDUCE_BITWISE_TYPES(_op, _algo)                           \
        SHIM_REDUCE_DECLARE(double,     double,      _op, _algo)    \
        SHIM_REDUCE_DECLARE(float,      float,       _op, _algo)    \
        SHIM_REDUCE_DECLARE(longdouble, long double, _op, _algo)

#define SHIM_REDUCE_ARITH_TYPES(_op, _algo)                         \
    SHIM_REDUCE_MINMAX_TYPES(_op, _algo)                            \
        SHIM_REDUCE_DECLARE(complexd,  double _Complex, _op, _algo) \
        SHIM_REDUCE_DECLARE(complexf,   float _Complex, _op, _algo)

#define SHIM_REDUCE_BITWISE_ALL(_algo)          \
    SHIM_REDUCE_BITWISE_TYPES(or, _algo)        \
        SHIM_REDUCE_BITWISE_TYPES(xor, _algo)   \
        SHIM_REDUCE_BITWISE_TYPES(and, _algo)

#define SHIM_REDUCE_MINMAX_ALL(_algo)           \
    SHIM_REDUCE_MINMAX_TYPES(min, _algo)        \
        SHIM_REDUCE_MINMAX_TYPES(max, _algo)

#define SHIM_REDUCE_ARITH_ALL(_algo)            \
    SHIM_REDUCE_ARITH_TYPES(sum, _algo)         \
        SHIM_REDUCE_ARITH_TYPES(prod, _algo)

#define SHIM_REDUCE_ALL(_algo)                  \
    SHIM_REDUCE_BITWISE_ALL(_algo)              \
        SHIM_REDUCE_MINMAX_ALL(_algo)           \
        SHIM_REDUCE_ARITH_ALL(_algo)

#endif /* ! _REDUCTIONS_H */
