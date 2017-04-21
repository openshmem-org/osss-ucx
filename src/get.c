#include "shmem/defs.h"
#include "shmemc/shmemc.h"

#ifdef ENABLE_PSHMEM
extern void shmem_complexf_get(COMPLEXIFY(float) * dest,
                               const COMPLEXIFY (float) * src,
                               size_t nelems, int pe);  /* ! API */
extern void shmem_complexd_get(COMPLEXIFY(double) * dest,
                               const COMPLEXIFY (double) * src,
                               size_t nelems, int pe);    /* ! API */

#pragma weak shmem_short_get = pshmem_short_get
#define shmem_short_get pshmem_short_get
#pragma weak shmem_int_get = pshmem_int_get
#define shmem_int_get pshmem_int_get
#pragma weak shmem_char_get = pshmem_char_get
#define shmem_char_get pshmem_char_get
#pragma weak shmem_long_get = pshmem_long_get
#define shmem_long_get pshmem_long_get
#pragma weak shmem_longdouble_get = pshmem_longdouble_get
#define shmem_longdouble_get pshmem_longdouble_get
#pragma weak shmem_longlong_get = pshmem_longlong_get
#define shmem_longlong_get pshmem_longlong_get
#pragma weak shmem_double_get = pshmem_double_get
#define shmem_double_get pshmem_double_get
#pragma weak shmem_float_get = pshmem_float_get
#define shmem_float_get pshmem_float_get
#pragma weak shmem_complexf_get = pshmem_complexf_get
#define shmem_complexf_get pshmem_complexf_get
#pragma weak shmem_complexd_get = pshmem_complexd_get
#define shmem_complexd_get pshmem_complexd_get
#pragma weak shmem_getmem = pshmem_getmem
#define shmem_getmem pshmem_getmem
#pragma weak shmem_get32 = pshmem_get32
#define shmem_get32 pshmem_get32
#pragma weak shmem_get64 = pshmem_get64
#define shmem_get64 pshmem_get64
#pragma weak shmem_get128 = pshmem_get128
#define shmem_get128 pshmem_get128
/* # pragma weak pshmem_get = pshmem_long_get */
/* # pragma weak shmem_get = pshmem_get */
#endif /* ENABLE_PSHMEM */

#define SHMEM_TYPED_GET(_name, _type)                            \
    void                                                        \
    shmem_##_name##_get(_type *dest, const _type *src,          \
                        size_t nelems, int pe)                  \
    {                                                           \
        const size_t typed_nelems = nelems * sizeof (_type);    \
        shmemc_get(dest, src, typed_nelems, pe);                \
    }

SHMEM_TYPED_GET(char, char)
SHMEM_TYPED_GET(short, short)
SHMEM_TYPED_GET(int, int)
SHMEM_TYPED_GET(long, long)
SHMEM_TYPED_GET(longdouble, long double)
SHMEM_TYPED_GET(longlong, long long)
SHMEM_TYPED_GET(double, double)
SHMEM_TYPED_GET(float, float)
SHMEM_TYPED_GET(complexf, COMPLEXIFY(float))
SHMEM_TYPED_GET(complexd, COMPLEXIFY(double))

#define SHMEM_SIZED_GET(_name, _size)                   \
    void                                                \
    shmem_get##_name(void *dest, const void *src,       \
                     size_t nelems, int pe)             \
    {                                                   \
        const size_t sized_nelems = nelems * _size;     \
        shmemc_get(dest, src, sized_nelems, pe);        \
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

#define SHMEM_TYPED_G_WRAPPER(_name, _type)                          \
    _type                                                            \
    shmem_##_name##_g(const _type *addr, int pe)                     \
    {                                                                \
        _type val;                                                   \
        shmem_##_name##_get(&val, addr, 1, pe);                      \
        return val;                                                  \
    }

SHMEM_TYPED_G_WRAPPER(char, char)
SHMEM_TYPED_G_WRAPPER(short, short)
SHMEM_TYPED_G_WRAPPER(int, int)
SHMEM_TYPED_G_WRAPPER(long, long)
SHMEM_TYPED_G_WRAPPER(longlong, long long)
SHMEM_TYPED_G_WRAPPER(float, float)
SHMEM_TYPED_G_WRAPPER(double, double)
SHMEM_TYPED_G_WRAPPER(longdouble, long double)
SHMEM_TYPED_G_WRAPPER(complexf, COMPLEXIFY(float))
SHMEM_TYPED_G_WRAPPER(complexd, COMPLEXIFY(double))
