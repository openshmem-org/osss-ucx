/* For license: see LICENSE file at top-level */

#ifndef _PSHMEM_H
#define _PSHMEM_H 1

#include "shmem.h"

#include <stdint.h>             /* uint*_t */

#ifdef __cplusplus
extern "C"
{
#endif  /* __cplusplus */

    /*
     * init & query
     */

    void pstart_pes(int npes);
    void pshmem_init(void);
    void pshmem_finalize(void);

    void pshmem_info_get_version(int *major, int *minor);
    void pshmem_info_get_name(char *name);

    void pshmem_init_thread(int req, int *prov);
    int pshmem_query_thread(int *prov) _WUR;

    void pshmem_global_exit(int status);

    int p_my_pe(void) _WUR;
    int pshmem_my_pe(void) _WUR;

    int p_num_pes(void) _WUR;
    int pshmem_n_pes(void) _WUR;

    /*
     * I/O
     */

#define PSHMEM_DECL_PUTGET(_opname, _name, _type)                       \
    void pshmem_##_name##_##_opname(_type *dest, const _type *src,      \
                                    size_t nelems, int pe);

    PSHMEM_DECL_PUTGET(put, float, float)
    PSHMEM_DECL_PUTGET(put, double, double)
    PSHMEM_DECL_PUTGET(put, longdouble, long double)
    PSHMEM_DECL_PUTGET(put, schar, signed char)
    PSHMEM_DECL_PUTGET(put, char, char)
    PSHMEM_DECL_PUTGET(put, short, short)
    PSHMEM_DECL_PUTGET(put, int, int)
    PSHMEM_DECL_PUTGET(put, long, long)
    PSHMEM_DECL_PUTGET(put, longlong, long long)
    PSHMEM_DECL_PUTGET(put, uchar, unsigned char)
    PSHMEM_DECL_PUTGET(put, ushort, unsigned short)
    PSHMEM_DECL_PUTGET(put, uint, unsigned int)
    PSHMEM_DECL_PUTGET(put, ulong, unsigned long)
    PSHMEM_DECL_PUTGET(put, ulonglong, unsigned long long)
    PSHMEM_DECL_PUTGET(put, int8, int8_t)
    PSHMEM_DECL_PUTGET(put, int16, int16_t)
    PSHMEM_DECL_PUTGET(put, int32, int32_t)
    PSHMEM_DECL_PUTGET(put, int64, int64_t)
    PSHMEM_DECL_PUTGET(put, uint8, uint8_t)
    PSHMEM_DECL_PUTGET(put, uint16, uint16_t)
    PSHMEM_DECL_PUTGET(put, uint32, uint32_t)
    PSHMEM_DECL_PUTGET(put, uint64, uint64_t)
    PSHMEM_DECL_PUTGET(put, size, size_t)
    PSHMEM_DECL_PUTGET(put, ptrdiff, ptrdiff_t)

    PSHMEM_DECL_PUTGET(get, float, float)
    PSHMEM_DECL_PUTGET(get, double, double)
    PSHMEM_DECL_PUTGET(get, longdouble, long double)
    PSHMEM_DECL_PUTGET(get, schar, signed char)
    PSHMEM_DECL_PUTGET(get, char, char)
    PSHMEM_DECL_PUTGET(get, short, short)
    PSHMEM_DECL_PUTGET(get, int, int)
    PSHMEM_DECL_PUTGET(get, long, long)
    PSHMEM_DECL_PUTGET(get, longlong, long long)
    PSHMEM_DECL_PUTGET(get, uchar, unsigned char)
    PSHMEM_DECL_PUTGET(get, ushort, unsigned short)
    PSHMEM_DECL_PUTGET(get, uint, unsigned int)
    PSHMEM_DECL_PUTGET(get, ulong, unsigned long)
    PSHMEM_DECL_PUTGET(get, ulonglong, unsigned long long)
    PSHMEM_DECL_PUTGET(get, int8, int8_t)
    PSHMEM_DECL_PUTGET(get, int16, int16_t)
    PSHMEM_DECL_PUTGET(get, int32, int32_t)
    PSHMEM_DECL_PUTGET(get, int64, int64_t)
    PSHMEM_DECL_PUTGET(get, uint8, uint8_t)
    PSHMEM_DECL_PUTGET(get, uint16, uint16_t)
    PSHMEM_DECL_PUTGET(get, uint32, uint32_t)
    PSHMEM_DECL_PUTGET(get, uint64, uint64_t)
    PSHMEM_DECL_PUTGET(get, size, size_t)
    PSHMEM_DECL_PUTGET(get, ptrdiff, ptrdiff_t)

#undef PSHMEM_DECL_PUTGET

#define PSHMEM_DECL_PUTGET_SIZE(_opname, _size)                     \
        void pshmem_##_opname##_size(void *dest, const void *src,   \
                                     size_t nelems, int pe);

        PSHMEM_DECL_PUTGET_SIZE(put, 8)
    PSHMEM_DECL_PUTGET_SIZE(put, 16)
    PSHMEM_DECL_PUTGET_SIZE(put, 32)
    PSHMEM_DECL_PUTGET_SIZE(put, 64)
    PSHMEM_DECL_PUTGET_SIZE(put, 128)

    PSHMEM_DECL_PUTGET_SIZE(get, 8)
    PSHMEM_DECL_PUTGET_SIZE(get, 16)
    PSHMEM_DECL_PUTGET_SIZE(get, 32)
    PSHMEM_DECL_PUTGET_SIZE(get, 64)
    PSHMEM_DECL_PUTGET_SIZE(get, 128)

#undef PSHMEM_DECL_PUTGET_SIZE

    void pshmem_putmem(void *dest, const void *src, size_t nelems,
                       int pe);

    void pshmem_getmem(void *dest, const void *src, size_t nelems,
                       int pe);

#define PSHMEM_DECL_P(_name, _type)                             \
    void pshmem_##_name##_p(_type *dest, _type src, int pe);

    PSHMEM_DECL_P(float, float)
    PSHMEM_DECL_P(double, double)
    PSHMEM_DECL_P(longdouble, long double)
    PSHMEM_DECL_P(schar, signed char)
    PSHMEM_DECL_P(char, char)
    PSHMEM_DECL_P(short, short)
    PSHMEM_DECL_P(int, int)
    PSHMEM_DECL_P(long, long)
    PSHMEM_DECL_P(longlong, long long)
    PSHMEM_DECL_P(uchar, unsigned char)
    PSHMEM_DECL_P(ushort, unsigned short)
    PSHMEM_DECL_P(uint, unsigned int)
    PSHMEM_DECL_P(ulong, unsigned long)
    PSHMEM_DECL_P(ulonglong, unsigned long long)
    PSHMEM_DECL_P(int8, int8_t)
    PSHMEM_DECL_P(int16, int16_t)
    PSHMEM_DECL_P(int32, int32_t)
    PSHMEM_DECL_P(int64, int64_t)
    PSHMEM_DECL_P(uint8, uint8_t)
    PSHMEM_DECL_P(uint16, uint16_t)
    PSHMEM_DECL_P(uint32, uint32_t)
    PSHMEM_DECL_P(uint64, uint64_t)
    PSHMEM_DECL_P(size, size_t)
    PSHMEM_DECL_P(ptrdiff, ptrdiff_t)

