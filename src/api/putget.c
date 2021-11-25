/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"

#include "putget.h"

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
#pragma weak shmem_ctx_size_put = pshmem_ctx_size_put
#define shmem_ctx_size_put pshmem_ctx_size_put
#pragma weak shmem_ctx_ptrdiff_put = pshmem_ctx_ptrdiff_put
#define shmem_ctx_ptrdiff_put pshmem_ctx_ptrdiff_put
#endif /* ENABLE_PSHMEM */

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

SHMEM_CTX_SIZED_PUT(8)
SHMEM_CTX_SIZED_PUT(16)
SHMEM_CTX_SIZED_PUT(32)
SHMEM_CTX_SIZED_PUT(64)
SHMEM_CTX_SIZED_PUT(128)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_putmem = pshmem_ctx_putmem
#define shmem_ctx_putmem pshmem_ctx_putmem
#endif /* ENABLE_PSHMEM */

SHMEM_CTX_PUTMEM()

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
#pragma weak shmem_ctx_size_p = pshmem_ctx_size_p
#define shmem_ctx_size_p pshmem_ctx_size_p
#pragma weak shmem_ctx_ptrdiff_p = pshmem_ctx_ptrdiff_p
#define shmem_ctx_ptrdiff_p pshmem_ctx_ptrdiff_p
#endif /* ENABLE_PSHMEM */

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
#pragma weak shmem_ctx_size_iput = pshmem_ctx_size_iput
#define shmem_ctx_size_iput pshmem_ctx_size_iput
#pragma weak shmem_ctx_ptrdiff_iput = pshmem_ctx_ptrdiff_iput
#define shmem_ctx_ptrdiff_iput pshmem_ctx_ptrdiff_iput
#endif /* ENABLE_PSHMEM */

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

SHMEM_CTX_SIZED_IPUT(8)
SHMEM_CTX_SIZED_IPUT(16)
SHMEM_CTX_SIZED_IPUT(32)
SHMEM_CTX_SIZED_IPUT(64)
SHMEM_CTX_SIZED_IPUT(128)

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
#pragma weak shmem_ctx_size_put_nbi = pshmem_ctx_size_put_nbi
#define shmem_ctx_size_put_nbi pshmem_ctx_size_put_nbi
#pragma weak shmem_ctx_ptrdiff_put_nbi = pshmem_ctx_ptrdiff_put_nbi
#define shmem_ctx_ptrdiff_put_nbi pshmem_ctx_ptrdiff_put_nbi
#endif /* ENABLE_PSHMEM */

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

SHMEM_CTX_SIZED_PUT_NBI(8)
SHMEM_CTX_SIZED_PUT_NBI(16)
SHMEM_CTX_SIZED_PUT_NBI(32)
SHMEM_CTX_SIZED_PUT_NBI(64)
SHMEM_CTX_SIZED_PUT_NBI(128)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_putmem_nbi = pshmem_ctx_putmem_nbi
#define shmem_ctx_putmem_nbi pshmem_ctx_putmem_nbi
#endif /* ENABLE_PSHMEM */

SHMEM_CTX_PUTMEM_NBI()

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
#pragma weak shmem_size_put = pshmem_size_put
#define shmem_size_put pshmem_size_put
#pragma weak shmem_ptrdiff_put = pshmem_ptrdiff_put
#define shmem_ptrdiff_put pshmem_ptrdiff_put

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
#pragma weak shmem_size_iput = pshmem_size_iput
#define shmem_size_iput pshmem_size_iput
#pragma weak shmem_ptrdiff_iput = pshmem_ptrdiff_iput
#define shmem_ptrdiff_iput pshmem_ptrdiff_iput

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
#pragma weak shmem_size_put_nbi = pshmem_size_put_nbi
#define shmem_size_put_nbi pshmem_size_put_nbi
#pragma weak shmem_ptrdiff_put_nbi = pshmem_ptrdiff_put_nbi
#define shmem_ptrdiff_put_nbi pshmem_ptrdiff_put_nbi
#endif /* ENABLE_PSHMEM */

API_DECL_TYPED_PUT(float, float)
API_DECL_TYPED_PUT(double, double)
API_DECL_TYPED_PUT(longdouble, long double)
API_DECL_TYPED_PUT(schar, signed char)
API_DECL_TYPED_PUT(char, char)
API_DECL_TYPED_PUT(short, short)
API_DECL_TYPED_PUT(int, int)
API_DECL_TYPED_PUT(long, long)
API_DECL_TYPED_PUT(longlong, long long)
API_DECL_TYPED_PUT(uchar, unsigned char)
API_DECL_TYPED_PUT(ushort, unsigned short)
API_DECL_TYPED_PUT(uint, unsigned int)
API_DECL_TYPED_PUT(ulong, unsigned long)
API_DECL_TYPED_PUT(ulonglong, unsigned long long)
API_DECL_TYPED_PUT(int8, int8_t)
API_DECL_TYPED_PUT(int16, int16_t)
API_DECL_TYPED_PUT(int32, int32_t)
API_DECL_TYPED_PUT(int64, int64_t)
API_DECL_TYPED_PUT(uint8, uint8_t)
API_DECL_TYPED_PUT(uint16, uint16_t)
API_DECL_TYPED_PUT(uint32, uint32_t)
API_DECL_TYPED_PUT(uint64, uint64_t)
API_DECL_TYPED_PUT(size, size_t)
API_DECL_TYPED_PUT(ptrdiff, ptrdiff_t)

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

