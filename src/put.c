/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"

#include "shmem/api.h"

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
                                                                        \
        shmemc_ctx_put(ctx, dest, src, sized_nelems, pe);               \
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

#undef SHMEM_CTX_TYPED_PUT

#ifdef ENABLE_PSHMEM
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
#endif /* ENABLE_PSHMEM */

#define SHMEM_CTX_SIZED_PUT(_size)                                      \
    void                                                                \
    shmem_ctx_put##_size(shmem_ctx_t ctx,                               \
                         void *dest, const void *src,                   \
                         size_t nelems, int pe)                         \
    {                                                                   \
        const size_t sized_nelems = nelems * _size;                     \
                                                                        \
        shmemc_ctx_put(ctx, dest, src, sized_nelems, pe);               \
    }

SHMEM_CTX_SIZED_PUT(8)
SHMEM_CTX_SIZED_PUT(16)
SHMEM_CTX_SIZED_PUT(32)
SHMEM_CTX_SIZED_PUT(64)
SHMEM_CTX_SIZED_PUT(128)

#undef SHMEM_CTX_SIZED_PUT

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_putmem = pshmem_ctx_putmem
#define shmem_ctx_putmem pshmem_ctx_putmem
#endif /* ENABLE_PSHMEM */

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

/*
 * TODO: this is just layered over shmem_put/get linearly for now.
 * Looking for better iov method in UCX
 */

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_float_iput = pshmem_ctx_float_iput
#define shmem_ctx_float_iput pshmem_ctx_float_iput
#pragma weak shmem_ctx_double_iput = pshmem_ctx_double_iput
#define shmem_ctx_double_iput pshmem_ctx_double_iput
#pragma weak shmem_ctx_longdouble_iput = pshmem_ctx_longdouble_iput
#define shmem_ctx_longdouble_iput pshmem_ctx_longdouble_iput
#pragma weak shmem_ctx_char_iput = pshmem_ctx_char_iput
#define shmem_ctx_char_iput pshmem_ctx_char_iput
#pragma weak shmem_ctx_schar_iput = pshmem_ctx_schar_iput
#define shmem_ctx_schar_iput pshmem_ctx_schar_iput
#pragma weak shmem_ctx_short_iput = pshmem_ctx_short_iput
#define shmem_ctx_short_iput pshmem_ctx_short_iput
#pragma weak shmem_ctx_int_iput = pshmem_ctx_int_iput
#define shmem_ctx_int_iput pshmem_ctx_int_iput
#pragma weak shmem_ctx_long_iput = pshmem_ctx_long_iput
#define shmem_ctx_long_iput pshmem_ctx_long_iput
#pragma weak shmem_ctx_longlong_iput = pshmem_ctx_longlong_iput
#define shmem_ctx_longlong_iput pshmem_ctx_longlong_iput
#pragma weak shmem_ctx_uchar_iput = pshmem_ctx_uchar_iput
#define shmem_ctx_uchar_iput pshmem_ctx_uchar_iput
#pragma weak shmem_ctx_ushort_iput = pshmem_ctx_ushort_iput
#define shmem_ctx_ushort_iput pshmem_ctx_ushort_iput
#pragma weak shmem_ctx_uint_iput = pshmem_ctx_uint_iput
#define shmem_ctx_uint_iput pshmem_ctx_uint_iput
#pragma weak shmem_ctx_ulong_iput = pshmem_ctx_ulong_iput
#define shmem_ctx_ulong_iput pshmem_ctx_ulong_iput
#pragma weak shmem_ctx_ulonglong_iput = pshmem_ctx_ulonglong_iput
#define shmem_ctx_ulonglong_iput pshmem_ctx_ulonglong_iput
#pragma weak shmem_ctx_int8_iput = pshmem_ctx_int8_iput
#define shmem_ctx_int8_iput pshmem_ctx_int8_iput
#pragma weak shmem_ctx_int16_iput = pshmem_ctx_int16_iput
#define shmem_ctx_int16_iput pshmem_ctx_int16_iput
#pragma weak shmem_ctx_int32_iput = pshmem_ctx_int32_iput
#define shmem_ctx_int32_iput pshmem_ctx_int32_iput
#pragma weak shmem_ctx_int64_iput = pshmem_ctx_int64_iput
#define shmem_ctx_int64_iput pshmem_ctx_int64_iput
#pragma weak shmem_ctx_uint8_iput = pshmem_ctx_uint8_iput
#define shmem_ctx_uint8_iput pshmem_ctx_uint8_iput
#pragma weak shmem_ctx_uint16_iput = pshmem_ctx_uint16_iput
#define shmem_ctx_uint16_iput pshmem_ctx_uint16_iput
#pragma weak shmem_ctx_uint32_iput = pshmem_ctx_uint32_iput
#define shmem_ctx_uint32_iput pshmem_ctx_uint32_iput
#pragma weak shmem_ctx_uint64_iput = pshmem_ctx_uint64_iput
#define shmem_ctx_uint64_iput pshmem_ctx_uint64_iput
#endif /* ENABLE_PSHMEM */