#undef PSHMEM_DECL_P

#define PSHMEM_DECL_G(_name, _type)                 \
    _type pshmem_##_name##_g(const _type *dest, int pe);

    PSHMEM_DECL_G(float, float)
    PSHMEM_DECL_G(double, double)
    PSHMEM_DECL_G(longdouble, long double)
    PSHMEM_DECL_G(schar, signed char)
    PSHMEM_DECL_G(char, char)
    PSHMEM_DECL_G(short, short)
    PSHMEM_DECL_G(int, int)
    PSHMEM_DECL_G(long, long)
    PSHMEM_DECL_G(longlong, long long)
    PSHMEM_DECL_G(uchar, unsigned char)
    PSHMEM_DECL_G(ushort, unsigned short)
    PSHMEM_DECL_G(uint, unsigned int)
    PSHMEM_DECL_G(ulong, unsigned long)
    PSHMEM_DECL_G(ulonglong, unsigned long long)
    PSHMEM_DECL_G(int8, int8_t)
    PSHMEM_DECL_G(int16, int16_t)
    PSHMEM_DECL_G(int32, int32_t)
    PSHMEM_DECL_G(int64, int64_t)
    PSHMEM_DECL_G(uint8, uint8_t)
    PSHMEM_DECL_G(uint16, uint16_t)
    PSHMEM_DECL_G(uint32, uint32_t)
    PSHMEM_DECL_G(uint64, uint64_t)
    PSHMEM_DECL_G(size, size_t)
    PSHMEM_DECL_G(ptrdiff, ptrdiff_t)

#undef PSHMEM_DECL_G

    /*
     * strided I/O
     */

#define PSHMEM_DECL_STRIDED_OP(_opname, _name, _type)                   \
    void pshmem_##_name##_i##_opname(_type *dest, const _type *src,     \
                                     ptrdiff_t tst, ptrdiff_t sst,      \
                                     size_t nelems,                     \
                                     int pe);

    PSHMEM_DECL_STRIDED_OP(put, float, float)
    PSHMEM_DECL_STRIDED_OP(put, double, double)
    PSHMEM_DECL_STRIDED_OP(put, longdouble, long double)
    PSHMEM_DECL_STRIDED_OP(put, schar, signed char)
    PSHMEM_DECL_STRIDED_OP(put, char, char)
    PSHMEM_DECL_STRIDED_OP(put, short, short)
    PSHMEM_DECL_STRIDED_OP(put, int, int)
    PSHMEM_DECL_STRIDED_OP(put, long, long)
    PSHMEM_DECL_STRIDED_OP(put, longlong, long long)
    PSHMEM_DECL_STRIDED_OP(put, uchar, unsigned char)
    PSHMEM_DECL_STRIDED_OP(put, ushort, unsigned short)
    PSHMEM_DECL_STRIDED_OP(put, uint, unsigned int)
    PSHMEM_DECL_STRIDED_OP(put, ulong, unsigned long)
    PSHMEM_DECL_STRIDED_OP(put, ulonglong, unsigned long long)
    PSHMEM_DECL_STRIDED_OP(put, int8, int8_t)
    PSHMEM_DECL_STRIDED_OP(put, int16, int16_t)
    PSHMEM_DECL_STRIDED_OP(put, int32, int32_t)
    PSHMEM_DECL_STRIDED_OP(put, int64, int64_t)
    PSHMEM_DECL_STRIDED_OP(put, uint8, uint8_t)
    PSHMEM_DECL_STRIDED_OP(put, uint16, uint16_t)
    PSHMEM_DECL_STRIDED_OP(put, uint32, uint32_t)
    PSHMEM_DECL_STRIDED_OP(put, uint64, uint64_t)
    PSHMEM_DECL_STRIDED_OP(put, size, size_t)
    PSHMEM_DECL_STRIDED_OP(put, ptrdiff, ptrdiff_t)

    PSHMEM_DECL_STRIDED_OP(get, float, float)
    PSHMEM_DECL_STRIDED_OP(get, double, double)
    PSHMEM_DECL_STRIDED_OP(get, longdouble, long double)
    PSHMEM_DECL_STRIDED_OP(get, schar, signed char)
    PSHMEM_DECL_STRIDED_OP(get, char, char)
    PSHMEM_DECL_STRIDED_OP(get, short, short)
    PSHMEM_DECL_STRIDED_OP(get, int, int)
    PSHMEM_DECL_STRIDED_OP(get, long, long)
    PSHMEM_DECL_STRIDED_OP(get, longlong, long long)
    PSHMEM_DECL_STRIDED_OP(get, uchar, unsigned char)
    PSHMEM_DECL_STRIDED_OP(get, ushort, unsigned short)
    PSHMEM_DECL_STRIDED_OP(get, uint, unsigned int)
    PSHMEM_DECL_STRIDED_OP(get, ulong, unsigned long)
    PSHMEM_DECL_STRIDED_OP(get, ulonglong, unsigned long long)
    PSHMEM_DECL_STRIDED_OP(get, int8, int8_t)
    PSHMEM_DECL_STRIDED_OP(get, int16, int16_t)
    PSHMEM_DECL_STRIDED_OP(get, int32, int32_t)
    PSHMEM_DECL_STRIDED_OP(get, int64, int64_t)
    PSHMEM_DECL_STRIDED_OP(get, uint8, uint8_t)
    PSHMEM_DECL_STRIDED_OP(get, uint16, uint16_t)
    PSHMEM_DECL_STRIDED_OP(get, uint32, uint32_t)
    PSHMEM_DECL_STRIDED_OP(get, uint64, uint64_t)
    PSHMEM_DECL_STRIDED_OP(get, size, size_t)
    PSHMEM_DECL_STRIDED_OP(get, ptrdiff, ptrdiff_t)

#undef PSHMEM_DECL_STRIDED_OP

