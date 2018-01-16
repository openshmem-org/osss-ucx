/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"

#include "shmemc.h"
#include "shmem/api.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_float_get = pshmem_ctx_float_get
#define shmem_ctx_float_get pshmem_ctx_float_get
#pragma weak shmem_ctx_double_get = pshmem_ctx_double_get
#define shmem_ctx_double_get pshmem_ctx_double_get
#pragma weak shmem_ctx_longdouble_get = pshmem_ctx_longdouble_get
#define shmem_ctx_longdouble_get pshmem_ctx_longdouble_get
#pragma weak shmem_ctx_char_get = pshmem_ctx_char_get
#define shmem_ctx_char_get pshmem_ctx_char_get
#pragma weak shmem_ctx_schar_get = pshmem_ctx_schar_get
#define shmem_ctx_schar_get pshmem_ctx_schar_get
#pragma weak shmem_ctx_short_get = pshmem_ctx_short_get
#define shmem_ctx_short_get pshmem_ctx_short_get
#pragma weak shmem_ctx_int_get = pshmem_ctx_int_get
#define shmem_ctx_int_get pshmem_ctx_int_get
#pragma weak shmem_ctx_long_get = pshmem_ctx_long_get
#define shmem_ctx_long_get pshmem_ctx_long_get
#pragma weak shmem_ctx_longlong_get = pshmem_ctx_longlong_get
#define shmem_ctx_longlong_get pshmem_ctx_longlong_get
#pragma weak shmem_ctx_uchar_get = pshmem_ctx_uchar_get
#define shmem_ctx_uchar_get pshmem_ctx_uchar_get
#pragma weak shmem_ctx_ushort_get = pshmem_ctx_ushort_get
#define shmem_ctx_ushort_get pshmem_ctx_ushort_get
#pragma weak shmem_ctx_uint_get = pshmem_ctx_uint_get
#define shmem_ctx_uint_get pshmem_ctx_uint_get
#pragma weak shmem_ctx_ulong_get = pshmem_ctx_ulong_get
#define shmem_ctx_ulong_get pshmem_ctx_ulong_get
#pragma weak shmem_ctx_ulonglong_get = pshmem_ctx_ulonglong_get
#define shmem_ctx_ulonglong_get pshmem_ctx_ulonglong_get
#pragma weak shmem_ctx_int8_get = pshmem_ctx_int8_get
#define shmem_ctx_int8_get pshmem_ctx_int8_get
#pragma weak shmem_ctx_int16_get = pshmem_ctx_int16_get
#define shmem_ctx_int16_get pshmem_ctx_int16_get
#pragma weak shmem_ctx_int32_get = pshmem_ctx_int32_get
#define shmem_ctx_int32_get pshmem_ctx_int32_get
#pragma weak shmem_ctx_int64_get = pshmem_ctx_int64_get
#define shmem_ctx_int64_get pshmem_ctx_int64_get
#pragma weak shmem_ctx_uint8_get = pshmem_ctx_uint8_get
#define shmem_ctx_uint8_get pshmem_ctx_uint8_get
#pragma weak shmem_ctx_uint16_get = pshmem_ctx_uint16_get
#define shmem_ctx_uint16_get pshmem_ctx_uint16_get
#pragma weak shmem_ctx_uint32_get = pshmem_ctx_uint32_get
#define shmem_ctx_uint32_get pshmem_ctx_uint32_get
#pragma weak shmem_ctx_uint64_get = pshmem_ctx_uint64_get
#define shmem_ctx_uint64_get pshmem_ctx_uint64_get

/* for Fortran */
#pragma weak shmem_ctx_complexf_get = pshmem_ctx_complexf_get
#define shmem_ctx_complexf_get pshmem_ctx_complexf_get
#pragma weak shmem_ctx_complexd_get = pshmem_ctx_complexd_get
#define shmem_ctx_complexd_get pshmem_ctx_complexd_get
#endif /* ENABLE_PSHMEM */

#define SHMEM_CTX_TYPED_GET(_name, _type)                               \
    void                                                                \
    shmem_ctx_##_name##_get(shmem_ctx_t ctx,                            \
                            _type *dest, const _type *src,              \
                            size_t nelems, int pe)                      \
    {                                                                   \
        SHMEML_MUTEX_PROTECT(shmemc_ctx_get(ctx,                        \
                                            dest, src,                  \
                                            sizeof(_type) * nelems,     \
                                            pe));                       \
    }

