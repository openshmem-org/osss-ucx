/* For license: see LICENSE file at top-level */

#ifndef _SHMEM_SHEMU_H
#define _SHMEM_SHEMU_H 1

#include "state.h"
#include "shmemc.h"
#include "shmem/defs.h"
#include "threading.h"

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <sys/types.h>
#include <stdarg.h>
#include <string.h>

/* ------------------------------------------------------------------ */

/*
 * for branch prediction optimization
 *
 */
#ifdef HAVE___BUILTIN_EXPECT
# define shmemu_likely(_expr)   __builtin_expect(!!(_expr), 1)
# define shmemu_unlikely(_expr) __builtin_expect(!!(_expr), 0)
#else  /* HAVE___BUILTIN_EXPECT */
# define shmemu_likely(_expr)   (_expr)
# define shmemu_unlikely(_expr) (_expr)
#endif  /* HAVE___BUILTIN_EXPECT */

/*
 * safer string handling
 */
#ifdef HAVE_STRLCPY
# define STRNCPY_SAFE (void) strlcpy
#else
# define STRNCPY_SAFE (void) strncpy
#endif /* HAVE_STRLCPY */

#ifdef HAVE_STRLCAT
# define STRNCAT_SAFE (void) strlcat
#else
# define STRNCAT_SAFE (void) strncat
#endif /* HAVE_STRLCAT */

/*
 * rotate/spread PE communications
 */
#define SHIFT(_pe) ( ((_pe) + proc.rank) % proc.nranks )

/*
 * Return non-zero if PE is a valid rank, 0 otherwise
 */
#define IS_VALID_PE_NUMBER(_pe)                 \
    ((proc.nranks > (_pe) ) && ( (_pe) >= 0))

void shmemu_init(void);
void shmemu_finalize(void);

/*
 * elapsed time in seconds since program started
 */
void shmemu_timer_init(void);
void shmemu_timer_finalize(void);
double shmemu_timer(void);

/*
 * number manipulators
 */
char *shmemu_gethostname(void);
int shmemu_parse_size(char *size_str, size_t *bytes_p);
int shmemu_human_number(double bytes, char *buf, size_t buflen);
char *shmemu_human_option(int v);
int shmemu_parse_csv(char *str, int **out, size_t *nout);

/*
 * message logging (cf. logger.c to init these)
 */

#define LOG_ALL        "ALL"
#define LOG_FATAL      "FATAL"
#define LOG_INIT       "INIT"
#define LOG_FINALIZE   "FINALIZE"
#define LOG_MEMORY     "MEMORY"
#define LOG_RMA        "RMA"
#define LOG_FENCE      "FENCE"
#define LOG_HEAPS      "HEAPS"
#define LOG_CONTEXTS   "CONTEXTS"
#define LOG_RANKS      "RANKS"
#define LOG_INFO       "INFO"
#define LOG_REDUCTIONS "REDUCTIONS"
#define LOG_BARRIERS   "BARRIERS"
#define LOG_DEPRECATE  "DEPRECATE"
#define LOG_LOCKS      "LOCKS"
#define LOG_ATOMICS    "ATOMICS"
#define LOG_UNKNOWN    "UNKNOWN"

/*
 * bail out on major problems early/late on during setup/finalize
 */
void shmemu_fatal(const char *fmt, ...);

#ifdef ENABLE_LOGGING

typedef const char *shmemu_log_t;

typedef shmemu_log_t *shmemu_log_table_t;

void shmemu_logger_init(void);
void shmemu_logger_finalize(void);

void shmemu_logger(shmemu_log_t evt, const char *fmt, ...);
void shmemu_deprecate(const char *fn, int maj, int min);

# define logger(...) shmemu_logger(__VA_ARGS__)

# define deprecate(...) shmemu_deprecate(__VA_ARGS__)
void shmemu_deprecate_init(void);
void shmemu_deprecate_finalize(void);

#else  /* ENABLE_LOGGING */

# define shmemu_logger_init()
# define shmemu_logger_finalize()

# define logger(...)

# define deprecate(...)
# define shmemu_deprecate_init()
# define shmemu_deprecate_finalize()

#endif  /* ENABLE_LOGGING */

/*
 * mark args as unused
 */
#define NO_WARN_UNUSED(_obj) (void)(_obj)