#define PSHMEM_DECL_STRIDED_OPSIZE(_opname, _size)                      \
    void pshmem_i##_opname##_size(void *dest, const void *src,          \
                                     ptrdiff_t tst, ptrdiff_t sst,      \
                                     size_t nelems,                     \
                                     int pe);

    PSHMEM_DECL_STRIDED_OPSIZE(put, 8)
    PSHMEM_DECL_STRIDED_OPSIZE(put, 16)
    PSHMEM_DECL_STRIDED_OPSIZE(put, 32)
    PSHMEM_DECL_STRIDED_OPSIZE(put, 64)
    PSHMEM_DECL_STRIDED_OPSIZE(put, 128)

    PSHMEM_DECL_STRIDED_OPSIZE(get, 8)
    PSHMEM_DECL_STRIDED_OPSIZE(get, 16)
    PSHMEM_DECL_STRIDED_OPSIZE(get, 32)
    PSHMEM_DECL_STRIDED_OPSIZE(get, 64)
    PSHMEM_DECL_STRIDED_OPSIZE(get, 128)

#undef PSHMEM_DECL_STRIDED_OPSIZE

    /*
     * non-blocking implicit put/get
     *
     */

#define PSHMEM_DECL_PUTGET_NBI(_opname, _name, _type)                   \
    void pshmem_##_name##_##_opname##_nbi(_type *dest, const _type *src, \
                                          size_t nelems, int pe);

    PSHMEM_DECL_PUTGET_NBI(put, float, float)
    PSHMEM_DECL_PUTGET_NBI(put, double, double)
    PSHMEM_DECL_PUTGET_NBI(put, longdouble, long double)
    PSHMEM_DECL_PUTGET_NBI(put, schar, signed char)
    PSHMEM_DECL_PUTGET_NBI(put, char, char)
    PSHMEM_DECL_PUTGET_NBI(put, short, short)
    PSHMEM_DECL_PUTGET_NBI(put, int, int)
    PSHMEM_DECL_PUTGET_NBI(put, long, long)
    PSHMEM_DECL_PUTGET_NBI(put, longlong, long long)
    PSHMEM_DECL_PUTGET_NBI(put, uchar, unsigned char)
    PSHMEM_DECL_PUTGET_NBI(put, ushort, unsigned short)
    PSHMEM_DECL_PUTGET_NBI(put, uint, unsigned int)
    PSHMEM_DECL_PUTGET_NBI(put, ulong, unsigned long)
    PSHMEM_DECL_PUTGET_NBI(put, ulonglong, unsigned long long)
    PSHMEM_DECL_PUTGET_NBI(put, int8, int8_t)
    PSHMEM_DECL_PUTGET_NBI(put, int16, int16_t)
    PSHMEM_DECL_PUTGET_NBI(put, int32, int32_t)
    PSHMEM_DECL_PUTGET_NBI(put, int64, int64_t)
    PSHMEM_DECL_PUTGET_NBI(put, uint8, uint8_t)
    PSHMEM_DECL_PUTGET_NBI(put, uint16, uint16_t)
    PSHMEM_DECL_PUTGET_NBI(put, uint32, uint32_t)
    PSHMEM_DECL_PUTGET_NBI(put, uint64, uint64_t)
    PSHMEM_DECL_PUTGET_NBI(put, size, size_t)
    PSHMEM_DECL_PUTGET_NBI(put, ptrdiff, ptrdiff_t)

    PSHMEM_DECL_PUTGET_NBI(get, float, float)
    PSHMEM_DECL_PUTGET_NBI(get, double, double)
    PSHMEM_DECL_PUTGET_NBI(get, longdouble, long double)
    PSHMEM_DECL_PUTGET_NBI(get, schar, signed char)
    PSHMEM_DECL_PUTGET_NBI(get, char, char)
    PSHMEM_DECL_PUTGET_NBI(get, short, short)
    PSHMEM_DECL_PUTGET_NBI(get, int, int)
    PSHMEM_DECL_PUTGET_NBI(get, long, long)
    PSHMEM_DECL_PUTGET_NBI(get, longlong, long long)
    PSHMEM_DECL_PUTGET_NBI(get, uchar, unsigned char)
    PSHMEM_DECL_PUTGET_NBI(get, ushort, unsigned short)
    PSHMEM_DECL_PUTGET_NBI(get, uint, unsigned int)
    PSHMEM_DECL_PUTGET_NBI(get, ulong, unsigned long)
    PSHMEM_DECL_PUTGET_NBI(get, ulonglong, unsigned long long)
    PSHMEM_DECL_PUTGET_NBI(get, int8, int8_t)
    PSHMEM_DECL_PUTGET_NBI(get, int16, int16_t)
    PSHMEM_DECL_PUTGET_NBI(get, int32, int32_t)
    PSHMEM_DECL_PUTGET_NBI(get, int64, int64_t)
    PSHMEM_DECL_PUTGET_NBI(get, uint8, uint8_t)
    PSHMEM_DECL_PUTGET_NBI(get, uint16, uint16_t)
    PSHMEM_DECL_PUTGET_NBI(get, uint32, uint32_t)
    PSHMEM_DECL_PUTGET_NBI(get, uint64, uint64_t)
    PSHMEM_DECL_PUTGET_NBI(get, size, size_t)
    PSHMEM_DECL_PUTGET_NBI(get, ptrdiff, ptrdiff_t)

#undef PSHMEM_DECL_PUTGET_NBI

#define PSHMEM_DECL_PUTGET_SIZE_NBI(_opname, _size)                     \
    void pshmem_##_opname##_size##_nbi(void *dest, const void *src,     \
                                       size_t nelems, int pe);

    PSHMEM_DECL_PUTGET_SIZE_NBI(put, 8)
    PSHMEM_DECL_PUTGET_SIZE_NBI(put, 16)
    PSHMEM_DECL_PUTGET_SIZE_NBI(put, 32)
    PSHMEM_DECL_PUTGET_SIZE_NBI(put, 64)
    PSHMEM_DECL_PUTGET_SIZE_NBI(put, 128)

    PSHMEM_DECL_PUTGET_SIZE_NBI(get, 8)
    PSHMEM_DECL_PUTGET_SIZE_NBI(get, 16)
    PSHMEM_DECL_PUTGET_SIZE_NBI(get, 32)
    PSHMEM_DECL_PUTGET_SIZE_NBI(get, 64)
    PSHMEM_DECL_PUTGET_SIZE_NBI(get, 128)

#undef PSHMEM_DECL_PUTGET_NBI

    void pshmem_putmem_nbi(void *dest, const void *source, size_t nelems,
                           int pe);

    void pshmem_getmem_nbi(void *dest, const void *source, size_t nelems,
                           int pe);

#ifdef __STDC_VERSION__
#if  __STDC_VERSION__ >= 201112L

    /*
     * C11 Generic variants
     *
     */

    /* see \ref shmem_long_put() */
#define pshmem_put(dest, source, nelems, pe)                            \
    _Generic(*(dest),                                                   \
             float:       pshmem_float_put,                             \
             double:      pshmem_double_put,                            \
             long double: pshmem_longdouble_put,                        \
             char:        pshmem_char_put,                              \
             short:       pshmem_short_put,                             \
             int:         pshmem_int_put,                               \
             long:        pshmem_long_put,                              \
             long long:   pshmem_longlong_put)(dest, source, nelems, pe)

    /* see \ref shmem_long_get() */