#define SHMEM_CTX_TYPED_IPUT(_name, _type)                              \
    void                                                                \
    shmem_ctx_##_name##_iput(shmem_ctx_t ctx,                           \
                             _type *target, const _type *source,        \
                             ptrdiff_t tst, ptrdiff_t sst,              \
                             size_t nelems, int pe)                     \
    {                                                                   \
        const size_t the_size = sizeof(_type);                          \
        size_t ti = 0, si = 0;                                          \
        size_t i;                                                       \
                                                                        \
        for (i = 0; i < nelems; i += 1) {                               \
            shmemc_ctx_put(ctx,                                         \
                           &((char *)target)[ti],                       \
                           &((char *)source)[si],                       \
                           the_size, pe);                               \
            ti += tst * the_size;                                       \
            si += sst * the_size;                                       \
        }                                                               \
    }

SHMEM_CTX_TYPED_IPUT(float, float)
SHMEM_CTX_TYPED_IPUT(double, double)
SHMEM_CTX_TYPED_IPUT(longdouble, long double)
SHMEM_CTX_TYPED_IPUT(char, char)
SHMEM_CTX_TYPED_IPUT(schar, signed char)
SHMEM_CTX_TYPED_IPUT(short, short)
SHMEM_CTX_TYPED_IPUT(int, int)
SHMEM_CTX_TYPED_IPUT(long, long)
SHMEM_CTX_TYPED_IPUT(longlong, long long)
SHMEM_CTX_TYPED_IPUT(uchar, unsigned char)
SHMEM_CTX_TYPED_IPUT(ushort, unsigned short)
SHMEM_CTX_TYPED_IPUT(uint, unsigned int)
SHMEM_CTX_TYPED_IPUT(ulong, unsigned long)
SHMEM_CTX_TYPED_IPUT(ulonglong, unsigned long long)
SHMEM_CTX_TYPED_IPUT(int8, int8_t)
SHMEM_CTX_TYPED_IPUT(int16, int16_t)
SHMEM_CTX_TYPED_IPUT(int32, int32_t)
SHMEM_CTX_TYPED_IPUT(int64, int64_t)
SHMEM_CTX_TYPED_IPUT(uint8, uint8_t)
SHMEM_CTX_TYPED_IPUT(uint16, uint16_t)
SHMEM_CTX_TYPED_IPUT(uint32, uint32_t)
SHMEM_CTX_TYPED_IPUT(uint64, uint64_t)
SHMEM_CTX_TYPED_IPUT(size, size_t)
SHMEM_CTX_TYPED_IPUT(ptrdiff, ptrdiff_t)
/* for Fortran */
SHMEM_CTX_TYPED_IPUT(complexf, COMPLEXIFY(float))
SHMEM_CTX_TYPED_IPUT(complexd, COMPLEXIFY(double))

#undef SHMEM_CTX_TYPED_IPUT

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_iput8 = pshmem_ctx_iput8
#define shmem_ctx_iput8 pshmem_ctx_iput8
#pragma weak shmem_ctx_iput16 = pshmem_ctx_iput16
#define shmem_ctx_iput16 pshmem_ctx_iput16
#pragma weak shmem_ctx_iput32 = pshmem_ctx_iput32
#define shmem_ctx_iput32 pshmem_ctx_iput32
#pragma weak shmem_ctx_iput64 = pshmem_ctx_iput64
#define shmem_ctx_iput64 pshmem_ctx_iput64
#pragma weak shmem_ctx_iput128 = pshmem_ctx_iput128
#define shmem_ctx_iput128 pshmem_ctx_iput128
#endif /* ENABLE_PSHMEM */

#define SHMEM_CTX_SIZED_IPUT(_size)                                     \
    void                                                                \
    shmem_ctx_iput##_size(shmem_ctx_t ctx,                              \
                          void *target, const void *source,             \
                          ptrdiff_t tst, ptrdiff_t sst,                 \
                          size_t nelems, int pe)                        \
    {                                                                   \
        size_t ti = 0, si = 0;                                          \
        size_t i;                                                       \
                                                                        \
        for (i = 0; i < nelems; i += 1) {                               \
            shmemc_ctx_put(ctx,                                         \
                           &((char *)target)[ti],                       \
                           &((char *)source)[si],                       \
                           _size, pe);                                  \
            ti += tst * _size;                                          \
            si += sst * _size;                                          \
        }                                                               \
    }