API_DECL_SIZED_PUT(8)
API_DECL_SIZED_PUT(16)
API_DECL_SIZED_PUT(32)
API_DECL_SIZED_PUT(64)
API_DECL_SIZED_PUT(128)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_putmem = pshmem_putmem
#define shmem_putmem pshmem_putmem
#pragma weak shmem_putmem_nbi = pshmem_putmem_nbi
#define shmem_putmem_nbi pshmem_putmem_nbi
#endif /* ENABLE_PSHMEM */

API_DECL_PUTMEM()

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

API_DECL_TYPED_P(float, float)
API_DECL_TYPED_P(double, double)
API_DECL_TYPED_P(longdouble, long double)
API_DECL_TYPED_P(schar, signed char)
API_DECL_TYPED_P(char, char)
API_DECL_TYPED_P(short, short)
API_DECL_TYPED_P(int, int)
API_DECL_TYPED_P(long, long)
API_DECL_TYPED_P(longlong, long long)
API_DECL_TYPED_P(uchar, unsigned char)
API_DECL_TYPED_P(ushort, unsigned short)
API_DECL_TYPED_P(uint, unsigned int)
API_DECL_TYPED_P(ulong, unsigned long)
API_DECL_TYPED_P(ulonglong, unsigned long long)
API_DECL_TYPED_P(int8, int8_t)
API_DECL_TYPED_P(int16, int16_t)
API_DECL_TYPED_P(int32, int32_t)
API_DECL_TYPED_P(int64, int64_t)
API_DECL_TYPED_P(uint8, uint8_t)
API_DECL_TYPED_P(uint16, uint16_t)
API_DECL_TYPED_P(uint32, uint32_t)
API_DECL_TYPED_P(uint64, uint64_t)
API_DECL_TYPED_P(size, size_t)
API_DECL_TYPED_P(ptrdiff, ptrdiff_t)

/*
 * -- get --------------------------------------------------------------
 */

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
#pragma weak shmem_ctx_size_get = pshmem_ctx_size_get
#define shmem_ctx_size_get pshmem_ctx_size_get
#pragma weak shmem_ctx_ptrdiff_get = pshmem_ctx_ptrdiff_get
#define shmem_ctx_ptrdiff_get pshmem_ctx_ptrdiff_get
#endif /* ENABLE_PSHMEM */

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

SHMEM_CTX_SIZED_GET(8)
SHMEM_CTX_SIZED_GET(16)
SHMEM_CTX_SIZED_GET(32)
SHMEM_CTX_SIZED_GET(64)
SHMEM_CTX_SIZED_GET(128)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_getmem = pshmem_ctx_getmem
#define shmem_ctx_getmem pshmem_ctx_getmem
#endif /* ENABLE_PSHMEM */

SHMEM_CTX_GETMEM()

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
#pragma weak shmem_ctx_size_g = pshmem_ctx_size_g
#define shmem_ctx_size_g pshmem_ctx_size_g
#pragma weak shmem_ctx_ptrdiff_g = pshmem_ctx_ptrdiff_g
#define shmem_ctx_ptrdiff_g pshmem_ctx_ptrdiff_g
#endif /* ENABLE_PSHMEM */

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
#pragma weak shmem_ctx_size_iget = pshmem_ctx_size_iget
#define shmem_ctx_size_iget pshmem_ctx_size_iget
#pragma weak shmem_ctx_ptrdiff_iget = pshmem_ctx_ptrdiff_iget
#define shmem_ctx_ptrdiff_iget pshmem_ctx_ptrdiff_iget
#endif /* ENABLE_PSHMEM */

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

SHMEM_CTX_SIZED_IGET(8)
SHMEM_CTX_SIZED_IGET(16)
SHMEM_CTX_SIZED_IGET(32)
SHMEM_CTX_SIZED_IGET(64)
SHMEM_CTX_SIZED_IGET(128)

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
#pragma weak shmem_ctx_size_get_nbi = pshmem_ctx_size_get_nbi
#define shmem_ctx_size_get_nbi pshmem_ctx_size_get_nbi
#pragma weak shmem_ctx_ptrdiff_get_nbi = pshmem_ctx_ptrdiff_get_nbi
#define shmem_ctx_ptrdiff_get_nbi pshmem_ctx_ptrdiff_get_nbi
#endif /* ENABLE_PSHMEM */

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