SHMEM_CTX_TYPED_GET(float, float)
SHMEM_CTX_TYPED_GET(double, double)
SHMEM_CTX_TYPED_GET(longdouble, long double)
SHMEM_CTX_TYPED_GET(char, char)
SHMEM_CTX_TYPED_GET(schar, signed char)
SHMEM_CTX_TYPED_GET(short, short)
SHMEM_CTX_TYPED_GET(int, int)
SHMEM_CTX_TYPED_GET(long, long)
SHMEM_CTX_TYPED_GET(longlong, long long)
SHMEM_CTX_TYPED_GET(uchar, unsigned char)
SHMEM_CTX_TYPED_GET(ushort, unsigned short)
SHMEM_CTX_TYPED_GET(uint, unsigned int)
SHMEM_CTX_TYPED_GET(ulong, unsigned long)
SHMEM_CTX_TYPED_GET(ulonglong, unsigned long long)
SHMEM_CTX_TYPED_GET(int8, int8_t)
SHMEM_CTX_TYPED_GET(int16, int16_t)
SHMEM_CTX_TYPED_GET(int32, int32_t)
SHMEM_CTX_TYPED_GET(int64, int64_t)
SHMEM_CTX_TYPED_GET(uint8, uint8_t)
SHMEM_CTX_TYPED_GET(uint16, uint16_t)
SHMEM_CTX_TYPED_GET(uint32, uint32_t)
SHMEM_CTX_TYPED_GET(uint64, uint64_t)
SHMEM_CTX_TYPED_GET(size, size_t)
SHMEM_CTX_TYPED_GET(ptrdiff, ptrdiff_t)
/* for Fortran */
SHMEM_CTX_TYPED_GET(complexf, COMPLEXIFY(float))
SHMEM_CTX_TYPED_GET(complexd, COMPLEXIFY(double))

#undef SHMEM_CTX_TYPED_GET

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_get8 = pshmem_ctx_get8
#define shmem_ctx_get8 pshmem_ctx_get8
#pragma weak shmem_ctx_get16 = pshmem_ctx_get16
#define shmem_ctx_get16 pshmem_ctx_get16
#pragma weak shmem_ctx_get32 = pshmem_ctx_get32
#define shmem_ctx_get32 pshmem_ctx_get32
#pragma weak shmem_ctx_get64 = pshmem_ctx_get64
#define shmem_ctx_get64 pshmem_ctx_get64
#pragma weak shmem_ctx_get128 = pshmem_ctx_get128
#define shmem_ctx_get128 pshmem_ctx_get128
#endif /* ENABLE_PSHMEM */

#define SHMEM_CTX_SIZED_GET(_size)                                      \
    void                                                                \
    shmem_ctx_get##_size(shmem_ctx_t ctx,                               \
                         void *dest, const void *src,                   \
                         size_t nelems, int pe)                         \
    {                                                                   \
        SHMEML_MUTEX_PROTECT(shmemc_ctx_get(ctx,                        \
                                            dest, src,                  \
                                            _size * nelems,             \
                                            pe));                       \
    }

SHMEM_CTX_SIZED_GET(8)
SHMEM_CTX_SIZED_GET(16)
SHMEM_CTX_SIZED_GET(32)
SHMEM_CTX_SIZED_GET(64)
SHMEM_CTX_SIZED_GET(128)

#undef SHMEM_CTX_SIZED_GET

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_getmem = pshmem_ctx_getmem
#define shmem_ctx_getmem pshmem_ctx_getmem
#endif /* ENABLE_PSHMEM */

