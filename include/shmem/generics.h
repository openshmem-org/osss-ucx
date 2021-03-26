/* For license: see LICENSE file at top-level */

#ifndef _SHMEM_API_GENERICS_H
#define _SHMEM_API_GENERICS_H 1

#include <shmem/defs.h>
#include <shmem/depr.h>

#include <sys/types.h>
#include <stddef.h>               /* ptrdiff_t */
#include <stdint.h>               /* sized int types */
#include <stdarg.h>

#if SHMEM_HAS_C11

/**
 * Contexts-based generics
 *
 */

/*
 * This stops the not-a-context case turning into an error when
 * the value type doesn't match anything
 */
inline static void shmem_generics_nomatch(void) { }

/*
 * get numbered args out of parameter list
 * (thanks to SOS)
 */
#define SHC11_GET_ARG1_HELPER(_arg1, ...) _arg1

#define SHC11_GET_ARG1(...)                     \
    SHC11_GET_ARG1_HELPER(__VA_ARGS__, _extra)
#define SHC11_GET_ARG2(_arg1, ...)              \
    SHC11_GET_ARG1_HELPER(__VA_ARGS__, _extra)

#define SHC11_TYPE_EVAL_PTR(_arg) &*(_arg)
#define SHC11_TYPE_EVAL_PTR_OR_SCALAR(_arg) (_arg)+0

/* see \ref shmem_long_put() */
#define shmem_put(...)                                                  \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      float *: shmem_ctx_float_put,                     \
                      double *: shmem_ctx_double_put,                   \
                      long double *: shmem_ctx_longdouble_put,          \
                      signed char *: shmem_ctx_schar_put,               \
                      char *: shmem_ctx_char_put,                       \
                      short *: shmem_ctx_short_put,                     \
                      int *: shmem_ctx_int_put,                         \
                      long *: shmem_ctx_long_put,                       \
                      long long *: shmem_ctx_longlong_put,              \
                      unsigned char *: shmem_ctx_uchar_put,             \
                      unsigned short *: shmem_ctx_ushort_put,           \
                      unsigned int *: shmem_ctx_uint_put,               \
                      unsigned long *: shmem_ctx_ulong_put,             \
                      unsigned long long *: shmem_ctx_ulonglong_put,    \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             float *: shmem_float_put,                                  \
             double *: shmem_double_put,                                \
             long double *: shmem_longdouble_put,                       \
             signed char *: shmem_schar_put,                            \
             char *: shmem_char_put,                                    \
             short *: shmem_short_put,                                  \
             int *: shmem_int_put,                                      \
             long *: shmem_long_put,                                    \
             long long *: shmem_longlong_put,                           \
             unsigned char *: shmem_uchar_put,                          \
             unsigned short *: shmem_ushort_put,                        \
             unsigned int *: shmem_uint_put,                            \
             unsigned long *: shmem_ulong_put,                          \
             unsigned long long *: shmem_ulonglong_put,                 \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_put_signal() */
#define shmem_put_signal(...)                                           \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      float *: shmem_ctx_float_put_signal,              \
                      double *: shmem_ctx_double_put_signal,            \
                      long double *: shmem_ctx_longdouble_put_signal,   \
                      signed char *: shmem_ctx_schar_put_signal,        \
                      char *: shmem_ctx_char_put_signal,                \
                      short *: shmem_ctx_short_put_signal,              \
                      int *: shmem_ctx_int_put_signal,                  \
                      long *: shmem_ctx_long_put_signal,                \
                      long long *: shmem_ctx_longlong_put_signal,       \
                      unsigned char *: shmem_ctx_uchar_put_signal,      \
                      unsigned short *: shmem_ctx_ushort_put_signal,    \
                      unsigned int *: shmem_ctx_uint_put_signal,        \
                      unsigned long *: shmem_ctx_ulong_put_signal,      \
                      unsigned long long *: shmem_ctx_ulonglong_put_signal, \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             float *: shmem_float_put_signal,                           \
             double *: shmem_double_put_signal,                         \
             long double *: shmem_longdouble_put_signal,                \
             signed char *: shmem_schar_put_signal,                     \
             char *: shmem_char_put_signal,                             \
             short *: shmem_short_put_signal,                           \
             int *: shmem_int_put_signal,                               \
             long *: shmem_long_put_signal,                             \
             long long *: shmem_longlong_put_signal,                    \
             unsigned char *: shmem_uchar_put_signal,                   \
             unsigned short *: shmem_ushort_put_signal,                 \
             unsigned int *: shmem_uint_put_signal,                     \
             unsigned long *: shmem_ulong_put_signal,                   \
             unsigned long long *: shmem_ulonglong_put_signal,          \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_put_nbi() */
#define shmem_put_nbi(...)                                              \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      float *: shmem_ctx_float_put_nbi,                 \
                      double *: shmem_ctx_double_put_nbi,               \
                      long double *: shmem_ctx_longdouble_put_nbi,      \
                      signed char *: shmem_ctx_schar_put_nbi,           \
                      char *: shmem_ctx_char_put_nbi,                   \
                      short *: shmem_ctx_short_put_nbi,                 \
                      int *: shmem_ctx_int_put_nbi,                     \
                      long *: shmem_ctx_long_put_nbi,                   \
                      long long *: shmem_ctx_longlong_put_nbi,          \
                      unsigned char *: shmem_ctx_uchar_put_nbi,         \
                      unsigned short *: shmem_ctx_ushort_put_nbi,       \
                      unsigned int *: shmem_ctx_uint_put_nbi,           \
                      unsigned long *: shmem_ctx_ulong_put_nbi,         \
                      unsigned long long *: shmem_ctx_ulonglong_put_nbi, \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             float *: shmem_float_put_nbi,                              \
             double *: shmem_double_put_nbi,                            \
             long double *: shmem_longdouble_put_nbi,                   \
             signed char *: shmem_schar_put_nbi,                        \
             char *: shmem_char_put_nbi,                                \
             short *: shmem_short_put_nbi,                              \
             int *: shmem_int_put_nbi,                                  \
             long *: shmem_long_put_nbi,                                \
             long long *: shmem_longlong_put_nbi,                       \
             unsigned char *: shmem_uchar_put_nbi,                      \
             unsigned short *: shmem_ushort_put_nbi,                    \
             unsigned int *: shmem_uint_put_nbi,                        \
             unsigned long *: shmem_ulong_put_nbi,                      \
             unsigned long long *: shmem_ulonglong_put_nbi,             \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_put_signal_nbi() */