SHMEM_CTX_SIZED_IPUT(8)
SHMEM_CTX_SIZED_IPUT(16)
SHMEM_CTX_SIZED_IPUT(32)
SHMEM_CTX_SIZED_IPUT(64)
SHMEM_CTX_SIZED_IPUT(128)

#undef SHMEM_CTX_SIZED_IPUT

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_float_put_nbi = pshmem_ctx_float_put_nbi
#define shmem_ctx_float_put_nbi pshmem_ctx_float_put_nbi
#pragma weak shmem_ctx_double_put_nbi = pshmem_ctx_double_put_nbi
#define shmem_ctx_double_put_nbi pshmem_ctx_double_put_nbi
#pragma weak shmem_ctx_longdouble_put_nbi = pshmem_ctx_longdouble_put_nbi
#define shmem_ctx_longdouble_put_nbi pshmem_ctx_longdouble_put_nbi
#pragma weak shmem_ctx_char_put_nbi = pshmem_ctx_char_put_nbi
#define shmem_ctx_char_put_nbi pshmem_ctx_char_put_nbi
#pragma weak shmem_ctx_schar_put_nbi = pshmem_ctx_schar_put_nbi
#define shmem_ctx_schar_put_nbi pshmem_ctx_schar_put_nbi
#pragma weak shmem_ctx_short_put_nbi = pshmem_ctx_short_put_nbi
#define shmem_ctx_short_put_nbi pshmem_ctx_short_put_nbi
#pragma weak shmem_ctx_int_put_nbi = pshmem_ctx_int_put_nbi
#define shmem_ctx_int_put_nbi pshmem_ctx_int_put_nbi
#pragma weak shmem_ctx_long_put_nbi = pshmem_ctx_long_put_nbi
#define shmem_ctx_long_put_nbi pshmem_ctx_long_put_nbi
#pragma weak shmem_ctx_longlong_put_nbi = pshmem_ctx_longlong_put_nbi
#define shmem_ctx_longlong_put_nbi pshmem_ctx_longlong_put_nbi
#pragma weak shmem_ctx_uchar_put_nbi = pshmem_ctx_uchar_put_nbi
#define shmem_ctx_uchar_put_nbi pshmem_ctx_uchar_put_nbi
#pragma weak shmem_ctx_ushort_put_nbi = pshmem_ctx_ushort_put_nbi
#define shmem_ctx_ushort_put_nbi pshmem_ctx_ushort_put_nbi
#pragma weak shmem_ctx_uint_put_nbi = pshmem_ctx_uint_put_nbi
#define shmem_ctx_uint_put_nbi pshmem_ctx_uint_put_nbi
#pragma weak shmem_ctx_ulong_put_nbi = pshmem_ctx_ulong_put_nbi
#define shmem_ctx_ulong_put_nbi pshmem_ctx_ulong_put_nbi
#pragma weak shmem_ctx_ulonglong_put_nbi = pshmem_ctx_ulonglong_put_nbi
#define shmem_ctx_ulonglong_put_nbi pshmem_ctx_ulonglong_put_nbi
#pragma weak shmem_ctx_int8_put_nbi = pshmem_ctx_int8_put_nbi
#define shmem_ctx_int8_put_nbi pshmem_ctx_int8_put_nbi
#pragma weak shmem_ctx_int16_put_nbi = pshmem_ctx_int16_put_nbi
#define shmem_ctx_int16_put_nbi pshmem_ctx_int16_put_nbi
#pragma weak shmem_ctx_int32_put_nbi = pshmem_ctx_int32_put_nbi
#define shmem_ctx_int32_put_nbi pshmem_ctx_int32_put_nbi
#pragma weak shmem_ctx_int64_put_nbi = pshmem_ctx_int64_put_nbi
#define shmem_ctx_int64_put_nbi pshmem_ctx_int64_put_nbi
#pragma weak shmem_ctx_uint8_put_nbi = pshmem_ctx_uint8_put_nbi
#define shmem_ctx_uint8_put_nbi pshmem_ctx_uint8_put_nbi
#pragma weak shmem_ctx_uint16_put_nbi = pshmem_ctx_uint16_put_nbi
#define shmem_ctx_uint16_put_nbi pshmem_ctx_uint16_put_nbi
#pragma weak shmem_ctx_uint32_put_nbi = pshmem_ctx_uint32_put_nbi
#define shmem_ctx_uint32_put_nbi pshmem_ctx_uint32_put_nbi
#pragma weak shmem_ctx_uint64_put_nbi = pshmem_ctx_uint64_put_nbi
#define shmem_ctx_uint64_put_nbi pshmem_ctx_uint64_put_nbi
/* for Fortran */
#pragma weak shmem_ctx_complexf_put_nbi = pshmem_ctx_complexf_put_nbi
#define shmem_ctx_complexf_put_nbi pshmem_ctx_complexf_put_nbi
#pragma weak shmem_ctx_complexd_put_nbi = pshmem_ctx_complexd_put_nbi
#define shmem_ctx_complexd_put_nbi pshmem_ctx_complexd_put_nbi
#endif /* ENABLE_PSHMEM */