void
shmem_ctx_getmem(shmem_ctx_t ctx,
                 void *dest, const void *src,
                 size_t nelems, int pe)
{
    SHMEML_MUTEX_PROTECT(shmemc_ctx_get(ctx,
                                        dest, src,
                                        nelems,
                                        pe));
}

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_float_g = pshmem_ctx_float_g
#define shmem_ctx_float_g pshmem_ctx_float_g
#pragma weak shmem_ctx_double_g = pshmem_ctx_double_g
#define shmem_ctx_double_g pshmem_ctx_double_g
#pragma weak shmem_ctx_longdouble_g = pshmem_ctx_longdouble_g
#define shmem_ctx_longdouble_g pshmem_ctx_longdouble_g
#pragma weak shmem_ctx_char_g = pshmem_ctx_char_g
#define shmem_ctx_char_g pshmem_ctx_char_g
#pragma weak shmem_ctx_schar_g = pshmem_ctx_schar_g
#define shmem_ctx_schar_g pshmem_ctx_schar_g
#pragma weak shmem_ctx_short_g = pshmem_ctx_short_g
#define shmem_ctx_short_g pshmem_ctx_short_g
#pragma weak shmem_ctx_int_g = pshmem_ctx_int_g
#define shmem_ctx_int_g pshmem_ctx_int_g
#pragma weak shmem_ctx_long_g = pshmem_ctx_long_g
#define shmem_ctx_long_g pshmem_ctx_long_g
#pragma weak shmem_ctx_longlong_g = pshmem_ctx_longlong_g
#define shmem_ctx_longlong_g pshmem_ctx_longlong_g
#pragma weak shmem_ctx_uchar_g = pshmem_ctx_uchar_g
#define shmem_ctx_uchar_g pshmem_ctx_uchar_g
#pragma weak shmem_ctx_ushort_g = pshmem_ctx_ushort_g
#define shmem_ctx_ushort_g pshmem_ctx_ushort_g
#pragma weak shmem_ctx_uint_g = pshmem_ctx_uint_g
#define shmem_ctx_uint_g pshmem_ctx_uint_g
#pragma weak shmem_ctx_ulong_g = pshmem_ctx_ulong_g
#define shmem_ctx_ulong_g pshmem_ctx_ulong_g
#pragma weak shmem_ctx_ulonglong_g = pshmem_ctx_ulonglong_g
#define shmem_ctx_ulonglong_g pshmem_ctx_ulonglong_g
#pragma weak shmem_ctx_int8_g = pshmem_ctx_int8_g
#define shmem_ctx_int8_g pshmem_ctx_int8_g
#pragma weak shmem_ctx_int16_g = pshmem_ctx_int16_g
#define shmem_ctx_int16_g pshmem_ctx_int16_g
#pragma weak shmem_ctx_int32_g = pshmem_ctx_int32_g
#define shmem_ctx_int32_g pshmem_ctx_int32_g
#pragma weak shmem_ctx_int64_g = pshmem_ctx_int64_g
#define shmem_ctx_int64_g pshmem_ctx_int64_g
#pragma weak shmem_ctx_uint8_g = pshmem_ctx_uint8_g
#define shmem_ctx_uint8_g pshmem_ctx_uint8_g
#pragma weak shmem_ctx_uint16_g = pshmem_ctx_uint16_g
#define shmem_ctx_uint16_g pshmem_ctx_uint16_g
#pragma weak shmem_ctx_uint32_g = pshmem_ctx_uint32_g
#define shmem_ctx_uint32_g pshmem_ctx_uint32_g
#pragma weak shmem_ctx_uint64_g = pshmem_ctx_uint64_g
#define shmem_ctx_uint64_g pshmem_ctx_uint64_g
/* for Fortran */
#pragma weak shmem_ctx_complexf_g = pshmem_ctx_complexf_g
#define shmem_ctx_complexf_g pshmem_ctx_complexf_g
#pragma weak shmem_ctx_complexd_g = pshmem_ctx_complexd_g
#define shmem_ctx_complexd_g pshmem_ctx_complexd_g
#endif /* ENABLE_PSHMEM */

#define SHMEM_CTX_TYPED_G(_name, _type)                                 \
    _type                                                               \
    shmem_ctx_##_name##_g(shmem_ctx_t ctx,                              \
                          const _type *addr, int pe)                    \
    {                                                                   \
        _type val;                                                      \
                                                                        \
        SHMEML_MUTEX_PROTECT(shmemc_ctx_get(ctx,                        \
                                            &val, addr,                 \
                                            sizeof(val),                \
                                            pe));                       \
                                                                        \
        return val;                                                     \
    }

SHMEM_CTX_TYPED_G(float, float)
SHMEM_CTX_TYPED_G(double, double)
SHMEM_CTX_TYPED_G(longdouble, long double)
SHMEM_CTX_TYPED_G(char, char)
SHMEM_CTX_TYPED_G(schar, signed char)
SHMEM_CTX_TYPED_G(short, short)
SHMEM_CTX_TYPED_G(int, int)
SHMEM_CTX_TYPED_G(long, long)
SHMEM_CTX_TYPED_G(longlong, long long)
SHMEM_CTX_TYPED_G(uchar, unsigned char)
SHMEM_CTX_TYPED_G(ushort, unsigned short)
SHMEM_CTX_TYPED_G(uint, unsigned int)
SHMEM_CTX_TYPED_G(ulong, unsigned long)
SHMEM_CTX_TYPED_G(ulonglong, unsigned long long)
SHMEM_CTX_TYPED_G(int8, int8_t)
SHMEM_CTX_TYPED_G(int16, int16_t)
SHMEM_CTX_TYPED_G(int32, int32_t)
SHMEM_CTX_TYPED_G(int64, int64_t)
SHMEM_CTX_TYPED_G(uint8, uint8_t)
SHMEM_CTX_TYPED_G(uint16, uint16_t)
SHMEM_CTX_TYPED_G(uint32, uint32_t)
SHMEM_CTX_TYPED_G(uint64, uint64_t)
SHMEM_CTX_TYPED_G(size, size_t)
SHMEM_CTX_TYPED_G(ptrdiff, ptrdiff_t)
/* for Fortran */
SHMEM_CTX_TYPED_G(complexf, COMPLEXIFY(float))
SHMEM_CTX_TYPED_G(complexd, COMPLEXIFY(double))