#define shmem_put_signal_nbi(...)                                       \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      float *: shmem_ctx_float_put_signal_nbi,          \
                      double *: shmem_ctx_double_put_signal_nbi,        \
                      long double *: shmem_ctx_longdouble_put_signal_nbi, \
                      signed char *: shmem_ctx_schar_put_signal_nbi,    \
                      char *: shmem_ctx_char_put_signal_nbi,            \
                      short *: shmem_ctx_short_put_signal_nbi,          \
                      int *: shmem_ctx_int_put_signal_nbi,              \
                      long *: shmem_ctx_long_put_signal_nbi,            \
                      long long *: shmem_ctx_longlong_put_signal_nbi,   \
                      unsigned char *: shmem_ctx_uchar_put_signal_nbi,  \
                      unsigned short *: shmem_ctx_ushort_put_signal_nbi, \
                      unsigned int *: shmem_ctx_uint_put_signal_nbi,    \
                      unsigned long *: shmem_ctx_ulong_put_signal_nbi,  \
                      unsigned long long *: shmem_ctx_ulonglong_put_signal_nbi, \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             float *: shmem_float_put_signal_nbi,                       \
             double *: shmem_double_put_signal_nbi,                     \
             long double *: shmem_longdouble_put_signal_nbi,            \
             signed char *: shmem_schar_put_signal_nbi,                 \
             char *: shmem_char_put_signal_nbi,                         \
             short *: shmem_short_put_signal_nbi,                       \
             int *: shmem_int_put_signal_nbi,                           \
             long *: shmem_long_put_signal_nbi,                         \
             long long *: shmem_longlong_put_signal_nbi,                \
             unsigned char *: shmem_uchar_put_signal_nbi,               \
             unsigned short *: shmem_ushort_put_signal_nbi,             \
             unsigned int *: shmem_uint_put_signal_nbi,                 \
             unsigned long *: shmem_ulong_put_signal_nbi,               \
             unsigned long long *: shmem_ulonglong_put_signal_nbi,      \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_get() */
#define shmem_get(...)                                                  \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      float *: shmem_ctx_float_get,                     \
                      double *: shmem_ctx_double_get,                   \
                      long double *: shmem_ctx_longdouble_get,          \
                      signed char *: shmem_ctx_schar_get,               \
                      char *: shmem_ctx_char_get,                       \
                      short *: shmem_ctx_short_get,                     \
                      int *: shmem_ctx_int_get,                         \
                      long *: shmem_ctx_long_get,                       \
                      long long *: shmem_ctx_longlong_get,              \
                      unsigned char *: shmem_ctx_uchar_get,             \
                      unsigned short *: shmem_ctx_ushort_get,           \
                      unsigned int *: shmem_ctx_uint_get,               \
                      unsigned long *: shmem_ctx_ulong_get,             \
                      unsigned long long *: shmem_ctx_ulonglong_get,    \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             float *: shmem_float_get,                                  \
             double *: shmem_double_get,                                \
             long double *: shmem_longdouble_get,                       \
             signed char *: shmem_schar_get,                            \
             char *: shmem_char_get,                                    \
             short *: shmem_short_get,                                  \
             int *: shmem_int_get,                                      \
             long *: shmem_long_get,                                    \
             long long *: shmem_longlong_get,                           \
             unsigned char *: shmem_uchar_get,                          \
             unsigned short *: shmem_ushort_get,                        \
             unsigned int *: shmem_uint_get,                            \
             unsigned long *: shmem_ulong_get,                          \
             unsigned long long *: shmem_ulonglong_get,                 \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_get_nbi() */
#define shmem_get_nbi(...)                                              \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      float *: shmem_ctx_float_get_nbi,                 \
                      double *: shmem_ctx_double_get_nbi,               \
                      long double *: shmem_ctx_longdouble_get_nbi,      \
                      signed char *: shmem_ctx_schar_get_nbi,           \
                      char *: shmem_ctx_char_get_nbi,                   \
                      short *: shmem_ctx_short_get_nbi,                 \
                      int *: shmem_ctx_int_get_nbi,                     \
                      long *: shmem_ctx_long_get_nbi,                   \
                      long long *: shmem_ctx_longlong_get_nbi,          \
                      unsigned char *: shmem_ctx_uchar_get_nbi,         \
                      unsigned short *: shmem_ctx_ushort_get_nbi,       \
                      unsigned int *: shmem_ctx_uint_get_nbi,           \
                      unsigned long *: shmem_ctx_ulong_get_nbi,         \
                      unsigned long long *: shmem_ctx_ulonglong_get_nbi, \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             float *: shmem_float_get_nbi,                              \
             double *: shmem_double_get_nbi,                            \
             long double *: shmem_longdouble_get_nbi,                   \
             signed char *: shmem_schar_get_nbi,                        \
             char *: shmem_char_get_nbi,                                \
             short *: shmem_short_get_nbi,                              \
             int *: shmem_int_get_nbi,                                  \
             long *: shmem_long_get_nbi,                                \
             long long *: shmem_longlong_get_nbi,                       \
             unsigned char *: shmem_uchar_get_nbi,                      \
             unsigned short *: shmem_ushort_get_nbi,                    \
             unsigned int *: shmem_uint_get_nbi,                        \
             unsigned long *: shmem_ulong_get_nbi,                      \
             unsigned long long *: shmem_ulonglong_get_nbi,             \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_p() */