#define pshmem_get(dest, source, nelems, pe)                            \
    _Generic(*(dest),                                                   \
             float:       pshmem_float_get,                             \
             double:      pshmem_double_get,                            \
             long double: pshmem_longdouble_get,                        \
             char:        pshmem_char_get,                              \
             short:       pshmem_short_get,                             \
             int:         pshmem_int_get,                               \
             long:        pshmem_long_get,                              \
             long long:   pshmem_longlong_get)(dest, source, nelems, pe)

    /* see \ref shmem_long_p() */
#define pshmem_p(dest, value, pe)                               \
    _Generic(*(dest),                                           \
             float:       pshmem_float_p,                       \
             double:      pshmem_double_p,                      \
             long double: pshmem_longdouble_p,                  \
             char:        pshmem_char_p,                        \
             short:       pshmem_short_p,                       \
             int:         pshmem_int_p,                         \
             long:        pshmem_long_p,                        \
             long long:   pshmem_longlong_p)(dest, value, pe)


    /* see \ref shmem_long_g() */
#define pshmem_g(addr, pe)                              \
    _Generic((addr),                                    \
             float:       pshmem_float_g,               \
             double:      pshmem_double_g,              \
             long double: pshmem_longdouble_g,          \
             char:        pshmem_char_g,                \
             short:       pshmem_short_g,               \
             int:         pshmem_int_g,                 \
             long:        pshmem_long_g,                \
             long long:   pshmem_longlong_g)(addr, pe)

    /* see \ref shmem_long_iput() */
#define pshmem_iput(dest, source, dst, sst, nelems, pe)                 \
    _Generic(*(dest),                                                   \
             float:       pshmem_float_iput,                            \
             double:      pshmem_double_iput,                           \
             long double: pshmem_longdouble_iput,                       \
             char:        pshmem_char_iput,                             \
             short:       pshmem_short_iput,                            \
             int:         pshmem_int_iput,                              \
             long:        pshmem_long_iput,                             \
             long long:   pshmem_longlong_iput)(dest, source, dst, sst, \
                                                nelems, pe)

    /* see \ref shmem_long_iput() */
#define pshmem_iput(dest, source, dst, sst, nelems, pe)                 \
    _Generic(*(dest),                                                   \
             float:       pshmem_float_iput,                            \
             double:      pshmem_double_iput,                           \
             long double: pshmem_longdouble_iput,                       \
             char:        pshmem_char_iput,                             \
             short:       pshmem_short_iput,                            \
             int:         pshmem_int_iput,                              \
             long:        pshmem_long_iput,                             \
             long long:   pshmem_longlong_iput)(dest, source, dst, sst, \
                                                nelems, pe)

    /* see \ref shmem_long_swap() */
#define pshmem_swap(dest, value, pe)                            \
    _Generic(*(dest),                                           \
             int:          pshmem_int_swap,                     \
             long:         pshmem_long_swap,                    \
             long long:    pshmem_longlong_swap,                \
             float:        pshmem_float_swap,                   \
             double:       pshmem_double_swap)(dest, value, pe)

    /* see \ref shmem_long_cswap() */
#define pshmem_cswap(dest, cond, value, pe)                             \
    _Generic(*(dest),                                                   \
             int:          pshmem_int_cswap,                            \
             long:         pshmem_long_cswap,                           \
             long long:    pshmem_longlong_cswap)(dest, cond, value, pe)

    /* see \ref shmem_long_fadd() */
#define pshmem_fadd(dest, value, pe)                                \
    _Generic(*(dest),                                               \
             int:          pshmem_int_fadd,                         \
             long:         pshmem_long_fadd,                        \
             long long:    pshmem_longlong_fadd)(dest, value, pe)


    /* see \ref shmem_long_finc() */
#define pshmem_finc(dest, pe)                               \
    _Generic(*(dest),                                       \
             int:          pshmem_int_finc,                 \
             long:         pshmem_long_finc,                \
             long long:    pshmem_longlong_finc)(dest, pe)

    /* see \ref shmem_long_add() */
#define pshmem_add(dest, value, pe)                                 \
    _Generic(*(dest),                                               \
             int:          pshmem_int_add,                          \
             long:         pshmem_long_add,                         \
             long long:    pshmem_longlong_add)(dest, value, pe)

    /* see \ref shmem_long_add() */
#define pshmem_add(dest, value, pe)                                 \
    _Generic(*(dest),                                               \
             int:          pshmem_int_add,                          \
             long:         pshmem_long_add,                         \
             long long:    pshmem_longlong_add)(dest, value, pe)

    /* see \ref shmem_long_inc() */
#define pshmem_inc(dest, pe)                                \
    _Generic(*(dest),                                       \
             int:          pshmem_int_inc,                  \
             long:         pshmem_long_inc,                 \
             long long:    pshmem_longlong_inc)(dest, pe)

    /* see \ref shmem_long_fetch() */
#define pshmem_fetch(dest, pe)                              \
    _Generic(*(dest),                                       \
             int:          pshmem_int_fetch,                \
             const int:    pshmem_int_fetch,                \
             long:         pshmem_long_fetch,               \
             const long:   pshmem_long_fetch,               \
             long long:    pshmem_longlong_fetch,           \
             const long long: pshmem_longlong_fetch,        \
             float:        pshmem_float_fetch,              \
             const float:  pshmem_float_fetch,              \
             double:       pshmem_double_fetch,             \
             const double: pshmem_double_fetch)(dest, pe)

    /* see \ref shmem_long_set() */
#define pshmem_set(dest, value, pe)                             \
    _Generic(*(dest),                                           \
             int:          pshmem_int_set,                      \
             long:         pshmem_long_set,                     \
             long long:    pshmem_longlong_set,                 \
             float:        pshmem_float_set,                    \
             double:       pshmem_double_set)(dest, value, pe)

