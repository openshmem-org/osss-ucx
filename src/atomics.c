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
        return shmemc_swap##_size(target, value, pe);                   \
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
        return shmemc_cswap##_size(target, cond, value, pe);            \
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
        return shmemc_fadd##_size(target, value, pe);                   \
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
        return shmemc_finc##_size(target, pe);                          \
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
        shmemc_add##_size(target, value, pe);                           \
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
        shmemc_inc##_size(target, pe);                                  \
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
        return shmemc_fetch##_size(target, pe);                         \
    }

SHMEM_TYPE_FETCH(float, float, 32)
SHMEM_TYPE_FETCH(double, double, 64)
SHMEM_TYPE_FETCH(int, int, 32)
#if __WORDSIZE == 64
SHMEM_TYPE_FETCH(long, long, 64)
#else
SHMEM_TYPE_FETCH(long, long, 32)
#endif
SHMEM_TYPE_FETCH(longlong, long long, 64)
SHMEM_TYPE_FETCH(uint, unsigned int, 32)
SHMEM_TYPE_FETCH(ulong, unsigned long, 64)
SHMEM_TYPE_FETCH(ulonglong, unsigned long long, 64)
SHMEM_TYPE_FETCH(int32, int32_t, 32)
SHMEM_TYPE_FETCH(int64, int64_t, 64)
SHMEM_TYPE_FETCH(uint32, uint32_t, 32)
SHMEM_TYPE_FETCH(uint64, uint64_t, 64)
SHMEM_TYPE_FETCH(size, size_t, 64)
SHMEM_TYPE_FETCH(ptrdiff, ptrdiff_t, 64)

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
 * set
 */

#define SHMEM_TYPE_SET(_name, _type, _size)                             \
    void                                                                \
    shmem_##_name##_atomic_set(_type *target, _type value, int pe)      \
    {                                                                   \
        shmemc_set##_size(target, value, pe);                           \
    }

SHMEM_TYPE_SET(float, float, 32)
SHMEM_TYPE_SET(double, double, 64)
SHMEM_TYPE_SET(int, int, 32)
#if __WORDSIZE == 64
SHMEM_TYPE_SET(long, long, 64)
#else
SHMEM_TYPE_SET(long, long, 32)
#endif
SHMEM_TYPE_SET(longlong, long long, 64)
SHMEM_TYPE_SET(uint, unsigned int, 32)
SHMEM_TYPE_SET(ulong, unsigned long, 64)
SHMEM_TYPE_SET(ulonglong, unsigned long long, 64)
SHMEM_TYPE_SET(int32, int32_t, 32)
SHMEM_TYPE_SET(int64, int64_t, 64)
SHMEM_TYPE_SET(uint32, uint32_t, 32)
SHMEM_TYPE_SET(uint64, uint64_t, 64)
SHMEM_TYPE_SET(size, size_t, 64)
SHMEM_TYPE_SET(ptrdiff, ptrdiff_t, 64)

/*
 * xor
 */

#define SHMEM_TYPE_XOR(_name, _type, _size)                             \
    void                                                                \
    shmem_##_name##_atomic_xor(_type *target, _type value, int pe)      \
    {                                                                   \
        shmemc_xor##_size(target, value, pe);                           \
    }

SHMEM_TYPE_XOR(uint, unsigned int, 32)
SHMEM_TYPE_XOR(ulong, unsigned long, 64)
SHMEM_TYPE_XOR(ulonglong, unsigned long long, 64)
SHMEM_TYPE_XOR(int32, int32_t, 32)
SHMEM_TYPE_XOR(int64, int64_t, 64)
SHMEM_TYPE_XOR(uint32, uint32_t, 32)
SHMEM_TYPE_XOR(uint64, uint64_t, 64)


/*
 *-----------------------------------------------------------------------
 *
 * Deprecations as of 1.4.  3 different macros for different parameter
 * counts.  Provide variants for return types.  This needs to match
 * deprecation information in the header file.
 *
 * TODO: flesh out new 1.4 standard AMO types
 */

#define SHMEM_DEPRECATE_VOID_AMO1(_op, _name, _type, _size)             \
    void                                                                \
    shmem_##_name##_##_op(_type *target, int pe)                        \
    {                                                                   \
        deprecate(__func__);                                            \
        shmemc_##_op##_size(target, pe);                                \
    }