#define shmem_p(...)                                                    \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      float *: shmem_ctx_float_p,                       \
                      double *: shmem_ctx_double_p,                     \
                      long double *: shmem_ctx_longdouble_p,            \
                      signed char *: shmem_ctx_schar_p,                 \
                      char *: shmem_ctx_char_p,                         \
                      short *: shmem_ctx_short_p,                       \
                      int *: shmem_ctx_int_p,                           \
                      long *: shmem_ctx_long_p,                         \
                      long long *: shmem_ctx_longlong_p,                \
                      unsigned char *: shmem_ctx_uchar_p,               \
                      unsigned short *: shmem_ctx_ushort_p,             \
                      unsigned int *: shmem_ctx_uint_p,                 \
                      unsigned long *: shmem_ctx_ulong_p,               \
                      unsigned long long *: shmem_ctx_ulonglong_p,      \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             float *: shmem_float_p,                                    \
             double *: shmem_double_p,                                  \
             long double *: shmem_longdouble_p,                         \
             signed char *: shmem_schar_p,                              \
             char *: shmem_char_p,                                      \
             short *: shmem_short_p,                                    \
             int *: shmem_int_p,                                        \
             long *: shmem_long_p,                                      \
             long long *: shmem_longlong_p,                             \
             unsigned char *: shmem_uchar_p,                            \
             unsigned short *: shmem_ushort_p,                          \
             unsigned int *: shmem_uint_p,                              \
             unsigned long *: shmem_ulong_p,                            \
             unsigned long long *: shmem_ulonglong_p,                   \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_g() */
#define shmem_g(...)                                                    \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      float *: shmem_ctx_float_g,                       \
                      double *: shmem_ctx_double_g,                     \
                      long double *: shmem_ctx_longdouble_g,            \
                      signed char *: shmem_ctx_schar_g,                 \
                      char *: shmem_ctx_char_g,                         \
                      short *: shmem_ctx_short_g,                       \
                      int *: shmem_ctx_int_g,                           \
                      long *: shmem_ctx_long_g,                         \
                      long long *: shmem_ctx_longlong_g,                \
                      unsigned char *: shmem_ctx_uchar_g,               \
                      unsigned short *: shmem_ctx_ushort_g,             \
                      unsigned int *: shmem_ctx_uint_g,                 \
                      unsigned long *: shmem_ctx_ulong_g,               \
                      unsigned long long *: shmem_ctx_ulonglong_g,      \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             float *: shmem_float_g,                                    \
             double *: shmem_double_g,                                  \
             long double *: shmem_longdouble_g,                         \
             signed char *: shmem_schar_g,                              \
             char *: shmem_char_g,                                      \
             short *: shmem_short_g,                                    \
             int *: shmem_int_g,                                        \
             long *: shmem_long_g,                                      \
             long long *: shmem_longlong_g,                             \
             unsigned char *: shmem_uchar_g,                            \
             unsigned short *: shmem_ushort_g,                          \
             unsigned int *: shmem_uint_g,                              \
             unsigned long *: shmem_ulong_g,                            \
             unsigned long long *: shmem_ulonglong_g,                   \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_iput() */
#define shmem_iput(...)                                                 \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      float *: shmem_ctx_float_iput,                    \
                      double *: shmem_ctx_double_iput,                  \
                      long double *: shmem_ctx_longdouble_iput,         \
                      signed char *: shmem_ctx_schar_iput,              \
                      char *: shmem_ctx_char_iput,                      \
                      short *: shmem_ctx_short_iput,                    \
                      int *: shmem_ctx_int_iput,                        \
                      long *: shmem_ctx_long_iput,                      \
                      long long *: shmem_ctx_longlong_iput,             \
                      unsigned char *: shmem_ctx_uchar_iput,            \
                      unsigned short *: shmem_ctx_ushort_iput,          \
                      unsigned int *: shmem_ctx_uint_iput,              \
                      unsigned long *: shmem_ctx_ulong_iput,            \
                      unsigned long long *: shmem_ctx_ulonglong_iput,   \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             float *: shmem_float_iput,                                 \
             double *: shmem_double_iput,                               \
             long double *: shmem_longdouble_iput,                      \
             signed char *: shmem_schar_iput,                           \
             char *: shmem_char_iput,                                   \
             short *: shmem_short_iput,                                 \
             int *: shmem_int_iput,                                     \
             long *: shmem_long_iput,                                   \
             long long *: shmem_longlong_iput,                          \
             unsigned char *: shmem_uchar_iput,                         \
             unsigned short *: shmem_ushort_iput,                       \
             unsigned int *: shmem_uint_iput,                           \
             unsigned long *: shmem_ulong_iput,                         \
             unsigned long long *: shmem_ulonglong_iput,                \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)


/* see \ref shmem_long_iget() */
#define shmem_iget(...)                                                 \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      float *: shmem_ctx_float_iget,                    \
                      double *: shmem_ctx_double_iget,                  \
                      long double *: shmem_ctx_longdouble_iget,         \
                      signed char *: shmem_ctx_schar_iget,              \
                      char *: shmem_ctx_char_iget,                      \
                      short *: shmem_ctx_short_iget,                    \
                      int *: shmem_ctx_int_iget,                        \
                      long *: shmem_ctx_long_iget,                      \
                      long long *: shmem_ctx_longlong_iget,             \
                      unsigned char *: shmem_ctx_uchar_iget,            \
                      unsigned short *: shmem_ctx_ushort_iget,          \
                      unsigned int *: shmem_ctx_uint_iget,              \
                      unsigned long *: shmem_ctx_ulong_iget,            \
                      unsigned long long *: shmem_ctx_ulonglong_iget,   \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             float *: shmem_float_iget,                                 \
             double *: shmem_double_iget,                               \
             long double *: shmem_longdouble_iget,                      \
             signed char *: shmem_schar_iget,                           \
             char *: shmem_char_iget,                                   \
             short *: shmem_short_iget,                                 \
             int *: shmem_int_iget,                                     \
             long *: shmem_long_iget,                                   \
             long long *: shmem_longlong_iget,                          \
             unsigned char *: shmem_uchar_iget,                         \
             unsigned short *: shmem_ushort_iget,                       \
             unsigned int *: shmem_uint_iget,                           \
             unsigned long *: shmem_ulong_iget,                         \
             unsigned long long *: shmem_ulonglong_iget,                \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_atomic_swap() */
#define shmem_atomic_swap(...)                                          \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      float *: shmem_ctx_float_atomic_swap,             \
                      double *: shmem_ctx_double_atomic_swap,           \
                      int *: shmem_ctx_int_atomic_swap,                 \
                      long *: shmem_ctx_long_atomic_swap,               \
                      long long *: shmem_ctx_longlong_atomic_swap,      \
                      unsigned int *: shmem_ctx_uint_atomic_swap,       \
                      unsigned long *: shmem_ctx_ulong_atomic_swap,     \
                      unsigned long long *: shmem_ctx_ulonglong_atomic_swap, \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             float *: shmem_float_atomic_swap,                          \
             double *: shmem_double_atomic_swap,                        \
             int *: shmem_int_atomic_swap,                              \
             long *: shmem_long_atomic_swap,                            \
             long long *: shmem_longlong_atomic_swap,                   \
             unsigned int *: shmem_uint_atomic_swap,                    \
             unsigned long *: shmem_ulong_atomic_swap,                  \
             unsigned long long *: shmem_ulonglong_atomic_swap,         \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_atomic_swap_nbi() */
