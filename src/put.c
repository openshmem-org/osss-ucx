#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem/defs.h"
#include "shmemc.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_short_put = pshmem_short_put
#define shmem_short_put pshmem_short_put
#pragma weak shmem_int_put = pshmem_int_put
#define shmem_int_put pshmem_int_put
#pragma weak shmem_char_put = pshmem_char_put
#define shmem_char_put pshmem_char_put
#pragma weak shmem_long_put = pshmem_long_put
#define shmem_long_put pshmem_long_put
#pragma weak shmem_longdouble_put = pshmem_longdouble_put
#define shmem_longdouble_put pshmem_longdouble_put
#pragma weak shmem_longlong_put = pshmem_longlong_put
#define shmem_longlong_put pshmem_longlong_put
#pragma weak shmem_double_put = pshmem_double_put
#define shmem_double_put pshmem_double_put
#pragma weak shmem_float_put = pshmem_float_put
#define shmem_float_put pshmem_float_put
#pragma weak shmem_complexf_put = pshmem_complexf_put
#define shmem_complexf_put pshmem_complexf_put
#pragma weak shmem_complexd_put = pshmem_complexd_put
#define shmem_complexd_put pshmem_complexd_put
#pragma weak shmem_putmem = pshmem_putmem
#define shmem_putmem pshmem_putmem
#pragma weak shmem_put32 = pshmem_put32
#define shmem_put32 pshmem_put32
#pragma weak shmem_put64 = pshmem_put64
#define shmem_put64 pshmem_put64
#pragma weak shmem_put128 = pshmem_put128
#define shmem_put128 pshmem_put128
/* # pragma weak pshmem_put = pshmem_long_put */
/* # pragma weak shmem_put = pshmem_put */
#endif /* ENABLE_PSHMEM */


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
/* for Fortran */
SHMEM_TYPED_PUT(complexf, COMPLEXIFY(float))
SHMEM_TYPED_PUT(complexd, COMPLEXIFY(double))

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

#define SHMEM_TYPED_P_WRAPPER(_name, _type)                          \
    void                                                             \
    shmem_##_name##_p(_type *addr, _type val, int pe)                \
    {                                                                \
        shmemc_put(addr, &val, sizeof(val), pe);                     \
    }

SHMEM_TYPED_P_WRAPPER(char, char)
SHMEM_TYPED_P_WRAPPER(short, short)
SHMEM_TYPED_P_WRAPPER(int, int)
SHMEM_TYPED_P_WRAPPER(long, long)
SHMEM_TYPED_P_WRAPPER(longlong, long long)
SHMEM_TYPED_P_WRAPPER(float, float)
SHMEM_TYPED_P_WRAPPER(double, double)
SHMEM_TYPED_P_WRAPPER(longdouble, long double)
/* for Fortran */
SHMEM_TYPED_P_WRAPPER(complexf, COMPLEXIFY(float))
SHMEM_TYPED_P_WRAPPER(complexd, COMPLEXIFY(double))
