#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem/defs.h"
#include "shmemc/shmemc.h"

#ifdef ENABLE_PSHMEM
extern void shmem_complexf_put_nbi(COMPLEXIFY(float) * dest,
                                   const COMPLEXIFY(float) * src,
                                   size_t nelems, int pe); /* ! API */
extern void shmem_complexd_put_nbi(COMPLEXIFY(double) * dest,
                                   const COMPLEXIFY(double) * src,
                                   size_t nelems, int pe); /* ! API */
#pragma weak shmem_short_put_nbi = pshmem_short_put_nbi
#define shmem_short_put_nbi pshmem_short_put_nbi
#pragma weak shmem_int_put_nbi = pshmem_int_put_nbi
#define shmem_int_put_nbi pshmem_int_put_nbi
#pragma weak shmem_char_put_nbi = pshmem_char_put_nbi
#define shmem_char_put_nbi pshmem_char_put_nbi
#pragma weak shmem_long_put_nbi = pshmem_long_put_nbi
#define shmem_long_put_nbi pshmem_long_put_nbi
#pragma weak shmem_longdouble_put_nbi = pshmem_longdouble_put_nbi
#define shmem_longdouble_put_nbi pshmem_longdouble_put_nbi
#pragma weak shmem_longlong_put_nbi = pshmem_longlong_put_nbi
#define shmem_longlong_put_nbi pshmem_longlong_put_nbi
#pragma weak shmem_double_put_nbi = pshmem_double_put_nbi
#define shmem_double_put_nbi pshmem_double_put_nbi
#pragma weak shmem_float_put_nbi = pshmem_float_put_nbi
#define shmem_float_put_nbi pshmem_float_put_nbi
#pragma weak shmem_complexf_put_nbi = pshmem_complexf_put_nbi
#define shmem_complexf_put_nbi pshmem_complexf_put_nbi
#pragma weak shmem_complexd_put_nbi = pshmem_complexd_put_nbi
#define shmem_complexd_put_nbi pshmem_complexd_put_nbi
#pragma weak shmem_putmem_nbi = pshmem_putmem_nbi
#define shmem_putmem_nbi pshmem_putmem_nbi
#pragma weak shmem_put32_nbi = pshmem_put32_nbi
#define shmem_put32_nbi pshmem_put32_nbi
#pragma weak shmem_put64_nbi = pshmem_put64_nbi
#define shmem_put64_nbi pshmem_put64_nbi
#pragma weak shmem_put128_nbi = pshmem_put128_nbi
#define shmem_put128_nbi pshmem_put128_nbi
/* # pragma weak pshmem_put_nbi = pshmem_long_put_nbi */
/* # pragma weak shmem_put_nbi = pshmem_put_nbi */
#endif /* ENABLE_PSHMEM */

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
    shmem_put##_name##_nbi(void *dest, const void *src,                 \
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