#endif   /* __STDC_VERSION__ >= 201112L test */
#endif /* __STDC_VERSION__ defined test */

    /*
     * barriers & syncs
     */

    void pshmem_barrier_all(void);
    void pshmem_barrier(int PE_start, int logPE_stride, int PE_size,
                        long *pSync);
    void pshmem_sync_all(void);
    void pshmem_sync(int PE_start, int logPE_stride, int PE_size,
                     long *pSync);
    void pshmem_fence(void);
    void pshmem_quiet(void);

    /*
     * accessibility
     */

    int pshmem_pe_accessible(int pe) _WUR;
    int pshmem_addr_accessible(const void *addr, int pe) _WUR;

    /*
     * symmetric memory management
     */

    void *pshmalloc(size_t size) _WUR;
    void pshfree(void *ptr);
    void *pshrealloc(void *ptr, size_t size) _WUR;
    void *pshmemalign(size_t alignment, size_t size) _WUR;

    void *pshmem_malloc(size_t size) _WUR;
    void *pshmem_calloc(size_t count, size_t size) _WUR;
    void pshmem_free(void *ptr);
    void *pshmem_realloc(void *ptr, size_t size) _WUR;
    void *pshmem_memalign(size_t alignment, size_t size) _WUR;

    /*
     * wait operations
     */

    int pshmem_short_test(short *ivar, int cmp,
                          short cmp_value);
    int pshmem_int_test(int *ivar, int cmp,
                        int cmp_value);
    int pshmem_long_test(long *ivar, int cmp,
                         long cmp_value);
    int pshmem_longlong_test(long long *ivar, int cmp,
                             long long cmp_value);
    int pshmem_ushort_test(unsigned short *ivar, int cmp,
                           unsigned short cmp_value);
    int pshmem_uint_test(unsigned int *ivar, int cmp,
                         unsigned int cmp_value);
    int pshmem_ulong_test(unsigned long *ivar, int cmp,
                          unsigned long cmp_value);
    int pshmem_ulonglong_test(unsigned long long *ivar, int cmp,
                              unsigned long long cmp_value);
    int pshmem_int32_test(int32_t *ivar, int cmp,
                          int32_t cmp_value);
    int pshmem_int64_test(int64_t *ivar, int cmp,
                          int64_t cmp_value);
    int pshmem_uint32_test(uint32_t *ivar, int cmp,
                           uint32_t cmp_value);
    int pshmem_uint64_test(uint64_t *ivar, int cmp,
                           uint64_t cmp_value);
    int pshmem_size_test(size_t *ivar, int cmp,
                         size_t cmp_value);
    int pshmem_ptrdiff_test(ptrdiff_t *ivar, int cmp,
                            ptrdiff_t cmp_value);

    void pshmem_short_wait_until(short *ivar, int cmp,
                                 short cmp_value);
    void pshmem_int_wait_until(int *ivar, int cmp,
                               int cmp_value);
    void pshmem_long_wait_until(long *ivar, int cmp,
                                long cmp_value);
    void pshmem_longlong_wait_until(long long *ivar, int cmp,
                                    long long cmp_value);
    void pshmem_ushort_wait_until(unsigned short *ivar, int cmp,
                                  unsigned short cmp_value);
    void pshmem_uint_wait_until(unsigned int *ivar, int cmp,
                                unsigned int cmp_value);
    void pshmem_ulong_wait_until(unsigned long *ivar, int cmp,
                                 unsigned long cmp_value);
    void pshmem_ulonglong_wait_until(unsigned long long *ivar, int cmp,
                                     unsigned long long cmp_value);
    void pshmem_int32_wait_until(int32_t *ivar, int cmp,
                                 int32_t cmp_value);
    void pshmem_int64_wait_until(int64_t *ivar, int cmp,
                                 int64_t cmp_value);
    void pshmem_uint32_wait_until(uint32_t *ivar, int cmp,
                                  uint32_t cmp_value);
    void pshmem_uint64_wait_until(uint64_t *ivar, int cmp,
                                  uint64_t cmp_value);
    void pshmem_size_wait_until(size_t *ivar, int cmp,
                                size_t cmp_value);
    void pshmem_ptrdiff_wait_until(ptrdiff_t *ivar, int cmp,
                                   ptrdiff_t cmp_value);

    /* deprecations */
    void pshmem_short_wait(short *ivar, short cmp_value);
    void pshmem_int_wait(int *ivar, int cmp_value);
    void pshmem_long_wait(long *ivar, long cmp_value);
    void pshmem_longlong_wait(long long *ivar, long long cmp_value);
    void pshmem_ushort_wait(unsigned short *ivar,
                            unsigned short cmp_value);
    void pshmem_uint_wait(unsigned int *ivar,
                          unsigned int cmp_value);
    void pshmem_ulong_wait(unsigned long *ivar,
                           unsigned long cmp_value);
    void pshmem_ulonglong_wait(unsigned long long *ivar,
                               unsigned long long cmp_value);
    void pshmem_int32_wait(int32_t *ivar,
                           int32_t cmp_value);
    void pshmem_int64_wait(int64_t *ivar,
                           int64_t cmp_value);
    void pshmem_uint32_wait(uint32_t *ivar,
                            uint32_t cmp_value);
    void pshmem_uint64_wait(uint64_t *ivar,
                            uint64_t cmp_value);
    void pshmem_size_wait(size_t *ivar,
                          size_t cmp_value);
    void pshmem_ptrdiff_wait(ptrdiff_t *ivar,
                             ptrdiff_t cmp_value);

    /*
     * atomic swaps
     */

    float pshmem_float_atomic_swap(float *target, float value,
                                   int pe) _WUR;
    double pshmem_double_atomic_swap(double *target, double value,
                                     int pe) _WUR;
    short pshmem_short_atomic_swap(short *target, short value,
                                   int pe) _WUR;
    int pshmem_int_atomic_swap(int *target, int value,
                               int pe) _WUR;
    long pshmem_long_atomic_swap(long *target, long value,
                                 int pe) _WUR;
    long long pshmem_longlong_atomic_swap(long long *target, long long value,
                                          int pe) _WUR;
    unsigned short pshmem_ushort_atomic_swap(unsigned short *target,
                                             unsigned short value,
                                             int pe) _WUR;
    unsigned int pshmem_uint_atomic_swap(unsigned int *target,
                                         unsigned int value,
                                         int pe) _WUR;
    unsigned long pshmem_ulong_atomic_swap(unsigned long *target,
                                           unsigned long value,
                                           int pe) _WUR;
    unsigned long long pshmem_ulonglong_atomic_swap(unsigned long long *target,
                                                    unsigned long long value,
                                                    int pe) _WUR;
    int32_t pshmem_int32_atomic_swap(int32_t *target, int32_t value, int pe) _WUR;
    int64_t pshmem_int64_atomic_swap(int64_t *target, int64_t value, int pe) _WUR;
    uint32_t pshmem_uint32_atomic_swap(uint32_t *target, uint32_t value,
                                     int pe) _WUR;
    uint64_t pshmem_uint64_atomic_swap(uint64_t *target, uint64_t value,
                                     int pe) _WUR;
    size_t pshmem_size_atomic_swap(size_t *target, size_t value,
                                   int pe) _WUR;
    ptrdiff_t pshmem_ptrdiff_atomic_swap(ptrdiff_t *target,
                                         ptrdiff_t value,
                                         int pe) _WUR;

    short pshmem_short_atomic_compare_swap(short *target,
                                           short value,
                                           int pe) _WUR;
    int pshmem_int_atomic_compare_swap(int *target,
                                       int value,
                                       int pe) _WUR;
    long pshmem_long_atomic_compare_swap(long *target,
                                         long value,
                                         int pe) _WUR;
    long long pshmem_longlong_atomic_compare_swap(long long *target,
                                                  long long value,
                                                  int pe) _WUR;
    unsigned short pshmem_ushort_atomic_compare_swap(unsigned short *target,
                                                     unsigned short value,
                                                     int pe) _WUR;
    unsigned int pshmem_uint_atomic_compare_swap(unsigned int *target,
                                                 unsigned int value,
                                                 int pe) _WUR;
    unsigned long pshmem_ulong_atomic_compare_swap(unsigned long *target,
                                                   unsigned long value,
                                                   int pe) _WUR;
    unsigned long long
    pshmem_ulonglong_atomic_compare_swap(unsigned long long *target,
                                         unsigned long long value,
                                         int pe) _WUR;
    int32_t pshmem_int32_atomic_compare_swap(int32_t *target, int32_t value,
                                           int pe) _WUR;
    int64_t pshmem_int64_atomic_compare_swap(int64_t *target, int64_t value,
                                           int pe) _WUR;
    uint32_t pshmem_uint32_atomic_compare_swap(uint32_t *target, uint32_t value,
                                             int pe) _WUR;
    uint64_t pshmem_uint64_atomic_compare_swap(uint64_t *target, uint64_t value,
                                             int pe) _WUR;
    size_t pshmem_size_atomic_compare_swap(size_t *target, size_t value,
                                           int pe) _WUR;
    ptrdiff_t pshmem_ptrdiff_atomic_compare_swap(ptrdiff_t *target,
                                                 ptrdiff_t value,
                                                 int pe) _WUR;


    /*
     * atomic fetch-{add,inc} & add,inc
     */

    int pshmem_int_fadd(int *target, int value, int pe) _WUR;
    long pshmem_long_fadd(long *target, long value, int pe) _WUR;
    long long pshmem_longlong_fadd(long long *target, long long value,
                                   int pe) _WUR;
    int pshmem_int_finc(int *target, int pe) _WUR;
    long pshmem_long_finc(long *target, int pe) _WUR;
    long long pshmem_longlong_finc(long long *target, int pe) _WUR;

    void pshmem_int_add(int *target, int value, int pe);
    void pshmem_long_add(long *target, long value, int pe);
    void pshmem_longlong_add(long long *target, long long value,
                             int pe);
    void pshmem_int_inc(int *target, int pe);
    void pshmem_long_inc(long *target, int pe);
    void pshmem_longlong_inc(long long *target, int pe);

    /*
     * cache flushing
     */

    void pshmem_clear_cache_inv(void);
    void pshmem_set_cache_inv(void);
    void pshmem_clear_cache_line_inv(void *target);
    void pshmem_set_cache_line_inv(void *target);
    void pshmem_udcflush(void);
    void pshmem_udcflush_line(void *target);

    /*
     * reductions
     */

    void pshmem_complexd_sum_to_all(COMPLEXIFY(double) * target,
                                    const COMPLEXIFY(double) * source,
                                    int nreduce,
                                    int PE_start, int logPE_stride,
                                    int PE_size,
                                    COMPLEXIFY(double) * pWrk,
                                    long *pSync);
    void pshmem_complexf_sum_to_all(COMPLEXIFY(float) * target,
                                    const COMPLEXIFY(float) * source,
                                    int nreduce, int PE_start,
                                    int logPE_stride, int PE_size,
                                    COMPLEXIFY(float) * pWrk,
                                    long *pSync);
    void pshmem_double_sum_to_all(double *target, const double *source,
                                  int nreduce, int PE_start,
                                  int logPE_stride, int PE_size,
                                  double *pWrk, long *pSync);
    void pshmem_float_sum_to_all(float *target, const float *source,
                                 int nreduce, int PE_start,
                                 int logPE_stride, int PE_size,
                                 float *pWrk, long *pSync);
    void pshmem_int_sum_to_all(int *target, const int *source, int nreduce,
                               int PE_start, int logPE_stride,
                               int PE_size, int *pWrk, long *pSync);
    void pshmem_long_sum_to_all(long *target, const long *source, int nreduce,
                                int PE_start, int logPE_stride,
                                int PE_size, long *pWrk, long *pSync);
    void pshmem_longdouble_sum_to_all(long double *target,
                                      const long double *source, int nreduce,
                                      int PE_start, int logPE_stride,
                                      int PE_size, long double *pWrk,
                                      long *pSync);
    void pshmem_longlong_sum_to_all(long long *target,
                                    const long long *source, int nreduce,
                                    int PE_start, int logPE_stride,
                                    int PE_size, long long *pWrk,
                                    long *pSync);
    void pshmem_short_sum_to_all(short *target, const short *source,
                                 int nreduce, int PE_start,
                                 int logPE_stride, int PE_size,
                                 short *pWrk, long *pSync);

    void pshmem_complexd_prod_to_all(COMPLEXIFY(double) * target,
                                     const COMPLEXIFY(double) * source,
                                     int nreduce,
                                     int PE_start, int logPE_stride,
                                     int PE_size,
                                     COMPLEXIFY(double) * pWrk,
                                     long *pSync);
    void pshmem_complexf_prod_to_all(COMPLEXIFY(float) * target,
                                     const COMPLEXIFY(float) * source,
                                     int nreduce, int PE_start,
                                     int logPE_stride, int PE_size,
                                     COMPLEXIFY(float) * pWrk,
                                     long *pSync);
    void pshmem_double_prod_to_all(double *target, const double *source,
                                   int nreduce, int PE_start,
                                   int logPE_stride, int PE_size,
                                   double *pWrk, long *pSync);
    void pshmem_float_prod_to_all(float *target, const float *source,
                                  int nreduce, int PE_start,
                                  int logPE_stride, int PE_size,
                                  float *pWrk, long *pSync);
    void pshmem_int_prod_to_all(int *target, const int *source, int nreduce,
                                int PE_start, int logPE_stride,
                                int PE_size, int *pWrk, long *pSync);
    void pshmem_long_prod_to_all(long *target, const long *source,
                                 int nreduce, int PE_start,
                                 int logPE_stride, int PE_size,
                                 long *pWrk, long *pSync);
    void pshmem_longdouble_prod_to_all(long double *target,
                                       const long double *source, int nreduce,
                                       int PE_start, int logPE_stride,
                                       int PE_size, long double *pWrk,
                                       long *pSync);
    void pshmem_longlong_prod_to_all(long long *target,
                                     const long long *source, int nreduce,
                                     int PE_start, int logPE_stride,
                                     int PE_size, long long *pWrk,
                                     long *pSync);
    void pshmem_short_prod_to_all(short *target, const short *source,
                                  int nreduce, int PE_start,
                                  int logPE_stride, int PE_size,
                                  short *pWrk, long *pSync);

    void pshmem_int_and_to_all(int *target,
                               const int *source,
                               int nreduce,
                               int PE_start, int logPE_stride,
                               int PE_size, int *pWrk, long *pSync);
    void pshmem_long_and_to_all(long *target, const long *source, int nreduce,
                                int PE_start, int logPE_stride,
                                int PE_size, long *pWrk, long *pSync);
    void pshmem_longlong_and_to_all(long long *target,
                                    const long long *source, int nreduce,
                                    int PE_start, int logPE_stride,
                                    int PE_size, long long *pWrk,
                                    long *pSync);
    void pshmem_short_and_to_all(short *target, const short *source,
                                 int nreduce, int PE_start,
                                 int logPE_stride, int PE_size,
                                 short *pWrk, long *pSync);

    void pshmem_int_or_to_all(int *target,
                              const int *source,
                              int nreduce,
                              int PE_start, int logPE_stride,
                              int PE_size, int *pWrk, long *pSync);
    void pshmem_long_or_to_all(long *target, const long *source, int nreduce,
                               int PE_start, int logPE_stride,
                               int PE_size, long *pWrk, long *pSync);
    void pshmem_longlong_or_to_all(long long *target,
                                   const long long *source,
                                   int nreduce, int PE_start,
                                   int logPE_stride, int PE_size,
                                   long long *pWrk, long *pSync);
    void pshmem_short_or_to_all(short *target, const short *source,
                                int nreduce, int PE_start,
                                int logPE_stride, int PE_size,
                                short *pWrk, long *pSync);

    void pshmem_int_xor_to_all(int *target,
                               const int *source,
                               int nreduce,
                               int PE_start, int logPE_stride,
                               int PE_size, int *pWrk, long *pSync);
    void pshmem_long_xor_to_all(long *target, const long *source, int nreduce,
                                int PE_start, int logPE_stride,
                                int PE_size, long *pWrk, long *pSync);
    void pshmem_longlong_xor_to_all(long long *target,
                                    const long long *source, int nreduce,
                                    int PE_start, int logPE_stride,
                                    int PE_size, long long *pWrk,
                                    long *pSync);
    void pshmem_short_xor_to_all(short *target, const short *source,
                                 int nreduce, int PE_start,
                                 int logPE_stride, int PE_size,
                                 short *pWrk, long *pSync);

    void pshmem_int_max_to_all(int *target,
                               const int *source,
                               int nreduce,
                               int PE_start, int logPE_stride,
                               int PE_size, int *pWrk, long *pSync);
    void pshmem_long_max_to_all(long *target, const long *source, int nreduce,
                                int PE_start, int logPE_stride,
                                int PE_size, long *pWrk, long *pSync);
    void pshmem_longlong_max_to_all(long long *target,
                                    const long long *source, int nreduce,
                                    int PE_start, int logPE_stride,
                                    int PE_size, long long *pWrk,
                                    long *pSync);
    void pshmem_short_max_to_all(short *target, const short *source,
                                 int nreduce, int PE_start,
                                 int logPE_stride, int PE_size,
                                 short *pWrk, long *pSync);
    void pshmem_longdouble_max_to_all(long double *target,
                                      const long double *source, int nreduce,
                                      int PE_start, int logPE_stride,
                                      int PE_size, long double *pWrk,
                                      long *pSync);
    void pshmem_float_max_to_all(float *target, const float *source,
                                 int nreduce, int PE_start,
                                 int logPE_stride, int PE_size,
                                 float *pWrk, long *pSync);
    void pshmem_double_max_to_all(double *target, const double *source,
                                  int nreduce, int PE_start,
                                  int logPE_stride, int PE_size,
                                  double *pWrk, long *pSync);

    void pshmem_int_min_to_all(int *target,
                               const int *source,
                               int nreduce,
                               int PE_start, int logPE_stride,
                               int PE_size, int *pWrk, long *pSync);
    void pshmem_long_min_to_all(long *target, const long *source, int nreduce,
                                int PE_start, int logPE_stride,
                                int PE_size, long *pWrk, long *pSync);
    void pshmem_longlong_min_to_all(long long *target,
                                    const long long *source, int nreduce,
                                    int PE_start, int logPE_stride,
                                    int PE_size, long long *pWrk,
                                    long *pSync);
    void pshmem_short_min_to_all(short *target, const short *source,
                                 int nreduce, int PE_start,
                                 int logPE_stride, int PE_size,
                                 short *pWrk, long *pSync);
    void pshmem_longdouble_min_to_all(long double *target,
                                      const long double *source, int nreduce,
                                      int PE_start, int logPE_stride,
                                      int PE_size, long double *pWrk,
                                      long *pSync);
    void pshmem_float_min_to_all(float *target, const float *source,
                                 int nreduce, int PE_start,
                                 int logPE_stride, int PE_size,
                                 float *pWrk, long *pSync);
    void pshmem_double_min_to_all(double *target, const double *source,
                                  int nreduce, int PE_start,
                                  int logPE_stride, int PE_size,
                                  double *pWrk, long *pSync);

    /*
     * broadcasts
     */

    void pshmem_broadcast32(void *target, const void *source,
                            size_t nelems, int PE_root, int PE_start,
                            int logPE_stride, int PE_size, long *pSync);

    void pshmem_broadcast64(void *target, const void *source,
                            size_t nelems, int PE_root, int PE_start,
                            int logPE_stride, int PE_size, long *pSync);

    /*
     * collects
     */

    void pshmem_fcollect32(void *target, const void *source,
                           size_t nelems, int PE_start,
                           int logPE_stride, int PE_size, long *pSync);
    void pshmem_fcollect64(void *target, const void *source,
                           size_t nelems, int PE_start,
                           int logPE_stride, int PE_size, long *pSync);

    void pshmem_collect32(void *target, const void *source,
                          size_t nelems, int PE_start, int logPE_stride,
                          int PE_size, long *pSync);
    void pshmem_collect64(void *target, const void *source,
                          size_t nelems, int PE_start, int logPE_stride,
                          int PE_size, long *pSync);

    /*
     * locks/critical section
     */

    void pshmem_set_lock(long *lock);
    void pshmem_clear_lock(long *lock);
    int  pshmem_test_lock(long *lock) _WUR;

    /*
     * atomic fetch and set
     */

    int pshmem_int_fetch(const int *target, int pe);
    long pshmem_long_fetch(const long *target, int pe);
    long long pshmem_longlong_fetch(const long long *target, int pe);
    float pshmem_float_fetch(const float *target, int pe);
    double pshmem_double_fetch(const double *target, int pe);

    void pshmem_int_set(int *target, int value, int pe);
    void pshmem_long_set(long *target, long value, int pe);
    void pshmem_longlong_set(long long *target, long long value, int pe);
    void pshmem_float_set(float *target, float value, int pe);
    void pshmem_double_set(double *target, double value, int pe);