#undef SHMEM_CTX_TYPED_G

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_float_iget = pshmem_ctx_float_iget
#define shmem_ctx_float_iget pshmem_ctx_float_iget
#pragma weak shmem_ctx_double_iget = pshmem_ctx_double_iget
#define shmem_ctx_double_iget pshmem_ctx_double_iget
#pragma weak shmem_ctx_longdouble_iget = pshmem_ctx_longdouble_iget
#define shmem_ctx_longdouble_iget pshmem_ctx_longdouble_iget
#pragma weak shmem_ctx_char_iget = pshmem_ctx_char_iget
#define shmem_ctx_char_iget pshmem_ctx_char_iget
#pragma weak shmem_ctx_schar_iget = pshmem_ctx_schar_iget
#define shmem_ctx_schar_iget pshmem_ctx_schar_iget
#pragma weak shmem_ctx_short_iget = pshmem_ctx_short_iget
#define shmem_ctx_short_iget pshmem_ctx_short_iget
#pragma weak shmem_ctx_int_iget = pshmem_ctx_int_iget
#define shmem_ctx_int_iget pshmem_ctx_int_iget
#pragma weak shmem_ctx_long_iget = pshmem_ctx_long_iget
#define shmem_ctx_long_iget pshmem_ctx_long_iget
#pragma weak shmem_ctx_longlong_iget = pshmem_ctx_longlong_iget
#define shmem_ctx_longlong_iget pshmem_ctx_longlong_iget
#pragma weak shmem_ctx_uchar_iget = pshmem_ctx_uchar_iget
#define shmem_ctx_uchar_iget pshmem_ctx_uchar_iget
#pragma weak shmem_ctx_ushort_iget = pshmem_ctx_ushort_iget
#define shmem_ctx_ushort_iget pshmem_ctx_ushort_iget
#pragma weak shmem_ctx_uint_iget = pshmem_ctx_uint_iget
#define shmem_ctx_uint_iget pshmem_ctx_uint_iget
#pragma weak shmem_ctx_ulong_iget = pshmem_ctx_ulong_iget
#define shmem_ctx_ulong_iget pshmem_ctx_ulong_iget
#pragma weak shmem_ctx_ulonglong_iget = pshmem_ctx_ulonglong_iget
#define shmem_ctx_ulonglong_iget pshmem_ctx_ulonglong_iget
#pragma weak shmem_ctx_int8_iget = pshmem_ctx_int8_iget
#define shmem_ctx_int8_iget pshmem_ctx_int8_iget
#pragma weak shmem_ctx_int16_iget = pshmem_ctx_int16_iget
#define shmem_ctx_int16_iget pshmem_ctx_int16_iget
#pragma weak shmem_ctx_int32_iget = pshmem_ctx_int32_iget
#define shmem_ctx_int32_iget pshmem_ctx_int32_iget
#pragma weak shmem_ctx_int64_iget = pshmem_ctx_int64_iget
#define shmem_ctx_int64_iget pshmem_ctx_int64_iget
#pragma weak shmem_ctx_uint8_iget = pshmem_ctx_uint8_iget
#define shmem_ctx_uint8_iget pshmem_ctx_uint8_iget
#pragma weak shmem_ctx_uint16_iget = pshmem_ctx_uint16_iget
#define shmem_ctx_uint16_iget pshmem_ctx_uint16_iget
#pragma weak shmem_ctx_uint32_iget = pshmem_ctx_uint32_iget
#define shmem_ctx_uint32_iget pshmem_ctx_uint32_iget
#pragma weak shmem_ctx_uint64_iget = pshmem_ctx_uint64_iget
#define shmem_ctx_uint64_iget pshmem_ctx_uint64_iget
#endif /* ENABLE_PSHMEM */

#define SHMEM_CTX_TYPED_IGET(_name, _type)                              \
    void                                                                \
    shmem_ctx_##_name##_iget(shmem_ctx_t ctx,                           \
                             _type *target, const _type *source,        \
                             ptrdiff_t tst, ptrdiff_t sst,              \
                             size_t nelems, int pe)                     \
    {                                                                   \
        size_t ti = 0, si = 0;                                          \
        size_t i;                                                       \
                                                                        \
        for (i = 0; i < nelems; i += 1) {                               \
            shmem_ctx_##_name##_get(ctx,                                \
                                    &(target[ti]),                      \
                                    &(source[si]),                      \
                                    1, pe);                             \
            ti += tst;                                                  \
            si += sst;                                                  \
        }                                                               \
    }