#define SHMEM_CTX_TYPED_PUT_NBI(_name, _type)                           \
    void                                                                \
    shmem_ctx_##_name##_put_nbi(shmem_ctx_t ctx,                        \
                                _type *dest, const _type *src,          \
                                size_t nelems, int pe)                  \
    {                                                                   \
        const size_t typed_nelems = nelems * sizeof(_type);             \
                                                                        \
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

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_put8_nbi = pshmem_ctx_put8_nbi
#define shmem_ctx_put8_nbi pshmem_ctx_put8_nbi
#pragma weak shmem_ctx_put16_nbi = pshmem_ctx_put16_nbi
#define shmem_ctx_put16_nbi pshmem_ctx_put16_nbi
#pragma weak shmem_ctx_put32_nbi = pshmem_ctx_put32_nbi
#define shmem_ctx_put32_nbi pshmem_ctx_put32_nbi
#pragma weak shmem_ctx_put64_nbi = pshmem_ctx_put64_nbi
#define shmem_ctx_put64_nbi pshmem_ctx_put64_nbi
#pragma weak shmem_ctx_put128_nbi = pshmem_ctx_put128_nbi
#define shmem_ctx_put128_nbi pshmem_ctx_put128_nbi
#endif /* ENABLE_PSHMEM */

#define SHMEM_CTX_SIZED_PUT_NBI(_size)                                  \
    void                                                                \
    shmem_ctx_put##_size##_nbi(shmem_ctx_t ctx,                         \
                               void *dest, const void *src,             \
                           size_t nelems, int pe)                       \
    {                                                                   \
        const size_t sized_nelems = nelems * _size;                     \
                                                                        \
        shmemc_ctx_put_nbi(ctx, dest, src, sized_nelems, pe);           \
    }

SHMEM_CTX_SIZED_PUT_NBI(8)
SHMEM_CTX_SIZED_PUT_NBI(16)
SHMEM_CTX_SIZED_PUT_NBI(32)
SHMEM_CTX_SIZED_PUT_NBI(64)
SHMEM_CTX_SIZED_PUT_NBI(128)

#undef SHMEM_CTX_SIZED_PUT_NBI

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_putmem_nbi = pshmem_ctx_putmem_nbi
#define shmem_ctx_putmem_nbi pshmem_ctx_putmem_nbi
#endif /* ENABLE_PSHMEM */

void
shmem_ctx_putmem_nbi(shmem_ctx_t ctx,
                     void *dest, const void *src, size_t nelems, int pe)
{
    shmemc_ctx_put_nbi(ctx, dest, src, nelems, pe);
}


/* ------------------------------------------------------------------------ */

#ifdef ENABLE_PSHMEM
#pragma weak shmem_float_put = pshmem_float_put
#define shmem_float_put pshmem_float_put
#pragma weak shmem_double_put = pshmem_double_put
#define shmem_double_put pshmem_double_put
#pragma weak shmem_longdouble_put = pshmem_longdouble_put
#define shmem_longdouble_put pshmem_longdouble_put
#pragma weak shmem_char_put = pshmem_char_put
#define shmem_char_put pshmem_char_put
#pragma weak shmem_schar_put = pshmem_schar_put
#define shmem_schar_put pshmem_schar_put
#pragma weak shmem_short_put = pshmem_short_put
#define shmem_short_put pshmem_short_put
#pragma weak shmem_int_put = pshmem_int_put
#define shmem_int_put pshmem_int_put
#pragma weak shmem_long_put = pshmem_long_put
#define shmem_long_put pshmem_long_put
#pragma weak shmem_longlong_put = pshmem_longlong_put
#define shmem_longlong_put pshmem_longlong_put
#pragma weak shmem_uchar_put = pshmem_uchar_put
#define shmem_uchar_put pshmem_uchar_put
#pragma weak shmem_ushort_put = pshmem_ushort_put
#define shmem_ushort_put pshmem_ushort_put
#pragma weak shmem_uint_put = pshmem_uint_put
#define shmem_uint_put pshmem_uint_put
#pragma weak shmem_ulong_put = pshmem_ulong_put
#define shmem_ulong_put pshmem_ulong_put
#pragma weak shmem_ulonglong_put = pshmem_ulonglong_put
#define shmem_ulonglong_put pshmem_ulonglong_put
#pragma weak shmem_int8_put = pshmem_int8_put
#define shmem_int8_put pshmem_int8_put
#pragma weak shmem_int16_put = pshmem_int16_put
#define shmem_int16_put pshmem_int16_put
#pragma weak shmem_int32_put = pshmem_int32_put
#define shmem_int32_put pshmem_int32_put
#pragma weak shmem_int64_put = pshmem_int64_put
#define shmem_int64_put pshmem_int64_put
#pragma weak shmem_uint8_put = pshmem_uint8_put
#define shmem_uint8_put pshmem_uint8_put
#pragma weak shmem_uint16_put = pshmem_uint16_put
#define shmem_uint16_put pshmem_uint16_put
#pragma weak shmem_uint32_put = pshmem_uint32_put
#define shmem_uint32_put pshmem_uint32_put
#pragma weak shmem_uint64_put = pshmem_uint64_put
#define shmem_uint64_put pshmem_uint64_put

