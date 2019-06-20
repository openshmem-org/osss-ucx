/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"

#include "shmemu.h"
#include "shmemc.h"
#include "shmem/api.h"

#include <bits/wordsize.h>
#include <sys/types.h>
#include <stdint.h>
#include <stddef.h>

/*
 * ---------------------------------------------------------------------
 *
 * Deprecations as of 1.4.  3 different macros for different parameter
 * counts.  Provide variants for return types.  This needs to match
 * deprecation information in the header file.
 *
 */

#define DEPR_SINCE 1.4

#define SHMEM_DEPRECATE_VOID_AMO1(_op, _name, _type)        \
    void                                                    \
    shmem_##_name##_##_op(_type *target, int pe)            \
    {                                                       \
        deprecate(__func__, DEPR_SINCE);                    \
        shmemc_##_op(target, pe);                           \
    }

#define SHMEM_DEPRECATE_VOID_AMO2(_op, _name, _type)            \
    void                                                        \
    shmem_##_name##_##_op(_type *target, _type value, int pe)   \
    {                                                           \
        deprecate(__func__, DEPR_SINCE);                        \
        shmemc_##_op(target, &value, sizeof(value), pe);        \
    }

#define SHMEM_DEPRECATE_VOID_AMO3(_op, _name, _type)            \
    void                                                        \
    shmem_##_name##_##_op(_type *target,                        \
                          _type cond, _type value,              \
                          int pe)                               \
    {                                                           \
        deprecate(__func__, DEPR_SINCE);                        \
        shmemc_##_op(target, cond, &value, sizeof(value), pe);  \
    }

#define SHMEM_DEPRECATE_AMO1(_op, _name, _type)         \
    _type                                               \
    shmem_##_name##_##_op(_type *target, int pe)        \
    {                                                   \
        _type v;                                        \
                                                        \
        deprecate(__func__, DEPR_SINCE);                \
        shmemc_##_op(target, pe, &v);                   \
        return v;                                       \
    }

#define SHMEM_DEPRECATE_CONST_AMO1(_op, _name, _type)   \
    _type                                               \
    shmem_##_name##_##_op(const _type *target, int pe)  \
    {                                                   \
        _type v;                                        \
                                                        \
        deprecate(__func__, DEPR_SINCE);                \
        shmemc_##_op((_type *) target, pe, &v);         \
        return v;                                       \
    }

#define SHMEM_DEPRECATE_AMO2(_op, _name, _type)                     \
    _type                                                           \
    shmem_##_name##_##_op(_type *target, _type value, int pe)       \
    {                                                               \
        _type v;                                                    \
                                                                    \
        deprecate(__func__, DEPR_SINCE);                            \
        shmemc_##_op(target, &value, sizeof(value), pe, &v);        \
        return v;                                                   \
    }

#define SHMEM_DEPRECATE_AMO3(_op, _name, _type)                     \
    _type                                                           \
    shmem_##_name##_##_op(_type *target,                            \
                          _type cond, _type value,                  \
                          int pe)                                   \
    {                                                               \
        _type v;                                                    \
                                                                    \
        deprecate(__func__, DEPR_SINCE);                            \
        shmemc_##_op(target, &cond, &value, sizeof(value), pe, &v); \
        return v;                                                   \
    }

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_set = pshmem_int_set
#define shmem_int_set pshmem_int_set
#pragma weak shmem_long_set = pshmem_long_set
#define shmem_long_set pshmem_long_set
#pragma weak shmem_longlong_set = pshmem_longlong_set
#define shmem_longlong_set pshmem_longlong_set
#pragma weak shmem_float_set = pshmem_float_set
#define shmem_float_set pshmem_float_set
#pragma weak shmem_double_set = pshmem_double_set
#define shmem_double_set pshmem_double_set
#endif /* ENABLE_PSHMEM */

SHMEM_DEPRECATE_VOID_AMO2(set, int, int)
SHMEM_DEPRECATE_VOID_AMO2(set, long, long)
SHMEM_DEPRECATE_VOID_AMO2(set, longlong, long long)
SHMEM_DEPRECATE_VOID_AMO2(set, float, float)
SHMEM_DEPRECATE_VOID_AMO2(set, double, double)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_inc = pshmem_int_inc
#define shmem_int_inc pshmem_int_inc
#pragma weak shmem_long_inc = pshmem_long_inc
#define shmem_long_inc pshmem_long_inc
#pragma weak shmem_longlong_inc = pshmem_longlong_inc
#define shmem_longlong_inc pshmem_longlong_inc
#endif /* ENABLE_PSHMEM */