#define shmem_atomic_swap_nbi(...)                                      \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      float *: shmem_ctx_float_atomic_swap_nbi,         \
                      double *: shmem_ctx_double_atomic_swap_nbi,       \
                      int *: shmem_ctx_int_atomic_swap_nbi,             \
                      long *: shmem_ctx_long_atomic_swap_nbi,           \
                      long long *: shmem_ctx_longlong_atomic_swap_nbi,  \
                      unsigned int *: shmem_ctx_uint_atomic_swap_nbi,   \
                      unsigned long *: shmem_ctx_ulong_atomic_swap_nbi, \
                      unsigned long long *: shmem_ctx_ulonglong_atomic_swap_nbi, \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             float *: shmem_float_atomic_swap_nbi,                      \
             double *: shmem_double_atomic_swap_nbi,                    \
             int *: shmem_int_atomic_swap_nbi,                          \
             long *: shmem_long_atomic_swap_nbi,                        \
             long long *: shmem_longlong_atomic_swap_nbi,               \
             unsigned int *: shmem_uint_atomic_swap_nbi,                \
             unsigned long *: shmem_ulong_atomic_swap_nbi,              \
             unsigned long long *: shmem_ulonglong_atomic_swap_nbi,     \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_atomic_compare_swap() */
#define shmem_atomic_compare_swap(...)                                  \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      int *: shmem_ctx_int_atomic_compare_swap,         \
                      long *: shmem_ctx_long_atomic_compare_swap,       \
                      long long *: shmem_ctx_longlong_atomic_compare_swap, \
                      unsigned int *: shmem_ctx_uint_atomic_compare_swap, \
                      unsigned long *: shmem_ctx_ulong_atomic_compare_swap, \
                      unsigned long long *:                             \
                      shmem_ctx_ulonglong_atomic_compare_swap,          \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             int *: shmem_int_atomic_compare_swap,                      \
             long *: shmem_long_atomic_compare_swap,                    \
             long long *: shmem_longlong_atomic_compare_swap,           \
             unsigned int *: shmem_uint_atomic_compare_swap,            \
             unsigned long *: shmem_ulong_atomic_compare_swap,          \
             unsigned long long *: shmem_ulonglong_atomic_compare_swap, \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_atomic_fetch_add() */
#define shmem_atomic_fetch_add(...)                                     \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      int *: shmem_ctx_int_atomic_fetch_add,            \
                      long *: shmem_ctx_long_atomic_fetch_add,          \
                      long long *: shmem_ctx_longlong_atomic_fetch_add, \
                      unsigned int *: shmem_ctx_uint_atomic_fetch_add,  \
                      unsigned long *: shmem_ctx_ulong_atomic_fetch_add, \
                      unsigned long long *:                             \
                      shmem_ctx_ulonglong_atomic_fetch_add,             \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             int *: shmem_int_atomic_fetch_add,                         \
             long *: shmem_long_atomic_fetch_add,                       \
             long long *: shmem_longlong_atomic_fetch_add,              \
             unsigned int *: shmem_uint_atomic_fetch_add,               \
             unsigned long *: shmem_ulong_atomic_fetch_add,             \
             unsigned long long *: shmem_ulonglong_atomic_fetch_add,    \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_atomic_fetch_add_nbi() */
#define shmem_atomic_fetch_add_nbi(...)                                 \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      int *: shmem_ctx_int_atomic_fetch_add_nbi,        \
                      long *: shmem_ctx_long_atomic_fetch_add_nbi,      \
                      long long *: shmem_ctx_longlong_atomic_fetch_add_nbi, \
                      unsigned int *: shmem_ctx_uint_atomic_fetch_add_nbi, \
                      unsigned long *: shmem_ctx_ulong_atomic_fetch_add_nbi, \
                      unsigned long long *:                             \
                      shmem_ctx_ulonglong_atomic_fetch_add_nbi,         \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             int *: shmem_int_atomic_fetch_add_nbi,                     \
             long *: shmem_long_atomic_fetch_add_nbi,                   \
             long long *: shmem_longlong_atomic_fetch_add_nbi,          \
             unsigned int *: shmem_uint_atomic_fetch_add_nbi,           \
             unsigned long *: shmem_ulong_atomic_fetch_add_nbi,         \
             unsigned long long *: shmem_ulonglong_atomic_fetch_add_nbi, \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_atomic_fetch_inc() */
#define shmem_atomic_fetch_inc(...)                                     \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__))), \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      int *: shmem_ctx_int_atomic_fetch_inc,            \
                      long *: shmem_ctx_long_atomic_fetch_inc,          \
                      long long *: shmem_ctx_longlong_atomic_fetch_inc, \
                      unsigned int *: shmem_ctx_uint_atomic_fetch_inc,  \
                      unsigned long *: shmem_ctx_ulong_atomic_fetch_inc, \
                      unsigned long long *:                             \
                      shmem_ctx_ulonglong_atomic_fetch_inc,             \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             int *: shmem_int_atomic_fetch_inc,                         \
             long *: shmem_long_atomic_fetch_inc,                       \
             long long *: shmem_longlong_atomic_fetch_inc,              \
             unsigned int *: shmem_uint_atomic_fetch_inc,               \
             unsigned long *: shmem_ulong_atomic_fetch_inc,             \
             unsigned long long *: shmem_ulonglong_atomic_fetch_inc,    \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_atomic_fetch_inc_nbi() */
