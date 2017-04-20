#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc/shmemc.h"

#include <bits/wordsize.h>

/*
 * swap
 */

#define SHMEM_TYPE_SWAP(_name, _type, _size)                            \
    _type                                                               \
    shmem_##_name##_swap(_type *target, _type value, int pe)            \
    {                                                                   \
        return shmemc_swap##_size((int##_size##_t *) target, value, pe); \
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

/*
 * conditional swap
 */

#define SHMEM_TYPE_CSWAP(_name, _type, _size)                           \
    _type                                                               \
    shmem_##_name##_cswap(_type *target, _type cond, _type value, int pe) \
    {                                                                   \
        return shmemc_cswap##_size((int##_size##_t *) target, cond, value, pe); \
    }

SHMEM_TYPE_CSWAP(int, int, 32)
#if __WORDSIZE == 64
SHMEM_TYPE_CSWAP(long, long, 64)
#else
SHMEM_TYPE_CSWAP(long, long, 32)
#endif
SHMEM_TYPE_CSWAP(longlong, long long, 64)
SHMEM_TYPE_CSWAP(float, float, 32)
SHMEM_TYPE_CSWAP(double, double, 32)

/*
 * fetch-and-add
 */

#define SHMEM_TYPE_FADD(_name, _type, _size)                            \
    _type                                                               \
    shmem_##_name##_fadd(_type *target, _type value, int pe)            \
    {                                                                   \
        return shmemc_fadd##_size((int##_size##_t *) target, value, pe); \
    }

SHMEM_TYPE_FADD(int, int, 32)
#if __WORDSIZE == 64
SHMEM_TYPE_FADD(long, long, 64)
#else
SHMEM_TYPE_FADD(long, long, 32)
#endif
SHMEM_TYPE_FADD(longlong, long long, 64)

/*
 * fetch-and-increment
 */

#define SHMEM_TYPE_FINC(_name, _type, _size)                            \
    _type                                                               \
    shmem_##_name##_finc(_type *target, int pe)                         \
    {                                                                   \
        return shmemc_finc##_size((int##_size##_t *) target, pe);       \
    }

SHMEM_TYPE_FINC(int, int, 32)
#if __WORDSIZE == 64
SHMEM_TYPE_FINC(long, long, 64)
#else
SHMEM_TYPE_FINC(long, long, 32)
#endif
SHMEM_TYPE_FINC(longlong, long long, 64)

/*
 * add
 */

#define SHMEM_TYPE_ADD(_name, _type, _size)                             \
    void                                                                \
    shmem_##_name##_add(_type *target, _type value, int pe)             \
    {                                                                   \
        shmemc_add##_size((int##_size##_t *) target, value, pe);        \
    }

SHMEM_TYPE_ADD(int, int, 32)
#if __WORDSIZE == 64
SHMEM_TYPE_ADD(long, long, 64)
#else
SHMEM_TYPE_ADD(long, long, 32)
#endif
SHMEM_TYPE_ADD(longlong, long long, 64)

/*
 * increment
 */

#define SHMEM_TYPE_INC(_name, _type, _size)                             \
    void                                                                \
    shmem_##_name##_inc(_type *target, int pe)                          \
    {                                                                   \
        shmemc_inc##_size((int##_size##_t *) target, pe);               \
    }

SHMEM_TYPE_INC(int, int, 32)
#if __WORDSIZE == 64
SHMEM_TYPE_INC(long, long, 64)
#else
SHMEM_TYPE_INC(long, long, 32)
#endif
SHMEM_TYPE_INC(longlong, long long, 64)

/*
 * fetch
 */

#define SHMEM_TYPE_FETCH(_name, _type, _size)                           \
    _type                                                               \
    shmem_##_name##_fetch(_type *target, int pe)                        \
    {                                                                   \
        return shmemc_fetch##_size((int##_size##_t *) target, pe);      \
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

/*
 * set
 */

#define SHMEM_TYPE_SET(_name, _type, _size)                             \
    void                                                                \
    shmem_##_name##_set(_type *target, _type value, int pe)             \
    {                                                                   \
        shmemc_set##_size((int##_size##_t *) target, value, pe);        \
    }

SHMEM_TYPE_SET(int, int, 32)
#if __WORDSIZE == 64
SHMEM_TYPE_SET(long, long, 64)
#else
SHMEM_TYPE_SET(long, long, 32)
#endif
SHMEM_TYPE_SET(longlong, long long, 64)
SHMEM_TYPE_SET(float, float, 32)
SHMEM_TYPE_SET(double, double, 64)
