/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem/defs.h"
#include "shmemc.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_float_get_nbi = pshmem_ctx_float_get_nbi
#define shmem_ctx_float_get_nbi pshmem_ctx_float_get_nbi
#pragma weak shmem_ctx_double_get_nbi = pshmem_ctx_double_get_nbi
#define shmem_ctx_double_get_nbi pshmem_ctx_double_get_nbi
#pragma weak shmem_ctx_longdouble_get_nbi = pshmem_ctx_longdouble_get_nbi
#define shmem_ctx_longdouble_get_nbi pshmem_ctx_longdouble_get_nbi
#pragma weak shmem_ctx_char_get_nbi = pshmem_ctx_char_get_nbi
#define shmem_ctx_char_get_nbi pshmem_ctx_char_get_nbi
#pragma weak shmem_ctx_schar_get_nbi = pshmem_ctx_schar_get_nbi
#define shmem_ctx_schar_get_nbi pshmem_ctx_schar_get_nbi
#pragma weak shmem_ctx_short_get_nbi = pshmem_ctx_short_get_nbi
#define shmem_ctx_short_get_nbi pshmem_ctx_short_get_nbi
#pragma weak shmem_ctx_int_get_nbi = pshmem_ctx_int_get_nbi
#define shmem_ctx_int_get_nbi pshmem_ctx_int_get_nbi
#pragma weak shmem_ctx_long_get_nbi = pshmem_ctx_long_get_nbi
#define shmem_ctx_long_get_nbi pshmem_ctx_long_get_nbi
#pragma weak shmem_ctx_longlong_get_nbi = pshmem_ctx_longlong_get_nbi
#define shmem_ctx_longlong_get_nbi pshmem_ctx_longlong_get_nbi
#pragma weak shmem_ctx_uchar_get_nbi = pshmem_ctx_uchar_get_nbi
#define shmem_ctx_uchar_get_nbi pshmem_ctx_uchar_get_nbi
#pragma weak shmem_ctx_ushort_get_nbi = pshmem_ctx_ushort_get_nbi
#define shmem_ctx_ushort_get_nbi pshmem_ctx_ushort_get_nbi
#pragma weak shmem_ctx_uint_get_nbi = pshmem_ctx_uint_get_nbi
#define shmem_ctx_uint_get_nbi pshmem_ctx_uint_get_nbi
#pragma weak shmem_ctx_ulong_get_nbi = pshmem_ctx_ulong_get_nbi
#define shmem_ctx_ulong_get_nbi pshmem_ctx_ulong_get_nbi
#pragma weak shmem_ctx_ulonglong_get_nbi = pshmem_ctx_ulonglong_get_nbi
#define shmem_ctx_ulonglong_get_nbi pshmem_ctx_ulonglong_get_nbi
#pragma weak shmem_ctx_int8_get_nbi = pshmem_ctx_int8_get_nbi
#define shmem_ctx_int8_get_nbi pshmem_ctx_int8_get_nbi
#pragma weak shmem_ctx_int16_get_nbi = pshmem_ctx_int16_get_nbi
#define shmem_ctx_int16_get_nbi pshmem_ctx_int16_get_nbi
#pragma weak shmem_ctx_int32_get_nbi = pshmem_ctx_int32_get_nbi
#define shmem_ctx_int32_get_nbi pshmem_ctx_int32_get_nbi
#pragma weak shmem_ctx_int64_get_nbi = pshmem_ctx_int64_get_nbi
#define shmem_ctx_int64_get_nbi pshmem_ctx_int64_get_nbi
#pragma weak shmem_ctx_uint8_get_nbi = pshmem_ctx_uint8_get_nbi
#define shmem_ctx_uint8_get_nbi pshmem_ctx_uint8_get_nbi
#pragma weak shmem_ctx_uint16_get_nbi = pshmem_ctx_uint16_get_nbi
#define shmem_ctx_uint16_get_nbi pshmem_ctx_uint16_get_nbi
#pragma weak shmem_ctx_uint32_get_nbi = pshmem_ctx_uint32_get_nbi
#define shmem_ctx_uint32_get_nbi pshmem_ctx_uint32_get_nbi
#pragma weak shmem_ctx_uint64_get_nbi = pshmem_ctx_uint64_get_nbi
#define shmem_ctx_uint64_get_nbi pshmem_ctx_uint64_get_nbi
/* for Fortran */
#pragma weak shmem_ctx_complexf_get_nbi = pshmem_ctx_complexf_get_nbi
#define shmem_ctx_complexf_get_nbi pshmem_ctx_complexf_get_nbi
#pragma weak shmem_ctx_complexd_get_nbi = pshmem_ctx_complexd_get_nbi
#define shmem_ctx_complexd_get_nbi pshmem_ctx_complexd_get_nbi
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

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_get8_nbi = pshmem_ctx_get8_nbi
#define shmem_ctx_get8_nbi pshmem_ctx_get8_nbi
#pragma weak shmem_ctx_get16_nbi = pshmem_ctx_get16_nbi
#define shmem_ctx_get16_nbi pshmem_ctx_get16_nbi
#pragma weak shmem_ctx_get32_nbi = pshmem_ctx_get32_nbi
#define shmem_ctx_get32_nbi pshmem_ctx_get32_nbi
#pragma weak shmem_ctx_get64_nbi = pshmem_ctx_get64_nbi
#define shmem_ctx_get64_nbi pshmem_ctx_get64_nbi
#pragma weak shmem_ctx_get128_nbi = pshmem_ctx_get128_nbi
#define shmem_ctx_get128_nbi pshmem_ctx_get128_nbi
#endif /* ENABLE_PSHMEM */

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

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_getmem_nbi = pshmem_ctx_getmem_nbi
#define shmem_ctx_getmem_nbi pshmem_ctx_getmem_nbi
#endif /* ENABLE_PSHMEM */

void
shmem_ctx_getmem_nbi(shmem_ctx_t ctx,
                     void *dest, const void *src, size_t nelems, int pe)
{
    shmemc_ctx_get_nbi(ctx, dest, src, nelems, pe);
}
