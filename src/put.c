/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"

#include "shmem/defs.h"

#if 0
extern void shmem_ctx_complexf_put(COMPLEXIFY(float) * dest,
                               const COMPLEXIFY(float) * src,
                               size_t nelems, int pe);  /* ! API */
extern void shmem_ctx_complexd_put(COMPLEXIFY(double) * dest,
                               const COMPLEXIFY(double) * src,
                               size_t nelems, int pe);    /* ! API */
#endif

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_float_put = pshmem_ctx_float_put
#define shmem_ctx_float_put pshmem_ctx_float_put
#pragma weak shmem_ctx_double_put = pshmem_ctx_double_put
#define shmem_ctx_double_put pshmem_ctx_double_put
#pragma weak shmem_ctx_longdouble_put = pshmem_ctx_longdouble_put
#define shmem_ctx_longdouble_put pshmem_ctx_longdouble_put
#pragma weak shmem_ctx_char_put = pshmem_ctx_char_put
#define shmem_ctx_char_put pshmem_ctx_char_put
#pragma weak shmem_ctx_schar_put = pshmem_ctx_schar_put
#define shmem_ctx_schar_put pshmem_ctx_schar_put
#pragma weak shmem_ctx_short_put = pshmem_ctx_short_put
#define shmem_ctx_short_put pshmem_ctx_short_put
#pragma weak shmem_ctx_int_put = pshmem_ctx_int_put
#define shmem_ctx_int_put pshmem_ctx_int_put
#pragma weak shmem_ctx_long_put = pshmem_ctx_long_put
#define shmem_ctx_long_put pshmem_ctx_long_put
#pragma weak shmem_ctx_longlong_put = pshmem_ctx_longlong_put
#define shmem_ctx_longlong_put pshmem_ctx_longlong_put
#pragma weak shmem_ctx_uchar_put = pshmem_ctx_uchar_put
#define shmem_ctx_uchar_put pshmem_ctx_uchar_put
#pragma weak shmem_ctx_ushort_put = pshmem_ctx_ushort_put
#define shmem_ctx_ushort_put pshmem_ctx_ushort_put
#pragma weak shmem_ctx_uint_put = pshmem_ctx_uint_put
#define shmem_ctx_uint_put pshmem_ctx_uint_put
#pragma weak shmem_ctx_ulong_put = pshmem_ctx_ulong_put
#define shmem_ctx_ulong_put pshmem_ctx_ulong_put
#pragma weak shmem_ctx_ulonglong_put = pshmem_ctx_ulonglong_put
#define shmem_ctx_ulonglong_put pshmem_ctx_ulonglong_put
#pragma weak shmem_ctx_int8_put = pshmem_ctx_int8_put
#define shmem_ctx_int8_put pshmem_ctx_int8_put
#pragma weak shmem_ctx_int16_put = pshmem_ctx_int16_put
#define shmem_ctx_int16_put pshmem_ctx_int16_put
#pragma weak shmem_ctx_int32_put = pshmem_ctx_int32_put
#define shmem_ctx_int32_put pshmem_ctx_int32_put
#pragma weak shmem_ctx_int64_put = pshmem_ctx_int64_put
#define shmem_ctx_int64_put pshmem_ctx_int64_put
#pragma weak shmem_ctx_uint8_put = pshmem_ctx_uint8_put
#define shmem_ctx_uint8_put pshmem_ctx_uint8_put
#pragma weak shmem_ctx_uint16_put = pshmem_ctx_uint16_put
#define shmem_ctx_uint16_put pshmem_ctx_uint16_put
#pragma weak shmem_ctx_uint32_put = pshmem_ctx_uint32_put
#define shmem_ctx_uint32_put pshmem_ctx_uint32_put
#pragma weak shmem_ctx_uint64_put = pshmem_ctx_uint64_put
#define shmem_ctx_uint64_put pshmem_ctx_uint64_put

#pragma weak shmem_ctx_put8 = pshmem_ctx_put8
#define shmem_ctx_put8 pshmem_ctx_put8
#pragma weak shmem_ctx_put16 = pshmem_ctx_put16
#define shmem_ctx_put16 pshmem_ctx_put16
#pragma weak shmem_ctx_put32 = pshmem_ctx_put32
#define shmem_ctx_put32 pshmem_ctx_put32
#pragma weak shmem_ctx_put64 = pshmem_ctx_put64
#define shmem_ctx_put64 pshmem_ctx_put64
#pragma weak shmem_ctx_put128 = pshmem_ctx_put128
#define shmem_ctx_put128 pshmem_ctx_put128

#pragma weak shmem_ctx_putmem = pshmem_ctx_putmem
#define shmem_ctx_putmem pshmem_ctx_putmem

/* for Fortran */
#pragma weak shmem_ctx_complexf_put = pshmem_ctx_complexf_put
#define shmem_ctx_complexf_put pshmem_ctx_complexf_put
#pragma weak shmem_ctx_complexd_put = pshmem_ctx_complexd_put
#define shmem_ctx_complexd_put pshmem_ctx_complexd_put