#ifdef __STDC_VERSION__
#if  __STDC_VERSION__ >= 201112L

    /*
     * C11 Generic variants
     *
     */

#define pshmem_put(dest, source, nelems, pe)                            \
    _Generic(*(dest),                                                   \
             float:       pshmem_float_put,                             \
             double:      pshmem_double_put,                            \
             long double: pshmem_longdouble_put,                        \
             char:        pshmem_char_put,                              \
             short:       pshmem_short_put,                             \
             int:         pshmem_int_put,                               \
             long:        pshmem_long_put,                              \
             long long:   pshmem_longlong_put)(dest, source, nelems, pe)

#define pshmem_get(dest, source, nelems, pe)                            \
    _Generic(*(dest),                                                   \
             float:       pshmem_float_get,                             \
             double:      pshmem_double_get,                            \
             long double: pshmem_longdouble_get,                        \
             char:        pshmem_char_get,                              \
             short:       pshmem_short_get,                             \
             int:         pshmem_int_get,                               \
             long:        pshmem_long_get,                              \
             long long:   pshmem_longlong_get)(dest, source, nelems, pe)

#define pshmem_p(dest, value, pe)                               \
    _Generic(*(dest),                                           \
             float:       pshmem_float_p,                       \
             double:      pshmem_double_p,                      \
             long double: pshmem_longdouble_p,                  \
             char:        pshmem_char_p,                        \
             short:       pshmem_short_p,                       \
             int:         pshmem_int_p,                         \
             long:        pshmem_long_p,                        \
             long long:   pshmem_longlong_p)(dest, value, pe)