#pragma weak shmem_float_iput = pshmem_float_iput
#define shmem_float_iput pshmem_float_iput
#pragma weak shmem_double_iput = pshmem_double_iput
#define shmem_double_iput pshmem_double_iput
#pragma weak shmem_longdouble_iput = pshmem_longdouble_iput
#define shmem_longdouble_iput pshmem_longdouble_iput
#pragma weak shmem_char_iput = pshmem_char_iput
#define shmem_char_iput pshmem_char_iput
#pragma weak shmem_schar_iput = pshmem_schar_iput
#define shmem_schar_iput pshmem_schar_iput
#pragma weak shmem_short_iput = pshmem_short_iput
#define shmem_short_iput pshmem_short_iput
#pragma weak shmem_int_iput = pshmem_int_iput
#define shmem_int_iput pshmem_int_iput
#pragma weak shmem_long_iput = pshmem_long_iput
#define shmem_long_iput pshmem_long_iput
#pragma weak shmem_longlong_iput = pshmem_longlong_iput
#define shmem_longlong_iput pshmem_longlong_iput
#pragma weak shmem_uchar_iput = pshmem_uchar_iput
#define shmem_uchar_iput pshmem_uchar_iput
#pragma weak shmem_ushort_iput = pshmem_ushort_iput
#define shmem_ushort_iput pshmem_ushort_iput
#pragma weak shmem_uint_iput = pshmem_uint_iput
#define shmem_uint_iput pshmem_uint_iput
#pragma weak shmem_ulong_iput = pshmem_ulong_iput
#define shmem_ulong_iput pshmem_ulong_iput
#pragma weak shmem_ulonglong_iput = pshmem_ulonglong_iput
#define shmem_ulonglong_iput pshmem_ulonglong_iput
#pragma weak shmem_int8_iput = pshmem_int8_iput
#define shmem_int8_iput pshmem_int8_iput
#pragma weak shmem_int16_iput = pshmem_int16_iput
#define shmem_int16_iput pshmem_int16_iput
#pragma weak shmem_int32_iput = pshmem_int32_iput
#define shmem_int32_iput pshmem_int32_iput
#pragma weak shmem_int64_iput = pshmem_int64_iput
#define shmem_int64_iput pshmem_int64_iput
#pragma weak shmem_uint8_iput = pshmem_uint8_iput
#define shmem_uint8_iput pshmem_uint8_iput
#pragma weak shmem_uint16_iput = pshmem_uint16_iput
#define shmem_uint16_iput pshmem_uint16_iput
#pragma weak shmem_uint32_iput = pshmem_uint32_iput
#define shmem_uint32_iput pshmem_uint32_iput
#pragma weak shmem_uint64_iput = pshmem_uint64_iput
#define shmem_uint64_iput pshmem_uint64_iput