#define SHMEM_DEPRECATE_VOID_AMO2(_op, _name, _type, _size)             \
    void                                                                \
    shmem_##_name##_##_op(_type *target, _type value, int pe)           \
    {                                                                   \
        deprecate(__func__);                                            \
        shmemc_##_op##_size(target, value, pe);                         \
    }

#define SHMEM_DEPRECATE_VOID_AMO3(_op, _name, _type, _soze)             \
    void                                                                \
    shmem_##_name##_##_op(_type *target,                                \
                          _type cond, _type value,                      \
                          int pe)                                       \
    {                                                                   \
        deprecate(__func__);                                            \
        shmemc_##_op##_size(target, cond, value, pe);                   \
    }

#define SHMEM_DEPRECATE_AMO1(_op, _name, _type, _size)                  \
    _type                                                               \
    shmem_##_name##_##_op(_type *target, int pe)                        \
    {                                                                   \
        deprecate(__func__);                                            \
        return shmemc_##_op##_size(target, pe);                         \
    }

#define SHMEM_DEPRECATE_AMO2(_op, _name, _type, _size)                  \
    _type                                                               \
    shmem_##_name##_##_op(_type *target, _type value, int pe)           \
    {                                                                   \
        deprecate(__func__);                                            \
        return shmemc_##_op##_size(target, value, pe);                  \
    }

#define SHMEM_DEPRECATE_AMO3(_op, _name, _type, _size)                  \
    _type                                                               \
    shmem_##_name##_##_op(_type *target,                                \
                          _type cond, _type value,                      \
                          int pe)                                       \
    {                                                                   \
        deprecate(__func__);                                            \
        return shmemc_##_op##_size(target, cond, value, pe);            \
    }

SHMEM_DEPRECATE_VOID_AMO2(set, int, int, 32)
SHMEM_DEPRECATE_VOID_AMO2(set, long, long, 64)
SHMEM_DEPRECATE_VOID_AMO2(set, longlong, long long, 64)
SHMEM_DEPRECATE_VOID_AMO2(set, float, float, 32)
SHMEM_DEPRECATE_VOID_AMO2(set, double, double, 64)

SHMEM_DEPRECATE_VOID_AMO1(inc, int, int, 32)
SHMEM_DEPRECATE_VOID_AMO1(inc, long, long, 64)
SHMEM_DEPRECATE_VOID_AMO1(inc, longlong, long long, 64)

SHMEM_DEPRECATE_VOID_AMO2(add, int, int, 32)
SHMEM_DEPRECATE_VOID_AMO2(add, long, long, 64)
SHMEM_DEPRECATE_VOID_AMO2(add, longlong, long long, 64)

SHMEM_DEPRECATE_AMO1(fetch, int, int, 32)
SHMEM_DEPRECATE_AMO1(fetch, long, long, 64)
SHMEM_DEPRECATE_AMO1(fetch, longlong, long long, 64)
SHMEM_DEPRECATE_AMO1(fetch, float, float, 32)
SHMEM_DEPRECATE_AMO1(fetch, double, double, 64)

SHMEM_DEPRECATE_AMO1(finc, int, int, 32)
SHMEM_DEPRECATE_AMO1(finc, long, long, 64)
SHMEM_DEPRECATE_AMO1(finc, longlong, long long, 64)

SHMEM_DEPRECATE_AMO2(fadd, int, int, 32)
SHMEM_DEPRECATE_AMO2(fadd, long, long, 64)
SHMEM_DEPRECATE_AMO2(fadd, longlong, long long, 64)

SHMEM_DEPRECATE_AMO2(swap, int, int, 32)
SHMEM_DEPRECATE_AMO2(swap, long, long, 64)
SHMEM_DEPRECATE_AMO2(swap, longlong, long long, 64)
SHMEM_DEPRECATE_AMO2(swap, float, float, 32)
SHMEM_DEPRECATE_AMO2(swap, double, double, 64)

SHMEM_DEPRECATE_AMO3(cswap, int, int, 32)
SHMEM_DEPRECATE_AMO3(cswap, long, long, 64)
SHMEM_DEPRECATE_AMO3(cswap, longlong, long long, 64)
