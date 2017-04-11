#include "shmem/defs.h"
#include "shmemc/shmemc.h"

#define SHMEM_TYPED_GET_NBI(_name, _type)                               \
    void                                                                \
    shmem_##_name##_get_nbi(_type *dest, const _type *src,              \
                            size_t nelems, int pe)                      \
    {                                                                   \
        const size_t typed_nelems = nelems * sizeof (_type);            \
        shmemc_get_nbi(dest, src, typed_nelems, pe);                    \
    }

SHMEM_TYPED_GET_NBI(char, char)
SHMEM_TYPED_GET_NBI(short, short)
SHMEM_TYPED_GET_NBI(int, int)
SHMEM_TYPED_GET_NBI(long, long)
SHMEM_TYPED_GET_NBI(longlong, long long)
SHMEM_TYPED_GET_NBI(longdouble, long double)
SHMEM_TYPED_GET_NBI(double, double)
SHMEM_TYPED_GET_NBI(float, float)
SHMEM_TYPED_GET_NBI(complexf, COMPLEXIFY(float))
SHMEM_TYPED_GET_NBI(complexd, COMPLEXIFY(double))

#define SHMEM_SIZED_GET_NBI(_name, _size)                               \
    void                                                                \
    shmem_get_##_name##_nbi(void *dest, const void *src,                \
                            size_t nelems, int pe)                      \
    {                                                                   \
        const size_t sized_nelems = nelems * _size;                     \
        shmemc_get_nbi(dest, src, sized_nelems, pe);                    \
    }

SHMEM_SIZED_GET_NBI(32, 32)
SHMEM_SIZED_GET_NBI(4, 32)
SHMEM_SIZED_GET_NBI(64, 64)
SHMEM_SIZED_GET_NBI(8, 64)
SHMEM_SIZED_GET_NBI(128, 128)

void
shmem_getmem_nbi(void *dest, const void *src, size_t nelems, int pe)
{
    shmemc_get_nbi(dest, src, nelems, pe);
}