#pragma weak shmem_float_put_nbi = pshmem_float_put_nbi
#define shmem_float_put_nbi pshmem_float_put_nbi
#pragma weak shmem_double_put_nbi = pshmem_double_put_nbi
#define shmem_double_put_nbi pshmem_double_put_nbi
#pragma weak shmem_longdouble_put_nbi = pshmem_longdouble_put_nbi
#define shmem_longdouble_put_nbi pshmem_longdouble_put_nbi
#pragma weak shmem_char_put_nbi = pshmem_char_put_nbi
#define shmem_char_put_nbi pshmem_char_put_nbi
#pragma weak shmem_schar_put_nbi = pshmem_schar_put_nbi
#define shmem_schar_put_nbi pshmem_schar_put_nbi
#pragma weak shmem_short_put_nbi = pshmem_short_put_nbi
#define shmem_short_put_nbi pshmem_short_put_nbi
#pragma weak shmem_int_put_nbi = pshmem_int_put_nbi
#define shmem_int_put_nbi pshmem_int_put_nbi
#pragma weak shmem_long_put_nbi = pshmem_long_put_nbi
#define shmem_long_put_nbi pshmem_long_put_nbi
#pragma weak shmem_longlong_put_nbi = pshmem_longlong_put_nbi
#define shmem_longlong_put_nbi pshmem_longlong_put_nbi
#pragma weak shmem_uchar_put_nbi = pshmem_uchar_put_nbi
#define shmem_uchar_put_nbi pshmem_uchar_put_nbi
#pragma weak shmem_ushort_put_nbi = pshmem_ushort_put_nbi
#define shmem_ushort_put_nbi pshmem_ushort_put_nbi
#pragma weak shmem_uint_put_nbi = pshmem_uint_put_nbi
#define shmem_uint_put_nbi pshmem_uint_put_nbi
#pragma weak shmem_ulong_put_nbi = pshmem_ulong_put_nbi
#define shmem_ulong_put_nbi pshmem_ulong_put_nbi
#pragma weak shmem_ulonglong_put_nbi = pshmem_ulonglong_put_nbi
#define shmem_ulonglong_put_nbi pshmem_ulonglong_put_nbi
#pragma weak shmem_int8_put_nbi = pshmem_int8_put_nbi
#define shmem_int8_put_nbi pshmem_int8_put_nbi
#pragma weak shmem_int16_put_nbi = pshmem_int16_put_nbi
#define shmem_int16_put_nbi pshmem_int16_put_nbi
#pragma weak shmem_int32_put_nbi = pshmem_int32_put_nbi
#define shmem_int32_put_nbi pshmem_int32_put_nbi
#pragma weak shmem_int64_put_nbi = pshmem_int64_put_nbi
#define shmem_int64_put_nbi pshmem_int64_put_nbi
#pragma weak shmem_uint8_put_nbi = pshmem_uint8_put_nbi
#define shmem_uint8_put_nbi pshmem_uint8_put_nbi
#pragma weak shmem_uint16_put_nbi = pshmem_uint16_put_nbi
#define shmem_uint16_put_nbi pshmem_uint16_put_nbi
#pragma weak shmem_uint32_put_nbi = pshmem_uint32_put_nbi
#define shmem_uint32_put_nbi pshmem_uint32_put_nbi
#pragma weak shmem_uint64_put_nbi = pshmem_uint64_put_nbi
#define shmem_uint64_put_nbi pshmem_uint64_put_nbi
#endif /* ENABLE_PSHMEM */

#define API_DECL_PUTGET(_opname, _name, _type)                          \
    void                                                                \
    shmem_##_name##_##_opname(_type *dest, const _type *src,            \
                              size_t nelems, int pe)                    \
    {                                                                   \
        shmem_ctx_##_name##_##_opname(SHMEM_CTX_DEFAULT,                \
                                      dest, src, nelems, pe);           \
    }                                                                   \
    void                                                                \
    shmem_##_name##_i##_opname(_type *dest,                             \
                               const _type *src,                        \
                               ptrdiff_t tst, ptrdiff_t sst,            \
                               size_t nelems, int pe)                   \
    {                                                                   \
        shmem_ctx_##_name##_i##_opname(SHMEM_CTX_DEFAULT,               \
                                       dest, src,                       \
                                       tst, sst,                        \
                                       nelems, pe);                     \
    }                                                                   \
    void                                                                \
    shmem_##_name##_##_opname##_nbi(_type *dest,                        \
                                    const _type *src,                   \
                                    size_t nelems, int pe)              \
    {                                                                   \
        shmem_ctx_##_name##_##_opname##_nbi(SHMEM_CTX_DEFAULT,          \
                                            dest, src, nelems, pe);     \
    }

API_DECL_PUTGET(put, float, float)
API_DECL_PUTGET(put, double, double)
API_DECL_PUTGET(put, longdouble, long double)
API_DECL_PUTGET(put, schar, signed char)
API_DECL_PUTGET(put, char, char)
API_DECL_PUTGET(put, short, short)
API_DECL_PUTGET(put, int, int)
API_DECL_PUTGET(put, long, long)
API_DECL_PUTGET(put, longlong, long long)
API_DECL_PUTGET(put, uchar, unsigned char)
API_DECL_PUTGET(put, ushort, unsigned short)
API_DECL_PUTGET(put, uint, unsigned int)
API_DECL_PUTGET(put, ulong, unsigned long)
API_DECL_PUTGET(put, ulonglong, unsigned long long)
API_DECL_PUTGET(put, int8, int8_t)
API_DECL_PUTGET(put, int16, int16_t)
API_DECL_PUTGET(put, int32, int32_t)
API_DECL_PUTGET(put, int64, int64_t)
API_DECL_PUTGET(put, uint8, uint8_t)
API_DECL_PUTGET(put, uint16, uint16_t)
API_DECL_PUTGET(put, uint32, uint32_t)
API_DECL_PUTGET(put, uint64, uint64_t)
API_DECL_PUTGET(put, size, size_t)
API_DECL_PUTGET(put, ptrdiff, ptrdiff_t)

