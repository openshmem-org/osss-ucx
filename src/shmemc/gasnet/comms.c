#include <gasnet.h>

#include "gasnet-common.h"
#include "service.h"
#include "locality.h"

#include "shmemu.h"
#include "shmemi.h"
#include "memfence.h"

#if defined(GASNET_SEGMENT_FAST)
# define HAVE_MANAGED_SEGMENTS 1
#elif defined(GASNET_SEGMENT_LARGE)
# define HAVE_MANAGED_SEGMENTS 1
#elif defined(GASNET_SEGMENT_EVERYTHING)
# undef HAVE_MANAGED_SEGMENTS
#else
# error "I don't know what kind of GASNet segment model you're trying to use"
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

typedef struct globalvar_payload {
    size_t nbytes;              /* size of write */
    void *target;               /* where to write */
    void *source;               /* data we want to get */
    volatile int completed;     /* transaction end marker */
    volatile int *completed_addr;   /* addr of marker */
} globalvar_payload_t;

#endif /* ! HAVE_MANAGED_SEGMENTS */

#define AMO_PAYLOAD_TYPE_EMIT(_name, _type)                             \
    typedef struct amo_payload_##_name                                  \
    {                                                                   \
        _type *r_symm_addr;            /* recipient symmetric var */    \
        _type value;                   /* value to be swapped */        \
        _type *value_addr;             /* where value lives */          \
        _type cond;                    /* conditional value */          \
                                                                        \
        volatile int completed;       /* transaction end marker */      \
        volatile int *completed_addr; /* addr of marker */              \
    } amo_payload_##_name##_t;

AMO_PAYLOAD_TYPE_EMIT(int, int)
AMO_PAYLOAD_TYPE_EMIT(long, long)
AMO_PAYLOAD_TYPE_EMIT(longlong, long long)
AMO_PAYLOAD_TYPE_EMIT(float, float)
AMO_PAYLOAD_TYPE_EMIT(double, double)

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

#define AMO_LOCK_DECL_EMIT(_name) \
    gasnet_hsl_t amo_lock_##_name = GASNET_HSL_INITIALIZER;

AMO_LOCK_DECL_EMIT(int)
AMO_LOCK_DECL_EMIT(long)
AMO_LOCK_DECL_EMIT(longlong)
AMO_LOCK_DECL_EMIT(float)
AMO_LOCK_DECL_EMIT(double)

/**
 * global barrier counters
 */

long barcount = 0;
int barflag = 0;


/**
 * This file provides the layer on top of GASNet, ARMCI or whatever.
 * API should be formalized at some point, but basically everything
 * non-static that starts with "shmemc_"
 */

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>

/**
 * --------------- real work starts here ---------------------
 *
 */

void shmemc_exit(int status);
void *shmemi_symmetric_addr_lookup(void *dest, int pe);

/**
 * ---------------------------------------------------------------------------
 *
 * global barrier done through gasnet
 *
 */

void
shmemc_barrier_all(void)
{
    /* use gasnet's global barrier */
    gasnet_barrier_notify(barcount, barflag);
    GASNET_SAFE(gasnet_barrier_wait(barcount, barflag));

    barcount += 1;
}

/**
 * ---------------------------------------------------------------------------
 *
 * lookup where another PE stores things
 *
 */

/**
 * translate my "dest" to corresponding address on PE "pe"
 */
void *
shmemi_symmetric_addr_lookup(void *dest, int pe)
{
    /* globals are in same place everywhere */
    if (shmemi_symmetric_is_globalvar(dest)) {
        return dest;
    }

    /* symmetric if inside of heap */
    {
        const size_t al =
            (size_t) SHMEM_SYMMETRIC_HEAP_BASE(proc.rank); /* lower bound */
        const size_t aao = (size_t) dest; /* my addr as offset */
        const long offset = aao - al;

        /* trap addresses outside the heap */
        if (offset < 0) {
            return NULL;
        }
        if (offset > SHMEM_SYMMETRIC_HEAP_SIZE(proc.rank)) {
            return NULL;
        }

        /* and where it is in the remote heap */
        return SHMEM_SYMMETRIC_HEAP_BASE(pe) + offset;
    }
}

/*
 * --------------------------------------------------------------
 *
 * GASNet allows applications to use handler codes 128-255.
 *
 * See http://gasnet.cs.berkeley.edu/dist/docs/gasnet.html, under
 * description of gasnet_attach ()
 */

#define AMO_HANDLER_DEF(_op, _name)              \
    GASNET_HANDLER_##_op##_out_##_name, GASNET_HANDLER_##_op##_bak_##_name

enum {
    GASNET_HANDLER_setup_out = 128,
    GASNET_HANDLER_setup_bak,

    AMO_HANDLER_DEF(swap, int),
    AMO_HANDLER_DEF(swap, long),
    AMO_HANDLER_DEF(swap, longlong),
    AMO_HANDLER_DEF(swap, float),
    AMO_HANDLER_DEF(swap, double),

    AMO_HANDLER_DEF(cswap, int),
    AMO_HANDLER_DEF(cswap, long),
    AMO_HANDLER_DEF(cswap, longlong),

    AMO_HANDLER_DEF(fadd, int),
    AMO_HANDLER_DEF(fadd, long),
    AMO_HANDLER_DEF(fadd, longlong),

    AMO_HANDLER_DEF(finc, int),
    AMO_HANDLER_DEF(finc, long),
    AMO_HANDLER_DEF(finc, longlong),

    AMO_HANDLER_DEF(add, int),
    AMO_HANDLER_DEF(add, long),
    AMO_HANDLER_DEF(add, longlong),

    AMO_HANDLER_DEF(inc, int),
    AMO_HANDLER_DEF(inc, long),
    AMO_HANDLER_DEF(inc, longlong),

    AMO_HANDLER_DEF(xor, int),
    AMO_HANDLER_DEF(xor, long),
    AMO_HANDLER_DEF(xor, longlong),

    AMO_HANDLER_DEF(fetch, int),
    AMO_HANDLER_DEF(fetch, long),
    AMO_HANDLER_DEF(fetch, longlong),
    AMO_HANDLER_DEF(fetch, float),
    AMO_HANDLER_DEF(fetch, double),

    AMO_HANDLER_DEF(set, int),
    AMO_HANDLER_DEF(set, long),
    AMO_HANDLER_DEF(set, longlong),
    AMO_HANDLER_DEF(set, float),
    AMO_HANDLER_DEF(set, double),

    GASNET_HANDLER_globalvar_put_out,
    GASNET_HANDLER_globalvar_put_bak,
    GASNET_HANDLER_globalvar_get_out,
    GASNET_HANDLER_globalvar_get_bak,

    GASNET_HANDLER_globalexit_out
    /* no reply partner for global_exit */
};

/**
 * can't just call getenv, it might not pass through environment
 * info to other nodes from launch.
 */
char *
shmemc_getenv(const char *name)
{
    return gasnet_getenv(name);
}

/**
 * work out how big the symmetric segment areas should be.
 *
 * Either from environment setting, or default value from
 * implementation
 */
size_t
shmemc_get_segment_size(void)
{
    char *mlss_str = shmemc_getenv("SHMEM_SYMMETRIC_HEAP_SIZE");
    size_t retval;
    int ok;

    if (mlss_str == NULL) {
#ifdef HAVE_MANAGED_SEGMENTS
        return (size_t) gasnet_getMaxLocalSegmentSize();
#else
        return DEFAULT_HEAP_SIZE;
#endif
    }

    ok = shmemu_parse_size(mlss_str, &retval);
    if (ok) {
        /* make sure aligned to page size multiples */
        const size_t mod = retval % GASNET_PAGESIZE;

        if (mod != 0) {
            const size_t div = retval / GASNET_PAGESIZE;
            retval = (div + 1) * GASNET_PAGESIZE;
        }

        return retval;
        /* NOT REACHED */
    }

    shmemc_bailout("Unusable symmetric heap size \"%s\"", mlss_str);
    /* NOT REACHED */
    return 0;
}

/**
 * ---------------------------------------------------------------------------
 *
 * initialize the symmetric segments.
 *
 * In the gasnet fast/large models, use the attached segments and
 * manage address translations through the segment table
 *
 * In the everything model, we allocate on our own heap and send out
 * the addresses with active messages
 */

#if ! defined(HAVE_MANAGED_SEGMENTS)

/**
 * unpack buf from sender PE and store seg info locally.  Ack. receipt.
 */
static void
handler_segsetup_out(gasnet_token_t token, void *buf, size_t bufsiz)
{
    gasnet_node_t src_pe;
    gasnet_seginfo_t *gsp = (gasnet_seginfo_t *) buf;

    /*
     * no lock here: each PE writes exactly once to its own array index,
     * and only to that...
     */

    /* gasnet_hsl_lock(& setup_out_lock); */

    GASNET_SAFE(gasnet_AMGetMsgSource(token, &src_pe));

    seginfo_table[(int) src_pe].addr = gsp->addr;
    seginfo_table[(int) src_pe].size = gsp->size;

    /* gasnet_hsl_unlock(& setup_out_lock); */

    gasnet_AMReplyMedium0(token, GASNET_HANDLER_setup_bak, (void *) NULL, 0);
}