#define pshmem_g(addr, pe)                              \
    _Generic((addr),                                    \
             float:       pshmem_float_g,               \
             double:      pshmem_double_g,              \
             long double: pshmem_longdouble_g,          \
             char:        pshmem_char_g,                \
             short:       pshmem_short_g,               \
             int:         pshmem_int_g,                 \
             long:        pshmem_long_g,                \
             long long:   pshmem_longlong_g)(addr, pe)

#define pshmem_iput(dest, source, dst, sst, nelems, pe)                 \
    _Generic(*(dest),                                                   \
             float:       pshmem_float_iput,                            \
             double:      pshmem_double_iput,                           \
             long double: pshmem_longdouble_iput,                       \
             char:        pshmem_char_iput,                             \
             short:       pshmem_short_iput,                            \
             int:         pshmem_int_iput,                              \
             long:        pshmem_long_iput,                             \
             long long:   pshmem_longlong_iput)(dest, source, dst, sst, \
                                                nelems, pe)

#define pshmem_iput(dest, source, dst, sst, nelems, pe)                 \
    _Generic(*(dest),                                                   \
             float:       pshmem_float_iput,                            \
             double:      pshmem_double_iput,                           \
             long double: pshmem_longdouble_iput,                       \
             char:        pshmem_char_iput,                             \
             short:       pshmem_short_iput,                            \
             int:         pshmem_int_iput,                              \
             long:        pshmem_long_iput,                             \
             long long:   pshmem_longlong_iput)(dest, source, dst, sst, \
                                                nelems, pe)