SHMEM_CTX_TYPED_IGET(float, float)
SHMEM_CTX_TYPED_IGET(double, double)
SHMEM_CTX_TYPED_IGET(longdouble, long double)
SHMEM_CTX_TYPED_IGET(char, char)
SHMEM_CTX_TYPED_IGET(schar, signed char)
SHMEM_CTX_TYPED_IGET(short, short)
SHMEM_CTX_TYPED_IGET(int, int)
SHMEM_CTX_TYPED_IGET(long, long)
SHMEM_CTX_TYPED_IGET(longlong, long long)
SHMEM_CTX_TYPED_IGET(uchar, unsigned char)
SHMEM_CTX_TYPED_IGET(ushort, unsigned short)
SHMEM_CTX_TYPED_IGET(uint, unsigned int)
SHMEM_CTX_TYPED_IGET(ulong, unsigned long)
SHMEM_CTX_TYPED_IGET(ulonglong, unsigned long long)
SHMEM_CTX_TYPED_IGET(int8, int8_t)
SHMEM_CTX_TYPED_IGET(int16, int16_t)
SHMEM_CTX_TYPED_IGET(int32, int32_t)
SHMEM_CTX_TYPED_IGET(int64, int64_t)
SHMEM_CTX_TYPED_IGET(uint8, uint8_t)
SHMEM_CTX_TYPED_IGET(uint16, uint16_t)
SHMEM_CTX_TYPED_IGET(uint32, uint32_t)
SHMEM_CTX_TYPED_IGET(uint64, uint64_t)
SHMEM_CTX_TYPED_IGET(size, size_t)
SHMEM_CTX_TYPED_IGET(ptrdiff, ptrdiff_t)
/* for Fortran */
SHMEM_CTX_TYPED_IGET(complexf, COMPLEXIFY(float))
SHMEM_CTX_TYPED_IGET(complexd, COMPLEXIFY(double))

#undef SHMEM_CTX_TYPED_IGET

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_iget8 = pshmem_ctx_iget8
#define shmem_ctx_iget8 pshmem_ctx_iget8
#pragma weak shmem_ctx_iget16 = pshmem_ctx_iget16
#define shmem_ctx_iget16 pshmem_ctx_iget16
#pragma weak shmem_ctx_iget32 = pshmem_ctx_iget32
#define shmem_ctx_iget32 pshmem_ctx_iget32
#pragma weak shmem_ctx_iget64 = pshmem_ctx_iget64
#define shmem_ctx_iget64 pshmem_ctx_iget64
#pragma weak shmem_ctx_iget128 = pshmem_ctx_iget128
#define shmem_ctx_iget128 pshmem_ctx_iget128
#endif /* ENABLE_PSHMEM */

#define SHMEM_CTX_SIZED_IGET(_size)                                     \
    void                                                                \
    shmem_ctx_iget##_size(shmem_ctx_t ctx,                              \
                          void *target, const void *source,             \
                          ptrdiff_t tst, ptrdiff_t sst,                 \
                          size_t nelems, int pe)                        \
    {                                                                   \
        size_t ti = 0, si = 0;                                          \
        size_t i;                                                       \
                                                                        \
        for (i = 0; i < nelems; i += 1) {                               \
            shmem_ctx_get##_size(ctx,                                   \
                                 (void *) &((char *) target)[ti],       \
                                 (void *) &((char *) source)[si],       \
                                 1, pe);                                \
            ti += tst;                                                  \
            si += sst;                                                  \
        }                                                               \
    }

SHMEM_CTX_SIZED_IGET(8)
SHMEM_CTX_SIZED_IGET(16)
SHMEM_CTX_SIZED_IGET(32)
SHMEM_CTX_SIZED_IGET(64)
SHMEM_CTX_SIZED_IGET(128)

#undef SHMEM_CTX_SIZED_IGET

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
        SHMEML_MUTEX_PROTECT(shmemc_ctx_get_nbi(ctx,                    \
                                                dest, src,              \
                                                sizeof(_type) * nelems, \
                                                pe));                   \
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

#define SHMEM_CTX_SIZED_GET_NBI(_size)                                  \
    void                                                                \
    shmem_ctx_get##_size##_nbi(shmem_ctx_t ctx,                         \
                               void *dest, const void *src,             \
                               size_t nelems, int pe)                   \
    {                                                                   \
        SHMEML_MUTEX_PROTECT(shmemc_ctx_get_nbi(ctx,                    \
                                                dest, src,              \
                                                _size * nelems,         \
                                                pe));                   \
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
    SHMEML_MUTEX_PROTECT(shmemc_ctx_get_nbi(ctx,
                                            dest, src,
                                            nelems,
                                            pe));
}

