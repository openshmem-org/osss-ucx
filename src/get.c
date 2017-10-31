/* For license: see LICENSE file at top-level */

#include "shmem/defs.h"
#include "shmemc.h"

#ifdef ENABLE_PSHMEM
extern void shmem_complexf_get(COMPLEXIFY(float) * dest,
                               const COMPLEXIFY(float) * src,
                               size_t nelems, int pe);  /* ! API */
extern void shmem_complexd_get(COMPLEXIFY(double) * dest,
                               const COMPLEXIFY(double) * src,
                               size_t nelems, int pe);    /* ! API */

#pragma weak shmem_float_get = pshmem_float_get
#define shmem_float_get pshmem_float_get
#pragma weak shmem_double_get = pshmem_double_get
#define shmem_double_get pshmem_double_get
#pragma weak shmem_longdouble_get = pshmem_longdouble_get
#define shmem_longdouble_get pshmem_longdouble_get
#pragma weak shmem_char_get = pshmem_char_get
#define shmem_char_get pshmem_char_get
#pragma weak shmem_schar_get = pshmem_schar_get
#define shmem_schar_get pshmem_schar_get
#pragma weak shmem_short_get = pshmem_short_get
#define shmem_short_get pshmem_short_get
#pragma weak shmem_int_get = pshmem_int_get
#define shmem_int_get pshmem_int_get
#pragma weak shmem_long_get = pshmem_long_get
#define shmem_long_get pshmem_long_get
#pragma weak shmem_longlong_get = pshmem_longlong_get
#define shmem_longlong_get pshmem_longlong_get
#pragma weak shmem_uchar_get = pshmem_uchar_get
#define shmem_uchar_get pshmem_uchar_get
#pragma weak shmem_ushort_get = pshmem_ushort_get
#define shmem_ushort_get pshmem_ushort_get
#pragma weak shmem_uint_get = pshmem_uint_get
#define shmem_uint_get pshmem_uint_get
#pragma weak shmem_ulong_get = pshmem_ulong_get
#define shmem_ulong_get pshmem_ulong_get
#pragma weak shmem_ulonglong_get = pshmem_ulonglong_get
#define shmem_ulonglong_get pshmem_ulonglong_get
#pragma weak shmem_int8_get = pshmem_int8_get
#define shmem_int8_get pshmem_int8_get
#pragma weak shmem_int16_get = pshmem_int16_get
#define shmem_int16_get pshmem_int16_get
#pragma weak shmem_int32_get = pshmem_int32_get
#define shmem_int32_get pshmem_int32_get
#pragma weak shmem_int64_get = pshmem_int64_get
#define shmem_int64_get pshmem_int64_get
#pragma weak shmem_uint8_get = pshmem_uint8_get
#define shmem_uint8_get pshmem_uint8_get
#pragma weak shmem_uint16_get = pshmem_uint16_get
#define shmem_uint16_get pshmem_uint16_get
#pragma weak shmem_uint32_get = pshmem_uint32_get
#define shmem_uint32_get pshmem_uint32_get
#pragma weak shmem_uint64_get = pshmem_uint64_get
#define shmem_uint64_get pshmem_uint64_get

#pragma weak shmem_get8 = pshmem_get8
#define shmem_get8 pshmem_get8
#pragma weak shmem_get16 = pshmem_get16
#define shmem_get16 pshmem_get16
#pragma weak shmem_get32 = pshmem_get32
#define shmem_get32 pshmem_get32
#pragma weak shmem_get64 = pshmem_get64
#define shmem_get64 pshmem_get64
#pragma weak shmem_get128 = pshmem_get128
#define shmem_get128 pshmem_get128

#pragma weak shmem_getmem = pshmem_getmem
#define shmem_getmem pshmem_getmem

/* for Fortran */
#pragma weak shmem_complexf_get = pshmem_complexf_get
#define shmem_complexf_get pshmem_complexf_get
#pragma weak shmem_complexd_get = pshmem_complexd_get
#define shmem_complexd_get pshmem_complexd_get
#endif /* ENABLE_PSHMEM */

