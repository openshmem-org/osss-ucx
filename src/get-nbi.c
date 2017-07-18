#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem/defs.h"
#include "shmemc.h"

#ifdef ENABLE_PSHMEM
extern void shmem_complexf_get_nbi(COMPLEXIFY (float) * dest,
                                   const COMPLEXIFY (float) * src,
                                   size_t nelems, int pe); /* ! API */
extern void shmem_complexd_get_nbi(COMPLEXIFY (double) * dest,
                                   const COMPLEXIFY (double) * src,
                                   size_t nelems, int pe); /* ! API */
#pragma weak shmem_short_get_nbi = pshmem_short_get_nbi
#define shmem_short_get_nbi pshmem_short_get_nbi
#pragma weak shmem_int_get_nbi = pshmem_int_get_nbi
#define shmem_int_get_nbi pshmem_int_get_nbi
#pragma weak shmem_char_get_nbi = pshmem_char_get_nbi
#define shmem_char_get_nbi pshmem_char_get_nbi
#pragma weak shmem_long_get_nbi = pshmem_long_get_nbi
#define shmem_long_get_nbi pshmem_long_get_nbi
#pragma weak shmem_longdouble_get_nbi = pshmem_longdouble_get_nbi
#define shmem_longdouble_get_nbi pshmem_longdouble_get_nbi
#pragma weak shmem_longlong_get_nbi = pshmem_longlong_get_nbi
#define shmem_longlong_get_nbi pshmem_longlong_get_nbi
#pragma weak shmem_double_get_nbi = pshmem_double_get_nbi
#define shmem_double_get_nbi pshmem_double_get_nbi
#pragma weak shmem_float_get_nbi = pshmem_float_get_nbi
#define shmem_float_get_nbi pshmem_float_get_nbi
#pragma weak shmem_complexf_get_nbi = pshmem_complexf_get_nbi
#define shmem_complexf_get_nbi pshmem_complexf_get_nbi
#pragma weak shmem_complexd_get_nbi = pshmem_complexd_get_nbi
#define shmem_complexd_get_nbi pshmem_complexd_get_nbi
#pragma weak shmem_getmem_nbi = pshmem_getmem_nbi
#define shmem_getmem_nbi pshmem_getmem_nbi
#pragma weak shmem_get32_nbi = pshmem_get32_nbi
#define shmem_get32_nbi pshmem_get32_nbi
#pragma weak shmem_get64_nbi = pshmem_get64_nbi
#define shmem_get64_nbi pshmem_get64_nbi
#pragma weak shmem_get128_nbi = pshmem_get128_nbi
#define shmem_get128_nbi pshmem_get128_nbi
/* # pragma weak pshmem_get_nbi = pshmem_long_get_nbi */
/* # pragma weak shmem_get_nbi = pshmem_get_nbi */
#endif /* ENABLE_PSHMEM */


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
    shmem_get##_name##_nbi(void *dest, const void *src,                 \
                           size_t nelems, int pe)                       \
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