/* ------------------------------------------------------------------------ */


#ifdef ENABLE_PSHMEM
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

#pragma weak shmem_float_iget = pshmem_float_iget
#define shmem_float_iget pshmem_float_iget
#pragma weak shmem_double_iget = pshmem_double_iget
#define shmem_double_iget pshmem_double_iget
#pragma weak shmem_longdouble_iget = pshmem_longdouble_iget
#define shmem_longdouble_iget pshmem_longdouble_iget
#pragma weak shmem_char_iget = pshmem_char_iget
#define shmem_char_iget pshmem_char_iget
#pragma weak shmem_schar_iget = pshmem_schar_iget
#define shmem_schar_iget pshmem_schar_iget
#pragma weak shmem_short_iget = pshmem_short_iget
#define shmem_short_iget pshmem_short_iget
#pragma weak shmem_int_iget = pshmem_int_iget
#define shmem_int_iget pshmem_int_iget
#pragma weak shmem_long_iget = pshmem_long_iget
#define shmem_long_iget pshmem_long_iget
#pragma weak shmem_longlong_iget = pshmem_longlong_iget
#define shmem_longlong_iget pshmem_longlong_iget
#pragma weak shmem_uchar_iget = pshmem_uchar_iget
#define shmem_uchar_iget pshmem_uchar_iget
#pragma weak shmem_ushort_iget = pshmem_ushort_iget
#define shmem_ushort_iget pshmem_ushort_iget
#pragma weak shmem_uint_iget = pshmem_uint_iget
#define shmem_uint_iget pshmem_uint_iget
#pragma weak shmem_ulong_iget = pshmem_ulong_iget
#define shmem_ulong_iget pshmem_ulong_iget
#pragma weak shmem_ulonglong_iget = pshmem_ulonglong_iget
#define shmem_ulonglong_iget pshmem_ulonglong_iget
#pragma weak shmem_int8_iget = pshmem_int8_iget
#define shmem_int8_iget pshmem_int8_iget
#pragma weak shmem_int16_iget = pshmem_int16_iget
#define shmem_int16_iget pshmem_int16_iget
#pragma weak shmem_int32_iget = pshmem_int32_iget
#define shmem_int32_iget pshmem_int32_iget
#pragma weak shmem_int64_iget = pshmem_int64_iget
#define shmem_int64_iget pshmem_int64_iget
#pragma weak shmem_uint8_iget = pshmem_uint8_iget
#define shmem_uint8_iget pshmem_uint8_iget
#pragma weak shmem_uint16_iget = pshmem_uint16_iget
#define shmem_uint16_iget pshmem_uint16_iget
#pragma weak shmem_uint32_iget = pshmem_uint32_iget
#define shmem_uint32_iget pshmem_uint32_iget
#pragma weak shmem_uint64_iget = pshmem_uint64_iget
#define shmem_uint64_iget pshmem_uint64_iget

