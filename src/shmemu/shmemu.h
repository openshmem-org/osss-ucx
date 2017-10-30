/* For license: see LICENSE file at top-level */

#ifndef _SHMEM_SHEMU_H
#define _SHMEM_SHEMU_H 1

#include "shmem/defs.h"

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <sys/types.h>

/*
 * how many elements in array T?
 *
 */
#define TABLE_SIZE(_t) ( sizeof(_t) / sizeof((_t)[0]) )

void shmemu_init(void);
void shmemu_finalize(void);

void shmemu_timer_init(void);
void shmemu_timer_finalize(void);
double shmemu_timer(void);

char *shmemu_gethostname(void);

int shmemu_parse_size(char *size_str, size_t *bytes_p);

/*
 * message logging
 */

void shmemu_logger_init(void);
void shmemu_logger_finalize(void);

typedef enum shmemu_log {
    LOG_FATAL       = SHMEM_BIT_SET(0),
    LOG_INIT        = SHMEM_BIT_SET(1),
    LOG_FINALIZE    = SHMEM_BIT_SET(2),
    LOG_MEMORY      = SHMEM_BIT_SET(3),
    LOG_HEAP        = SHMEM_BIT_SET(4),
    LOG_WORKER      = SHMEM_BIT_SET(5),
    LOG_INFO        = SHMEM_BIT_SET(6),
    LOG_REDUCTION   = SHMEM_BIT_SET(7),
    LOG_BARRIER     = SHMEM_BIT_SET(8),
    LOG_DEPRECATE   = SHMEM_BIT_SET(9),
    LOG_LOCK        = SHMEM_BIT_SET(10),
    LOG_ALL         = (LOG_FATAL | LOG_INIT | LOG_FINALIZE |
                       LOG_MEMORY | LOG_HEAP | LOG_WORKER | LOG_INFO |
                       LOG_REDUCTION | LOG_BARRIER | LOG_DEPRECATE | LOG_LOCK),
    LOG_UNKNOWN     = -1
} shmemu_log_t;

#ifdef ENABLE_DEBUG

void shmemu_logger(shmemu_log_t level, const char *fmt, ...);
void shmemu_deprecate(const char *fn);

# define logger(...) shmemu_logger(__VA_ARGS__)
# define deprecate(_fn) shmemu_deprecate(_fn)
void shmemu_deprecate_init();
void shmemu_deprecate_finalize();

#else

# define logger(...)
# define deprecate(_fn)
# define shmemu_deprecate_init()
# define shmemu_deprecate_finalize()

#endif /* ENABLE_DEBUG */



#include <sys/types.h>
#include <stdint.h>
#include <stddef.h>
#include <complex.h>

#define SHMEMU_DECL_MATH_FUNC(_name, _type)                 \
    _type shmemu_sum_##_name##_func(_type _a, _type _b);    \
    _type shmemu_prod_##_name##_func(_type _a, _type _b);

SHMEMU_DECL_MATH_FUNC(float, float)
SHMEMU_DECL_MATH_FUNC(double, double)
SHMEMU_DECL_MATH_FUNC(short, short)
SHMEMU_DECL_MATH_FUNC(int, int)
SHMEMU_DECL_MATH_FUNC(long, long)
SHMEMU_DECL_MATH_FUNC(longlong, long long)
SHMEMU_DECL_MATH_FUNC(longdouble, long double)
SHMEMU_DECL_MATH_FUNC(uint, unsigned int)
SHMEMU_DECL_MATH_FUNC(ulong, unsigned long)
SHMEMU_DECL_MATH_FUNC(ulonglong, unsigned long long)
SHMEMU_DECL_MATH_FUNC(int32, int32_t)
SHMEMU_DECL_MATH_FUNC(int64, int64_t)
SHMEMU_DECL_MATH_FUNC(uint32, uint32_t)
SHMEMU_DECL_MATH_FUNC(uint64, uint64_t)
SHMEMU_DECL_MATH_FUNC(complexd, double complex)
SHMEMU_DECL_MATH_FUNC(complexf, float complex)

/**
 * these are the logical operations.  Note: these are *bitwise*.
 *
 */

#define SHMEMU_DECL_LOGIC_FUNC(_name, _type)                \
    _type shmemu_and_##_name##_func(_type _a, _type _b);    \
    _type shmemu_or_##_name##_func(_type _a, _type _b);     \
    _type shmemu_xor_##_name##_func(_type _a, _type _b);

SHMEMU_DECL_LOGIC_FUNC(short, short)
SHMEMU_DECL_LOGIC_FUNC(int, int)
SHMEMU_DECL_LOGIC_FUNC(long, long)
SHMEMU_DECL_LOGIC_FUNC(longlong, long long)
SHMEMU_DECL_LOGIC_FUNC(uint, unsigned int)
SHMEMU_DECL_LOGIC_FUNC(ulong, unsigned long)
SHMEMU_DECL_LOGIC_FUNC(ulonglong, unsigned long long)
SHMEMU_DECL_LOGIC_FUNC(int32, int32_t)
SHMEMU_DECL_LOGIC_FUNC(int64, int64_t)
SHMEMU_DECL_LOGIC_FUNC(uint32, uint32_t)
SHMEMU_DECL_LOGIC_FUNC(uint64, uint64_t)

/**
 * these are the minima/maxima operations
 *
 */

#define SHMEMU_DECL_MINIMAX_FUNC(_name, _type)              \
    _type shmemu_min_##_name##_func(_type _a, _type _b);    \
    _type shmemu_max_##_name##_func(_type _a, _type _b);

SHMEMU_DECL_MINIMAX_FUNC(short, short)
SHMEMU_DECL_MINIMAX_FUNC(int, int)
SHMEMU_DECL_MINIMAX_FUNC(long, long)
SHMEMU_DECL_MINIMAX_FUNC(longlong, long long)
SHMEMU_DECL_MINIMAX_FUNC(float, float)
SHMEMU_DECL_MINIMAX_FUNC(double, double)
SHMEMU_DECL_MINIMAX_FUNC(longdouble, long double)
SHMEMU_DECL_MINIMAX_FUNC(uint, unsigned int)
SHMEMU_DECL_MINIMAX_FUNC(ulong, unsigned long)
SHMEMU_DECL_MINIMAX_FUNC(ulonglong, unsigned long long)
SHMEMU_DECL_MINIMAX_FUNC(int32, int32_t)
SHMEMU_DECL_MINIMAX_FUNC(int64, int64_t)
SHMEMU_DECL_MINIMAX_FUNC(uint32, uint32_t)
SHMEMU_DECL_MINIMAX_FUNC(uint64, uint64_t)

#endif /* ! _SHMEM_SHEMU_H */
