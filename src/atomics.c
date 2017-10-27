/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"

#include <bits/wordsize.h>

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_atomic_swap = pshmem_int_atomic_swap
#define shmem_int_atomic_swap pshmem_int_atomic_swap
#pragma weak shmem_long_atomic_swap = pshmem_long_atomic_swap
#define shmem_long_atomic_swap pshmem_long_atomic_swap
#pragma weak shmem_longlong_atomic_swap = pshmem_longlong_atomic_swap
#define shmem_longlong_atomic_swap pshmem_longlong_atomic_swap
#pragma weak shmem_float_atomic_swap = pshmem_float_atomic_swap
#define shmem_float_atomic_swap pshmem_float_atomic_swap
#pragma weak shmem_double_atomic_swap = pshmem_double_atomic_swap
#define shmem_double_atomic_swap pshmem_double_atomic_swap
#endif /* ENABLE_PSHMEM */

/*
 * swap
 */

#define SHMEM_TYPE_SWAP(_name, _type, _size)                            \
    _type                                                               \
    shmem_##_name##_atomic_swap(_type *target, _type value, int pe)     \
    {                                                                   \
        return shmemc_##_name##_swap(target, value, pe);                \
    }

SHMEM_TYPE_SWAP(int, int, 32)
#if __WORDSIZE == 64
SHMEM_TYPE_SWAP(long, long, 64)
#else
SHMEM_TYPE_SWAP(long, long, 32)
#endif
SHMEM_TYPE_SWAP(longlong, long long, 64)
SHMEM_TYPE_SWAP(float, float, 32)
SHMEM_TYPE_SWAP(double, double, 32)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_atomic_compare_swap = pshmem_int_atomic_compare_swap
#define shmem_int_atomic_compare_swap pshmem_int_atomic_compare_swap
#pragma weak shmem_long_atomic_compare_swap = pshmem_long_atomic_compare_swap
#define shmem_long_atomic_compare_swap pshmem_long_atomic_compare_swap
#pragma weak shmem_longlong_atomic_compare_swap = pshmem_longlong_atomic_compare_swap
#define shmem_longlong_atomic_compare_swap pshmem_longlong_atomic_compare_swap
#endif /* ENABLE_PSHMEM */

/*
 * conditional swap
 */

#define SHMEM_TYPE_CSWAP(_name, _type, _size)                           \
    _type                                                               \
    shmem_##_name##_atomic_compare_swap(_type *target,                  \
                                        _type cond, _type value,        \
                                        int pe)                         \
    {                                                                   \
        return shmemc_##_name##_cswap(target, cond, value, pe);         \
    }

SHMEM_TYPE_CSWAP(int, int, 32)
#if __WORDSIZE == 64
SHMEM_TYPE_CSWAP(long, long, 64)
#else
SHMEM_TYPE_CSWAP(long, long, 32)
#endif
SHMEM_TYPE_CSWAP(longlong, long long, 64)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_atomic_fetch_add = pshmem_int_atomic_fetch_add
#define shmem_int_atomic_fetch_add pshmem_int_atomic_fetch_add
#pragma weak shmem_long_atomic_fetch_add = pshmem_long_atomic_fetch_add
#define shmem_long_atomic_fetch_add pshmem_long_atomic_fetch_add
#pragma weak shmem_longlong_atomic_fetch_add = pshmem_longlong_atomic_fetch_add
#define shmem_longlong_atomic_fetch_add pshmem_longlong_atomic_fetch_add
#endif /* ENABLE_PSHMEM */

/*
 * fetch-and-add
 */

#define SHMEM_TYPE_FADD(_name, _type, _size)                            \
    _type                                                               \
    shmem_##_name##_atomic_fetch_add(_type *target,                     \
                                     _type value, int pe)               \
    {                                                                   \
        return shmemc_##_name##_fadd(target, value, pe);                \
    }

SHMEM_TYPE_FADD(int, int, 32)
#if __WORDSIZE == 64
SHMEM_TYPE_FADD(long, long, 64)
#else
SHMEM_TYPE_FADD(long, long, 32)
#endif
SHMEM_TYPE_FADD(longlong, long long, 64)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_atomic_fetch_inc = pshmem_int_atomic_fetch_inc
#define shmem_int_atomic_fetch_inc pshmem_int_atomic_fetch_inc
#pragma weak shmem_long_atomic_fetch_inc = pshmem_long_atomic_fetch_inc
#define shmem_long_atomic_fetch_inc pshmem_long_atomic_fetch_inc
#pragma weak shmem_longlong_atomic_fetch_inc = pshmem_longlong_atomic_fetch_inc
#define shmem_longlong_atomic_fetch_inc pshmem_longlong_atomic_fetch_inc
#endif /* ENABLE_PSHMEM */

/*
 * fetch-and-increment
 */

#define SHMEM_TYPE_FINC(_name, _type, _size)                            \
    _type                                                               \
    shmem_##_name##_atomic_fetch_inc(_type *target, int pe)             \
    {                                                                   \
        return shmemc_##_name##_finc(target, pe);                       \
    }