#define pshmem_swap(dest, value, pe)                            \
    _Generic(*(dest),                                           \
             int:          pshmem_int_swap,                     \
             long:         pshmem_long_swap,                    \
             long long:    pshmem_longlong_swap,                \
             float:        pshmem_float_swap,                   \
             double:       pshmem_double_swap)(dest, value, pe)

#define pshmem_cswap(dest, cond, value, pe)                             \
    _Generic(*(dest),                                                   \
             int:          pshmem_int_cswap,                            \
             long:         pshmem_long_cswap,                           \
             long long:    pshmem_longlong_cswap)(dest, cond, value, pe)

#define pshmem_fadd(dest, value, pe)                                \
    _Generic(*(dest),                                               \
             int:          pshmem_int_fadd,                         \
             long:         pshmem_long_fadd,                        \
             long long:    pshmem_longlong_fadd)(dest, value, pe)

#define pshmem_finc(dest, pe)                               \
    _Generic(*(dest),                                       \
             int:          pshmem_int_finc,                 \
             long:         pshmem_long_finc,                \
             long long:    pshmem_longlong_finc)(dest, pe)

#define pshmem_add(dest, value, pe)                                 \
    _Generic(*(dest),                                               \
             int:          pshmem_int_add,                          \
             long:         pshmem_long_add,                         \
             long long:    pshmem_longlong_add)(dest, value, pe)

#define pshmem_add(dest, value, pe)                                 \
    _Generic(*(dest),                                               \
             int:          pshmem_int_add,                          \
             long:         pshmem_long_add,                         \
             long long:    pshmem_longlong_add)(dest, value, pe)

#define pshmem_inc(dest, pe)                                \
    _Generic(*(dest),                                       \
             int:          pshmem_int_inc,                  \
             long:         pshmem_long_inc,                 \
             long long:    pshmem_longlong_inc)(dest, pe)

#define pshmem_fetch(dest, pe)                              \
    _Generic(*(dest),                                       \
             int:          pshmem_int_fetch,                \
             const int:    pshmem_int_fetch,                \
             long:         pshmem_long_fetch,               \
             const long:   pshmem_long_fetch,               \
             long long:    pshmem_longlong_fetch,           \
             const long long: pshmem_longlong_fetch,        \
             float:        pshmem_float_fetch,              \
             const float:  pshmem_float_fetch,              \
             double:       pshmem_double_fetch,             \
             const double: pshmem_double_fetch)(dest, pe)

#define pshmem_set(dest, value, pe)                             \
    _Generic(*(dest),                                           \
             int:          pshmem_int_set,                      \
             long:         pshmem_long_set,                     \
             long long:    pshmem_longlong_set,                 \
             float:        pshmem_float_set,                    \
             double:       pshmem_double_set)(dest, value, pe)

#define pshmem_wait(ivar, cmp_value)                                \
    _Generic(*(ivar),                                               \
             short:        pshmem_short_wait,                       \
             int:          pshmem_int_wait,                         \
             long:         pshmem_long_wait,                        \
             long long:    pshmem_longlong_wait)(ivar, cmp_value)

#define pshmem_wait_until(ivar, cmp, cmp_value)                         \
    _Generic(*(ivar),                                                   \
             short:        pshmem_short_wait_until,                     \
             int:          pshmem_int_wait_until,                       \
             long:         pshmem_long_wait_until,                      \
             long long:    pshmem_longlong_wait_until)(ivar, cmp, cmp_value)

#endif   /* __STDC_VERSION__ >= 201112L test */
#endif /* __STDC_VERSION__ defined test */

    /*
     * --end--
     */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* _PSHMEM_H */