#pragma weak shmem_float_get_nbi = pshmem_float_get_nbi
#define shmem_float_get_nbi pshmem_float_get_nbi
#pragma weak shmem_double_get_nbi = pshmem_double_get_nbi
#define shmem_double_get_nbi pshmem_double_get_nbi
#pragma weak shmem_longdouble_get_nbi = pshmem_longdouble_get_nbi
#define shmem_longdouble_get_nbi pshmem_longdouble_get_nbi
#pragma weak shmem_char_get_nbi = pshmem_char_get_nbi
#define shmem_char_get_nbi pshmem_char_get_nbi
#pragma weak shmem_schar_get_nbi = pshmem_schar_get_nbi
#define shmem_schar_get_nbi pshmem_schar_get_nbi
#pragma weak shmem_short_get_nbi = pshmem_short_get_nbi
#define shmem_short_get_nbi pshmem_short_get_nbi
#pragma weak shmem_int_get_nbi = pshmem_int_get_nbi
#define shmem_int_get_nbi pshmem_int_get_nbi
#pragma weak shmem_long_get_nbi = pshmem_long_get_nbi
#define shmem_long_get_nbi pshmem_long_get_nbi
#pragma weak shmem_longlong_get_nbi = pshmem_longlong_get_nbi
#define shmem_longlong_get_nbi pshmem_longlong_get_nbi
#pragma weak shmem_uchar_get_nbi = pshmem_uchar_get_nbi
#define shmem_uchar_get_nbi pshmem_uchar_get_nbi
#pragma weak shmem_ushort_get_nbi = pshmem_ushort_get_nbi
#define shmem_ushort_get_nbi pshmem_ushort_get_nbi
#pragma weak shmem_uint_get_nbi = pshmem_uint_get_nbi
#define shmem_uint_get_nbi pshmem_uint_get_nbi
#pragma weak shmem_ulong_get_nbi = pshmem_ulong_get_nbi
#define shmem_ulong_get_nbi pshmem_ulong_get_nbi
#pragma weak shmem_ulonglong_get_nbi = pshmem_ulonglong_get_nbi
#define shmem_ulonglong_get_nbi pshmem_ulonglong_get_nbi
#pragma weak shmem_int8_get_nbi = pshmem_int8_get_nbi
#define shmem_int8_get_nbi pshmem_int8_get_nbi
#pragma weak shmem_int16_get_nbi = pshmem_int16_get_nbi
#define shmem_int16_get_nbi pshmem_int16_get_nbi
#pragma weak shmem_int32_get_nbi = pshmem_int32_get_nbi
#define shmem_int32_get_nbi pshmem_int32_get_nbi
#pragma weak shmem_int64_get_nbi = pshmem_int64_get_nbi
#define shmem_int64_get_nbi pshmem_int64_get_nbi
#pragma weak shmem_uint8_get_nbi = pshmem_uint8_get_nbi
#define shmem_uint8_get_nbi pshmem_uint8_get_nbi
#pragma weak shmem_uint16_get_nbi = pshmem_uint16_get_nbi
#define shmem_uint16_get_nbi pshmem_uint16_get_nbi
#pragma weak shmem_uint32_get_nbi = pshmem_uint32_get_nbi
#define shmem_uint32_get_nbi pshmem_uint32_get_nbi
#pragma weak shmem_uint64_get_nbi = pshmem_uint64_get_nbi
#define shmem_uint64_get_nbi pshmem_uint64_get_nbi
#endif /* ENABLE_PSHMEM */

/*
 * note use of shmem layer to allow the thread wrappers to propagate
 */

#define API_DECL_PUTGET(_opname, _name, _type)                          \
    void                                                                \
    shmem_##_name##_##_opname(_type *dest, const _type *src,            \
                              size_t nelems, int pe)                    \
    {                                                                   \
        shmem_ctx_##_name##_##_opname(SHMEM_CTX_DEFAULT,                \
                                      dest, src,                        \
                                      nelems, pe);                      \
    }                                                                   \
    void                                                                \
    shmem_##_name##_##_opname##_nbi(_type *dest,                        \
                                    const _type *src,                   \
                                    size_t nelems, int pe)              \
    {                                                                   \
        shmem_ctx_##_name##_##_opname##_nbi(SHMEM_CTX_DEFAULT,          \
                                            dest, src,                  \
                                            nelems, pe);                \
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
                                       sizeof(_type) * nelems, pe);     \
    }

API_DECL_PUTGET(get, float, float)
API_DECL_PUTGET(get, double, double)
API_DECL_PUTGET(get, longdouble, long double)
API_DECL_PUTGET(get, schar, signed char)
API_DECL_PUTGET(get, char, char)
API_DECL_PUTGET(get, short, short)
API_DECL_PUTGET(get, int, int)
API_DECL_PUTGET(get, long, long)
API_DECL_PUTGET(get, longlong, long long)
API_DECL_PUTGET(get, uchar, unsigned char)
API_DECL_PUTGET(get, ushort, unsigned short)
API_DECL_PUTGET(get, uint, unsigned int)
API_DECL_PUTGET(get, ulong, unsigned long)
API_DECL_PUTGET(get, ulonglong, unsigned long long)
API_DECL_PUTGET(get, int8, int8_t)
API_DECL_PUTGET(get, int16, int16_t)
API_DECL_PUTGET(get, int32, int32_t)
API_DECL_PUTGET(get, int64, int64_t)
API_DECL_PUTGET(get, uint8, uint8_t)
API_DECL_PUTGET(get, uint16, uint16_t)
API_DECL_PUTGET(get, uint32, uint32_t)
API_DECL_PUTGET(get, uint64, uint64_t)
API_DECL_PUTGET(get, size, size_t)
API_DECL_PUTGET(get, ptrdiff, ptrdiff_t)

#undef API_DECL_PUTGET

#ifdef ENABLE_PSHMEM
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

#pragma weak shmem_iget8 = pshmem_iget8
#define shmem_iget8 pshmem_iget8
#pragma weak shmem_iget16 = pshmem_iget16
#define shmem_iget16 pshmem_iget16
#pragma weak shmem_iget32 = pshmem_iget32
#define shmem_iget32 pshmem_iget32
#pragma weak shmem_iget64 = pshmem_iget64
#define shmem_iget64 pshmem_iget64
#pragma weak shmem_iget128 = pshmem_iget128
#define shmem_iget128 pshmem_iget128

