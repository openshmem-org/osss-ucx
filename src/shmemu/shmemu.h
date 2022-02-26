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

#include <values.h>

#define BITS2BYTES(_n) ((_n) / CHARBITS)

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
 * communication progress
 */

void shmemu_progress_init(void);
void shmemu_progress_finalize(void);
void shmemu_progress_set_delay(long newdelay);

/*
 * rotate/spread PE communications
 */
inline static int
shmemu_shift(int pe)
{
    return (pe + proc.li.rank) % proc.li.nranks;
}

/*
 * Return non-zero if PE is a valid rank, 0 otherwise
 */
inline static int
shmemu_valid_pe_number(int pe)
{
    return (proc.li.nranks > pe) && (pe >= 0);
}

void shmemu_init(void);
void shmemu_finalize(void);

/*
 * elapsed time in seconds since program started
 */
double shmemu_timer(void);

#ifdef ENABLE_ALIGNED_ADDRESSES
void shmemu_test_asr_mismatch(void);
#endif /* ENABLE_ALIGNED_ADDRESSES */

/*
 * number manipulators
 */
int shmemu_parse_size(const char *size_str, size_t *bytes_p);
int shmemu_human_number(double bytes, char *buf, size_t buflen);
const char *shmemu_human_option(int v);
int shmemu_parse_csv(char *str, int **out, size_t *nout);

const char *shmemu_plural(size_t n);

/*
 * message logging (cf. logger.c to init these)
 */

#define LOG_ALL        "ALL"
#define LOG_INIT       "INIT"
#define LOG_FINALIZE   "FINALIZE"
#define LOG_MEMORY     "MEMORY"
#define LOG_RMA        "RMA"
#define LOG_FENCE      "FENCE"
#define LOG_QUIET      "QUIET"
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
 * bail out on major problems early/late on during setup/finalize,
 * also report unconditional warnings
 */
void shmemu_fatal(const char *fmt, ...);
void shmemu_warn(const char *fmt, ...);

typedef struct shmemu_version {
    int major;
    int minor;
} shmemu_version_t;

#ifdef ENABLE_LOGGING

typedef const char *shmemu_log_t;

typedef shmemu_log_t *shmemu_log_table_t;

void shmemu_logger(shmemu_log_t evt, const char *fmt, ...);

void shmemu_deprecate(const char *fn, const shmemu_version_t *vp);

# define logger(...) shmemu_logger(__VA_ARGS__)

# define deprecate(...) shmemu_deprecate(__VA_ARGS__)

#else  /* ENABLE_LOGGING */

# define logger(...)

# define deprecate(...)

#endif  /* ENABLE_LOGGING */

/*
 * translate between tread levels and their names
 */

const char *shmemu_thread_name(int tl);
int shmemu_thread_level(const char *tn);

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
        const int top_pe = proc.li.nranks - 1;                  \
                                                                \
        if (shmemu_unlikely((_pe < 0) || (_pe > top_pe))) {     \
            shmemu_fatal("In %s(), PE argument #%d is %d: "     \
                         "outside allocated range [%d, %d]",    \
                         __func__,                              \
                         _argpos,                               \
                         _pe,                                   \
                         0, top_pe                              \
                         );                                     \
            /* NOT REACHED */                                   \
        }                                                       \
    } while (0)

# define SHMEMU_CHECK_SYMMETRIC(_addr, _argpos)                         \
    do {                                                                \
        if (shmemu_unlikely(! shmemc_addr_accessible(_addr,             \
                                                     proc.li.rank))) {  \
            shmemu_fatal("In %s(), address %p in argument #%d "         \
                         "is not symmetric",                            \
                         __func__,                                      \
                         _addr, _argpos                                 \
                         );                                             \
            /* NOT REACHED */                                           \
        }                                                               \
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

# define SHMEMU_CHECK_NOT_NULL(_ptr, _argpos)                           \
    do {                                                                \
        if ((_ptr) == NULL) {                                           \
            shmemu_fatal("In %s(), parameter #%d \"%s\" cannot be NULL", \
                         __func__, _argpos, #_ptr                       \
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
                const unsigned long ulme = (unsigned long) me;          \
                const unsigned long ulcr = (unsigned long) cr;          \
                                                                        \
                if (! threadwrap_thread_equal(cr,  me)) {               \
                    shmemu_fatal("In %s(), invoking thread #%lu"        \
                                 " not owner thread #%lu"               \
                                 "in private context #%lu",             \
                                 __func__,                              \
                                 ulme, ulcr,                            \
                                 ch->id                                 \
                                 );                                     \
                    /* NOT REACHED */                                   \
                }                                                       \
            }                                                           \
        }                                                               \
    } while (0)

# define SHMEMU_CHECK_HEAP_INDEX(_idx)                                  \
    do {                                                                \
        const int top_heap = proc.heaps.nheaps - 1;                     \
                                                                        \
        if (shmemu_unlikely( ((_idx) < 0) || ((_idx) > top_heap) )) {   \
            shmemu_fatal("In %s(), heap index #%d"                      \
                         "is outside allocated range [%d, %d]",         \
                         __func__,                                      \
                         _idx,                                          \
                         0, top_heap                                    \
                         );                                             \
            /* NOT REACHED */                                           \
        }                                                               \
    } while (0)

# define SHMEMU_CHECK_ALLOC(_addr, _bytes)                              \
    do {                                                                \
        if ((_addr) == NULL) {                                          \
            char buf_b[16];                                             \
            char buf_h[16];                                             \
                                                                        \
            shmemu_human_number(proc.heaps.heapsize[0], buf_h, 15);     \
            shmemu_human_number(_bytes, buf_b, 15);                     \
                                                                        \
            shmemu_warn("allocating %s would overrun "                  \
                        "heap size of %s",                              \
                        buf_b, buf_h                                    \
                        );                                              \
            shmemu_warn("try increasing value of "                      \
                        "environment variable "                         \
                        "SHMEM_SYMMETRIC_SIZE");                        \
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
# define SHMEMU_CHECK_NOT_NULL(_ptr, _argpos)
# define SHMEMU_CHECK_SAME_THREAD(_ctx)
# define SHMEMU_CHECK_HEAP_INDEX(_idx)
# define SHMEMU_CHECK_ALLOC(_addr, _bytes)

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