#endif /* ENABLE_PSHMEM */

#define SHMEM_CTX_TYPED_PUT(_name, _type)                               \
    void                                                                \
    shmem_ctx_##_name##_put(shmem_ctx_t ctx,                            \
                            _type *dest, const _type *src,              \
                            size_t nelems, int pe)                      \
    {                                                                   \
        const size_t sized_nelems = nelems * sizeof(_type);             \
        shmemc_ctx_put(ctx, dest, src, sized_nelems, pe);               \
    }                                                                   \
    void                                                                \
    shmem_##_name##_put(_type *dest, const _type *src,                  \
                        size_t nelems, int pe)                          \
    {                                                                   \
        const size_t sized_nelems = nelems * sizeof(_type);             \
        shmemc_ctx_put(SHMEM_CTX_DEFAULT,                               \
                       dest, src, sized_nelems, pe);                    \
    }

SHMEM_CTX_TYPED_PUT(float, float)
SHMEM_CTX_TYPED_PUT(double, double)
SHMEM_CTX_TYPED_PUT(longdouble, long double)
SHMEM_CTX_TYPED_PUT(char, char)
SHMEM_CTX_TYPED_PUT(schar, signed char)
SHMEM_CTX_TYPED_PUT(short, short)
SHMEM_CTX_TYPED_PUT(int, int)
SHMEM_CTX_TYPED_PUT(long, long)
SHMEM_CTX_TYPED_PUT(longlong, long long)
SHMEM_CTX_TYPED_PUT(uchar, unsigned char)
SHMEM_CTX_TYPED_PUT(ushort, unsigned short)
SHMEM_CTX_TYPED_PUT(uint, unsigned int)
SHMEM_CTX_TYPED_PUT(ulong, unsigned long)
SHMEM_CTX_TYPED_PUT(ulonglong, unsigned long long)
SHMEM_CTX_TYPED_PUT(int8, int8_t)
SHMEM_CTX_TYPED_PUT(int16, int16_t)
SHMEM_CTX_TYPED_PUT(int32, int32_t)
SHMEM_CTX_TYPED_PUT(int64, int64_t)
SHMEM_CTX_TYPED_PUT(uint8, uint8_t)
SHMEM_CTX_TYPED_PUT(uint16, uint16_t)
SHMEM_CTX_TYPED_PUT(uint32, uint32_t)
SHMEM_CTX_TYPED_PUT(uint64, uint64_t)
SHMEM_CTX_TYPED_PUT(size, size_t)
SHMEM_CTX_TYPED_PUT(ptrdiff, ptrdiff_t)
/* for Fortran */
SHMEM_CTX_TYPED_PUT(complexf, COMPLEXIFY(float))
SHMEM_CTX_TYPED_PUT(complexd, COMPLEXIFY(double))

#define SHMEM_CTX_SIZED_PUT(_size)                                      \
    void                                                                \
    shmem_ctx_put##_size(shmem_ctx_t ctx,                               \
                         void *dest, const void *src,                   \
                         size_t nelems, int pe)                         \
    {                                                                   \
        const size_t sized_nelems = nelems * _size;                     \
        shmemc_ctx_put(ctx, dest, src, sized_nelems, pe);               \
    }

SHMEM_CTX_SIZED_PUT(8)
SHMEM_CTX_SIZED_PUT(16)
SHMEM_CTX_SIZED_PUT(32)
SHMEM_CTX_SIZED_PUT(64)
SHMEM_CTX_SIZED_PUT(128)

#undef SHMEM_CTX_SIZED_PUT

