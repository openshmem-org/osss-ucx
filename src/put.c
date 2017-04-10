#include "shmem/defs.h"
#include "shmemc/shmemc.h"

#define SHMEM_TYPED_PUT(_name, _type)                                   \
    void                                                                \
    shmem_##_name##_put(_type *dest, const _type *src,                  \
                        size_t nelems, int pe)                          \
    {                                                                   \
        const size_t sized_nelems = nelems * sizeof(_type);             \
        shmemc_put(dest, src, sized_nelems, pe);                        \
    }

SHMEM_TYPED_PUT(char, char)
SHMEM_TYPED_PUT(short, short)
SHMEM_TYPED_PUT(int, int)
SHMEM_TYPED_PUT(long, long)
SHMEM_TYPED_PUT(longlong, long long)
SHMEM_TYPED_PUT(longdouble, long double)
SHMEM_TYPED_PUT(double, double)
SHMEM_TYPED_PUT(float, float)
SHMEM_TYPED_PUT(complexf, COMPLEXIFY (float))
SHMEM_TYPED_PUT(complexd, COMPLEXIFY (double))

#define SHMEM_SIZED_PUT(_name, _size)                                   \
    void                                                                \
    shmem_put##_name(void *dest, const void *src,                       \
                     size_t nelems, int pe)                             \
    {                                                                   \
        const size_t sized_nelems = nelems * _size;                     \
        shmemc_put(dest, src, sized_nelems, pe);                        \
    }

SHMEM_SIZED_PUT(32, 32)
SHMEM_SIZED_PUT(4, 32)
SHMEM_SIZED_PUT(64, 64)
SHMEM_SIZED_PUT(8, 64)
SHMEM_SIZED_PUT(128, 128)

void
shmem_putmem(void *dest, const void *src,
             size_t nelems, int pe)
{
    shmemc_put(dest, src, nelems, pe);
}

/* TODO */

#define SHMEM_TYPED_P_WRAPPER(_name, _type)                          \
    void                                                             \
    shmem_##_name##_p(_type *addr, _type val, int pe)                \
    {                                                                \
        shmem_##_name##_put(addr, &val, 1, pe);                      \
    }

SHMEM_TYPED_P_WRAPPER(char, char)
SHMEM_TYPED_P_WRAPPER(short, short)
SHMEM_TYPED_P_WRAPPER(int, int)
SHMEM_TYPED_P_WRAPPER(long, long)
SHMEM_TYPED_P_WRAPPER(longlong, long long)
SHMEM_TYPED_P_WRAPPER(float, float)
SHMEM_TYPED_P_WRAPPER(double, double)
SHMEM_TYPED_P_WRAPPER(longdouble, long double)
SHMEM_TYPED_P_WRAPPER(complexf, COMPLEXIFY(float))
SHMEM_TYPED_P_WRAPPER(complexd, COMPLEXIFY(double))
