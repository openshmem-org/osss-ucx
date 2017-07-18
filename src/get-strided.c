#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem/defs.h"
#include "shmem/api.h"

#include "shmemc.h"

/*
 * these are needed for propagating into Fortran,
 * but aren't actually part of the API
 */
extern COMPLEXIFY(float) shmem_complexf_g(COMPLEXIFY(float) *addr, int pe);
extern COMPLEXIFY(double) shmem_complexd_g(COMPLEXIFY(double) *addr, int pe);
extern void shmem_char_iget(char *target, const char *source, ptrdiff_t tst,
                            ptrdiff_t sst, size_t nelems, int pe);

#ifdef ENABLE_PSHMEM
#pragma weak shmem_char_iget = pshmem_char_iget
#define shmem_char_iget pshmem_char_iget
#pragma weak shmem_short_iget = pshmem_short_iget
#define shmem_short_iget pshmem_short_iget
#pragma weak shmem_int_iget = pshmem_int_iget
#define shmem_int_iget pshmem_int_iget
#pragma weak shmem_long_iget = pshmem_long_iget
#define shmem_long_iget pshmem_long_iget
#pragma weak shmem_double_iget = pshmem_double_iget
#define shmem_double_iget pshmem_double_iget
#pragma weak shmem_float_iget = pshmem_float_iget
#define shmem_float_iget pshmem_float_iget
#pragma weak shmem_longdouble_iget = pshmem_longdouble_iget
#define shmem_longdouble_iget pshmem_longdouble_iget
#pragma weak shmem_longlong_iget = pshmem_longlong_iget
#define shmem_longlong_iget pshmem_longlong_iget
#pragma weak shmem_iget32 = pshmem_iget32
#define shmem_iget32 pshmem_iget32
#pragma weak shmem_iget64 = pshmem_iget64
#define shmem_iget64 pshmem_iget64
#pragma weak shmem_iget128 = pshmem_iget128
#define shmem_iget128 pshmem_iget128
#endif /* ENABLE_PSHMEM */

#define SHMEM_TYPE_IGET(_name, _type)                                   \
    void                                                                \
    shmem_##_name##_iget(_type *target, const _type *source,            \
                         ptrdiff_t tst, ptrdiff_t sst,                  \
                         size_t nelems, int pe)                         \
    {                                                                   \
        const size_t sized_nelems = nelems * sizeof(_type);             \
        shmem_##_name##_iget(target, source, tst, sst, sized_nelems, pe); \
    }

SHMEM_TYPE_IGET(char, char)
SHMEM_TYPE_IGET(short, short)
SHMEM_TYPE_IGET(int, int)
SHMEM_TYPE_IGET(long, long)
SHMEM_TYPE_IGET(float, float)
SHMEM_TYPE_IGET(double, double)
SHMEM_TYPE_IGET(longlong, long long)
SHMEM_TYPE_IGET(longdouble, long double)
SHMEM_TYPE_IGET(complexf, COMPLEXIFY(float))
SHMEM_TYPE_IGET(complexd, COMPLEXIFY(double))

#define SHMEM_SIZED_IGET(_bits, _size)                                  \
    void                                                                \
    shmem_iget##_bits(void *target, const void *source,                 \
                      ptrdiff_t tst, ptrdiff_t sst,                     \
                      size_t nelems, int pe)                            \
    {                                                                   \
        const size_t sized_nelems = nelems * _size;                     \
        shmem_##_name##_iget(target, source, tst, sst, sized_nelems, pe); \
    }

SHMEM_SIZED_IGET(32, 32)
SHMEM_SIZED_IGET(4, 32)
#if __WORDSIZE == 64
SHMEM_SIZED_IGET(64, 64)
SHMEM_SIZED_IGET(8, 64)
#else
SHMEM_SIZED_IGET(32, 32)
SHMEM_SIZED_IGET(4, 32)
#endif
/* fake it for now */
SHMEM_SIZED_IGET(128, 128)