void
shmem_ctx_putmem(shmem_ctx_t ctx,
                 void *dest, const void *src,
                 size_t nelems, int pe)
{
    shmemc_ctx_put(ctx, dest, src, nelems, pe);
}

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_float_p = pshmem_ctx_float_p
#define shmem_ctx_float_p pshmem_ctx_float_p
#pragma weak shmem_ctx_double_p = pshmem_ctx_double_p
#define shmem_ctx_double_p pshmem_ctx_double_p
#pragma weak shmem_ctx_longdouble_p = pshmem_ctx_longdouble_p
#define shmem_ctx_longdouble_p pshmem_ctx_longdouble_p
#pragma weak shmem_ctx_char_p = pshmem_ctx_char_p
#define shmem_ctx_char_p pshmem_ctx_char_p
#pragma weak shmem_ctx_schar_p = pshmem_ctx_schar_p
#define shmem_ctx_schar_p pshmem_ctx_schar_p
#pragma weak shmem_ctx_short_p = pshmem_ctx_short_p
#define shmem_ctx_short_p pshmem_ctx_short_p
#pragma weak shmem_ctx_int_p = pshmem_ctx_int_p
#define shmem_ctx_int_p pshmem_ctx_int_p
#pragma weak shmem_ctx_long_p = pshmem_ctx_long_p
#define shmem_ctx_long_p pshmem_ctx_long_p
#pragma weak shmem_ctx_longlong_p = pshmem_ctx_longlong_p
#define shmem_ctx_longlong_p pshmem_ctx_longlong_p
#pragma weak shmem_ctx_uchar_p = pshmem_ctx_uchar_p
#define shmem_ctx_uchar_p pshmem_ctx_uchar_p
#pragma weak shmem_ctx_ushort_p = pshmem_ctx_ushort_p
#define shmem_ctx_ushort_p pshmem_ctx_ushort_p
#pragma weak shmem_ctx_uint_p = pshmem_ctx_uint_p
#define shmem_ctx_uint_p pshmem_ctx_uint_p
#pragma weak shmem_ctx_ulong_p = pshmem_ctx_ulong_p
#define shmem_ctx_ulong_p pshmem_ctx_ulong_p
#pragma weak shmem_ctx_ulonglong_p = pshmem_ctx_ulonglong_p
#define shmem_ctx_ulonglong_p pshmem_ctx_ulonglong_p
#pragma weak shmem_ctx_int8_p = pshmem_ctx_int8_p
#define shmem_ctx_int8_p pshmem_ctx_int8_p
#pragma weak shmem_ctx_int16_p = pshmem_ctx_int16_p
#define shmem_ctx_int16_p pshmem_ctx_int16_p
#pragma weak shmem_ctx_int32_p = pshmem_ctx_int32_p
#define shmem_ctx_int32_p pshmem_ctx_int32_p
#pragma weak shmem_ctx_int64_p = pshmem_ctx_int64_p
#define shmem_ctx_int64_p pshmem_ctx_int64_p
#pragma weak shmem_ctx_uint8_p = pshmem_ctx_uint8_p
#define shmem_ctx_uint8_p pshmem_ctx_uint8_p
#pragma weak shmem_ctx_uint16_p = pshmem_ctx_uint16_p
#define shmem_ctx_uint16_p pshmem_ctx_uint16_p
#pragma weak shmem_ctx_uint32_p = pshmem_ctx_uint32_p
#define shmem_ctx_uint32_p pshmem_ctx_uint32_p
#pragma weak shmem_ctx_uint64_p = pshmem_ctx_uint64_p
#define shmem_ctx_uint64_p pshmem_ctx_uint64_p
/* for Fortran */
#pragma weak shmem_ctx_complexf_p = pshmem_ctx_complexf_p
#define shmem_ctx_complexf_p pshmem_ctx_complexf_p
#pragma weak shmem_ctx_complexd_p = pshmem_ctx_complexd_p
#define shmem_ctx_complexd_p pshmem_ctx_complexd_p
#endif /* ENABLE_PSHMEM */

#define SHMEM_CTX_TYPED_P(_name, _type)                              \
    void                                                             \
    shmem_ctx_##_name##_p(shmem_ctx_t ctx,                           \
                          _type *addr, _type val, int pe)            \
    {                                                                \
        shmemc_ctx_put(ctx, addr, &val, sizeof(val), pe);            \
    }

SHMEM_CTX_TYPED_P(float, float)
SHMEM_CTX_TYPED_P(double, double)
SHMEM_CTX_TYPED_P(longdouble, long double)
SHMEM_CTX_TYPED_P(char, char)
SHMEM_CTX_TYPED_P(schar, signed char)
SHMEM_CTX_TYPED_P(short, short)
SHMEM_CTX_TYPED_P(int, int)
SHMEM_CTX_TYPED_P(long, long)
SHMEM_CTX_TYPED_P(longlong, long long)
SHMEM_CTX_TYPED_P(uchar, unsigned char)
SHMEM_CTX_TYPED_P(ushort, unsigned short)
SHMEM_CTX_TYPED_P(uint, unsigned int)
SHMEM_CTX_TYPED_P(ulong, unsigned long)
SHMEM_CTX_TYPED_P(ulonglong, unsigned long long)
SHMEM_CTX_TYPED_P(int8, int8_t)
SHMEM_CTX_TYPED_P(int16, int16_t)
SHMEM_CTX_TYPED_P(int32, int32_t)
SHMEM_CTX_TYPED_P(int64, int64_t)
SHMEM_CTX_TYPED_P(uint8, uint8_t)
SHMEM_CTX_TYPED_P(uint16, uint16_t)
SHMEM_CTX_TYPED_P(uint32, uint32_t)
SHMEM_CTX_TYPED_P(uint64, uint64_t)
SHMEM_CTX_TYPED_P(size, size_t)
SHMEM_CTX_TYPED_P(ptrdiff, ptrdiff_t)
/* for Fortran */
SHMEM_CTX_TYPED_P(complexf, COMPLEXIFY(float))
SHMEM_CTX_TYPED_P(complexd, COMPLEXIFY(double))

#undef SHMEM_CTX_TYPED_P