#define shmem_atomic_fetch_inc_nbi(...)                                 \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      int *: shmem_ctx_int_atomic_fetch_inc_nbi,        \
                      long *: shmem_ctx_long_atomic_fetch_inc_nbi,      \
                      long long *: shmem_ctx_longlong_atomic_fetch_inc_nbi, \
                      unsigned int *: shmem_ctx_uint_atomic_fetch_inc_nbi, \
                      unsigned long *: shmem_ctx_ulong_atomic_fetch_inc_nbi, \
                      unsigned long long *:                             \
                      shmem_ctx_ulonglong_atomic_fetch_inc_nbi,         \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             int *: shmem_int_atomic_fetch_inc_nbi,                     \
             long *: shmem_long_atomic_fetch_inc_nbi,                   \
             long long *: shmem_longlong_atomic_fetch_inc_nbi,          \
             unsigned int *: shmem_uint_atomic_fetch_inc_nbi,           \
             unsigned long *: shmem_ulong_atomic_fetch_inc_nbi,         \
             unsigned long long *: shmem_ulonglong_atomic_fetch_inc_nbi, \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_atomic_fetch_and() */
#define shmem_atomic_fetch_and(...)                                     \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      int32_t *: shmem_ctx_int32_atomic_fetch_and,      \
                      int64_t *: shmem_ctx_int64_atomic_fetch_and,      \
                      unsigned int *: shmem_ctx_uint_atomic_fetch_and,  \
                      unsigned long *: shmem_ctx_ulong_atomic_fetch_and, \
                      unsigned long long *:                             \
                      shmem_ctx_ulonglong_atomic_fetch_and,             \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             int32_t *: shmem_int32_atomic_fetch_and,                   \
             int64_t *: shmem_int64_atomic_fetch_and,                   \
             unsigned int *: shmem_uint_atomic_fetch_and,               \
             unsigned long *: shmem_ulong_atomic_fetch_and,             \
             unsigned long long *: shmem_ulonglong_atomic_fetch_and,    \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_atomic_fetch_and_nbi() */
#define shmem_atomic_fetch_and_nbi(...)                                 \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      int32_t *: shmem_ctx_int32_atomic_fetch_and_nbi,  \
                      int64_t *: shmem_ctx_int64_atomic_fetch_and_nbi,  \
                      unsigned int *: shmem_ctx_uint_atomic_fetch_and_nbi, \
                      unsigned long *: shmem_ctx_ulong_atomic_fetch_and_nbi, \
                      unsigned long long *:                             \
                      shmem_ctx_ulonglong_atomic_fetch_and_nbi,         \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             int32_t *: shmem_int32_atomic_fetch_and_nbi,               \
             int64_t *: shmem_int64_atomic_fetch_and_nbi,               \
             unsigned int *: shmem_uint_atomic_fetch_and_nbi,           \
             unsigned long *: shmem_ulong_atomic_fetch_and_nbi,         \
             unsigned long long *: shmem_ulonglong_atomic_fetch_and_nbi, \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_atomic_fetch_or() */
#define shmem_atomic_fetch_or(...)                                      \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      int32_t *: shmem_ctx_int32_atomic_fetch_or,       \
                      int64_t *: shmem_ctx_int64_atomic_fetch_or,       \
                      unsigned int *: shmem_ctx_uint_atomic_fetch_or,   \
                      unsigned long *: shmem_ctx_ulong_atomic_fetch_or, \
                      unsigned long long *:                             \
                      shmem_ctx_ulonglong_atomic_fetch_or,              \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             int32_t *: shmem_int32_atomic_fetch_or,                    \
             int64_t *: shmem_int64_atomic_fetch_or,                    \
             unsigned int *: shmem_uint_atomic_fetch_or,                \
             unsigned long *: shmem_ulong_atomic_fetch_or,              \
             unsigned long long *: shmem_ulonglong_atomic_fetch_or,     \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_atomic_fetch_or_nbi() */
#define shmem_atomic_fetch_or_nbi(...)                                  \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      int32_t *: shmem_ctx_int32_atomic_fetch_or_nbi,   \
                      int64_t *: shmem_ctx_int64_atomic_fetch_or_nbi,   \
                      unsigned int *: shmem_ctx_uint_atomic_fetch_or_nbi, \
                      unsigned long *: shmem_ctx_ulong_atomic_fetch_or_nbi, \
                      unsigned long long *:                             \
                      shmem_ctx_ulonglong_atomic_fetch_or_nbi,          \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             int32_t *: shmem_int32_atomic_fetch_or_nbi,                \
             int64_t *: shmem_int64_atomic_fetch_or_nbi,                \
             unsigned int *: shmem_uint_atomic_fetch_or_nbi,            \
             unsigned long *: shmem_ulong_atomic_fetch_or_nbi,          \
             unsigned long long *: shmem_ulonglong_atomic_fetch_or_nbi, \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_atomic_fetch_xor() */
#define shmem_atomic_fetch_xor(...)                                     \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      int32_t *: shmem_ctx_int32_atomic_fetch_xor,      \
                      int64_t *: shmem_ctx_int64_atomic_fetch_xor,      \
                      unsigned int *: shmem_ctx_uint_atomic_fetch_xor,  \
                      unsigned long *: shmem_ctx_ulong_atomic_fetch_xor, \
                      unsigned long long *:                             \
                      shmem_ctx_ulonglong_atomic_fetch_xor,             \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             int32_t *: shmem_int32_atomic_fetch_xor,                   \
             int64_t *: shmem_int64_atomic_fetch_xor,                   \
             unsigned int *: shmem_uint_atomic_fetch_xor,               \
             unsigned long *: shmem_ulong_atomic_fetch_xor,             \
             unsigned long long *: shmem_ulonglong_atomic_fetch_xor,    \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_atomic_fetch_xor_nbi() */
#define shmem_atomic_fetch_xor_nbi(...)                                 \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      int32_t *: shmem_ctx_int32_atomic_fetch_xor_nbi,  \
                      int64_t *: shmem_ctx_int64_atomic_fetch_xor_nbi,  \
                      unsigned int *: shmem_ctx_uint_atomic_fetch_xor_nbi, \
                      unsigned long *: shmem_ctx_ulong_atomic_fetch_xor_nbi, \
                      unsigned long long *:                             \
                      shmem_ctx_ulonglong_atomic_fetch_xor_nbi,         \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             int32_t *: shmem_int32_atomic_fetch_xor_nbi,               \
             int64_t *: shmem_int64_atomic_fetch_xor_nbi,               \
             unsigned int *: shmem_uint_atomic_fetch_xor_nbi,           \
             unsigned long *: shmem_ulong_atomic_fetch_xor_nbi,         \
             unsigned long long *: shmem_ulonglong_atomic_fetch_xor_nbi, \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_atomic_add() */