/**
 * record receipt ack.  We only need to count the number of replies
 */
static void
handler_segsetup_bak(gasnet_token_t token, void *buf, size_t bufsiz)
{
    gasnet_hsl_lock(&setup_bak_lock);

    seg_setup_replies_received += 1;

    gasnet_hsl_unlock(&setup_bak_lock);
}

#endif /* ! HAVE_MANAGED_SEGMENTS */

/**
 * initialize the symmetric memory, taking into account the different
 * gasnet configurations
 */

void
shmemi_symmetric_memory_init(void)
{
    /*
     * calloc zeroes for us
     */
    seginfo_table =
        (gasnet_seginfo_t *) calloc(proc.nranks, sizeof(gasnet_seginfo_t));
    if (seginfo_table == NULL) {
        shmemc_bailout("could not allocate GASNet segments (%s)",
                       strerror(errno)
                       );
        /* NOT REACHED */
    }

    /*
     * prep the segments for use across all PEs
     *
     */

    {
#ifdef HAVE_MANAGED_SEGMENTS

        /* gasnet handles the segment allocation for us */
        GASNET_SAFE(gasnet_getSegmentInfo(seginfo_table, proc.nranks));

#else

        const size_t heapsize = proc.heapsize;
        int pm_r;

        /* allocate the heap - has to be pagesize aligned */
        pm_r = posix_memalign(&great_big_heap, GASNET_PAGESIZE, heapsize);
        if (pm_r != 0) {
            shmemc_bailout("unable to allocate "
                           "symmetric heap "
                           "(%s)",
                           strerror(pm_r)
                           );
            /* NOT REACHED */
        }

        /* everyone has their local info before exchanging messages */
        shmemc_barrier_all();

        /* store my own heap entry */
        seginfo_table[proc.rank].addr = great_big_heap;
        seginfo_table[proc.rank].size = heapsize;

        {
            gasnet_seginfo_t gs;
            int pe;

            gs.addr = great_big_heap;
            gs.size = heapsize;

            for (pe = 0; pe < proc.nranks; pe += 1) {
                /* send to everyone else */
                if (proc.rank != pe) {
                    gasnet_AMRequestMedium0(pe, GASNET_HANDLER_setup_out,
                                            &gs, sizeof(gs)
                                            );
                }
            }

            /* now wait on the AM replies (0-based AND don't count myself) */
            GASNET_BLOCKUNTIL(seg_setup_replies_received == proc.nranks - 1);
        }

#endif /* HAVE_MANAGED_SEGMENTS */
    }

    /* initialize my heap */
    // shmemi_mem_init(seginfo_table[me].addr, seginfo_table[me].size);

    /* and make sure everyone is up-to-speed */
    /* shmemc_barrier_all (); */

}

/**
 * shut down the memory allocation handler
 */
void
shmemi_symmetric_memory_finalize(void)
{
    // shmemi_mem_finalize();
#if ! defined(HAVE_MANAGED_SEGMENTS)
    free(great_big_heap);
#endif /* HAVE_MANAGED_SEGMENTS */
}

/**
 * -- atomics handlers ------------------------------------------------------
 */

/*
 * to wait on remote updates
 */

#define VOLATILIZE(_type, _var) (* ( volatile _type *) (_var))

#define COMMS_WAIT_TYPE(_name, _type, _opname, _op)                     \
    void                                                                \
    shmemc_wait_##_opname##_##_name(volatile _type *var, _type cmp_value) \
    {                                                                   \
        GASNET_BLOCKUNTIL( VOLATILIZE(_type, var) _op cmp_value );      \
    }

COMMS_WAIT_TYPE(int, int, eq, ==)
COMMS_WAIT_TYPE(long, long, eq, ==)
COMMS_WAIT_TYPE(longlong, long long, eq, ==)

COMMS_WAIT_TYPE(int, int, ne, !=)
COMMS_WAIT_TYPE(long, long, ne, !=)
COMMS_WAIT_TYPE(longlong, long long, ne, !=)

COMMS_WAIT_TYPE(int, int, gt, >)
COMMS_WAIT_TYPE(long, long, gt, >)
COMMS_WAIT_TYPE(longlong, long long, gt, >)

COMMS_WAIT_TYPE(int, int, le, <=)
COMMS_WAIT_TYPE(long, long, le, <=)
COMMS_WAIT_TYPE(longlong, long long, le, <=)

COMMS_WAIT_TYPE(int, int, lt, <)
COMMS_WAIT_TYPE(long, int, lt, <)
COMMS_WAIT_TYPE(longlong, long long, lt, <)

COMMS_WAIT_TYPE(int, int, ge, >=)
COMMS_WAIT_TYPE(long, long, ge, >=)
COMMS_WAIT_TYPE(longlong, long long, ge, >=)

#define WAIT_ON_COMPLETION(Cond)   GASNET_BLOCKUNTIL(Cond)

/* TODO: need a handler per-datatype to get the correct handler lock.
   We can do this easily with a template for the out/bak RPCs and the
   request generator itself. */

/**
 * called by remote PE to do the swap.  Store new value, send back old value
 */
