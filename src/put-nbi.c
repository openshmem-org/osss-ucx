/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem/defs.h"
#include "shmemc.h"

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

#define SHMEM_CTX_TYPED_PUT_NBI(_name, _type)                           \
    void                                                                \
    shmem_ctx_##_name##_put_nbi(shmem_ctx_t ctx,                        \
                                _type *dest, const _type *src,          \
                                size_t nelems, int pe)                  \
    {                                                                   \
        const size_t typed_nelems = nelems * sizeof(_type);             \
        shmemc_ctx_put_nbi(ctx, dest, src, typed_nelems, pe);           \
    }


SHMEM_CTX_TYPED_PUT_NBI(float, float)
SHMEM_CTX_TYPED_PUT_NBI(double, double)
SHMEM_CTX_TYPED_PUT_NBI(longdouble, long double)
SHMEM_CTX_TYPED_PUT_NBI(char, char)
SHMEM_CTX_TYPED_PUT_NBI(schar, signed char)
SHMEM_CTX_TYPED_PUT_NBI(short, short)
SHMEM_CTX_TYPED_PUT_NBI(int, int)
SHMEM_CTX_TYPED_PUT_NBI(long, long)
SHMEM_CTX_TYPED_PUT_NBI(longlong, long long)
SHMEM_CTX_TYPED_PUT_NBI(uchar, unsigned char)
SHMEM_CTX_TYPED_PUT_NBI(ushort, unsigned short)
SHMEM_CTX_TYPED_PUT_NBI(uint, unsigned int)
SHMEM_CTX_TYPED_PUT_NBI(ulong, unsigned long)
SHMEM_CTX_TYPED_PUT_NBI(ulonglong, unsigned long long)
SHMEM_CTX_TYPED_PUT_NBI(int8, int8_t)
SHMEM_CTX_TYPED_PUT_NBI(int16, int16_t)
SHMEM_CTX_TYPED_PUT_NBI(int32, int32_t)
SHMEM_CTX_TYPED_PUT_NBI(int64, int64_t)
SHMEM_CTX_TYPED_PUT_NBI(uint8, uint8_t)
SHMEM_CTX_TYPED_PUT_NBI(uint16, uint16_t)
SHMEM_CTX_TYPED_PUT_NBI(uint32, uint32_t)
SHMEM_CTX_TYPED_PUT_NBI(uint64, uint64_t)
SHMEM_CTX_TYPED_PUT_NBI(size, size_t)
SHMEM_CTX_TYPED_PUT_NBI(ptrdiff, ptrdiff_t)
/* for Fortran */
SHMEM_CTX_TYPED_PUT_NBI(complexf, COMPLEXIFY(float))
SHMEM_CTX_TYPED_PUT_NBI(complexd, COMPLEXIFY(double))

#undef SHMEM_CTX_TYPED_PUT_NBI

#define SHMEM_CTX_SIZED_PUT_NBI(_size)                                  \
    void                                                                \
    shmem_ctx_put##_size##_nbi(shmem_ctx_t ctx,                         \
                               void *dest, const void *src,             \
                           size_t nelems, int pe)                       \
    {                                                                   \
        const size_t sized_nelems = nelems * _size;                     \
        shmemc_ctx_put_nbi(ctx, dest, src, sized_nelems, pe);           \
    }

SHMEM_CTX_SIZED_PUT_NBI(8)
SHMEM_CTX_SIZED_PUT_NBI(16)
SHMEM_CTX_SIZED_PUT_NBI(32)
SHMEM_CTX_SIZED_PUT_NBI(64)
SHMEM_CTX_SIZED_PUT_NBI(128)

#undef SHMEM_CTX_SIZED_PUT_NBI

void
shmem_ctx_putmem_nbi(shmem_ctx_t ctx,
                     void *dest, const void *src, size_t nelems, int pe)
{
    shmemc_ctx_put_nbi(ctx, dest, src, nelems, pe);
}