#define shmem_atomic_add(...)                                           \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      int *: shmem_ctx_int_atomic_add,                  \
                      long *: shmem_ctx_long_atomic_add,                \
                      long long *: shmem_ctx_longlong_atomic_add,       \
                      unsigned int *: shmem_ctx_uint_atomic_add,        \
                      unsigned long *: shmem_ctx_ulong_atomic_add,      \
                      unsigned long long *: shmem_ctx_ulonglong_atomic_add, \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             int *: shmem_int_atomic_add,                               \
             long *: shmem_long_atomic_add,                             \
             long long *: shmem_longlong_atomic_add,                    \
             unsigned int *: shmem_uint_atomic_add,                     \
             unsigned long *: shmem_ulong_atomic_add,                   \
             unsigned long long *: shmem_ulonglong_atomic_add,          \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_atomic_inc() */
#define shmem_atomic_inc(...)                                           \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      int *: shmem_ctx_int_atomic_inc,                  \
                      long *: shmem_ctx_long_atomic_inc,                \
                      long long *: shmem_ctx_longlong_atomic_inc,       \
                      unsigned int *: shmem_ctx_uint_atomic_inc,        \
                      unsigned long *: shmem_ctx_ulong_atomic_inc,      \
                      unsigned long long *: shmem_ctx_ulonglong_atomic_inc, \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             int *: shmem_int_atomic_inc,                               \
             long *: shmem_long_atomic_inc,                             \
             long long *: shmem_longlong_atomic_inc,                    \
             unsigned int *: shmem_uint_atomic_inc,                     \
             unsigned long *: shmem_ulong_atomic_inc,                   \
             unsigned long long *: shmem_ulonglong_atomic_inc,          \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_atomic_and() */
#define shmem_atomic_and(...)                                           \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      int32_t *: shmem_ctx_int32_atomic_and,            \
                      int64_t *: shmem_ctx_int64_atomic_and,            \
                      unsigned int *: shmem_ctx_uint_atomic_and,        \
                      unsigned long *: shmem_ctx_ulong_atomic_and,      \
                      unsigned long long *: shmem_ctx_ulonglong_atomic_and, \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             int32_t *: shmem_int32_atomic_and,                         \
             int64_t *: shmem_int64_atomic_and,                         \
             unsigned int *: shmem_uint_atomic_and,                     \
             unsigned long *: shmem_ulong_atomic_and,                   \
             unsigned long long *: shmem_ulonglong_atomic_and,          \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_atomic_or() */
#define shmem_atomic_or(...)                                            \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      int32_t *: shmem_ctx_int32_atomic_or,             \
                      int64_t *: shmem_ctx_int64_atomic_or,             \
                      unsigned int *: shmem_ctx_uint_atomic_or,         \
                      unsigned long *: shmem_ctx_ulong_atomic_or,       \
                      unsigned long long *: shmem_ctx_ulonglong_atomic_or, \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             int32_t *: shmem_int32_atomic_or,                          \
             int64_t *: shmem_int64_atomic_or,                          \
             unsigned int *: shmem_uint_atomic_or,                      \
             unsigned long *: shmem_ulong_atomic_or,                    \
             unsigned long long *: shmem_ulonglong_atomic_or,           \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_atomic_xor() */
#define shmem_atomic_xor(...)                                           \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      int32_t *: shmem_ctx_int32_atomic_xor,            \
                      int64_t *: shmem_ctx_int64_atomic_xor,            \
                      unsigned int *: shmem_ctx_uint_atomic_xor,        \
                      unsigned long *: shmem_ctx_ulong_atomic_xor,      \
                      unsigned long long *: shmem_ctx_ulonglong_atomic_xor, \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             int32_t *: shmem_int32_atomic_xor,                         \
             int64_t *: shmem_int64_atomic_xor,                         \
             unsigned int *: shmem_uint_atomic_xor,                     \
             unsigned long *: shmem_ulong_atomic_xor,                   \
             unsigned long long *: shmem_ulonglong_atomic_xor,          \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)


/* see \ref shmem_long_atomic_fetch() */
#define shmem_atomic_fetch(...)                                         \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      float *: shmem_ctx_float_atomic_fetch,            \
                      double *: shmem_ctx_double_atomic_fetch,          \
                      int *: shmem_ctx_int_atomic_fetch,                \
                      long *: shmem_ctx_long_atomic_fetch,              \
                      long long *: shmem_ctx_longlong_atomic_fetch,     \
                      unsigned int *: shmem_ctx_uint_atomic_fetch,      \
                      unsigned long *: shmem_ctx_ulong_atomic_fetch,    \
                      unsigned long long *: shmem_ctx_ulonglong_atomic_fetch, \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             float *: shmem_float_atomic_fetch,                         \
             double *: shmem_double_atomic_fetch,                       \
             int *: shmem_int_atomic_fetch,                             \
             long *: shmem_long_atomic_fetch,                           \
             long long *: shmem_longlong_atomic_fetch,                  \
             unsigned int *: shmem_uint_atomic_fetch,                   \
             unsigned long *: shmem_ulong_atomic_fetch,                 \
             unsigned long long *: shmem_ulonglong_atomic_fetch,        \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_atomic_fetch_nbi() */
#define shmem_atomic_fetch_nbi(...)                                     \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      float *: shmem_ctx_float_atomic_fetch_nbi,        \
                      double *: shmem_ctx_double_atomic_fetch_nbi,      \
                      int *: shmem_ctx_int_atomic_fetch_nbi,            \
                      long *: shmem_ctx_long_atomic_fetch_nbi,          \
                      long long *: shmem_ctx_longlong_atomic_fetch_nbi, \
                      unsigned int *: shmem_ctx_uint_atomic_fetch_nbi,  \
                      unsigned long *: shmem_ctx_ulong_atomic_fetch_nbi, \
                      unsigned long long *: shmem_ctx_ulonglong_atomic_fetch_nbi, \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             float *: shmem_float_atomic_fetch_nbi,                     \
             double *: shmem_double_atomic_fetch_nbi,                   \
             int *: shmem_int_atomic_fetch_nbi,                         \
             long *: shmem_long_atomic_fetch_nbi,                       \
             long long *: shmem_longlong_atomic_fetch_nbi,              \
             unsigned int *: shmem_uint_atomic_fetch_nbi,               \
             unsigned long *: shmem_ulong_atomic_fetch_nbi,             \
             unsigned long long *: shmem_ulonglong_atomic_fetch_nbi,    \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* see \ref shmem_long_atomic_set() */
