#include "shmem/defs.h"
#include "shmem/api.h"
#include "shmemc/shmemc.h"

/*
 * these are needed for propagating into Fortran,
 * but aren't actually part of the API
 */
extern void shmem_complexf_p(complex float *addr, complex float value, int pe);
extern void shmem_complexd_p(complex double *addr, complex double value, int pe);
extern void shmem_char_iput(char *target, const char *source, ptrdiff_t tst,
                            ptrdiff_t sst, size_t nelems, int pe);

#define SHMEM_EMIT_IPUT(_name, _type)                                   \
    void                                                                \
    shmem_##_name##_iput(_type *target, const _type *source,            \
                         ptrdiff_t tst, ptrdiff_t sst, size_t nelems, int pe) \
    {                                                                   \
        size_t ti = 0, si = 0;                                          \
        size_t i;                                                       \
        for (i = 0; i < nelems; i += 1) {                               \
            shmem_##_name##_p(& (target[ti]), source[si], pe);          \
            ti += tst;                                                  \
            si += sst;                                                  \
        }                                                               \
    }

SHMEM_EMIT_IPUT(char, char)
SHMEM_EMIT_IPUT(short, short)
SHMEM_EMIT_IPUT(int, int)
SHMEM_EMIT_IPUT(long, long)
SHMEM_EMIT_IPUT(float, float)
SHMEM_EMIT_IPUT(double, double)
SHMEM_EMIT_IPUT(longlong, long long)
SHMEM_EMIT_IPUT(longdouble, long double)
SHMEM_EMIT_IPUT(complexf, COMPLEXIFY(float))
SHMEM_EMIT_IPUT(complexd, COMPLEXIFY(double))

#define SHMEM_SIZED_IPUT(_name, _size)                                  \
    void                                                                \
    shmem_iput##_name(void *dest, const void *src,                      \
                      ptrdiff_t tst, ptrdiff_t sst, size_t nelems, int pe) \
    {                                                                   \
        const size_t sized_nelems = nelems * _size;                     \
        shmemc_iput(dest, src, tst, sst, sized_nelems, pe);             \
    }

SHMEM_SIZED_IPUT(32, 32)
SHMEM_SIZED_IPUT(4, 32)
SHMEM_SIZED_IPUT(64, 64)
SHMEM_SIZED_IPUT(8, 64)
SHMEM_SIZED_IPUT(128, 128)