SHMEM_DEPRECATE_VOID_AMO1(inc, int, int)
SHMEM_DEPRECATE_VOID_AMO1(inc, long, long)
SHMEM_DEPRECATE_VOID_AMO1(inc, longlong, long long)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_add = pshmem_int_add
#define shmem_int_add pshmem_int_add
#pragma weak shmem_long_add = pshmem_long_add
#define shmem_long_add pshmem_long_add
#pragma weak shmem_longlong_add = pshmem_longlong_add
#define shmem_longlong_add pshmem_longlong_add
#endif /* ENABLE_PSHMEM */

SHMEM_DEPRECATE_VOID_AMO2(add, int, int)
SHMEM_DEPRECATE_VOID_AMO2(add, long, long)
SHMEM_DEPRECATE_VOID_AMO2(add, longlong, long long)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_fetch = pshmem_int_fetch
#define shmem_int_fetch pshmem_int_fetch
#pragma weak shmem_long_fetch = pshmem_long_fetch
#define shmem_long_fetch pshmem_long_fetch
#pragma weak shmem_longlong_fetch = pshmem_longlong_fetch
#define shmem_longlong_fetch pshmem_longlong_fetch
#pragma weak shmem_float_fetch = pshmem_float_fetch
#define shmem_float_fetch pshmem_float_fetch
#pragma weak shmem_double_fetch = pshmem_double_fetch
#define shmem_double_fetch pshmem_double_fetch
#endif /* ENABLE_PSHMEM */

SHMEM_DEPRECATE_CONST_AMO1(fetch, int, int)
SHMEM_DEPRECATE_CONST_AMO1(fetch, long, long)
SHMEM_DEPRECATE_CONST_AMO1(fetch, longlong, long long)
SHMEM_DEPRECATE_CONST_AMO1(fetch, float, float)
SHMEM_DEPRECATE_CONST_AMO1(fetch, double, double)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_finc = pshmem_int_finc
#define shmem_int_finc pshmem_int_finc
#pragma weak shmem_long_finc = pshmem_long_finc
#define shmem_long_finc pshmem_long_finc
#pragma weak shmem_longlong_finc = pshmem_longlong_finc
#define shmem_longlong_finc pshmem_longlong_finc
#endif /* ENABLE_PSHMEM */

SHMEM_DEPRECATE_AMO1(finc, int, int)
SHMEM_DEPRECATE_AMO1(finc, long, long)
SHMEM_DEPRECATE_AMO1(finc, longlong, long long)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_fadd = pshmem_int_fadd
#define shmem_int_fadd pshmem_int_fadd
#pragma weak shmem_long_fadd = pshmem_long_fadd
#define shmem_long_fadd pshmem_long_fadd
#pragma weak shmem_longlong_fadd = pshmem_longlong_fadd
#define shmem_longlong_fadd pshmem_longlong_fadd
#endif /* ENABLE_PSHMEM */

SHMEM_DEPRECATE_AMO2(fadd, int, int)
SHMEM_DEPRECATE_AMO2(fadd, long, long)
SHMEM_DEPRECATE_AMO2(fadd, longlong, long long)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_float_swap = pshmem_float_swap
#define shmem_float_swap pshmem_float_swap
#pragma weak shmem_double_swap = pshmem_double_swap
#define shmem_double_swap pshmem_double_swap
#pragma weak shmem_int_swap = pshmem_int_swap
#define shmem_int_swap pshmem_int_swap
#pragma weak shmem_long_swap = pshmem_long_swap
#define shmem_long_swap pshmem_long_swap
#pragma weak shmem_longlong_swap = pshmem_longlong_swap
#define shmem_longlong_swap pshmem_longlong_swap
#endif /* ENABLE_PSHMEM */

SHMEM_DEPRECATE_AMO2(swap, int, int)
SHMEM_DEPRECATE_AMO2(swap, long, long)
SHMEM_DEPRECATE_AMO2(swap, longlong, long long)
SHMEM_DEPRECATE_AMO2(swap, float, float)
SHMEM_DEPRECATE_AMO2(swap, double, double)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_cswap = pshmem_int_cswap
#define shmem_int_cswap pshmem_int_cswap
#pragma weak shmem_long_cswap = pshmem_long_cswap
#define shmem_long_cswap pshmem_long_cswap
#pragma weak shmem_longlong_cswap = pshmem_longlong_cswap
#define shmem_longlong_cswap pshmem_longlong_cswap
#endif /* ENABLE_PSHMEM */

SHMEM_DEPRECATE_AMO3(cswap, int, int)
SHMEM_DEPRECATE_AMO3(cswap, long, long)
SHMEM_DEPRECATE_AMO3(cswap, longlong, long long)