#pragma weak shmem_get8_nbi = pshmem_get8_nbi
#define shmem_get8_nbi pshmem_get8_nbi
#pragma weak shmem_get16_nbi = pshmem_get16_nbi
#define shmem_get16_nbi pshmem_get16_nbi
#pragma weak shmem_get32_nbi = pshmem_get32_nbi
#define shmem_get32_nbi pshmem_get32_nbi
#pragma weak shmem_get64_nbi = pshmem_get64_nbi
#define shmem_get64_nbi pshmem_get64_nbi
#pragma weak shmem_get128_nbi = pshmem_get128_nbi
#define shmem_get128_nbi pshmem_get128_nbi
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
    shmem_##_opname##_size##_nbi(void *dest, const void *src,           \
                                 size_t nelems, int pe)                 \
    {                                                                   \
        shmem_ctx_##_opname##_size##_nbi(SHMEM_CTX_DEFAULT,             \
                                         dest, src, nelems, pe);        \
    }                                                                   \
    void                                                                \
    shmem_i##_opname##_size(void *dest, const void *src,                \
                            ptrdiff_t tst, ptrdiff_t sst,               \
                            size_t nelems, int pe)                      \
    {                                                                   \
        shmem_ctx_i##_opname##_size(SHMEM_CTX_DEFAULT,                  \
                                    dest, src, tst, sst,                \
                                    nelems, pe);                        \
    }

API_DECL_PUTGET_SIZE(get, 8)
API_DECL_PUTGET_SIZE(get, 16)
API_DECL_PUTGET_SIZE(get, 32)
API_DECL_PUTGET_SIZE(get, 64)
API_DECL_PUTGET_SIZE(get, 128)

#undef API_DECL_PUTGET_SIZE

#ifdef ENABLE_PSHMEM
#pragma weak shmem_getmem = pshmem_getmem
#define shmem_getmem pshmem_getmem
#pragma weak shmem_getmem_nbi = pshmem_getmem_nbi
#define shmem_getmem_nbi pshmem_getmem_nbi
#endif /* ENABLE_PSHMEM */

#define API_DECL_PUTGET_MEM(_opname)                                    \
    void                                                                \
    shmem_##_opname##mem(void *dest, const void *src,                   \
                         size_t nelems, int pe)                         \
    {                                                                   \
        shmem_ctx_##_opname##mem(SHMEM_CTX_DEFAULT,                     \
                                 dest, src, nelems, pe);                \
    }                                                                   \
    void                                                                \
    shmem_##_opname##mem_nbi(void *dest, const void *src,               \
                             size_t nelems, int pe)                     \
    {                                                                   \
        shmem_ctx_##_opname##mem_nbi(SHMEM_CTX_DEFAULT,                 \
                                     dest, src, nelems, pe);            \
    }

API_DECL_PUTGET_MEM(get)

#undef API_DECL_PUTGET_MEM

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
#pragma weak shmem_size_g = pshmem_size_g
#define shmem_size_g pshmem_size_g
#pragma weak shmem_ptrdiff_g = pshmem_ptrdiff_g
#define shmem_ptrdiff_g pshmem_ptrdiff_g
#endif /* ENABLE_PSHMEM */

#define API_DECL_G(_name, _type)                    \
    _type                                           \
    shmem_##_name##_g(const _type *src, int pe)     \
    {                                               \
        _type val;                                  \
                                                    \
        shmem_ctx_##_name##_get(SHMEM_CTX_DEFAULT,  \
                                &val, src, 1, pe);  \
        return val;                                 \
    }

API_DECL_G(float, float)
API_DECL_G(double, double)
API_DECL_G(longdouble, long double)
API_DECL_G(schar, signed char)
API_DECL_G(char, char)
API_DECL_G(short, short)
API_DECL_G(int, int)
API_DECL_G(long, long)
API_DECL_G(longlong, long long)
API_DECL_G(uchar, unsigned char)
API_DECL_G(ushort, unsigned short)
API_DECL_G(uint, unsigned int)
API_DECL_G(ulong, unsigned long)
API_DECL_G(ulonglong, unsigned long long)
API_DECL_G(int8, int8_t)
API_DECL_G(int16, int16_t)
API_DECL_G(int32, int32_t)
API_DECL_G(int64, int64_t)
API_DECL_G(uint8, uint8_t)
API_DECL_G(uint16, uint16_t)
API_DECL_G(uint32, uint32_t)
API_DECL_G(uint64, uint64_t)
API_DECL_G(size, size_t)
API_DECL_G(ptrdiff, ptrdiff_t)

#undef API_DECL_G