#define SHMEM_TYPED_GET(_name, _type)                            \
    void                                                         \
    shmem_##_name##_get(_type *dest, const _type *src,           \
                        size_t nelems, int pe)                   \
    {                                                            \
        const size_t typed_nelems = nelems * sizeof (_type);     \
        shmemc_get(dest, src, typed_nelems, pe);                 \
    }

SHMEM_TYPED_GET(float, float)
SHMEM_TYPED_GET(double, double)
SHMEM_TYPED_GET(longdouble, long double)
SHMEM_TYPED_GET(char, char)
SHMEM_TYPED_GET(schar, signed char)
SHMEM_TYPED_GET(short, short)
SHMEM_TYPED_GET(int, int)
SHMEM_TYPED_GET(long, long)
SHMEM_TYPED_GET(longlong, long long)
SHMEM_TYPED_GET(uchar, unsigned char)
SHMEM_TYPED_GET(ushort, unsigned short)
SHMEM_TYPED_GET(uint, unsigned int)
SHMEM_TYPED_GET(ulong, unsigned long)
SHMEM_TYPED_GET(ulonglong, unsigned long long)
SHMEM_TYPED_GET(int8, int8_t)
SHMEM_TYPED_GET(int16, int16_t)
SHMEM_TYPED_GET(int32, int32_t)
SHMEM_TYPED_GET(int64, int64_t)
SHMEM_TYPED_GET(uint8, uint8_t)
SHMEM_TYPED_GET(uint16, uint16_t)
SHMEM_TYPED_GET(uint32, uint32_t)
SHMEM_TYPED_GET(uint64, uint64_t)
SHMEM_TYPED_GET(size, size_t)
SHMEM_TYPED_GET(ptrdiff, ptrdiff_t)
/* for Fortran */
SHMEM_TYPED_GET(complexf, COMPLEXIFY(float))
SHMEM_TYPED_GET(complexd, COMPLEXIFY(double))

#define SHMEM_SIZED_GET(_size)                              \
    void                                                    \
    shmem_get##_size(void *dest, const void *src,           \
                     size_t nelems, int pe)                 \
    {                                                       \
        const size_t sized_nelems = nelems * _size;         \
        shmemc_get(dest, src, sized_nelems, pe);            \
    }

SHMEM_SIZED_GET(8)
SHMEM_SIZED_GET(16)
SHMEM_SIZED_GET(32)
SHMEM_SIZED_GET(64)
SHMEM_SIZED_GET(128)

void
shmem_getmem(void *dest, const void *src,
             size_t nelems, int pe)
{
    shmemc_get(dest, src, nelems, pe);
}

