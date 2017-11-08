/* For license: see LICENSE file at top-level */

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


#define SHMEM_CTX_TYPED_GET_NBI(_name, _type)                           \
    void                                                                \
    shmem_ctx_##_name##_get_nbi(shmem_ctx_t ctx,                        \
                                _type *dest, const _type *src,          \
                                size_t nelems, int pe)                  \
    {                                                                   \
        const size_t typed_nelems = nelems * sizeof (_type);            \
        shmemc_ctx_get_nbi(ctx, dest, src, typed_nelems, pe);           \
    }

SHMEM_CTX_TYPED_GET_NBI(float, float)
SHMEM_CTX_TYPED_GET_NBI(double, double)
SHMEM_CTX_TYPED_GET_NBI(longdouble, long double)
SHMEM_CTX_TYPED_GET_NBI(char, char)
SHMEM_CTX_TYPED_GET_NBI(schar, signed char)
SHMEM_CTX_TYPED_GET_NBI(short, short)
SHMEM_CTX_TYPED_GET_NBI(int, int)
SHMEM_CTX_TYPED_GET_NBI(long, long)
SHMEM_CTX_TYPED_GET_NBI(longlong, long long)
SHMEM_CTX_TYPED_GET_NBI(uchar, unsigned char)
SHMEM_CTX_TYPED_GET_NBI(ushort, unsigned short)
SHMEM_CTX_TYPED_GET_NBI(uint, unsigned int)
SHMEM_CTX_TYPED_GET_NBI(ulong, unsigned long)
SHMEM_CTX_TYPED_GET_NBI(ulonglong, unsigned long long)
SHMEM_CTX_TYPED_GET_NBI(int8, int8_t)
SHMEM_CTX_TYPED_GET_NBI(int16, int16_t)
SHMEM_CTX_TYPED_GET_NBI(int32, int32_t)
SHMEM_CTX_TYPED_GET_NBI(int64, int64_t)
SHMEM_CTX_TYPED_GET_NBI(uint8, uint8_t)
SHMEM_CTX_TYPED_GET_NBI(uint16, uint16_t)
SHMEM_CTX_TYPED_GET_NBI(uint32, uint32_t)
SHMEM_CTX_TYPED_GET_NBI(uint64, uint64_t)
SHMEM_CTX_TYPED_GET_NBI(size, size_t)
SHMEM_CTX_TYPED_GET_NBI(ptrdiff, ptrdiff_t)
/* for Fortran */
SHMEM_CTX_TYPED_GET_NBI(complexf, COMPLEXIFY(float))
SHMEM_CTX_TYPED_GET_NBI(complexd, COMPLEXIFY(double))

#undef SHMEM_CTX_TYPED_GET_NBI

#define SHMEM_CTX_SIZED_GET_NBI(_size)                          \
    void                                                        \
    shmem_ctx_get##_size##_nbi(shmem_ctx_t ctx,                 \
                               void *dest, const void *src,     \
                               size_t nelems, int pe)           \
    {                                                           \
        const size_t sized_nelems = nelems * _size;             \
        shmemc_ctx_get_nbi(ctx, dest, src, sized_nelems, pe);   \
    }

SHMEM_CTX_SIZED_GET_NBI(8)
SHMEM_CTX_SIZED_GET_NBI(16)
SHMEM_CTX_SIZED_GET_NBI(32)
SHMEM_CTX_SIZED_GET_NBI(64)
SHMEM_CTX_SIZED_GET_NBI(128)

#undef SHMEM_CTX_SIZED_GET_NBI

void
shmem_ctx_getmem_nbi(shmem_ctx_t ctx,
                     void *dest, const void *src, size_t nelems, int pe)
{
    shmemc_ctx_get_nbi(ctx, dest, src, nelems, pe);
}