#undef API_DECL_PUTGET

#ifdef ENABLE_PSHMEM
#pragma weak shmem_put8 = pshmem_put8
#define shmem_put8 pshmem_put8
#pragma weak shmem_put16 = pshmem_put16
#define shmem_put16 pshmem_put16
#pragma weak shmem_put32 = pshmem_put32
#define shmem_put32 pshmem_put32
#pragma weak shmem_put64 = pshmem_put64
#define shmem_put64 pshmem_put64
#pragma weak shmem_put128 = pshmem_put128
#define shmem_put128 pshmem_put128

#pragma weak shmem_iput8 = pshmem_iput8
#define shmem_iput8 pshmem_iput8
#pragma weak shmem_iput16 = pshmem_iput16
#define shmem_iput16 pshmem_iput16
#pragma weak shmem_iput32 = pshmem_iput32
#define shmem_iput32 pshmem_iput32
#pragma weak shmem_iput64 = pshmem_iput64
#define shmem_iput64 pshmem_iput64
#pragma weak shmem_iput128 = pshmem_iput128
#define shmem_iput128 pshmem_iput128

#pragma weak shmem_put8_nbi = pshmem_put8_nbi
#define shmem_put8_nbi pshmem_put8_nbi
#pragma weak shmem_put16_nbi = pshmem_put16_nbi
#define shmem_put16_nbi pshmem_put16_nbi
#pragma weak shmem_put32_nbi = pshmem_put32_nbi
#define shmem_put32_nbi pshmem_put32_nbi
#pragma weak shmem_put64_nbi = pshmem_put64_nbi
#define shmem_put64_nbi pshmem_put64_nbi
#pragma weak shmem_put128_nbi = pshmem_put128_nbi
#define shmem_put128_nbi pshmem_put128_nbi
#endif /* ENABLE_PSHMEM */

#define API_DECL_PUTGET_SIZE(_opname, _size)                            \
    void                                                                \
    shmem_##_opname##_size(void *dest, const void *src,                 \
                           size_t nelems, int pe)                       \
    {                                                                   \
        shmem_ctx_##_opname##_size(SHMEM_CTX_DEFAULT,                   \
                                   dest, src, nelems, pe);              \
    }                                                                   \
    void                                                                \
    shmem_i##_opname##_size(void *dest, const void *src,                \
                            ptrdiff_t tst, ptrdiff_t sst,               \
                            size_t nelems, int pe)                      \
    {                                                                   \
        shmem_ctx_i##_opname##_size(SHMEM_CTX_DEFAULT,                  \
                                    dest, src, tst, sst, nelems, pe);   \
    }                                                                   \
    void                                                                \
    shmem_##_opname##_size##_nbi(void *dest, const void *src,           \
                                 size_t nelems, int pe)                 \
    {                                                                   \
        shmem_ctx_##_opname##_size##_nbi(SHMEM_CTX_DEFAULT,             \
                                         dest, src, nelems, pe);        \
    }

API_DECL_PUTGET_SIZE(put, 8)
API_DECL_PUTGET_SIZE(put, 16)
API_DECL_PUTGET_SIZE(put, 32)
API_DECL_PUTGET_SIZE(put, 64)
API_DECL_PUTGET_SIZE(put, 128)

#undef API_DECL_PUTGET_SIZE

#ifdef ENABLE_PSHMEM
#pragma weak shmem_putmem = pshmem_putmem
#define shmem_putmem pshmem_putmem
#pragma weak shmem_putmem_nbi = pshmem_putmem_nbi
#define shmem_putmem_nbi pshmem_putmem_nbi
#endif /* ENABLE_PSHMEM */

#define API_DECL_PUTGET_MEM(_opname)                                    \
    void                                                                \
    shmem_##_opname##mem(void *dest, const void *src,                   \
                         size_t nelems, int pe)                         \
    {                                                                   \
        shmem_ctx_##_opname##mem(SHMEM_CTX_DEFAULT,                     \
                                 dest, src, nelems, pe);                \
    }                                                                   \
    void                                                                 \
    shmem_##_opname##mem_nbi(void *dest, const void *src,               \
                             size_t nelems, int pe)                     \
    {                                                                   \
        shmem_ctx_##_opname##mem_nbi(SHMEM_CTX_DEFAULT,                 \
                                     dest, src, nelems, pe);            \
    }

