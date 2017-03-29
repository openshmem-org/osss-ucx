#include "shmem_defines.h"
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
    shmem_put_##_name(void *dest, const void *src,                      \
                       size_t nelems, int pe)                           \
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