SHMEM_TYPE_FINC(int, int, 32)
#if __WORDSIZE == 64
SHMEM_TYPE_FINC(long, long, 64)
#else
SHMEM_TYPE_FINC(long, long, 32)
#endif
SHMEM_TYPE_FINC(longlong, long long, 64)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_atomic_add = pshmem_int_atomic_add
#define shmem_int_atomic_add pshmem_int_atomic_add
#pragma weak shmem_long_atomic_add = pshmem_long_atomic_add
#define shmem_long_atomic_add pshmem_long_atomic_add
#pragma weak shmem_longlong_atomic_add = pshmem_longlong_atomic_add
#define shmem_longlong_atomic_add pshmem_longlong_atomic_add
#endif /* ENABLE_PSHMEM */

/*
 * add
 */

#define SHMEM_TYPE_ADD(_name, _type, _size)                             \
    void                                                                \
    shmem_##_name##_atomic_add(_type *target, _type value, int pe)      \
    {                                                                   \
        shmemc_##_name##_add(target, value, pe);                        \
    }

SHMEM_TYPE_ADD(int, int, 32)
#if __WORDSIZE == 64
SHMEM_TYPE_ADD(long, long, 64)
#else
SHMEM_TYPE_ADD(long, long, 32)
#endif
SHMEM_TYPE_ADD(longlong, long long, 64)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_atomic_inc = pshmem_int_atomic_inc
#define shmem_int_atomic_inc pshmem_int_atomic_inc
#pragma weak shmem_long_atomic_inc = pshmem_long_atomic_inc
#define shmem_long_atomic_inc pshmem_long_atomic_inc
#pragma weak shmem_longlong_atomic_inc = pshmem_longlong_atomic_inc
#define shmem_longlong_atomic_inc pshmem_longlong_atomic_inc
#endif /* ENABLE_PSHMEM */

/*
 * increment
 */

#define SHMEM_TYPE_INC(_name, _type, _size)                             \
    void                                                                \
    shmem_##_name##_atomic_inc(_type *target, int pe)                   \
    {                                                                   \
        shmemc_##_name##_inc(target, pe);                               \
    }

SHMEM_TYPE_INC(int, int, 32)
#if __WORDSIZE == 64
SHMEM_TYPE_INC(long, long, 64)
#else
SHMEM_TYPE_INC(long, long, 32)
#endif
SHMEM_TYPE_INC(longlong, long long, 64)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_atomic_fetch = pshmem_int_atomic_fetch
#define shmem_int_atomic_fetch pshmem_int_atomic_fetch
#pragma weak shmem_long_atomic_fetch = pshmem_long_atomic_fetch
#define shmem_long_atomic_fetch pshmem_long_atomic_fetch
#pragma weak shmem_longlong_atomic_fetch = pshmem_longlong_atomic_fetch
#define shmem_longlong_atomic_fetch pshmem_longlong_atomic_fetch
#pragma weak shmem_float_atomic_fetch = pshmem_float_atomic_fetch
#define shmem_float_atomic_fetch pshmem_float_atomic_fetch
#pragma weak shmem_double_atomic_fetch = pshmem_double_atomic_fetch
#define shmem_double_atomic_fetch pshmem_double_atomic_fetch
#endif /* ENABLE_PSHMEM */

/*
 * fetch
 */

#define SHMEM_TYPE_FETCH(_name, _type, _size)                           \
    _type                                                               \
    shmem_##_name##_atomic_fetch(_type *target, int pe)                 \
    {                                                                   \
        return shmemc_##_name##_fetch(target, pe);                      \
    }

SHMEM_TYPE_FETCH(int, int, 32)
#if __WORDSIZE == 64
SHMEM_TYPE_FETCH(long, long, 64)
#else
SHMEM_TYPE_FETCH(long, long, 32)
#endif
SHMEM_TYPE_FETCH(longlong, long long, 64)
SHMEM_TYPE_FETCH(float, float, 32)
SHMEM_TYPE_FETCH(double, double, 64)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_atomic_set = pshmem_int_atomic_set
#define shmem_int_atomic_set pshmem_int_atomic_set
#pragma weak shmem_long_atomic_set = pshmem_long_atomic_set
#define shmem_long_atomic_set pshmem_long_atomic_set
#pragma weak shmem_longlong_atomic_set = pshmem_longlong_atomic_set
#define shmem_longlong_atomic_set pshmem_longlong_atomic_set
#pragma weak shmem_float_atomic_set = pshmem_float_atomic_set
#define shmem_float_atomic_set pshmem_float_atomic_set
#pragma weak shmem_double_atomic_set = pshmem_double_atomic_set
#define shmem_double_atomic_set pshmem_double_atomic_set
#endif /* ENABLE_PSHMEM */

/*
 * xor
 */

#define SHMEM_TYPE_XOR(_name, _type)                                    \
    void                                                                \
    shmem_##_name##_atomic_xor(_type *target, _type value, int pe)      \
    {                                                                   \
        shmemc_##_name##_xor(target, value, pe);                        \
    }

