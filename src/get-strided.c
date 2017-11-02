/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"

#include "shmem/api.h"

/*
 * these are needed for propagating into Fortran,
 * but aren't actually part of the API
 */
extern void shmem_complexf_get(COMPLEXIFY(float) *dest,
                               const COMPLEXIFY(float) *src,
                               size_t nelems,
                               int pe);
extern void shmem_complexd_get(COMPLEXIFY(double) *dest,
                               const COMPLEXIFY(double) *src,
                               size_t nelems,
                               int pe);

#ifdef ENABLE_PSHMEM
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
#endif /* ENABLE_PSHMEM */

#define SHMEM_TYPED_IGET(_name, _type)                                  \
    void                                                                \
    shmem_##_name##_iget(_type *target, const _type *source,            \
                         ptrdiff_t tst, ptrdiff_t sst,                  \
                         size_t nelems, int pe)                         \
    {                                                                   \
        const size_t the_size = sizeof(_type);                          \
        size_t ti = 0, si = 0;                                          \
        size_t i;                                                       \
                                                                        \
        for (i = 0; i < nelems; i += 1) {                               \
            shmemc_get(target + ti, source + si, the_size, pe);         \
            ti += tst;                                                  \
            si += sst;                                                  \
        }                                                               \
    }

SHMEM_TYPED_IGET(float, float)
SHMEM_TYPED_IGET(double, double)
SHMEM_TYPED_IGET(longdouble, long double)
SHMEM_TYPED_IGET(char, char)
SHMEM_TYPED_IGET(schar, signed char)
SHMEM_TYPED_IGET(short, short)
SHMEM_TYPED_IGET(int, int)
SHMEM_TYPED_IGET(long, long)
SHMEM_TYPED_IGET(longlong, long long)
SHMEM_TYPED_IGET(uchar, unsigned char)
SHMEM_TYPED_IGET(ushort, unsigned short)
SHMEM_TYPED_IGET(uint, unsigned int)
SHMEM_TYPED_IGET(ulong, unsigned long)
SHMEM_TYPED_IGET(ulonglong, unsigned long long)
SHMEM_TYPED_IGET(int8, int8_t)
SHMEM_TYPED_IGET(int16, int16_t)
SHMEM_TYPED_IGET(int32, int32_t)
SHMEM_TYPED_IGET(int64, int64_t)
SHMEM_TYPED_IGET(uint8, uint8_t)
SHMEM_TYPED_IGET(uint16, uint16_t)
SHMEM_TYPED_IGET(uint32, uint32_t)
SHMEM_TYPED_IGET(uint64, uint64_t)
SHMEM_TYPED_IGET(size, size_t)
SHMEM_TYPED_IGET(ptrdiff, ptrdiff_t)
/* for Fortran */
SHMEM_TYPED_IGET(complexf, COMPLEXIFY(float))
SHMEM_TYPED_IGET(complexd, COMPLEXIFY(double))

#define SHMEM_SIZED_IGET(_size)                                         \
    void                                                                \
    shmem_iget##_size(void *target, const void *source,                 \
                      ptrdiff_t tst, ptrdiff_t sst,                     \
                      size_t nelems, int pe)                            \
    {                                                                   \
        size_t ti = 0, si = 0;                                          \
        size_t i;                                                       \
                                                                        \
        for (i = 0; i < nelems; i += 1) {                               \
            shmemc_get(target + ti, source + si, _size, pe);            \
            ti += tst;                                                  \
            si += sst;                                                  \
        }                                                               \
    }

SHMEM_SIZED_IGET(8)
SHMEM_SIZED_IGET(16)
SHMEM_SIZED_IGET(32)
SHMEM_SIZED_IGET(64)
SHMEM_SIZED_IGET(128)