API_DECL_PUTGET_MEM(put)

#undef API_DECL_PUTGET_MEM

#ifdef ENABLE_PSHMEM
#pragma weak shmem_float_p = pshmem_float_p
#define shmem_float_p pshmem_float_p
#pragma weak shmem_double_p = pshmem_double_p
#define shmem_double_p pshmem_double_p
#pragma weak shmem_longdouble_p = pshmem_longdouble_p
#define shmem_longdouble_p pshmem_longdouble_p
#pragma weak shmem_char_p = pshmem_char_p
#define shmem_char_p pshmem_char_p
#pragma weak shmem_schar_p = pshmem_schar_p
#define shmem_schar_p pshmem_schar_p
#pragma weak shmem_short_p = pshmem_short_p
#define shmem_short_p pshmem_short_p
#pragma weak shmem_int_p = pshmem_int_p
#define shmem_int_p pshmem_int_p
#pragma weak shmem_long_p = pshmem_long_p
#define shmem_long_p pshmem_long_p
#pragma weak shmem_longlong_p = pshmem_longlong_p
#define shmem_longlong_p pshmem_longlong_p
#pragma weak shmem_uchar_p = pshmem_uchar_p
#define shmem_uchar_p pshmem_uchar_p
#pragma weak shmem_ushort_p = pshmem_ushort_p
#define shmem_ushort_p pshmem_ushort_p
#pragma weak shmem_uint_p = pshmem_uint_p
#define shmem_uint_p pshmem_uint_p
#pragma weak shmem_ulong_p = pshmem_ulong_p
#define shmem_ulong_p pshmem_ulong_p
#pragma weak shmem_ulonglong_p = pshmem_ulonglong_p
#define shmem_ulonglong_p pshmem_ulonglong_p
#pragma weak shmem_int8_p = pshmem_int8_p
#define shmem_int8_p pshmem_int8_p
#pragma weak shmem_int16_p = pshmem_int16_p
#define shmem_int16_p pshmem_int16_p
#pragma weak shmem_int32_p = pshmem_int32_p
#define shmem_int32_p pshmem_int32_p
#pragma weak shmem_int64_p = pshmem_int64_p
#define shmem_int64_p pshmem_int64_p
#pragma weak shmem_uint8_p = pshmem_uint8_p
#define shmem_uint8_p pshmem_uint8_p
#pragma weak shmem_uint16_p = pshmem_uint16_p
#define shmem_uint16_p pshmem_uint16_p
#pragma weak shmem_uint32_p = pshmem_uint32_p
#define shmem_uint32_p pshmem_uint32_p
#pragma weak shmem_uint64_p = pshmem_uint64_p
#define shmem_uint64_p pshmem_uint64_p
#pragma weak shmem_size_p = pshmem_size_p
#define shmem_size_p pshmem_size_p
#pragma weak shmem_ptrdiff_p = pshmem_ptrdiff_p
#define shmem_ptrdiff_p pshmem_ptrdiff_p
#endif /* ENABLE_PSHMEM */

#define API_DECL_P(_name, _type)                                        \
    void                                                                \
    shmem_##_name##_p(_type *dest, _type src, int pe)                   \
    {                                                                   \
        shmem_ctx_##_name##_p(SHMEM_CTX_DEFAULT,                        \
                              dest, src, pe);                           \
    }

API_DECL_P(float, float)
API_DECL_P(double, double)
API_DECL_P(longdouble, long double)
API_DECL_P(schar, signed char)
API_DECL_P(char, char)
API_DECL_P(short, short)
API_DECL_P(int, int)
API_DECL_P(long, long)
API_DECL_P(longlong, long long)
API_DECL_P(uchar, unsigned char)
API_DECL_P(ushort, unsigned short)
API_DECL_P(uint, unsigned int)
API_DECL_P(ulong, unsigned long)
API_DECL_P(ulonglong, unsigned long long)
API_DECL_P(int8, int8_t)
API_DECL_P(int16, int16_t)
API_DECL_P(int32, int32_t)
API_DECL_P(int64, int64_t)
API_DECL_P(uint8, uint8_t)
API_DECL_P(uint16, uint16_t)
API_DECL_P(uint32, uint32_t)
API_DECL_P(uint64, uint64_t)
API_DECL_P(size, size_t)
API_DECL_P(ptrdiff, ptrdiff_t)

#undef API_DECL_P