#ifdef ENABLE_PSHMEM
#pragma weak shmem_float_g = pshmem_float_g
#define shmem_float_g pshmem_float_g
#pragma weak shmem_double_g = pshmem_double_g
#define shmem_double_g pshmem_double_g
#pragma weak shmem_longdouble_g = pshmem_longdouble_g
#define shmem_longdouble_g pshmem_longdouble_g
#pragma weak shmem_char_g = pshmem_char_g
#define shmem_char_g pshmem_char_g
#pragma weak shmem_schar_g = pshmem_schar_g
#define shmem_schar_g pshmem_schar_g
#pragma weak shmem_short_g = pshmem_short_g
#define shmem_short_g pshmem_short_g
#pragma weak shmem_int_g = pshmem_int_g
#define shmem_int_g pshmem_int_g
#pragma weak shmem_long_g = pshmem_long_g
#define shmem_long_g pshmem_long_g
#pragma weak shmem_longlong_g = pshmem_longlong_g
#define shmem_longlong_g pshmem_longlong_g
#pragma weak shmem_uchar_g = pshmem_uchar_g
#define shmem_uchar_g pshmem_uchar_g
#pragma weak shmem_ushort_g = pshmem_ushort_g
#define shmem_ushort_g pshmem_ushort_g
#pragma weak shmem_uint_g = pshmem_uint_g
#define shmem_uint_g pshmem_uint_g
#pragma weak shmem_ulong_g = pshmem_ulong_g
#define shmem_ulong_g pshmem_ulong_g
#pragma weak shmem_ulonglong_g = pshmem_ulonglong_g
#define shmem_ulonglong_g pshmem_ulonglong_g
#pragma weak shmem_int8_g = pshmem_int8_g
#define shmem_int8_g pshmem_int8_g
#pragma weak shmem_int16_g = pshmem_int16_g
#define shmem_int16_g pshmem_int16_g
#pragma weak shmem_int32_g = pshmem_int32_g
#define shmem_int32_g pshmem_int32_g
#pragma weak shmem_int64_g = pshmem_int64_g
#define shmem_int64_g pshmem_int64_g
#pragma weak shmem_uint8_g = pshmem_uint8_g
#define shmem_uint8_g pshmem_uint8_g
#pragma weak shmem_uint16_g = pshmem_uint16_g
#define shmem_uint16_g pshmem_uint16_g
#pragma weak shmem_uint32_g = pshmem_uint32_g
#define shmem_uint32_g pshmem_uint32_g
#pragma weak shmem_uint64_g = pshmem_uint64_g
#define shmem_uint64_g pshmem_uint64_g
/* for Fortran */
#pragma weak shmem_complexf_g = pshmem_complexf_g
#define shmem_complexf_g pshmem_complexf_g
#pragma weak shmem_complexd_g = pshmem_complexd_g
#define shmem_complexd_g pshmem_complexd_g
#endif /* ENABLE_PSHMEM */

#define SHMEM_TYPED_G_WRAPPER(_name, _type)                          \
    _type                                                            \
    shmem_##_name##_g(const _type *addr, int pe)                     \
    {                                                                \
        _type val;                                                   \
        shmemc_get(&val, addr, sizeof(val), pe);                     \
        return val;                                                  \
    }

SHMEM_TYPED_G_WRAPPER(float, float)
SHMEM_TYPED_G_WRAPPER(double, double)
SHMEM_TYPED_G_WRAPPER(longdouble, long double)
SHMEM_TYPED_G_WRAPPER(char, char)
SHMEM_TYPED_G_WRAPPER(schar, signed char)
SHMEM_TYPED_G_WRAPPER(short, short)
SHMEM_TYPED_G_WRAPPER(int, int)
SHMEM_TYPED_G_WRAPPER(long, long)
SHMEM_TYPED_G_WRAPPER(longlong, long long)
SHMEM_TYPED_G_WRAPPER(uchar, unsigned char)
SHMEM_TYPED_G_WRAPPER(ushort, unsigned short)
SHMEM_TYPED_G_WRAPPER(uint, unsigned int)
SHMEM_TYPED_G_WRAPPER(ulong, unsigned long)
SHMEM_TYPED_G_WRAPPER(ulonglong, unsigned long long)
SHMEM_TYPED_G_WRAPPER(int8, int8_t)
SHMEM_TYPED_G_WRAPPER(int16, int16_t)
SHMEM_TYPED_G_WRAPPER(int32, int32_t)
SHMEM_TYPED_G_WRAPPER(int64, int64_t)
SHMEM_TYPED_G_WRAPPER(uint8, uint8_t)
SHMEM_TYPED_G_WRAPPER(uint16, uint16_t)
SHMEM_TYPED_G_WRAPPER(uint32, uint32_t)
SHMEM_TYPED_G_WRAPPER(uint64, uint64_t)
SHMEM_TYPED_G_WRAPPER(size, size_t)
SHMEM_TYPED_G_WRAPPER(ptrdiff, ptrdiff_t)
/* for Fortran */
SHMEM_TYPED_G_WRAPPER(complexf, COMPLEXIFY(float))
SHMEM_TYPED_G_WRAPPER(complexd, COMPLEXIFY(double))