SHMEM_TYPE_XOR(uint, unsigned int)
SHMEM_TYPE_XOR(ulong, unsigned long)
SHMEM_TYPE_XOR(ulonglong, unsigned long long)
SHMEM_TYPE_XOR(int32, int32_t)
SHMEM_TYPE_XOR(int64, int64_t)
SHMEM_TYPE_XOR(uint32, uint32_t)
SHMEM_TYPE_XOR(uint64, uint64_t)


/*
 *-----------------------------------------------------------------------
 *
 * Deprecations as of 1.4.  3 different macros for different parameter
 * counts.  Provide variants for return types.  This needs to match
 * deprecation information in the header file.
 *
 * TODO: flesh out new 1.4 standard AMO types
 */

#define SHMEM_DEPRECATE_VOID_AMO1(_op, _name, _type)                    \
    void                                                                \
    shmem_##_name##_##_op(_type *target, int pe)                        \
    {                                                                   \
        deprecate(__func__);                                            \
        shmemc_##_name##_##_op(target, pe);                             \
    }

#define SHMEM_DEPRECATE_VOID_AMO2(_op, _name, _type)                    \
    void                                                                \
    shmem_##_name##_##_op(_type *target, _type value, int pe)           \
    {                                                                   \
        deprecate(__func__);                                            \
        shmemc_##_name##_##_op(target, value, pe);                      \
    }

#define SHMEM_DEPRECATE_VOID_AMO3(_op, _name, _type)                    \
    void                                                                \
    shmem_##_name##_##_op(_type *target,                                \
                             _type cond, _type value,                   \
                             int pe)                                    \
    {                                                                   \
        deprecate(__func__);                                            \
        shmemc_##_name##_##_op(target, cond, value, pe);                \
    }

#define SHMEM_DEPRECATE_AMO1(_op, _name, _type)                         \
    _type                                                               \
    shmem_##_name##_##_op(_type *target, int pe)                        \
    {                                                                   \
        deprecate(__func__);                                            \
        return shmemc_##_name##_##_op(target, pe);                      \
    }

#define SHMEM_DEPRECATE_AMO2(_op, _name, _type)                         \
    _type                                                               \
    shmem_##_name##_##_op(_type *target, _type value, int pe)           \
    {                                                                   \
        deprecate(__func__);                                            \
        return shmemc_##_name##_##_op(target, value, pe);               \
    }

#define SHMEM_DEPRECATE_AMO3(_op, _name, _type)                         \
    _type                                                               \
    shmem_##_name##_##_op(_type *target,                                \
                             _type cond, _type value,                   \
                             int pe)                                    \
    {                                                                   \
        deprecate(__func__);                                            \
        return shmemc_##_name##_##_op(target, cond, value, pe);         \
    }

SHMEM_DEPRECATE_VOID_AMO2(set, int, int)
SHMEM_DEPRECATE_VOID_AMO2(set, long, long)
SHMEM_DEPRECATE_VOID_AMO2(set, longlong, long long)
SHMEM_DEPRECATE_VOID_AMO2(set, float, float)
SHMEM_DEPRECATE_VOID_AMO2(set, double, double)

SHMEM_DEPRECATE_VOID_AMO1(inc, int, int)
SHMEM_DEPRECATE_VOID_AMO1(inc, long, long)
SHMEM_DEPRECATE_VOID_AMO1(inc, longlong, long long)

SHMEM_DEPRECATE_VOID_AMO2(add, int, int)
SHMEM_DEPRECATE_VOID_AMO2(add, long, long)
SHMEM_DEPRECATE_VOID_AMO2(add, longlong, long long)

SHMEM_DEPRECATE_AMO1(fetch, int, int)
SHMEM_DEPRECATE_AMO1(fetch, long, long)
SHMEM_DEPRECATE_AMO1(fetch, longlong, long long)
SHMEM_DEPRECATE_AMO1(fetch, float, float)
SHMEM_DEPRECATE_AMO1(fetch, double, double)

SHMEM_DEPRECATE_AMO1(finc, int, int)
SHMEM_DEPRECATE_AMO1(finc, long, long)
SHMEM_DEPRECATE_AMO1(finc, longlong, long long)

SHMEM_DEPRECATE_AMO2(fadd, int, int)
SHMEM_DEPRECATE_AMO2(fadd, long, long)
SHMEM_DEPRECATE_AMO2(fadd, longlong, long long)

SHMEM_DEPRECATE_AMO2(swap, int, int)
SHMEM_DEPRECATE_AMO2(swap, long, long)
SHMEM_DEPRECATE_AMO2(swap, longlong, long long)
SHMEM_DEPRECATE_AMO2(swap, float, float)
SHMEM_DEPRECATE_AMO2(swap, double, double)

SHMEM_DEPRECATE_AMO3(cswap, int, int)
SHMEM_DEPRECATE_AMO3(cswap, long, long)
SHMEM_DEPRECATE_AMO3(cswap, longlong, long long)
