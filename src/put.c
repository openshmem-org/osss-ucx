/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"

#include "shmem/defs.h"

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

#pragma weak shmem_putmem = pshmem_putmem
#define shmem_putmem pshmem_putmem

/* for Fortran */
#pragma weak shmem_complexf_put = pshmem_complexf_put
#define shmem_complexf_put pshmem_complexf_put
#pragma weak shmem_complexd_put = pshmem_complexd_put
#define shmem_complexd_put pshmem_complexd_put

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

#define SHMEM_SIZED_PUT(_size)                                          \
    void                                                                \
    shmem_put##_size(void *dest, const void *src,                       \
                     size_t nelems, int pe)                             \
    {                                                                   \
        const size_t sized_nelems = nelems * _size;                     \
        shmemc_put(dest, src, sized_nelems, pe);                        \
    }

SHMEM_SIZED_PUT(8)
SHMEM_SIZED_PUT(16)
SHMEM_SIZED_PUT(32)
SHMEM_SIZED_PUT(64)
SHMEM_SIZED_PUT(128)

void
shmem_putmem(void *dest, const void *src,
             size_t nelems, int pe)
{
    shmemc_put(dest, src, nelems, pe);
}

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
/* for Fortran */
#pragma weak shmem_complexf_p = pshmem_complexf_p
#define shmem_complexf_p pshmem_complexf_p
#pragma weak shmem_complexd_p = pshmem_complexd_p
#define shmem_complexd_p pshmem_complexd_p
#endif /* ENABLE_PSHMEM */

#define SHMEM_TYPED_P_WRAPPER(_name, _type)                          \
    void                                                             \
    shmem_##_name##_p(_type *addr, _type val, int pe)                \
    {                                                                \
        shmemc_put(addr, &val, sizeof(val), pe);                     \
    }

SHMEM_TYPED_P_WRAPPER(float, float)
SHMEM_TYPED_P_WRAPPER(double, double)
SHMEM_TYPED_P_WRAPPER(longdouble, long double)
SHMEM_TYPED_P_WRAPPER(char, char)
SHMEM_TYPED_P_WRAPPER(schar, signed char)
SHMEM_TYPED_P_WRAPPER(short, short)
SHMEM_TYPED_P_WRAPPER(int, int)
SHMEM_TYPED_P_WRAPPER(long, long)
SHMEM_TYPED_P_WRAPPER(longlong, long long)
SHMEM_TYPED_P_WRAPPER(uchar, unsigned char)
SHMEM_TYPED_P_WRAPPER(ushort, unsigned short)
SHMEM_TYPED_P_WRAPPER(uint, unsigned int)
SHMEM_TYPED_P_WRAPPER(ulong, unsigned long)
SHMEM_TYPED_P_WRAPPER(ulonglong, unsigned long long)
SHMEM_TYPED_P_WRAPPER(int8, int8_t)
SHMEM_TYPED_P_WRAPPER(int16, int16_t)
SHMEM_TYPED_P_WRAPPER(int32, int32_t)
SHMEM_TYPED_P_WRAPPER(int64, int64_t)
SHMEM_TYPED_P_WRAPPER(uint8, uint8_t)
SHMEM_TYPED_P_WRAPPER(uint16, uint16_t)
SHMEM_TYPED_P_WRAPPER(uint32, uint32_t)
SHMEM_TYPED_P_WRAPPER(uint64, uint64_t)
SHMEM_TYPED_P_WRAPPER(size, size_t)
SHMEM_TYPED_P_WRAPPER(ptrdiff, ptrdiff_t)
/* for Fortran */
SHMEM_TYPED_P_WRAPPER(complexf, COMPLEXIFY(float))
SHMEM_TYPED_P_WRAPPER(complexd, COMPLEXIFY(double))
