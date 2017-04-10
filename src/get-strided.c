#include "shmem/defs.h"
#include "shmem/api.h"
#include "shmemc/shmemc.h"

/*
 * these are needed for propagating into Fortran,
 * but aren't actually part of the API
 */
extern complex float shmem_complexf_g(complex float *addr, int pe);
extern complex double shmem_complexd_g(complex double *addr, int pe);
extern void shmem_char_iget(char *target, const char *source, ptrdiff_t tst,
                            ptrdiff_t sst, size_t nelems, int pe);

#define SHMEM_EMIT_IGET(_name, _type)                                   \
    void                                                                \
    shmem_##_name##_iget(_type *target, const _type *source,            \
                         ptrdiff_t tst, ptrdiff_t sst, size_t nelems, int pe) \
    {                                                                   \
        size_t ti = 0, si = 0;                                          \
        size_t i;                                                       \
        for (i = 0; i < nelems; i += 1) {                               \
            target[ti] = shmem_##_name##_g((_type *) & (source[si]), pe); \
            ti += tst;                                                  \
            si += sst;                                                  \
        }                                                               \
    }

SHMEM_EMIT_IGET(char, char)
SHMEM_EMIT_IGET(short, short)
SHMEM_EMIT_IGET(int, int)
SHMEM_EMIT_IGET(long, long)
SHMEM_EMIT_IGET(float, float)
SHMEM_EMIT_IGET(double, double)
SHMEM_EMIT_IGET(longlong, long long)
SHMEM_EMIT_IGET(longdouble, long double)
SHMEM_EMIT_IGET(complexf, COMPLEXIFY(float))
SHMEM_EMIT_IGET(complexd, COMPLEXIFY(double))

#define SHMEM_SIZED_IGET(_name, _size)                                  \
    void                                                                \
    shmem_iget##_name(void *dest, const void *src,                      \
                      ptrdiff_t tst, ptrdiff_t sst, size_t nelems, int pe) \
    {                                                                   \
        const size_t sized_nelems = nelems * _size;                     \
        shmemc_iget(dest, src, tst, sst, sized_nelems, pe);             \
    }

SHMEM_SIZED_IGET(32, 32)
SHMEM_SIZED_IGET(4, 32)
SHMEM_SIZED_IGET(64, 64)
SHMEM_SIZED_IGET(8, 64)
SHMEM_SIZED_IGET(128, 128)
