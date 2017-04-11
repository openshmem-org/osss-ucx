#include "shmem/defs.h"
#include "shmemc/shmemc.h"

#define SHMEM_TYPED_PUT_NBI(_name, _type)                               \
    void                                                                \
    shmem_##_name##_put_nbi(_type *dest, const _type *src,              \
                            size_t nelems, int pe)                      \
    {                                                                   \
        const size_t typed_nelems = nelems * sizeof (_type);            \
        shmemc_put_nbi(dest, src, typed_nelems, pe);                    \
    }

SHMEM_TYPED_PUT_NBI(char, char)
SHMEM_TYPED_PUT_NBI(short, short)
SHMEM_TYPED_PUT_NBI(int, int)
SHMEM_TYPED_PUT_NBI(long, long)
SHMEM_TYPED_PUT_NBI(longlong, long long)
SHMEM_TYPED_PUT_NBI(longdouble, long double)
SHMEM_TYPED_PUT_NBI(double, double)
SHMEM_TYPED_PUT_NBI(float, float)
SHMEM_TYPED_PUT_NBI(complexf, COMPLEXIFY(float))
SHMEM_TYPED_PUT_NBI(complexd, COMPLEXIFY(double))

#define SHMEM_SIZED_PUT_NBI(_name, _size)                               \
    void                                                                \
    shmem_put_##_name##_nbi(void *dest, const void *src,                \
                            size_t nelems, int pe)                      \
    {                                                                   \
        const size_t sized_nelems = nelems * _size;                     \
        shmemc_put_nbi(dest, src, sized_nelems, pe);                    \
    }

SHMEM_SIZED_PUT_NBI(32, 32)
SHMEM_SIZED_PUT_NBI(4, 32)
SHMEM_SIZED_PUT_NBI(64, 64)
SHMEM_SIZED_PUT_NBI(8, 64)
SHMEM_SIZED_PUT_NBI(128, 128)

void
shmem_putmem_nbi(void *dest, const void *src, size_t nelems, int pe)
{
    shmemc_put_nbi(dest, src, nelems, pe);
}
