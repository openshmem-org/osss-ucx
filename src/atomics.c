#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

/*
 * TODO these are all just stubs, of course, for now
 */

/*
 * swap
 */

#define SHMEM_TYPE_SWAP(_name, _type)                                   \
    _type                                                               \
    shmem_##_name##_swap(_type *target, _type value, int pe)            \
    {                                                                   \
        return value;                                                   \
    }

SHMEM_TYPE_SWAP(int, int)
SHMEM_TYPE_SWAP(long, long)
SHMEM_TYPE_SWAP(longlong, long long)
SHMEM_TYPE_SWAP(float, float)
SHMEM_TYPE_SWAP(double, double)

/*
 * conditional swap
 */

#define SHMEM_TYPE_CSWAP(_name, _type)                                  \
    _type                                                               \
    shmem_##_name##_cswap(_type *target, _type cond, _type value, int pe) \
    {                                                                   \
        return value;                                                   \
    }

SHMEM_TYPE_CSWAP(int, int)
SHMEM_TYPE_CSWAP(long, long)
SHMEM_TYPE_CSWAP(longlong, long long)

/*
 * fetch-and-add
 */

#define SHMEM_TYPE_FADD(_name, _type)                                   \
    _type                                                               \
    shmem_##_name##_fadd(_type *target, _type value, int pe)            \
    {                                                                   \
        return value;                                                   \
    }

SHMEM_TYPE_FADD(int, int)
SHMEM_TYPE_FADD(long, long)
SHMEM_TYPE_FADD(longlong, long long)

/*
 * fetch-and-increment
 */

#define SHMEM_TYPE_FINC(_name, _type)                                   \
    _type                                                               \
    shmem_##_name##_finc(_type *target, _type value, int pe)            \
    {                                                                   \
        return value;                                                   \
    }

SHMEM_TYPE_FINC(int, int)
SHMEM_TYPE_FINC(long, long)
SHMEM_TYPE_FINC(longlong, long long)

/*
 * add
 */

#define SHMEM_TYPE_ADD(_name, _type)                                    \
    _type                                                               \
    shmem_##_name##_add(_type *target, _type value, int pe)             \
    {                                                                   \
        return value;                                                   \
    }

SHMEM_TYPE_ADD(int, int)
SHMEM_TYPE_ADD(long, long)
SHMEM_TYPE_ADD(longlong, long long)

/*
 * increment
 */

#define SHMEM_TYPE_INC(_name, _type)                                    \
    _type                                                               \
    shmem_##_name##_inc(_type *target, _type value, int pe)             \
    {                                                                   \
        return value;                                                   \
    }

SHMEM_TYPE_INC(int, int)
SHMEM_TYPE_INC(long, long)
SHMEM_TYPE_INC(longlong, long long)

/*
 * fetch
 */

#define SHMEM_TYPE_FETCH(_name, _type)                                  \
    _type                                                               \
    shmem_##_name##_fetch(_type *target, _type value, int pe)           \
    {                                                                   \
        return value;                                                   \
    }

SHMEM_TYPE_FETCH(int, int)
SHMEM_TYPE_FETCH(long, long)
SHMEM_TYPE_FETCH(longlong, long long)
SHMEM_TYPE_FETCH(float, float)
SHMEM_TYPE_FETCH(double, double)

/*
 * set
 */

#define SHMEM_TYPE_SET(_name, _type)                                    \
    void                                                                \
    shmem_##_name##_set(_type *target, _type value, int pe)             \
    {                                                                   \
    }

SHMEM_TYPE_SET(int, int)
SHMEM_TYPE_SET(long, long)
SHMEM_TYPE_SET(longlong, long long)
SHMEM_TYPE_SET(float, float)
SHMEM_TYPE_SET(double, double)
