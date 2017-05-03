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

/**
 * set up segment/symmetric handling
 *
 */

gasnet_seginfo_t *seginfo_table;

#if ! defined(HAVE_MANAGED_SEGMENTS)

/**
 * this will be malloc'ed so we can respect setting from environment
 * variable
 */

#define DEFAULT_HEAP_SIZE 33554432L /* 32M */

#else

typedef struct globalvar_payload
{
    size_t nbytes;              /* size of write */
    void *target;               /* where to write */
    void *source;               /* data we want to get */
    volatile int completed;     /* transaction end marker */
    volatile int *completed_addr;   /* addr of marker */
} globalvar_payload_t;

#endif /* ! HAVE_MANAGED_SEGMENTS */

#define AMO_PAYLOAD_EMIT(Name, Type)                                \
    typedef struct                                                  \
    {                                                               \
        Type *r_symm_addr;            /* recipient symmetric var */ \
        Type value;                   /* value to be swapped */     \
        Type *value_addr;             /* where value lives */       \
        Type cond;                    /* conditional value */       \
                                                                    \
        volatile int completed;       /* transaction end marker */  \
        volatile int *completed_addr; /* addr of marker */          \
    } amo_payload_##Name##_t;

AMO_PAYLOAD_EMIT(int, int)
AMO_PAYLOAD_EMIT(long, long)
AMO_PAYLOAD_EMIT(longlong, long long)
AMO_PAYLOAD_EMIT(float, float)
AMO_PAYLOAD_EMIT(double, double)

/**
 * set up segment/symmetric handling
 *
 */

gasnet_seginfo_t *seginfo_table;

#if ! defined(HAVE_MANAGED_SEGMENTS)

/**
 * this will be malloc'ed so we can respect setting from environment
 * variable
 */

void *great_big_heap;

/**
 * remotely modified, stop it being put in a register
 */
volatile int seg_setup_replies_received = 0;

gasnet_hsl_t setup_out_lock = GASNET_HSL_INITIALIZER;
gasnet_hsl_t setup_bak_lock = GASNET_HSL_INITIALIZER;

#endif /* ! HAVE_MANAGED_SEGMENTS */

/**
 * remotely modified, stop it being put in a register
 */
volatile int globalexit_replies_received = 0;

gasnet_hsl_t globalexit_out_lock = GASNET_HSL_INITIALIZER;
gasnet_hsl_t globalexit_bak_lock = GASNET_HSL_INITIALIZER;

/**
 * Initialize handler locks.  OpenSHMEM 1.3++ guarantees per-datatype
 * exclusivity, so prep for that below.
 */

#define AMO_LOCK_DECL_EMIT(Name) \
    gasnet_hsl_t amo_lock_##Name = GASNET_HSL_INITIALIZER;

AMO_LOCK_DECL_EMIT(intt)
AMO_LOCK_DECL_EMIT(long)
AMO_LOCK_DECL_EMIT(longlong)
AMO_LOCK_DECL_EMIT(float)
AMO_LOCK_DECL_EMIT(double)

/**
 * global barrier counters
 */

long barcount = 0;
int barflag = 0;
