/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <sys/types.h>
#include <stdint.h>
#include <stddef.h>
#include <complex.h>

#define SHMEMU_MATH_FUNC(_name, _type)              \
    _type                                           \
    shmemu_sum_##_name##_func(_type _a, _type _b)   \
    {                                               \
        return (_a) + (_b);                         \
    }                                               \
    _type                                           \
    shmemu_prod_##_name##_func(_type _a, _type _b)  \
    {                                               \
        return (_a) * (_b);                         \
    }

SHMEMU_MATH_FUNC(float, float)
SHMEMU_MATH_FUNC(double, double)
SHMEMU_MATH_FUNC(short, short)
SHMEMU_MATH_FUNC(int, int)
SHMEMU_MATH_FUNC(long, long)
SHMEMU_MATH_FUNC(longlong, long long)
SHMEMU_MATH_FUNC(longdouble, long double)
SHMEMU_MATH_FUNC(uint, unsigned int)
SHMEMU_MATH_FUNC(ulong, unsigned long)
SHMEMU_MATH_FUNC(ulonglong, unsigned long long)
SHMEMU_MATH_FUNC(int32, int32_t)
SHMEMU_MATH_FUNC(int64, int64_t)
SHMEMU_MATH_FUNC(uint32, uint32_t)
SHMEMU_MATH_FUNC(uint64, uint64_t)
SHMEMU_MATH_FUNC(complexd, double complex)
SHMEMU_MATH_FUNC(complexf, float complex)

/**
 * these are the logical operations.  Note: these are *bitwise*.
 *
 */

#define SHMEMU_LOGIC_FUNC(_name, _type)             \
    _type                                           \
    shmemu_and_##_name##_func(_type _a, _type _b)   \
    {                                               \
        return (_a) & (_b);                         \
    }                                               \
    _type                                           \
    shmemu_or_##_name##_func(_type _a, _type _b)    \
    {                                               \
        return (_a) | (_b);                         \
    }                                               \
    _type                                           \
    shmemu_xor_##_name##_func(_type _a, _type _b)   \
    {                                               \
        return (_a) ^ (_b);                         \
    }

SHMEMU_LOGIC_FUNC(short, short)
SHMEMU_LOGIC_FUNC(int, int)
SHMEMU_LOGIC_FUNC(long, long)
SHMEMU_LOGIC_FUNC(longlong, long long)
SHMEMU_LOGIC_FUNC(uint, unsigned int)
SHMEMU_LOGIC_FUNC(ulong, unsigned long)
SHMEMU_LOGIC_FUNC(ulonglong, unsigned long long)
SHMEMU_LOGIC_FUNC(int32, int32_t)
SHMEMU_LOGIC_FUNC(int64, int64_t)
SHMEMU_LOGIC_FUNC(uint32, uint32_t)
SHMEMU_LOGIC_FUNC(uint64, uint64_t)

/**
 * these are the minima/maxima operations
 *
 */

#define SHMEMU_MINIMAX_FUNC(_name, _type)           \
    _type                                           \
    shmemu_min_##_name##_func(_type _a, _type _b)   \
    {                                               \
        return (_a) < (_b) ? (_a) : (_b);           \
    }                                               \
    _type                                           \
    shmemu_max_##_name##_func(_type _a, _type _b)   \
    {                                               \
        return (_a) > (_b) ? (_a) : (_b);           \
    }

SHMEMU_MINIMAX_FUNC(short, short)
SHMEMU_MINIMAX_FUNC(int, int)
SHMEMU_MINIMAX_FUNC(long, long)
SHMEMU_MINIMAX_FUNC(longlong, long long)
SHMEMU_MINIMAX_FUNC(float, float)
SHMEMU_MINIMAX_FUNC(double, double)
SHMEMU_MINIMAX_FUNC(longdouble, long double)
SHMEMU_MINIMAX_FUNC(uint, unsigned int)
SHMEMU_MINIMAX_FUNC(ulong, unsigned long)
SHMEMU_MINIMAX_FUNC(ulonglong, unsigned long long)
SHMEMU_MINIMAX_FUNC(int32, int32_t)
SHMEMU_MINIMAX_FUNC(int64, int64_t)
SHMEMU_MINIMAX_FUNC(uint32, uint32_t)
SHMEMU_MINIMAX_FUNC(uint64, uint64_t)
