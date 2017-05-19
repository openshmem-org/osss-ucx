#ifndef _SHMEMC_GASNET_COMMON_H
#define _SHMEMC_GASNET_COMMON_H 1

#include <gasnet.h>

#if defined(GASNET_SEGMENT_FAST)
#define HAVE_MANAGED_SEGMENTS 1
#elif defined(GASNET_SEGMENT_LARGE)
#define HAVE_MANAGED_SEGMENTS 1
#elif defined(GASNET_SEGMENT_EVERYTHING)
#undef HAVE_MANAGED_SEGMENTS
#else
#error "I don't know what kind of GASNet segment model you're trying to use"
#endif

#include "bail.h"

/**
 * trap gasnet errors gracefully
 *
 */
#define GASNET_SAFE(_fncall)                                        \
    do {                                                            \
        const int _retval = _fncall;                                \
        if (_retval != GASNET_OK) {                                 \
            shmemc_bailout("error calling: %s at %s:%i, %s (%s)\n", \
                           #_fncall, __FILE__, __LINE__,            \
                           gasnet_ErrorName(_retval),               \
                           gasnet_ErrorDesc(_retval)                \
                           );                                       \
        }                                                           \
    } while(0)

#define USING_IMPLICIT_HANDLES 1

#if USING_IMPLICIT_HANDLES
#define GASNET_PUT(pe, dst, src, len)           \
    gasnet_put_nbi (pe, dst, src, len)
#define GASNET_PUT_BULK(pe, dst, src, len)                              \
    gasnet_put_bulk (pe, dst, src, len) /* not nbi (see GASNet spec) */
#define GASNET_PUT_VAL(pe, dst, src, len)       \
    gasnet_put_nbi_val (pe, dst, src, len)
#define GASNET_WAIT_PUTS()                      \
    gasnet_wait_syncnbi_puts ()
#define GASNET_WAIT_ALL()                       \
    gasnet_wait_syncnbi_all ()
#else
#define GASNET_PUT(pe, dst, src, len)           \
    gasnet_put (pe, dst, src, len)
#define GASNET_PUT_BULK(pe, dst, src, len)      \
    gasnet_put_bulk (pe, dst, src, len)
#define GASNET_PUT_VAL(pe, dst, src, len)       \
    gasnet_put_val (pe, dst, src, len)
#define GASNET_WAIT_PUTS()
#define GASNET_WAIT_ALL()
#endif /* USING_IMPLICIT_HANDLES */

#define GASNET_GET(dst, pe, src, len)           \
    gasnet_get (dst, pe, src, len)
#define GASNET_GET_BULK(dst, pe, src, len)      \
    gasnet_get_bulk (dst, pe, src, len)

/**
 * explicitly use gasnet _nbi series function for implicit-handle nbi
 * operation and for shmem _nbi series function implementation
 *
 */
#define GASNET_PUT_NBI(pe, dst, src, len)       \
    gasnet_put_nbi (pe, dst, src, len)
#define GASNET_PUT_NBI_BULK(pe, dst, src, len)  \
    gasnet_put_nbi_bulk (pe, dst, src, len)
#define GASNET_GET_NBI(dst, pe, src, len)       \
    gasnet_get_nbi (dst, pe, src, len)
#define GASNET_GET_NBI_BULK(dst, pe, src, len)  \
    gasnet_get_nbi_bulk (dst, pe, src, len)

/**
 * ---------------------------------------------------------------------------
 *
 * lookup where another PE stores things
 *
 */

/**
 * where the symmetric memory lives on the given PE
 */
#define SHMEM_SYMMETRIC_HEAP_BASE(p) (seginfo_table[(p)].addr)
#define SHMEM_SYMMETRIC_HEAP_SIZE(p) (seginfo_table[(p)].size)

#endif /* ! _SHMEMC_GASNET_COMMON_H */