#define shmem_atomic_set(...)                                           \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             shmem_ctx_t:                                               \
             _Generic(SHC11_TYPE_EVAL_PTR_OR_SCALAR(SHC11_GET_ARG2(__VA_ARGS__)), \
                      float *: shmem_ctx_float_atomic_set,              \
                      double *: shmem_ctx_double_atomic_set,            \
                      int *: shmem_ctx_int_atomic_set,                  \
                      long *: shmem_ctx_long_atomic_set,                \
                      long long *: shmem_ctx_longlong_atomic_set,       \
                      unsigned int *: shmem_ctx_uint_atomic_set,        \
                      unsigned long *: shmem_ctx_ulong_atomic_set,      \
                      unsigned long long *: shmem_ctx_ulonglong_atomic_set, \
                      default: shmem_generics_nomatch                   \
                      ),                                                \
             float *: shmem_float_atomic_set,                           \
             double *: shmem_double_atomic_set,                         \
             int *: shmem_int_atomic_set,                               \
             long *: shmem_long_atomic_set,                             \
             long long *: shmem_longlong_atomic_set,                    \
             unsigned int *: shmem_uint_atomic_set,                     \
             unsigned long *: shmem_ulong_atomic_set,                   \
             unsigned long long *: shmem_ulonglong_atomic_set,          \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

/* waits and test have no context-based counterparts */

/* see \ref shmem_long_wait() */
#define shmem_wait(...)                                         \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),  \
             short *: shmem_short_wait,                         \
             int *: shmem_int_wait,                             \
             long *: shmem_long_wait,                           \
             long long *: shmem_longlong_wait,                  \
             unsigned short *: shmem_ushort_wait,               \
             unsigned int *: shmem_uint_wait,                   \
             unsigned long *: shmem_ulong_wait,                 \
             unsigned long long *: shmem_ulonglong_wait,        \
             default: shmem_generics_nomatch                    \
             )(__VA_ARGS__)

/* see \ref shmem_long_wait_until() */
#define shmem_wait_until(...)                                   \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),  \
             short *: shmem_short_wait_until,                   \
             int *: shmem_int_wait_until,                       \
             long *: shmem_long_wait_until,                     \
             long long *: shmem_longlong_wait_until,            \
             unsigned short *: shmem_ushort_wait_until,         \
             unsigned int *: shmem_uint_wait_until,             \
             unsigned long *: shmem_ulong_wait_until,           \
             unsigned long long *: shmem_ulonglong_wait_until,  \
             default: shmem_generics_nomatch                    \
             )(__VA_ARGS__)

/* see \ref shmem_long_test() */
#define shmem_test(...)                                         \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),  \
             short *: shmem_short_test,                         \
             int *: shmem_int_test,                             \
             long *: shmem_long_test,                           \
             long long *: shmem_longlong_test,                  \
             unsigned short *: shmem_ushort_test,               \
             unsigned int *: shmem_uint_test,                   \
             unsigned long *: shmem_ulong_test,                 \
             unsigned long long *: shmem_ulonglong_test,        \
             default: shmem_generics_nomatch                    \
             )(__VA_ARGS__)

#define shmem_wait_until_all(_ivars, _nelems,                       \
                             _status,                               \
                             _cmp, _cmp_value)                      \
    _Generic(_ivars,                                                \
             short *: shmem_short_wait_until_all,                   \
             int *: shmem_int_wait_until_all,                       \
             long *: shmem_long_wait_until_all,                     \
             long long *: shmem_longlong_wait_until_all,            \
             unsigned short *: shmem_ushort_wait_until_all,         \
             unsigned int *: shmem_uint_wait_until_all,             \
             unsigned long *: shmem_ulong_wait_until_all,           \
             unsigned long long *: shmem_ulonglong_wait_until_all,  \
             default: shmem_generics_nomatch                        \
             )(_ivars, _nelems, _status, _cmp, _cmp_value)

#define shmem_wait_until_any(_ivars, _nelems,                       \
                             _status,                               \
                             _cmp, _cmp_value)                      \
    _Generic(_ivars,                                                \
             short *: shmem_short_wait_until_any,                   \
             int *: shmem_int_wait_until_any,                       \
             long *: shmem_long_wait_until_any,                     \
             long long *: shmem_longlong_wait_until_any,            \
             unsigned short *: shmem_ushort_wait_until_any,         \
             unsigned int *: shmem_uint_wait_until_any,             \
             unsigned long *: shmem_ulong_wait_until_any,           \
             unsigned long long *: shmem_ulonglong_wait_until_any,  \
             default: shmem_generics_nomatch                        \
             )(_ivars, _nelems, _status, _cmp, _cmp_value)

#define shmem_wait_until_some(_ivars, _nelems, _indices,                \
                              _status,                                  \
                              _cmp, _cmp_value)                         \
    _Generic(_ivars,                                                    \
             short *: shmem_short_wait_until_some,                      \
             int *: shmem_int_wait_until_some,                          \
             long *: shmem_long_wait_until_some,                        \
             long long *: shmem_longlong_wait_until_some,               \
             unsigned short *: shmem_ushort_wait_until_some,            \
             unsigned int *: shmem_uint_wait_until_some,                \
             unsigned long *: shmem_ulong_wait_until_some,              \
             unsigned long long *: shmem_ulonglong_wait_until_some,     \
             default: shmem_generics_nomatch                            \
             )(_ivars, _nelems, _indices, _status, _cmp, _cmp_value)

/* see \ref shmem_long_wait_until() */
#define shmem_wait_until_all_vector(...)                                \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),          \
             short *: shmem_short_wait_until_all_vector,                \
             int *: shmem_int_wait_until_all_vector,                    \
             long *: shmem_long_wait_until_all_vector,                  \
             long long *: shmem_longlong_wait_until_all_vector,         \
             unsigned short *: shmem_ushort_wait_until_all_vector,      \
             unsigned int *: shmem_uint_wait_until_all_vector,          \
             unsigned long *: shmem_ulong_wait_until_all_vector,        \
             unsigned long long *: shmem_ulonglong_wait_until_all_vector, \
             default: shmem_generics_nomatch                            \
             )(__VA_ARGS__)

