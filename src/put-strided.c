#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem/defs.h"
#include "shmem/api.h"

/*
 * these are needed for propagating into Fortran,
 * but aren't actually part of the API
 */
extern void shmem_complexf_p(complex float *addr, complex float value, int pe);
extern void shmem_complexd_p(complex double *addr, complex double value, int pe);
extern void shmem_char_iput(char *target, const char *source, ptrdiff_t tst,
                            ptrdiff_t sst, size_t nelems, int pe);

#ifdef ENABLE_PSHMEM
#pragma weak shmem_short_iput = pshmem_short_iput
#define shmem_short_iput pshmem_short_iput
#pragma weak shmem_char_iput = pshmem_char_iput
#define shmem_char_iput pshmem_char_iput
#pragma weak shmem_int_iput = pshmem_int_iput
#define shmem_int_iput pshmem_int_iput
#pragma weak shmem_long_iput = pshmem_long_iput
#define shmem_long_iput pshmem_long_iput
#pragma weak shmem_double_iput = pshmem_double_iput
#define shmem_double_iput pshmem_double_iput
#pragma weak shmem_float_iput = pshmem_float_iput
#define shmem_float_iput pshmem_float_iput
#pragma weak shmem_longdouble_iput = pshmem_longdouble_iput
#define shmem_longdouble_iput pshmem_longdouble_iput
#pragma weak shmem_longlong_iput = pshmem_longlong_iput
#define shmem_longlong_iput pshmem_longlong_iput
#pragma weak shmem_iput32 = pshmem_iput32
#define shmem_iput32 pshmem_iput32
#pragma weak shmem_iput64 = pshmem_iput64
#define shmem_iput64 pshmem_iput64
#pragma weak shmem_iput128 = pshmem_iput128
#define shmem_iput128 pshmem_iput128
#endif /* ENABLE_PSHMEM */

#define SHMEM_EMIT_IPUT(_name, _type)                                   \
    void                                                                \
    shmem_##_name##_iput(_type *target, const _type *source,            \
                         ptrdiff_t tst, ptrdiff_t sst, size_t nelems, int pe) \
    {                                                                   \
        size_t ti = 0, si = 0;                                          \
        size_t i;                                                       \
        for (i = 0; i < nelems; i += 1) {                               \
            shmem_##_name##_p(& (target[ti]), source[si], pe);         \
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
        shmem_iput##_size(dest, src, tst, sst, sized_nelems, pe);       \
    }

SHMEM_SIZED_IPUT(32, 32)
SHMEM_SIZED_IPUT(4, 32)
SHMEM_SIZED_IPUT(64, 64)
SHMEM_SIZED_IPUT(8, 64)
SHMEM_SIZED_IPUT(128, 128)