#define AMO_SWAP_BAK_EMIT(_name, _type)                                 \
    static void                                                         \
    handler_swap_out_##_name(gasnet_token_t token, void *buf, size_t bufsiz) \
    {                                                                   \
        _type old;                                                      \
        amo_payload_##_name##_t *pp = (amo_payload_##_name##_t *) buf;  \
                                                                        \
        gasnet_hsl_lock(&amo_lock_##_name);                             \
        old = *(pp->r_symm_addr);                                       \
        *(pp->r_symm_addr) = pp->value;                                 \
        pp->value = old;                                                \
        LOAD_STORE_FENCE();                                             \
        gasnet_hsl_unlock(&amo_lock_##_name);                           \
        gasnet_AMReplyMedium0(token, GASNET_HANDLER_swap_bak_##_name,   \
                              buf, bufsiz);                             \
    }

AMO_SWAP_BAK_EMIT(int, int)
AMO_SWAP_BAK_EMIT(long, long)
AMO_SWAP_BAK_EMIT(longlong, long long)
AMO_SWAP_BAK_EMIT(float, float)
AMO_SWAP_BAK_EMIT(double, double)

/**
 * called by swap invoker when old value returned by remote PE
 */
#define AMO_SWAP_OUT_EMIT(_name, _type)                                 \
    static void                                                         \
    handler_swap_bak_##_name(gasnet_token_t token, void *buf, size_t bufsiz) \
    {                                                                   \
        amo_payload_##_name##_t *pp = (amo_payload_##_name##_t *) buf;  \
                                                                        \
        gasnet_hsl_lock(&amo_lock_##_name);                             \
        *(pp->value_addr) = pp->value;                                  \
        LOAD_STORE_FENCE();                                             \
        *(pp->completed_addr) = 1;                                      \
        gasnet_hsl_unlock(&amo_lock_##_name);                           \
    }

AMO_SWAP_OUT_EMIT(int, int)
AMO_SWAP_OUT_EMIT(long, long)
AMO_SWAP_OUT_EMIT(longlong, long long)
AMO_SWAP_OUT_EMIT(float, float)
AMO_SWAP_OUT_EMIT(double, double)

/*
 * TODO:
 *
 * This, as in all the atomic handlers, is where the opportunity gap
 * is.  We could do useful things between firing off the request and
 * waiting for the completion notification.  So split this out into
 * a post and wait/poll pair, post returning a handle for the atomic
 * op. in progress.
 */

/**
 * perform the swap
 */
#define AMO_SWAP_REQ_EMIT(_name, _type)                                 \
    _type                                                               \
    shmemc_##_name##_swap(_type *target, _type value, int pe)           \
    {                                                                   \
        _type save;                                                     \
        amo_payload_##_name##_t *p =                                    \
            (amo_payload_##_name##_t *) malloc(sizeof(*p));             \
                                                                        \
        if (p == NULL) {                                                \
            shmemc_bailout                                              \
                ("unable to allocate atomic "                           \
                 "swap"                                                 \
                 " payload memory");                                    \
        }                                                               \
        p->r_symm_addr = shmemi_symmetric_addr_lookup(target, pe);      \
        p->value = value;                                               \
        p->value_addr = &(p->value);                                    \
        p->completed = 0;                                               \
        p->completed_addr = &(p->completed);                            \
        gasnet_AMRequestMedium0(pe, GASNET_HANDLER_swap_out_##_name,    \
                                p, sizeof(*p));                         \
        WAIT_ON_COMPLETION(p->completed);                               \
        save = p->value;                                                \
        free(p);                                                        \
        return save;                                                    \
    }

AMO_SWAP_REQ_EMIT(int, int)
AMO_SWAP_REQ_EMIT(long, long)
AMO_SWAP_REQ_EMIT(longlong, long long)
AMO_SWAP_REQ_EMIT(float, float)
AMO_SWAP_REQ_EMIT(double, double)

/**
 * called by remote PE to do the swap.  Store new value if cond
 * matches, send back old value in either case
 */
#define AMO_CSWAP_OUT_EMIT(_name, _type)                                \
    static void                                                         \
    handler_cswap_out_##_name(gasnet_token_t token, void *buf, size_t bufsiz) \
    {                                                                   \
        _type old;                                                      \
        amo_payload_##_name##_t *pp =                                   \
            (amo_payload_##_name##_t *) buf;                            \
                                                                        \
        gasnet_hsl_lock(&amo_lock_##_name);                             \
        old = *(pp->r_symm_addr);                                       \
        if (pp->cond == old) {                                          \
            *(pp->r_symm_addr) = pp->value;                             \
        }                                                               \
        pp->value = old;                                                \
        LOAD_STORE_FENCE();                                             \
        gasnet_hsl_unlock(&amo_lock_##_name);                           \
        gasnet_AMReplyMedium0(token, GASNET_HANDLER_cswap_bak_##_name,  \
                              buf, bufsiz);                             \
    }

AMO_CSWAP_OUT_EMIT(int, int)
AMO_CSWAP_OUT_EMIT(long, long)
AMO_CSWAP_OUT_EMIT(longlong, long long)

/**
 * called by swap invoker when old value returned by remote PE
 * (same as swap_bak for now)
 */
#define AMO_CSWAP_BAK_EMIT(_name, _type)                                \
    static void                                                         \
    handler_cswap_bak_##_name(gasnet_token_t token, void *buf, size_t bufsiz) \
    {                                                                   \
        amo_payload_##_name##_t *pp =                                   \
            (amo_payload_##_name##_t *) buf;                            \
                                                                        \
        gasnet_hsl_lock(&amo_lock_##_name);                             \
        *(pp->value_addr) = pp->value;                                  \
        LOAD_STORE_FENCE();                                             \
        *(pp->completed_addr) = 1;                                      \
        gasnet_hsl_unlock (&amo_lock_##_name);                          \
    }

AMO_CSWAP_BAK_EMIT(int, int)
AMO_CSWAP_BAK_EMIT(long, long)
AMO_CSWAP_BAK_EMIT(longlong, long long)

/**
 * perform the conditional swap
 */
#define AMO_CSWAP_REQ_EMIT(_name, _type)                                \
    _type                                                               \
    shmemc_##_name##_cswap(_type *target, _type cond,                   \
                           _type value,                                 \
                           int pe)                                      \
    {                                                                   \
        _type save;                                                     \
        amo_payload_##_name##_t *cp =                                   \
            (amo_payload_##_name##_t *) malloc(sizeof(*cp));            \
                                                                        \
        if (cp == NULL) {                                               \
            shmemc_bailout                                              \
                ("unable to allocate atomic "                           \
                 "conditional-swap"                                     \
                 " payload memory");                                    \
        }                                                               \
        cp->r_symm_addr = shmemi_symmetric_addr_lookup(target, pe);     \
        cp->value = value;                                              \
        cp->value_addr = &(cp->value);                                  \
        cp->cond = cond;                                                \
        cp->completed = 0;                                              \
        cp->completed_addr = &(cp->completed);                          \
        LOAD_STORE_FENCE();                                             \
        gasnet_AMRequestMedium0(pe, GASNET_HANDLER_cswap_out_##_name,   \
                                cp, sizeof(*cp));                       \
        WAIT_ON_COMPLETION(cp->completed);                              \
        save = cp->value;                                               \
        free(cp);                                                       \
        return save;                                                    \
    }

AMO_CSWAP_REQ_EMIT(int, int)
AMO_CSWAP_REQ_EMIT(long, long)
AMO_CSWAP_REQ_EMIT(longlong, long long)

/**
 * fetch/add
 */

/**
 * called by remote PE to do the fetch and add.  Store new value, send
 * back old value
 */
#define AMO_FADD_OUT_EMIT(_name, _type)                                 \
    static void                                                         \
    handler_fadd_out_##_name(gasnet_token_t token, void *buf, size_t bufsiz) \
    {                                                                   \
        _type old;                                                      \
        amo_payload_##_name##_t *pp =                                   \
            (amo_payload_##_name##_t *) buf;                            \
                                                                        \
        gasnet_hsl_lock(&amo_lock_##_name);                             \
        old = *(pp->r_symm_addr);                                       \
        *(pp->r_symm_addr) += pp->value;                                \
        pp->value = old;                                                \
        LOAD_STORE_FENCE();                                             \
        gasnet_hsl_unlock(&amo_lock_##_name);                           \
        gasnet_AMReplyMedium0(token, GASNET_HANDLER_fadd_bak_##_name,   \
                              buf, bufsiz);                             \
    }

AMO_FADD_OUT_EMIT(int, int)
AMO_FADD_OUT_EMIT(long, long)
AMO_FADD_OUT_EMIT(longlong, long long)

/**
 * called by fadd invoker when old value returned by remote PE
 */
#define AMO_FADD_BAK_EMIT(_name, _type)                                 \
    static void                                                         \
    handler_fadd_bak_##_name(gasnet_token_t token, void *buf, size_t bufsiz) \
    {                                                                   \
        amo_payload_##_name##_t *pp =                                   \
            (amo_payload_##_name##_t *) buf;                            \
                                                                        \
        gasnet_hsl_lock(&amo_lock_##_name);                             \
        *(pp->value_addr) = pp->value;                                  \
        LOAD_STORE_FENCE();                                             \
        *(pp->completed_addr) = 1;                                      \
        gasnet_hsl_unlock(&amo_lock_##_name);                           \
    }

AMO_FADD_BAK_EMIT(int, int)
AMO_FADD_BAK_EMIT(long, long)
AMO_FADD_BAK_EMIT(longlong, long long)

/**
 * perform the fetch-and-add
 */
#define AMO_FADD_REQ_EMIT(_name, _type)                                 \
    _type                                                               \
    shmemc_##_name##_fadd(_type *target, _type value, int pe)           \
    {                                                                   \
        _type save;                                                     \
        amo_payload_##_name##_t *p =                                    \
            (amo_payload_##_name##_t *) malloc(sizeof(*p));             \
                                                                        \
        if (p == NULL) {                                                \
            shmemc_bailout                                              \
                ("unable to allocate atomic "                           \
                 "fetch-and-add"                                        \
                 " payload memory");                                    \
        }                                                               \
        p->r_symm_addr = shmemi_symmetric_addr_lookup(target, pe);      \
        p->value = value;                                               \
        p->value_addr = &(p->value);                                    \
        p->completed = 0;                                               \
        p->completed_addr = &(p->completed);                            \
        gasnet_AMRequestMedium0(pe, GASNET_HANDLER_fadd_out_##_name,    \
                                p, sizeof(*p));                         \
        WAIT_ON_COMPLETION(p->completed);                               \
        save = p->value;                                                \
        free(p);                                                        \
        return save;                                                    \
    }

AMO_FADD_REQ_EMIT(int, int)
AMO_FADD_REQ_EMIT(long, long)
AMO_FADD_REQ_EMIT(longlong, long long)

/**
 * fetch/increment
 */

/**
 * called by remote PE to do the fetch and increment.  Store new
 * value, send back old value
 */
#define AMO_FINC_OUT_EMIT(_name, _type)                                 \
    static void                                                         \
    handler_finc_out_##_name(gasnet_token_t token, void *buf, size_t bufsiz) \
    {                                                                   \
        _type old;                                                      \
        amo_payload_##_name##_t *pp = (amo_payload_##_name##_t *) buf;  \
                                                                        \
        gasnet_hsl_lock(&amo_lock_##_name);                             \
        old = *(pp->r_symm_addr);                                       \
        *(pp->r_symm_addr) += 1;                                        \
        pp->value = old;                                                \
        LOAD_STORE_FENCE();                                             \
        gasnet_hsl_unlock(&amo_lock_##_name);                           \
        gasnet_AMReplyMedium0(token, GASNET_HANDLER_finc_bak_##_name,   \
                              buf, bufsiz);                             \
    }

AMO_FINC_OUT_EMIT(int, int)
AMO_FINC_OUT_EMIT(long, long)
AMO_FINC_OUT_EMIT(longlong, long long)

/**
 * called by finc invoker when old value returned by remote PE
 */
#define AMO_FINC_BAK_EMIT(_name, _type)                                 \
    static void                                                         \
    handler_finc_bak_##_name(gasnet_token_t token, void *buf, size_t bufsiz) \
    {                                                                   \
        amo_payload_##_name##_t *pp = (amo_payload_##_name##_t *) buf;  \
                                                                        \
        gasnet_hsl_lock(&amo_lock_##_name);                             \
        *(pp->value_addr) = pp->value;                                  \
        LOAD_STORE_FENCE();                                             \
        *(pp->completed_addr) = 1;                                      \
        gasnet_hsl_unlock(&amo_lock_##_name);                           \
    }

AMO_FINC_BAK_EMIT(int, int)
AMO_FINC_BAK_EMIT(long, long)
AMO_FINC_BAK_EMIT(longlong, long long)

/**
 * perform the fetch-and-increment
 */
#define AMO_FINC_REQ_EMIT(_name, _type)                                 \
    _type                                                               \
    shmemc_##_name##_finc(_type *target, int pe)                        \
    {                                                                   \
        _type save;                                                     \
        amo_payload_##_name##_t *p =                                    \
            (amo_payload_##_name##_t *) malloc(sizeof(*p));             \
                                                                        \
        if (p == NULL) {                                                \
            shmemc_bailout                                              \
                ("unable to allocate atomic "                           \
                 "fetch-and-increment"                                  \
                 " payload memory");                                    \
        }                                                               \
        p->r_symm_addr = shmemi_symmetric_addr_lookup(target, pe);      \
        p->value_addr = &(p->value);                                    \
        p->completed = 0;                                               \
        p->completed_addr = &(p->completed);                            \
        gasnet_AMRequestMedium0(pe, GASNET_HANDLER_finc_out_##_name,    \
                                p, sizeof(*p));                         \
        WAIT_ON_COMPLETION(p->completed);                               \
        save = p->value;                                                \
        free(p);                                                        \
        return save;                                                    \
    }

AMO_FINC_REQ_EMIT(int, int)
AMO_FINC_REQ_EMIT(long, long)
AMO_FINC_REQ_EMIT(longlong, long long)

/**
 * remote add
 */

/**
 * called by remote PE to do the remote add.
 */
#define AMO_ADD_OUT_EMIT(_name, _type)                                  \
    static void                                                         \
    handler_add_out_##_name(gasnet_token_t token, void *buf, size_t bufsiz) \
    {                                                                   \
        amo_payload_##_name##_t *pp =                                   \
            (amo_payload_##_name##_t *) buf;                            \
                                                                        \
        gasnet_hsl_lock(&amo_lock_##_name);                             \
        *(pp->r_symm_addr) += pp->value;                                \
        LOAD_STORE_FENCE();                                             \
        gasnet_hsl_unlock(&amo_lock_##_name);                           \
        gasnet_AMReplyMedium0(token, GASNET_HANDLER_add_bak_##_name,    \
                              buf, bufsiz);                             \
    }

AMO_ADD_OUT_EMIT(int, int)
AMO_ADD_OUT_EMIT(long, long)
AMO_ADD_OUT_EMIT(longlong, long long)

/**
 * called by remote add invoker when store done
 */
#define AMO_ADD_BAK_EMIT(_name, _type)                                  \
    static void                                                         \
    handler_add_bak_##_name(gasnet_token_t token, void *buf, size_t bufsiz) \
    {                                                                   \
        amo_payload_##_name##_t *pp =                                   \
            (amo_payload_##_name##_t *) buf;                            \
                                                                        \
        gasnet_hsl_lock(&amo_lock_##_name);                             \
        *(pp->completed_addr) = 1;                                      \
        gasnet_hsl_unlock(&amo_lock_##_name);                           \
    }

AMO_ADD_BAK_EMIT(int, int)
AMO_ADD_BAK_EMIT(long, long)
AMO_ADD_BAK_EMIT(longlong, long long)

/**
 * perform the add
 */
#define AMO_ADD_REQ_EMIT(_name, _type)                              \
    void                                                            \
    shmemc_##_name##_add(_type *target, _type value, int pe)        \
    {                                                               \
        amo_payload_##_name##_t *p =                                \
            (amo_payload_##_name##_t *) malloc(sizeof(*p));         \
                                                                    \
        if (p == NULL) {                                            \
            shmemc_bailout                                          \
                ("unable to allocate atomic "                       \
                 "add"                                              \
                 " payload memory");                                \
        }                                                           \
        p->r_symm_addr = shmemi_symmetric_addr_lookup(target, pe);  \
        p->value = value;                                           \
        p->value_addr = &(p->value);                                \
        p->completed = 0;                                           \
        p->completed_addr = &(p->completed);                        \
        gasnet_AMRequestMedium0(pe, GASNET_HANDLER_add_out_##_name, \
                                p, sizeof(*p));                     \
        WAIT_ON_COMPLETION (p->completed);                          \
        free(p);                                                    \
    }

AMO_ADD_REQ_EMIT(int, int)
AMO_ADD_REQ_EMIT(long, long)
AMO_ADD_REQ_EMIT(longlong, long long)

/**
 * remote increment
 */

/**
 * called by remote PE to do the remote increment
 */
#define AMO_INC_OUT_EMIT(_name, _type)                                  \
    static void                                                         \
    handler_inc_out_##_name(gasnet_token_t token, void *buf, size_t bufsiz) \
    {                                                                   \
        amo_payload_##_name##_t *pp =                                   \
            (amo_payload_##_name##_t *) buf;                            \
                                                                        \
        gasnet_hsl_lock(&amo_lock_##_name);                             \
        *(pp->r_symm_addr) += 1;                                        \
        LOAD_STORE_FENCE();                                             \
        gasnet_hsl_unlock(&amo_lock_##_name);                           \
        gasnet_AMReplyMedium0(token, GASNET_HANDLER_inc_bak_##_name,    \
                              buf, bufsiz);                             \
    }

AMO_INC_OUT_EMIT(int, int)
AMO_INC_OUT_EMIT(long, long)
AMO_INC_OUT_EMIT(longlong, long long)

/**
 * called by remote increment invoker when store done
 */
#define AMO_INC_BAK_EMIT(_name, _type)                                  \
    static void                                                         \
    handler_inc_bak_##_name(gasnet_token_t token, void *buf, size_t bufsiz) \
    {                                                                   \
        amo_payload_##_name##_t *pp =                                   \
            (amo_payload_##_name##_t *) buf;                            \
                                                                        \
        gasnet_hsl_lock(&amo_lock_##_name);                             \
        *(pp->completed_addr) = 1;                                      \
        gasnet_hsl_unlock(&amo_lock_##_name);                           \
    }

AMO_INC_BAK_EMIT(int, int)
AMO_INC_BAK_EMIT(long, long)
AMO_INC_BAK_EMIT(longlong, long long)

/**
 * perform the increment
 */
#define AMO_INC_REQ_EMIT(_name, _type)                              \
    void                                                            \
    shmemc_##_name##_inc(_type *target, int pe)                     \
    {                                                               \
        amo_payload_##_name##_t *p =                                \
            (amo_payload_##_name##_t *) malloc(sizeof(*p));         \
                                                                    \
        if (p == NULL) {                                            \
            shmemc_bailout                                          \
                ("unable to allocate atomic "                       \
                 "increment"                                        \
                 " payload memory");                                \
        }                                                           \
        p->r_symm_addr = shmemi_symmetric_addr_lookup(target, pe);  \
        p->completed = 0;                                           \
        p->completed_addr = &(p->completed);                        \
        gasnet_AMRequestMedium0(pe, GASNET_HANDLER_inc_out_##_name, \
                                p, sizeof(*p));                     \
        WAIT_ON_COMPLETION(p->completed);                           \
        free(p);                                                    \
    }

AMO_INC_REQ_EMIT(int, int)
AMO_INC_REQ_EMIT(long, long)
AMO_INC_REQ_EMIT(longlong, long long)


/**
 * fetch & set
 */

/**
 * called by remote PE to do the fetch.  Store new value, send back
 * old value
 */
#define AMO_FETCH_OUT_EMIT(_name, _type)                                \
    static void                                                         \
    handler_fetch_out_##_name(gasnet_token_t token, void *buf, size_t bufsiz) \
    {                                                                   \
        amo_payload_##_name##_t *pp =                                   \
            (amo_payload_##_name##_t *) buf;                            \
                                                                        \
        gasnet_hsl_lock(&amo_lock_##_name);                             \
        pp->value = *(pp->r_symm_addr);                                 \
        LOAD_STORE_FENCE();                                             \
        gasnet_hsl_unlock(&amo_lock_##_name);                           \
        gasnet_AMReplyMedium0(token, GASNET_HANDLER_fetch_bak_##_name,  \
                              buf, bufsiz);                             \
    }

AMO_FETCH_OUT_EMIT(int, int)
AMO_FETCH_OUT_EMIT(long, long)
AMO_FETCH_OUT_EMIT(longlong, long long)
AMO_FETCH_OUT_EMIT(float, float)
AMO_FETCH_OUT_EMIT(double, double)

/**
 * called by fetch invoker when value returned by remote PE
 */
#define AMO_FETCH_BAK_EMIT(_name, _type)                                \
    static void                                                         \
    handler_fetch_bak_##_name(gasnet_token_t token, void *buf, size_t bufsiz) \
    {                                                                   \
        amo_payload_##_name##_t *pp =                                   \
            (amo_payload_##_name##_t *) buf;                            \
                                                                        \
        gasnet_hsl_lock(&amo_lock_##_name);                             \
        *(pp->value_addr) = pp->value;                                  \
        LOAD_STORE_FENCE();                                             \
        *(pp->completed_addr) = 1;                                      \
        gasnet_hsl_unlock(&amo_lock_##_name);                           \
    }

AMO_FETCH_BAK_EMIT(int, int)
AMO_FETCH_BAK_EMIT(long, long)
AMO_FETCH_BAK_EMIT(longlong, long long)
AMO_FETCH_BAK_EMIT(float, float)
AMO_FETCH_BAK_EMIT(double, double)

/**
 * perform the fetch
 */
#define AMO_FETCH_REQ_EMIT(_name, _type)                                \
    _type                                                               \
    shmemc_##_name##_fetch(_type *target, int pe)                       \
    {                                                                   \
        _type save;                                                     \
        amo_payload_##_name##_t *p =                                    \
            (amo_payload_##_name##_t *) malloc(sizeof(*p));             \
                                                                        \
        if (p == NULL) {                                                \
            shmemc_bailout                                              \
                ("unable to allocate atomic "                           \
                 "fetch"                                                \
                 " payload memory");                                    \
        }                                                               \
        p->r_symm_addr = shmemi_symmetric_addr_lookup(target, pe);      \
        p->value_addr = &(p->value);                                    \
        p->completed = 0;                                               \
        p->completed_addr = &(p->completed);                            \
        gasnet_AMRequestMedium0(pe, GASNET_HANDLER_fetch_out_##_name,   \
                                p, sizeof(*p));                         \
        WAIT_ON_COMPLETION(p->completed);                               \
        save = p->value;                                                \
        free(p);                                                        \
        return save;                                                    \
    }

AMO_FETCH_REQ_EMIT(int, int)
AMO_FETCH_REQ_EMIT(long, long)
AMO_FETCH_REQ_EMIT(longlong, long long)
AMO_FETCH_REQ_EMIT(float, float)
AMO_FETCH_REQ_EMIT(double, double)

/**
 * called by remote PE to do the set
 */
#define AMO_SET_OUT_EMIT(_name, _type)                                  \
    static void                                                         \
    handler_set_out_##_name(gasnet_token_t token, void *buf, size_t bufsiz) \
    {                                                                   \
        amo_payload_##_name##_t *pp =                                   \
            (amo_payload_##_name##_t *) buf;                            \
                                                                        \
        gasnet_hsl_lock(&amo_lock_##_name);                             \
        *(pp->r_symm_addr) = pp->value;                                 \
        LOAD_STORE_FENCE();                                             \
        gasnet_hsl_unlock(&amo_lock_##_name);                           \
        gasnet_AMReplyMedium0(token, GASNET_HANDLER_set_bak_##_name,    \
                              buf, bufsiz);                             \
    }

AMO_SET_OUT_EMIT(int, int)
AMO_SET_OUT_EMIT(long, long)
AMO_SET_OUT_EMIT(longlong, long long)
AMO_SET_OUT_EMIT(float, float)
AMO_SET_OUT_EMIT(double, double)

/**
 * called by set invoker when remote PE replies
 */
#define AMO_SET_BAK_EMIT(_name, _type)                                  \
    static void                                                         \
    handler_set_bak_##_name(gasnet_token_t token, void *buf, size_t bufsiz) \
    {                                                                   \
        amo_payload_##_name##_t *pp =                                   \
            (amo_payload_##_name##_t *) buf;                            \
                                                                        \
        gasnet_hsl_lock(&amo_lock_##_name);                             \
        *(pp->value_addr) = pp->value;                                  \
        LOAD_STORE_FENCE();                                             \
        *(pp->completed_addr) = 1;                                      \
        gasnet_hsl_unlock(&amo_lock_##_name);                           \
    }

AMO_SET_BAK_EMIT(int, int)
AMO_SET_BAK_EMIT(long, long)
AMO_SET_BAK_EMIT(longlong, long long)
AMO_SET_BAK_EMIT(float, float)
AMO_SET_BAK_EMIT(double, double)

/**
 * perform the set
 */
#define AMO_SET_REQ_EMIT(_name, _type)                                  \
    void                                                                \
    shmemc_##_name##_set(_type *target, _type value, int pe)            \
    {                                                                   \
        amo_payload_##_name##_t *p =                                    \
            (amo_payload_##_name##_t *) malloc(sizeof(*p));             \
                                                                        \
        if (p == NULL) {                                                \
            shmemc_bailout                                              \
                ("unable to allocate atomic "                           \
                 "set"                                                  \
                 " payload memory");                                    \
        }                                                               \
        p->r_symm_addr = shmemi_symmetric_addr_lookup(target, pe);      \
        p->value = value;                                               \
        p->value_addr = &(p->value);                                    \
        p->completed = 0;                                               \
        p->completed_addr = &(p->completed);                            \
        gasnet_AMRequestMedium0(pe, GASNET_HANDLER_set_out_##_name,     \
                                p, sizeof(*p));                         \
        WAIT_ON_COMPLETION(p->completed);                               \
        free(p);                                                        \
    }

AMO_SET_REQ_EMIT(int, int)
AMO_SET_REQ_EMIT(long, long)
AMO_SET_REQ_EMIT(longlong, long long)
AMO_SET_REQ_EMIT(float, float)
AMO_SET_REQ_EMIT(double, double)

#if defined(HAVE_FEATURE_EXPERIMENTAL)

/**
 * Proposed by IBM Zurich
 *
 * remote xor
 */

/**
 * called by remote PE to do the remote xor
 */
#define AMO_XOR_OUT_EMIT(_name, _type)                                  \
    static void                                                         \
    handler_xor_out_##_name(gasnet_token_t token, void *buf, size_t bufsiz) \
    {                                                                   \
        amo_payload_##_name##_t *pp =                                   \
            (amo_payload_##_name##_t *) buf;                            \
                                                                        \
        gasnet_hsl_lock(&amo_lock_##_name);                             \
        *(pp->r_symm_addr) ^= pp->value;                                \
        LOAD_STORE_FENCE();                                             \
        gasnet_hsl_unlock(&amo_lock_##_name);                           \
        gasnet_AMReplyMedium0(token, GASNET_HANDLER_xor_bak_##_name,    \
                              buf, bufsiz);                             \
    }

AMO_XOR_OUT_EMIT(int, int)
AMO_XOR_OUT_EMIT(long, long)
AMO_XOR_OUT_EMIT(longlong, long long)

/**
 * called by remote xor invoker when store done
 */
#define AMO_XOR_BAK_EMIT(_name, _type)                                  \
    static void                                                         \
    handler_xor_bak_##_name(gasnet_token_t token, void *buf, size_t bufsiz) \
    {                                                                   \
        amo_payload_##_name##_t *pp =                                   \
            (amo_payload_##_name##_t *) buf;                            \
                                                                        \
        gasnet_hsl_lock(&amo_lock_##_name);                             \
        *(pp->completed_addr) = 1;                                      \
        gasnet_hsl_unlock(&amo_lock_##_name);                           \
    }

AMO_XOR_BAK_EMIT(int, int)
AMO_XOR_BAK_EMIT(long, long)
AMO_XOR_BAK_EMIT(longlong, long long)

/**
 * perform the xor
 */
#define AMO_XOR_REQ_EMIT(_name, _type)                              \
    void                                                            \
    shmemc_##_name##_xor(_type *target, _type value, int pe)        \
    {                                                               \
        amo_payload_##_name##_t *p =                                \
            (amo_payload_##_name##_t *) malloc(sizeof(*p));         \
                                                                    \
        if (p == NULL) {                                            \
            shmemc_bailout                                          \
                ("unable to allocate atomic "                       \
                 "exclusive-or"                                     \
                 " payload memory");                                \
        }                                                           \
        p->r_symm_addr = shmemi_symmetric_addr_lookup(target, pe);  \
        p->value = value;                                           \
        p->value_addr = &(p->value);                                \
        p->completed = 0;                                           \
        p->completed_addr = &(p->completed);                        \
        gasnet_AMRequestMedium0(pe, GASNET_HANDLER_xor_out_##_name, \
                                p, sizeof(*p));                     \
        WAIT_ON_COMPLETION(p->completed);                           \
        free(p);                                                    \
    }

AMO_XOR_REQ_EMIT(int, int)
AMO_XOR_REQ_EMIT(long, long)
AMO_XOR_REQ_EMIT(longlong, long long)

#endif /* HAVE_FEATURE_EXPERIMENTAL */

/**
 * ---------------------------------------------------------------------------
 */

/**
 * perform the ping
 *
 * TODO: JUST RETURN TRUE FOR NOW IF GOOD PE, NEED TO WORK ON PROGRESS LOGIC
 *
 */
int
shmemc_ping_request(int pe)
{
    if ( (pe >= 0) && (pe < proc.nranks) ) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * ---------------------------------------------------------------------------
 */

#if defined(HAVE_MANAGED_SEGMENTS)

/* TODO non-static */

/**
 * atomic counters
 */
static volatile unsigned long put_counter = 0L;
static volatile unsigned long get_counter = 0L;

static gasnet_hsl_t put_counter_lock = GASNET_HSL_INITIALIZER;
static gasnet_hsl_t get_counter_lock = GASNET_HSL_INITIALIZER;

static void
atomic_inc_put_counter (void)
{
    gasnet_hsl_lock(&put_counter_lock);
    put_counter += 1L;
    gasnet_hsl_unlock(&put_counter_lock);
}

static void
atomic_dec_put_counter (void)
{
    gasnet_hsl_lock(&put_counter_lock);
    put_counter -= 1L;
    gasnet_hsl_unlock(&put_counter_lock);
}

static void
atomic_wait_put_zero (void)
{
    WAIT_ON_COMPLETION(put_counter == 0L);
}

static void
atomic_inc_get_counter (void)
{
    gasnet_hsl_lock(&get_counter_lock);
    get_counter += 1L;
    gasnet_hsl_unlock(&get_counter_lock);
}

static void
atomic_dec_get_counter (void)
{
    gasnet_hsl_lock(&get_counter_lock);
    get_counter -= 1L;
    gasnet_hsl_unlock(&get_counter_lock);
}

static void
atomic_wait_get_zero (void)
{
    WAIT_ON_COMPLETION(get_counter == 0L);
}

#else /* ! HAVE_MANAGED_SEGMENTS */

#define atomic_inc_put_counter()
#define atomic_dec_put_counter()

#define atomic_inc_get_counter()
#define atomic_dec_get_counter()

#define atomic_wait_put_zero()
#define atomic_wait_get_zero()

#endif /* HAVE_MANAGED_SEGMENTS */


/**
 * ---------------------------------------------------------------------------
 *
 * global variable put/get handlers (for non-everything cases):
 *
 * TODO: locking feels too coarse-grained with static (single) buffer,
 * TODO: would love to find a better way of doing this bit
 *
 */

/**
 * suitably allocate buffer for transfers.
 *
 */

static void
allocate_buffer_and_check(void **buf, size_t siz)
{
    const int r = posix_memalign(buf, GASNET_PAGESIZE, siz);

    switch (r) {
    case 0:
        /* all ok, return */
        break;
    case EINVAL:
        shmemc_bailout("global variable payload not aligned correctly");
        /* NOT REACHED */
        break;
    case ENOMEM:
        shmemc_bailout("no memory to allocate global variable payload");
        /* NOT REACHED */
        break;
    default:
        shmemc_bailout("unknown error with global variable payload"
                       " (posix_memalign returned %d)",
                       r);
        /* NOT REACHED */
        break;
    }
}

#if defined(HAVE_MANAGED_SEGMENTS)

/**
 * Puts
 */

/**
 * called by remote PE to grab and write to its variable
 */
static void
handler_globalvar_put_out(gasnet_token_t token, void *buf, size_t bufsiz)
{
    globalvar_payload_t *pp = (globalvar_payload_t *) buf;
    void *data = buf + sizeof(*pp);

    memmove(pp->target, data, pp->nbytes);
    LOAD_STORE_FENCE();

    /* return ack, just need the control structure */
    gasnet_AMReplyMedium0(token, GASNET_HANDLER_globalvar_put_bak,
                          buf, sizeof(*pp)
                          );
}

/**
 * invoking PE just has to ack remote write
 */
static void
handler_globalvar_put_bak(gasnet_token_t token, void *buf, size_t bufsiz)
{
    globalvar_payload_t *pp = (globalvar_payload_t *) buf;

    *(pp->completed_addr) = 1;
}

/**
 * Generate the active message to do a put to a global variable.
 *
 * Put a lock around this bit so we know when it is safe to reuse the
 * buffer.
 *
 */
static void
put_a_chunk(void *buf, size_t bufsize,
            void *target, void *source,
            size_t offset, size_t bytes_to_send,
            int pe)
{
    globalvar_payload_t *p = buf;
    void *data = buf + sizeof(*p);

    /*
     * build payload to send
     * (global var is trivially symmetric here, no translation needed)
     */
    p->nbytes = bytes_to_send;
    p->source = NULL;            /* not used in put */
    p->target = target + offset; /* on the other PE */
    p->completed = 0;
    p->completed_addr = &(p->completed);

    atomic_inc_put_counter();

    /* data added after control structure */
    memmove(data, source + offset, bytes_to_send);
    LOAD_STORE_FENCE();

    gasnet_AMRequestMedium0(pe, GASNET_HANDLER_globalvar_put_out, p, bufsize);

    WAIT_ON_COMPLETION(p->completed);

    atomic_dec_put_counter();
}

/**
 * perform the put to a global variable
 */
static void
shmemc_globalvar_put_request(void *target, void *source,
                             size_t nbytes,
                             int pe)
{
    /* get the buffer size and chop off control structure */
    const size_t max_req = gasnet_AMMaxMedium();
    const size_t max_data = max_req - sizeof(globalvar_payload_t);
    /* how to split up transfers */
    const size_t nchunks = nbytes / max_data;
    const size_t rem_send = nbytes % max_data;
    /* track size and progress of transfers */
    size_t payload_size;
    size_t alloc_size;
    size_t offset = 0;
    void *put_buf;

    alloc_size = max_req;
    payload_size = max_data;

    allocate_buffer_and_check(&put_buf, alloc_size);

    if (nchunks > 0) {
        size_t i;

        for (i = 0; i < nchunks; i += 1) {
            put_a_chunk(put_buf, alloc_size,
                        target, source, offset, payload_size,
                        pe);
            offset += payload_size;
        }
    }

    if (rem_send > 0) {
        payload_size = rem_send;

        put_a_chunk (put_buf, alloc_size,
                     target, source, offset, payload_size, pe);
    }

    free(put_buf);
}

/**
 * Gets
 *
 */

/**
 * called by remote PE to grab remote data and return
 */
static void
handler_globalvar_get_out(gasnet_token_t token, void *buf, size_t bufsiz)
{
    globalvar_payload_t *pp = (globalvar_payload_t *) buf;
    globalvar_payload_t *datap = buf + sizeof(*pp);

    /* fetch from remote global var into payload */
    memmove(datap, pp->source, pp->nbytes);
    LOAD_STORE_FENCE();

    /* return ack, copied data is returned */
    gasnet_AMReplyMedium0(token, GASNET_HANDLER_globalvar_get_bak,
                          buf, bufsiz);
}

/**
 * called by invoking PE to write fetched data
 */
static void
handler_globalvar_get_bak(gasnet_token_t token, void *buf, size_t bufsiz)
{
    globalvar_payload_t *pp = (globalvar_payload_t *) buf;

    /* write back payload data here */
    memmove(pp->target, buf + sizeof(*pp), pp->nbytes);
    LOAD_STORE_FENCE();

    *(pp->completed_addr) = 1;
}

/**
 * Generate the active message to do a get from a global variable.
 *
 */
static void
get_a_chunk(globalvar_payload_t * p, size_t bufsize,
            void *target, void *source,
            size_t offset, size_t bytes_to_send,
            int pe)
{
    /*
     * build payload to send
     * (global var is trivially symmetric here, no translation needed)
     */
    p->nbytes = bytes_to_send;
    p->source = source + offset;    /* on the other PE */
    p->target = target + offset;    /* track my local writes upon return */
    p->completed = 0;
    p->completed_addr = &(p->completed);

    atomic_inc_get_counter();

    gasnet_AMRequestMedium0(pe, GASNET_HANDLER_globalvar_get_out, p, bufsize);

    WAIT_ON_COMPLETION(p->completed);

    atomic_dec_get_counter();
}

/**
 * perform the get from a global variable
 */

static void
shmemc_globalvar_get_request(void *target, void *source,
                             size_t nbytes,
                             int pe)
{
    /* get the buffer size and chop off control structure */
    const size_t max_req = gasnet_AMMaxMedium();
    const size_t max_data = max_req - sizeof(globalvar_payload_t);
    /* how to split up transfers */
    const size_t nchunks = nbytes / max_data;
    const size_t rem_send = nbytes % max_data;
    /* track size and progress of transfers */
    size_t payload_size;
    size_t alloc_size;
    size_t offset = 0;
    void *get_buf;

    alloc_size = max_req;

    allocate_buffer_and_check(&get_buf, alloc_size);

    if (nchunks > 0) {
        size_t i;

        payload_size = max_data;

        for (i = 0; i < nchunks; i += 1) {
            get_a_chunk(get_buf, alloc_size,
                        target, source, offset, payload_size,
                        pe);
            offset += payload_size;
        }
    }

    if (rem_send > 0) {
        payload_size = rem_send;

        get_a_chunk(get_buf, alloc_size,
                    target, source, offset, payload_size,
                    pe);
    }

    free(get_buf);
}

#endif /* HAVE_MANAGED_SEGMENTS */

/**
 * ---------------------------------------------------------------------------
 */

void
shmemc_put(void *dst, void *src, size_t len, int pe)
{
#if defined(HAVE_MANAGED_SEGMENTS)
    if (shmemi_symmetric_is_globalvar(dst)) {
        shmemc_globalvar_put_request(dst, src, len, pe);
    }
    else {
        void *their_dst = shmemi_symmetric_addr_lookup(dst, pe);
        GASNET_PUT(pe, their_dst, src, len);
    }
#else
    void *their_dst = shmemi_symmetric_addr_lookup(dst, pe);
    GASNET_PUT(pe, their_dst, src, len);
#endif /* HAVE_MANAGED_SEGMENTS */
}

void
shmemc_put_bulk(void *dst, void *src, size_t len, int pe)
{
#if defined(HAVE_MANAGED_SEGMENTS)
    if (shmemi_symmetric_is_globalvar(dst)) {
        shmemc_globalvar_put_request(dst, src, len, pe);
    }
    else {
        void *their_dst = shmemi_symmetric_addr_lookup(dst, pe);
        GASNET_PUT_BULK(pe, their_dst, src, len);
    }
#else
    void *their_dst = shmemi_symmetric_addr_lookup(dst, pe);
    GASNET_PUT_BULK(pe, their_dst, src, len);
#endif /* HAVE_MANAGED_SEGMENTS */
}

void
shmemc_get(void *dst, void *src, size_t len, int pe)
{
#if defined(HAVE_MANAGED_SEGMENTS)
    if (shmemi_symmetric_is_globalvar(src)) {
        shmemc_globalvar_get_request(dst, src, len, pe);
    }
    else {
        void *their_src = shmemi_symmetric_addr_lookup(src, pe);
        GASNET_GET(dst, pe, their_src, len);
    }
#else
    void *their_src = shmemi_symmetric_addr_lookup(src, pe);
    GASNET_GET(dst, pe, their_src, len);
#endif /* HAVE_MANAGED_SEGMENTS */
}

void
shmemc_get_bulk(void *dst, void *src, size_t len, int pe)
{
#if defined(HAVE_MANAGED_SEGMENTS)
    if (shmemi_symmetric_is_globalvar(src)) {
        shmemc_globalvar_get_request(dst, src, len, pe);
    }
    else {
        void *their_src = shmemi_symmetric_addr_lookup(src, pe);
        GASNET_GET_BULK(dst, pe, their_src, len);
    }
#else
    void *their_src = shmemi_symmetric_addr_lookup(src, pe);
    GASNET_GET_BULK(dst, pe, their_src, len);
#endif /* HAVE_MANAGED_SEGMENTS */
}

/**
 * not completely sure about using longs in these two:
 * it's big enough and hides the gasnet type: is that good enough?
 */

void
shmemc_put_val(void *dst, long src, size_t len, int pe)
{
#if defined(HAVE_MANAGED_SEGMENTS)
    if (shmemi_symmetric_is_globalvar(dst)) {
        shmemc_globalvar_put_request(dst, &src, len, pe);
    }
    else {
        void *their_dst = shmemi_symmetric_addr_lookup(dst, pe);
        GASNET_PUT_VAL(pe, their_dst, src, len);
    }
#else
    void *their_dst = shmemi_symmetric_addr_lookup(dst, pe);
    GASNET_PUT_VAL(pe, their_dst, src, len);
#endif /* HAVE_MANAGED_SEGMENTS */
}

long
shmemc_get_val(void *src, size_t len, int pe)
{
    long retval;

#if defined(HAVE_MANAGED_SEGMENTS)
    if (shmemi_symmetric_is_globalvar(src)) {
        shmemc_globalvar_get_request(&retval, src, len, pe);
    }
    else {
        void *their_src = shmemi_symmetric_addr_lookup(src, pe);
        retval = gasnet_get_val(pe, their_src, len);
    }
#else
    void *their_src = shmemi_symmetric_addr_lookup(src, pe);
    retval = gasnet_get_val(pe, their_src, len);
#endif /* HAVE_MANAGED_SEGMENTS */

    return retval;
}

/**
 * non-blocking implicit put/get
 */

void
shmemc_put_nbi(void *dst, void *src, size_t len, int pe)
{
#if defined(HAVE_MANAGED_SEGMENTS)
    if (shmemi_symmetric_is_globalvar(dst)) {
        shmemc_globalvar_put_request(dst, src, len, pe);
    }
    else {
        void *their_dst = shmemi_symmetric_addr_lookup(dst, pe);
        GASNET_PUT_NBI(pe, their_dst, src, len);
    }
#else
    void *their_dst = shmemi_symmetric_addr_lookup(dst, pe);
    GASNET_PUT_NBI(pe, their_dst, src, len);
#endif /* HAVE_MANAGED_SEGMENTS */
}

void
shmemc_put_nbi_bulk(void *dst, void *src, size_t len, int pe)
{
#if defined(HAVE_MANAGED_SEGMENTS)
    if (shmemi_symmetric_is_globalvar(dst)) {
        shmemc_globalvar_put_request(dst, src, len, pe);
    }
    else {
        void *their_dst = shmemi_symmetric_addr_lookup(dst, pe);
        GASNET_PUT_NBI_BULK(pe, their_dst, src, len);
    }
#else
    void *their_dst = shmemi_symmetric_addr_lookup(dst, pe);
    GASNET_PUT_NBI_BULK(pe, their_dst, src, len);
#endif /* HAVE_MANAGED_SEGMENTS */
}

void
shmemc_get_nbi(void *dst, void *src, size_t len, int pe)
{
#if defined(HAVE_MANAGED_SEGMENTS)
    if (shmemi_symmetric_is_globalvar(src)) {
        shmemc_globalvar_get_request(dst, src, len, pe);
    }
    else {
        void *their_src = shmemi_symmetric_addr_lookup(src, pe);
        GASNET_GET_NBI(dst, pe, their_src, len);
    }
#else
    void *their_src = shmemi_symmetric_addr_lookup(src, pe);
    GASNET_GET_NBI(dst, pe, their_src, len);
#endif /* HAVE_MANAGED_SEGMENTS */
}

void
shmemc_get_nbi_bulk(void *dst, void *src, size_t len, int pe)
{
#if defined(HAVE_MANAGED_SEGMENTS)
    if (shmemi_symmetric_is_globalvar(src)) {
        shmemc_globalvar_get_request(dst, src, len, pe);
    }
    else {
        void *their_src = shmemi_symmetric_addr_lookup(src, pe);
        GASNET_GET_NBI_BULK(dst, pe, their_src, len);
    }
#else
    void *their_src = shmemi_symmetric_addr_lookup(src, pe);
    GASNET_GET_NBI_BULK(dst, pe, their_src, len);
#endif /* HAVE_MANAGED_SEGMENTS */
}

/**
 * called by mainline to fence off outstanding requests
 *
 * chances here for fence/quiet differentiation and optimization, but
 * we'll just fence <=> quiet
 *
 * Waits should also be concurrent really
 */

inline static void
do_fencequiet(void)
{
    atomic_wait_put_zero();
    GASNET_WAIT_PUTS();
    LOAD_STORE_FENCE();
    return;
}

void
shmemc_quiet(void)
{
    do_fencequiet();
}

void
shmemc_fence(void)
{
    do_fencequiet();
}

/**
 * fence and quiet tests just call fence/quiet and then report success
 *
 */

int
shmemc_fence_test(void)
{
    shmemc_fence();
    return 1;
}

int
shmemc_quiet_test(void)
{
    shmemc_quiet();
    return 1;
}

/**
 * called by remote PE when global_exit demanded
 */
void
handler_globalexit_out(gasnet_token_t token, void *buf, size_t bufsiz)
{
    int status = *(int *) buf;

    shmemc_fence();

    _exit(status);
}

/**
 * called by initiator PE of global_exit
 *
 * TODO: tree-based setup would be more scalable.
 */
void
shmemc_globalexit_request(int status)
{
    int pe;

    for (pe = 0; pe < proc.nranks; pe += 1) {
        /* send to everyone else */
        if (pe != proc.rank) {
            gasnet_AMRequestMedium0(pe, GASNET_HANDLER_globalexit_out,
                                    &status, sizeof(status)
                                    );
        }
    }

    shmemc_fence();

    _exit(status);
}


/**
 * ---------------------------------------------------------------------------
 *
 * start of handlers
 */

#define AMO_HANDLER_LOOKUP(_op, _name)                                \
    {GASNET_HANDLER_##_op##_out_##_name, handler_##_op##_out_##_name},  \
    {GASNET_HANDLER_##_op##_bak_##_name, handler_##_op##_bak_##_name}

static gasnet_handlerentry_t handlers[] = {
#if ! defined(HAVE_MANAGED_SEGMENTS)
    {GASNET_HANDLER_setup_out, handler_segsetup_out},
    {GASNET_HANDLER_setup_bak, handler_segsetup_bak},
#endif /* ! HAVE_MANAGED_SEGMENTS */

    AMO_HANDLER_LOOKUP(swap, int),
    AMO_HANDLER_LOOKUP(swap, long),
    AMO_HANDLER_LOOKUP(swap, longlong),
    AMO_HANDLER_LOOKUP(swap, float),
    AMO_HANDLER_LOOKUP(swap, double),

    AMO_HANDLER_LOOKUP(cswap, int),
    AMO_HANDLER_LOOKUP(cswap, long),
    AMO_HANDLER_LOOKUP(cswap, longlong),

    AMO_HANDLER_LOOKUP(fadd, int),
    AMO_HANDLER_LOOKUP(fadd, long),
    AMO_HANDLER_LOOKUP(fadd, longlong),

    AMO_HANDLER_LOOKUP(finc, int),
    AMO_HANDLER_LOOKUP(finc, long),
    AMO_HANDLER_LOOKUP(finc, longlong),

    AMO_HANDLER_LOOKUP(add, int),
    AMO_HANDLER_LOOKUP(add, long),
    AMO_HANDLER_LOOKUP(add, longlong),

    AMO_HANDLER_LOOKUP(inc, int),
    AMO_HANDLER_LOOKUP(inc, long),
    AMO_HANDLER_LOOKUP(inc, longlong),

    AMO_HANDLER_LOOKUP(fetch, int),
    AMO_HANDLER_LOOKUP(fetch, long),
    AMO_HANDLER_LOOKUP(fetch, longlong),
    AMO_HANDLER_LOOKUP(fetch, float),
    AMO_HANDLER_LOOKUP(fetch, double),

    AMO_HANDLER_LOOKUP(set, int),
    AMO_HANDLER_LOOKUP(set, long),
    AMO_HANDLER_LOOKUP(set, longlong),
    AMO_HANDLER_LOOKUP(set, float),
    AMO_HANDLER_LOOKUP(set, double),

#if defined(HAVE_FEATURE_EXPERIMENTAL)
    AMO_HANDLER_LOOKUP(xor, int),
    AMO_HANDLER_LOOKUP(xor, long),
    AMO_HANDLER_LOOKUP(xor, longlong),
#endif /* HAVE_FEATURE_EXPERIMENTAL */

#if defined(HAVE_MANAGED_SEGMENTS)
    {GASNET_HANDLER_globalvar_put_out, handler_globalvar_put_out},
    {GASNET_HANDLER_globalvar_put_bak, handler_globalvar_put_bak},
    {GASNET_HANDLER_globalvar_get_out, handler_globalvar_get_out},
    {GASNET_HANDLER_globalvar_get_bak, handler_globalvar_get_bak},
#endif /* HAVE_MANAGED_SEGMENTS */
    {GASNET_HANDLER_globalexit_out, handler_globalexit_out}
    /* END: no reply partner for global_exit */
};

static const int nhandlers = TABLE_SIZE(handlers);

/**
 * end of handlers
 */

/**
 * First parse out the process' command-line.  This is important for
 * the UDP conduit, in which the number of PEs comes from the
 * command-line rather than a launcher program.
 */

static int argc = 0;
static char **argv = NULL;

static const char *cmdline = "/proc/self/cmdline";
static const char *cmdline_fmt = "/proc/%ld/cmdline";

static void
parse_cmdline(void)
{
    FILE *fp;
    char an_arg[1024];          /* TODO: arbitrary size */
    char *p = an_arg;
    int i = 0;
    int c;

    /*
     * try to find this process' command-line:
     * either from short-cut, or from pid
     *
     * Linux-specific but know how to do it for e.g. FreeBSD
     *
     */
    fp = fopen(cmdline, "r");
    if (fp == NULL) {
        const pid_t pid = getpid();
        char pidname[MAXPATHLEN];

        snprintf(pidname, MAXPATHLEN, cmdline_fmt, pid);
        fp = fopen(pidname, "r");
        if (fp == NULL) {
            shmemc_bailout("could not discover command-line of process %ld (%s)",
                           pid, strerror(errno)
                           );
            /* NOT REACHED */
        }
    }

    /* first count the number of nuls in cmdline to see how many args */
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\0') {
            argc += 1;
        }
    }
    rewind(fp);

    argv = (char **) malloc((argc + 1) * sizeof(*argv));
    if (argv == NULL) {
        shmemc_bailout("internal error: unable to allocate memory for"
                       " faked command-line arguments");
        /* NOT REACHED */
    }

    while (1) {
        int c = fgetc(fp);
        switch (c) {
        case EOF:              /* end of args */
            argv[i] = NULL;
            goto end;
            break;
        case '\0':             /* end of this arg */
            *p = c;
            argv[i++] = strdup(an_arg); /* unchecked return */
            p = an_arg;
            break;
        default:               /* copy out char in this arg */
            *p++ = c;
            break;
        }
    }
 end:
    fclose(fp);
}

static void
release_cmdline(void)
{
    if (argv != NULL) {
        int i;

        for (i = 0; i < argc; i += 1) {
            if (argv[i] != NULL) {
                free(argv[i]);
            }
        }
    }
}

/**
 * GASNet does this timeout thing if its collective routines
 * (e.g. barrier) go idle, so make this as long as possible
 */
static void
maximize_gasnet_timeout(void)
{
    char buf[32];

    snprintf(buf, 32, "%d", INT_MAX - 1);
    setenv("GASNET_ EXITTIMEOUT", buf, 1);
}

/**
 * -----------------------------------------------------------------------
 */

/**
 * bail out of run-time with STATUS error code
 */
void
shmemc_exit(int status)
{
    /*
     * calling multiple times is undefined, I'm just going to do nothing
     */
    if (proc.status == PE_SHUTDOWN) {
        return;
    }

    /* ok, no more pending I/O ... */
    shmemc_barrier_all();

    service_finalize();

    release_cmdline();

#if 0
    /**
     * SO THAT WE CAN COMPILE
     **/

    /* clean up atomics and memory */
    shmemi_atomic_finalize();
    shmemi_symmetric_memory_finalize();
    shmemi_symmetric_globalvar_table_finalize();

    /* clean up plugin modules */
    /* shmemi_modules_finalize (); */

    /* tidy up binary inspector */
    shmemi_executable_finalize();

    /* stop run time clock */
    shmemi_elapsed_clock_finalize();

#endif

    /* update our state */
    proc.status = PE_SHUTDOWN;

    logger(LOG_FINALIZE,
           "finalizing shutdown, handing off to communications layer");

    /*
     * TODO, tc: need to be better at cleanup for 1.2, since finalize
     * doesn't imply follow-on exit, merely end of OpenSHMEM portion.
     *
     */

    /* shmemc_barrier_all (); */
}

/**
 * finalize can now happen in 2 ways: (1) program finishes via
 * atexit(), or (2) user explicitly calls shmem_finalize().  Need to
 * detect explicit call and not terminate program until exit.
 *
 */
void
shmemc_finalize(void)
{
    shmemc_exit(EXIT_SUCCESS);
}

/**
 * This is where the communications layer gets set up and torn down
 */
void
shmemc_init(void)
{
    /*
     * prepare environment for GASNet
     */
    parse_cmdline();
    maximize_gasnet_timeout();

    GASNET_SAFE(gasnet_init(&argc, &argv));

    /* now we can ask about the node count & heap */
    proc.rank = (int) gasnet_mynode();
    proc.nranks = (int) gasnet_nodes();
    proc.nheaps = 1;
    proc.hsizep = (size_t *) malloc(proc.nheaps * sizeof(*proc.hsizep));
    proc.hsizep[0] = shmemc_get_segment_size();

    /*
     * not guarding the attach for different gasnet models,
     * since last 2 params are ignored if not needed
     */
    GASNET_SAFE(gasnet_attach(handlers,
                              nhandlers,
                              proc.hsizep[0],
                              0 /* min offset */
                              )
                );

    /* set up any locality information */
    place_init();

    /* fire up any needed progress management */
    service_init();

#if 0
    /**
     * SO THAT WE CAN COMPILE
     **/

    /* enable messages */
    shmemi_elapsed_clock_init();
    shmemi_tracers_init();

    /* who am I? */
    shmemi_executable_init();

    /* find global symbols */
    shmemi_symmetric_globalvar_table_init();

    /* handle the heap */
    shmemi_symmetric_memory_init();

    /* which message/trace levels are active */
    shmemi_maybe_tracers_show_info();
    shmemi_tracers_show();

    /* set up the atomic ops handling */
    shmemi_atomic_init();

    /* initialize collective algs */
    shmemi_barrier_dispatch_init();
    shmemi_barrier_all_dispatch_init();
    shmemi_broadcast_dispatch_init();
    shmemi_collect_dispatch_init();
    shmemi_fcollect_dispatch_init();
#endif

    /* register shutdown handler */
    if (atexit(shmemc_finalize) != 0) {
        logger(LOG_FATAL,
               "cannot register "
               "OpenSHMEM finalize handler: \"%s\"",
               strerror(errno));
        /* NOT REACHED */
    }

    proc.status = PE_RUNNING;

    /* Up and running! */
}