#define shmem_wait_until_some_vector(_ivars, _nelems, _indices,         \
                                     _status,                           \
                                     _cmp, _cmp_value)                  \
    _Generic(_ivars,                                                    \
             short *: shmem_short_wait_until_some_vector,               \
             int *: shmem_int_wait_until_some_vector,                   \
             long *: shmem_long_wait_until_some_vector,                 \
             long long *: shmem_longlong_wait_until_some_vector,        \
             unsigned short *: shmem_ushort_wait_until_some_vector,     \
             unsigned int *: shmem_uint_wait_until_some_vector,         \
             unsigned long *: shmem_ulong_wait_until_some_vector,       \
             unsigned long long *: shmem_ulonglong_wait_until_some_vector, \
             default: shmem_generics_nomatch                            \
             )(_ivars, _nelems, _indices, _status, _cmp, _cmp_value)

#define shmem_wait_until_any_vector(_ivars, _nelems,                    \
                                    _status,                            \
                                    _cmp, _cmp_value)                   \
    _Generic(_ivars,                                                    \
             short *: shmem_short_wait_until_any_vector,                \
             int *: shmem_int_wait_until_any_vector,                    \
             long *: shmem_long_wait_until_any_vector,                  \
             long long *: shmem_longlong_wait_until_any_vector,         \
             unsigned short *: shmem_ushort_wait_until_any_vector,      \
             unsigned int *: shmem_uint_wait_until_any_vector,          \
             unsigned long *: shmem_ulong_wait_until_any_vector,        \
             unsigned long long *: shmem_ulonglong_wait_until_any_vector, \
             default: shmem_generics_nomatch                            \
             )(_ivars, _nelems, _status, _cmp, _cmp_value)

#define shmem_test_all(_ivars, _nelems,                         \
                       _status,                                 \
                       _cmp, _cmp_value)                        \
    _Generic(_ivars,                                            \
             short *: shmem_short_test_all,                     \
             int *: shmem_int_test_all,                         \
             long *: shmem_long_test_all,                       \
             long long *: shmem_longlong_test_all,              \
             unsigned short *: shmem_ushort_test_all,           \
             unsigned int *: shmem_uint_test_all,               \
             unsigned long *: shmem_ulong_test_all,             \
             unsigned long long *: shmem_ulonglong_test_all,    \
             default: shmem_generics_nomatch                    \
             )(_ivars, _nelems, _status, _cmp, _cmp_value)

#define shmem_test_any(_ivars, _nelems,                         \
                       _status,                                 \
                       _cmp, _cmp_value)                        \
    _Generic(_ivars,                                            \
             short *: shmem_short_test_any,                     \
             int *: shmem_int_test_any,                         \
             long *: shmem_long_test_any,                       \
             long long *: shmem_longlong_test_any,              \
             unsigned short *: shmem_ushort_test_any,           \
             unsigned int *: shmem_uint_test_any,               \
             unsigned long *: shmem_ulong_test_any,             \
             unsigned long long *: shmem_ulonglong_test_any,    \
             default: shmem_generics_nomatch                    \
             )(_ivars, _nelems, _status, _cmp, _cmp_value)

#define shmem_test_some(_ivars, _nelems, _indices,                      \
                        _status,                                        \
                        _cmp, _cmp_value)                               \
    _Generic(_ivars,                                                    \
             short *: shmem_short_test_some,                            \
             int *: shmem_int_test_some,                                \
             long *: shmem_long_test_some,                              \
             long long *: shmem_longlong_test_some,                     \
             unsigned short *: shmem_ushort_test_some,                  \
             unsigned int *: shmem_uint_test_some,                      \
             unsigned long *: shmem_ulong_test_some,                    \
             unsigned long long *: shmem_ulonglong_test_some,           \
             default: shmem_generics_nomatch                            \
             )(_ivars, _nelems, _indices, _status, _cmp, _cmp_value)

/* see \ref shmem_long_test() */
#define shmem_test_all_vector(...)                                  \
    _Generic(SHC11_TYPE_EVAL_PTR(SHC11_GET_ARG1(__VA_ARGS__)),      \
             short *: shmem_short_test_all_vector,                  \
             int *: shmem_int_test_all_vector,                      \
             long *: shmem_long_test_all_vector,                    \
             long long *: shmem_longlong_test_all_vector,           \
             unsigned short *: shmem_ushort_test_all_vector,        \
             unsigned int *: shmem_uint_test_all_vector,            \
             unsigned long *: shmem_ulong_test_all_vector,          \
             unsigned long long *: shmem_ulonglong_test_all_vector, \
             default: shmem_generics_nomatch                        \
             )(__VA_ARGS__)

#define shmem_test_some_vector(_ivars, _nelems, _indices,               \
                               _status,                                 \
                               _cmp, _cmp_value)                        \
    _Generic(_ivars,                                                    \
             short *: shmem_short_test_some_vector,                     \
             int *: shmem_int_test_some_vector,                         \
             long *: shmem_long_test_some_vector,                       \
             long long *: shmem_longlong_test_some_vector,              \
             unsigned short *: shmem_ushort_test_some_vector,           \
             unsigned int *: shmem_uint_test_some_vector,               \
             unsigned long *: shmem_ulong_test_some_vector,             \
             unsigned long long *: shmem_ulonglong_test_some_vector,    \
             default: shmem_generics_nomatch                            \
             )(_ivars, _nelems, _indices, _status, _cmp, _cmp_value)

#define shmem_test_any_vector(_ivars, _nelems,                      \
                              _status,                              \
                              _cmp, _cmp_value)                     \
    _Generic(_ivars,                                                \
             short *: shmem_short_test_any_vector,                  \
             int *: shmem_int_test_any_vector,                      \
             long *: shmem_long_test_any_vector,                    \
             long long *: shmem_longlong_test_any_vector,           \
             unsigned short *: shmem_ushort_test_any_vector,        \
             unsigned int *: shmem_uint_test_any_vector,            \
             unsigned long *: shmem_ulong_test_any_vector,          \
             unsigned long long *: shmem_ulonglong_test_any_vector, \
             default: shmem_generics_nomatch                        \
             )(_ivars, _nelems, _status, _cmp, _cmp_value)

#endif /* SHMEM_HAS_C11 */

#endif  /* ! _SHMEM_API_GENERICS_H */