SHMEM_CTX_SIZED_GET_NBI(8)
SHMEM_CTX_SIZED_GET_NBI(16)
SHMEM_CTX_SIZED_GET_NBI(32)
SHMEM_CTX_SIZED_GET_NBI(64)
SHMEM_CTX_SIZED_GET_NBI(128)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_getmem_nbi = pshmem_ctx_getmem_nbi
#define shmem_ctx_getmem_nbi pshmem_ctx_getmem_nbi
#endif /* ENABLE_PSHMEM */

SHMEM_CTX_GETMEM_NBI()

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
#pragma weak shmem_size_get = pshmem_size_get
#define shmem_size_get pshmem_size_get
#pragma weak shmem_ptrdiff_get = pshmem_ptrdiff_get
#define shmem_ptrdiff_get pshmem_ptrdiff_get

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
#pragma weak shmem_size_iget = pshmem_size_iget
#define shmem_size_iget pshmem_size_iget
#pragma weak shmem_ptrdiff_iget = pshmem_ptrdiff_iget
#define shmem_ptrdiff_iget pshmem_ptrdiff_iget

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
#pragma weak shmem_size_get_nbi = pshmem_size_get_nbi
#define shmem_size_get_nbi pshmem_size_get_nbi
#pragma weak shmem_ptrdiff_get_nbi = pshmem_ptrdiff_get_nbi
#define shmem_ptrdiff_get_nbi pshmem_ptrdiff_get_nbi
#endif /* ENABLE_PSHMEM */

API_DECL_TYPED_GET(float, float)
API_DECL_TYPED_GET(double, double)
API_DECL_TYPED_GET(longdouble, long double)
API_DECL_TYPED_GET(schar, signed char)
API_DECL_TYPED_GET(char, char)
API_DECL_TYPED_GET(short, short)
API_DECL_TYPED_GET(int, int)
API_DECL_TYPED_GET(long, long)
API_DECL_TYPED_GET(longlong, long long)
API_DECL_TYPED_GET(uchar, unsigned char)
API_DECL_TYPED_GET(ushort, unsigned short)
API_DECL_TYPED_GET(uint, unsigned int)
API_DECL_TYPED_GET(ulong, unsigned long)
API_DECL_TYPED_GET(ulonglong, unsigned long long)
API_DECL_TYPED_GET(int8, int8_t)
API_DECL_TYPED_GET(int16, int16_t)
API_DECL_TYPED_GET(int32, int32_t)
API_DECL_TYPED_GET(int64, int64_t)
API_DECL_TYPED_GET(uint8, uint8_t)
API_DECL_TYPED_GET(uint16, uint16_t)
API_DECL_TYPED_GET(uint32, uint32_t)
API_DECL_TYPED_GET(uint64, uint64_t)
API_DECL_TYPED_GET(size, size_t)
API_DECL_TYPED_GET(ptrdiff, ptrdiff_t)

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

API_DECL_SIZED_GET(8)
API_DECL_SIZED_GET(16)
API_DECL_SIZED_GET(32)
API_DECL_SIZED_GET(64)
API_DECL_SIZED_GET(128)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_getmem = pshmem_getmem
#define shmem_getmem pshmem_getmem
#pragma weak shmem_getmem_nbi = pshmem_getmem_nbi
#define shmem_getmem_nbi pshmem_getmem_nbi
#endif /* ENABLE_PSHMEM */

API_DECL_GETMEM()

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

API_DECL_TYPED_G(float, float)
API_DECL_TYPED_G(double, double)
API_DECL_TYPED_G(longdouble, long double)
API_DECL_TYPED_G(schar, signed char)
API_DECL_TYPED_G(char, char)
API_DECL_TYPED_G(short, short)
API_DECL_TYPED_G(int, int)
API_DECL_TYPED_G(long, long)
API_DECL_TYPED_G(longlong, long long)
API_DECL_TYPED_G(uchar, unsigned char)
API_DECL_TYPED_G(ushort, unsigned short)
API_DECL_TYPED_G(uint, unsigned int)
API_DECL_TYPED_G(ulong, unsigned long)
API_DECL_TYPED_G(ulonglong, unsigned long long)
API_DECL_TYPED_G(int8, int8_t)
API_DECL_TYPED_G(int16, int16_t)
API_DECL_TYPED_G(int32, int32_t)
API_DECL_TYPED_G(int64, int64_t)
API_DECL_TYPED_G(uint8, uint8_t)
API_DECL_TYPED_G(uint16, uint16_t)
API_DECL_TYPED_G(uint32, uint32_t)
API_DECL_TYPED_G(uint64, uint64_t)
API_DECL_TYPED_G(size, size_t)
API_DECL_TYPED_G(ptrdiff, ptrdiff_t)