#ifdef ENABLE_DEBUG

/*
 * our own assertion check.  Usage:
 *
 * shmemu_assert(condition-check, shmemu_fatal args...)
 */
# define shmemu_assert(_cond, ...)              \
    do {                                        \
        if (! (_cond)) {                        \
            shmemu_fatal(__VA_ARGS__);          \
        }                                       \
    } while (0)

/*
 * sanity checks
 */
# define SHMEMU_CHECK_PE_ARG_RANGE(_pe, _argpos)                \
    do {                                                        \
        const int top_pe = proc.nranks - 1;                     \
                                                                \
        if (shmemu_unlikely((_pe < 0) || (_pe > top_pe))) {     \
            logger(LOG_FATAL,                                   \
                   "In %s(), PE argument #%d is %d: "           \
                   "outside allocated range [%d, %d]",          \
                   __func__,                                    \
                   _argpos,                                     \
                   _pe,                                         \
                   0, top_pe                                    \
                   );                                           \
            /* NOT REACHED */                                   \
        }                                                       \
    } while (0)

# define SHMEMU_CHECK_SYMMETRIC(_addr, _argpos)                     \
    do {                                                            \
        if (shmemu_unlikely(! shmemc_addr_accessible(_addr,         \
                                                     proc.rank))) { \
            logger(LOG_FATAL,                                       \
                   "In %s(), address %p in argument #%d "           \
                   "is not symmetric",                              \
                   __func__,                                        \
                   _addr, _argpos                                   \
                   );                                               \
            /* NOT REACHED */                                       \
        }                                                           \
    } while (0)

# define SHMEMU_CHECK_INIT()                                            \
    do {                                                                \
        if (shmemu_unlikely(proc.refcount < 1)) {                       \
            shmemu_fatal("In %s(), attempt to use OpenSHMEM library"    \
                         " before initialization",                      \
                         __func__                                       \
                         );                                             \
            /* NOT REACHED */                                           \
        }                                                               \
    } while (0)

# define SHMEMU_CHECK_SAME_THREAD(_ctx)                                 \
    do {                                                                \
        if (shmemu_likely(ctx != SHMEM_CTX_INVALID)) {                  \
            shmemc_context_h ch = (shmemc_context_h) (_ctx);            \
                                                                        \
            if (ch->attr.private) {                                     \
                const threadwrap_thread_t me = threadwrap_thread_id();  \
                const threadwrap_thread_t cr = ch->creator_thread;      \
                                                                        \
                if (! threadwrap_thread_equal(cr,  me)) {               \
                    shmemu_fatal("In %s(), invoking thread #%d"         \
                                 " not owner thread #%d"                \
                                 "in private context #%lu",             \
                                 __func__,                              \
                                 me, cr,                                \
                                 ch->id                                 \
                                 );                                     \
                    /* NOT REACHED */                                   \
                }                                                       \
            }                                                           \
        }                                                               \
    } while (0)

# define SHMEMU_CHECK_HEAP_INDEX(_idx)                                  \
    do {                                                                \
        const int top_heap = proc.env.heaps.nheaps - 1;                 \
                                                                        \
        if (shmemu_unlikely( ((_idx) < 0) || ((_idx) > top_heap) )) {   \
            logger(LOG_FATAL,                                           \
                   "In %s(), heap index #%d"                            \
                   "is outside allocated range [%d, %d]",               \
                   __func__,                                            \
                   _idx,                                                \
                   0, top_heap                                          \
                   );                                                   \
            /* NOT REACHED */                                           \
        }                                                               \
    } while (0)

#else  /* ! ENABLE_DEBUG */

/*
 * prevent unused-variable warnings
 */
# define shmemu_assert(_cond, ...) NO_WARN_UNUSED(_cond)

# define SHMEMU_CHECK_PE_ARG_RANGE(_pe, _argpos)
# define SHMEMU_CHECK_SYMMETRIC(_addr, _argpos)
# define SHMEMU_CHECK_INIT()
# define SHMEMU_CHECK_SAME_THREAD(_ctx)
# define SHMEMU_CHECK_HEAP_INDEX(idx)

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
/* for Fortran */
SHMEMU_DECL_MATH_FUNC(complexd, COMPLEXIFY(double))
SHMEMU_DECL_MATH_FUNC(complexf, COMPLEXIFY(float))

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
