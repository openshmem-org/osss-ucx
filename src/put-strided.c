/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem/defs.h"
#include "shmem/api.h"

/*
 * TODO: this is just layered over shmem_put/get linearly for now.
 * Looking for better iov method in UCX
 */


/*
 * these are needed for propagating into Fortran,
 * but aren't actually part of the API
 */
extern void shmem_complexf_put(COMPLEXIFY(float) *dest,
                               const COMPLEXIFY(float) *src,
                               size_t nelems,
                               int pe);
extern void shmem_complexd_put(COMPLEXIFY(double) *dest,
                               const COMPLEXIFY(double) *src,
                               size_t nelems,
                               int pe);

#ifdef ENABLE_PSHMEM
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
#endif /* ENABLE_PSHMEM */

#define SHMEM_TYPED_IPUT(_name, _type)                                  \
    void                                                                \
    shmem_##_name##_iput(_type *target, const _type *source,            \
                         ptrdiff_t tst, ptrdiff_t sst,                  \
                         size_t nelems, int pe)                         \
    {                                                                   \
        size_t ti = 0, si = 0;                                          \
        size_t i;                                                       \
                                                                        \
        for (i = 0; i < nelems; i += 1) {                               \
            shmem_##_name##_put(target + ti, source + si, 1, pe);       \
            ti += tst;                                                  \
            si += sst;                                                  \
        }                                                               \
    }

SHMEM_TYPED_IPUT(float, float)
SHMEM_TYPED_IPUT(double, double)
SHMEM_TYPED_IPUT(longdouble, long double)
SHMEM_TYPED_IPUT(char, char)
SHMEM_TYPED_IPUT(schar, signed char)
SHMEM_TYPED_IPUT(short, short)
SHMEM_TYPED_IPUT(int, int)
SHMEM_TYPED_IPUT(long, long)
SHMEM_TYPED_IPUT(longlong, long long)
SHMEM_TYPED_IPUT(uchar, unsigned char)
SHMEM_TYPED_IPUT(ushort, unsigned short)
SHMEM_TYPED_IPUT(uint, unsigned int)
SHMEM_TYPED_IPUT(ulong, unsigned long)
SHMEM_TYPED_IPUT(ulonglong, unsigned long long)
SHMEM_TYPED_IPUT(int8, int8_t)
SHMEM_TYPED_IPUT(int16, int16_t)
SHMEM_TYPED_IPUT(int32, int32_t)
SHMEM_TYPED_IPUT(int64, int64_t)
SHMEM_TYPED_IPUT(uint8, uint8_t)
SHMEM_TYPED_IPUT(uint16, uint16_t)
SHMEM_TYPED_IPUT(uint32, uint32_t)
SHMEM_TYPED_IPUT(uint64, uint64_t)
SHMEM_TYPED_IPUT(size, size_t)
SHMEM_TYPED_IPUT(ptrdiff, ptrdiff_t)
/* for Fortran */
SHMEM_TYPED_IPUT(complexf, COMPLEXIFY(float))
SHMEM_TYPED_IPUT(complexd, COMPLEXIFY(double))

#define SHMEM_SIZED_IPUT(_size)                                         \
    void                                                                \
    shmem_iput##_size(void *target, const void *source,                 \
                      ptrdiff_t tst, ptrdiff_t sst,                     \
                      size_t nelems, int pe)                            \
    {                                                                   \
        size_t ti = 0, si = 0;                                          \
        size_t i;                                                       \
                                                                        \
        for (i = 0; i < nelems; i += 1) {                               \
            shmem_put##_size(target + ti, source + si, 1, pe);          \
            ti += tst;                                                  \
            si += sst;                                                  \
        }                                                               \
    }

SHMEM_SIZED_IPUT(8)
SHMEM_SIZED_IPUT(16)
SHMEM_SIZED_IPUT(32)
SHMEM_SIZED_IPUT(64)
SHMEM_SIZED_IPUT(128)
