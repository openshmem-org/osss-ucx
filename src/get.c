#include "shmem/defs.h"
#include "shmemc/shmemc.h"

#define SHMEM_TYPE_GET(_name, _type)                            \
    void                                                        \
    shmem_##_name##_get(_type *dest, const _type *src,          \
                        size_t nelems, int pe)                  \
    {                                                           \
        const size_t typed_nelems = nelems * sizeof (_type);    \
        shmemc_get(dest, src, typed_nelems, pe);                \
    }

SHMEM_TYPE_GET(char, char)
SHMEM_TYPE_GET(short, short)
SHMEM_TYPE_GET(int, int)
SHMEM_TYPE_GET(long, long)
SHMEM_TYPE_GET(longdouble, long double)
SHMEM_TYPE_GET(longlong, long long)
SHMEM_TYPE_GET(double, double)
SHMEM_TYPE_GET(float, float)
SHMEM_TYPE_GET(complexf, COMPLEXIFY(float))
SHMEM_TYPE_GET(complexd, COMPLEXIFY(double))

#define SHMEM_SIZED_GET(_name, _size)               \
    void                                            \
    shmem_get_##_name(void *dest, const void *src,  \
                      size_t nelems, int pe)        \
    {                                               \
        const size_t sized_nelems = nelems * _size; \
        shmemc_get(dest, src, sized_nelems, pe);    \
    }

SHMEM_SIZED_GET(32, 32)
SHMEM_SIZED_GET(4, 32)
SHMEM_SIZED_GET(64, 64)
SHMEM_SIZED_GET(8, 64)
SHMEM_SIZED_GET(128, 128)

void
shmem_getmem(void *dest, const void *src,
             size_t nelems, int pe)
{
    shmemc_get(dest, src, nelems, pe);
}

/* TODO */

#define SHMEM_TYPE_G_WRAPPER(_name, _type)                          \
    _type                                                           \
    shmem_##_name##_g(const _type *addr, int pe)                    \
    {                                                               \
        _type retval;                                               \
        shmem_##_name##_get(&retval, addr, 1, pe);                  \
        return retval;                                              \
    }

SHMEM_TYPE_G_WRAPPER(float, float)
SHMEM_TYPE_G_WRAPPER(double, double)
SHMEM_TYPE_G_WRAPPER(longlong, long long)
SHMEM_TYPE_G_WRAPPER(longdouble, long double)
SHMEM_TYPE_G_WRAPPER(char, char)
SHMEM_TYPE_G_WRAPPER(short, short)
SHMEM_TYPE_G_WRAPPER(int, int)
SHMEM_TYPE_G_WRAPPER(long, long)
SHMEM_TYPE_G_WRAPPER(complexf, COMPLEXIFY(float))
SHMEM_TYPE_G_WRAPPER(complexd, COMPLEXIFY(double))
