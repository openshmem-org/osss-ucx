/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "thispe.h"
#include "shmemu.h"
#include "shmemc.h"
#include "state.h"
#include "ucx/api.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <pmix.h>

#include <ucp/api/ucp.h>

/* -------------------------------------------------------------- */

/*
 * Persistent local state
 */

static pmix_proc_t my_proc;     /* about me */
static pmix_proc_t wc_proc;     /* wildcard lookups */
static pmix_proc_t ex_proc;     /* internal exchanges */

static pmix_key_t k1;           /* re-usable key spaces */
#ifndef ENABLE_ALIGNED_ADDRESSES
static pmix_key_t k2;
#endif /* ENABLE_ALIGNED_ADDRESSES */

static pmix_status_t ps;        /* re-usable pmix status */

/*
 * Make local info avaialable to PMIx
 */

static const char *wrkr_exch_fmt   = "wrkr:%d";     /* pe */

void
shmemc_pmi_publish_worker(void)
{
    pmix_value_t v;

    /* everyone publishes their info */
    snprintf(k1, PMIX_MAX_KEYLEN, wrkr_exch_fmt, proc.rank);

    v.type = PMIX_BYTE_OBJECT;
    v.data.bo.bytes = (char *) proc.comms.xchg_wrkr_info[proc.rank].addr;
    v.data.bo.size = proc.comms.xchg_wrkr_info[proc.rank].len;

    ps = PMIx_Put(PMIX_GLOBAL, k1, &v);
    shmemu_assert(ps == PMIX_SUCCESS, "can't publish worker blob");
}

static const char *rkey_exch_fmt   = "rkey:%lu:%d"; /* region, pe */

inline static void
publish_one_rkeys(size_t r)
{
    pmix_value_t v;
    void *packed_rkey;
    size_t rkey_len;
    const ucs_status_t s =
        shmemc_ucx_rkey_pack(proc.comms.regions[r].minfo[proc.rank].mh,
                             &packed_rkey, &rkey_len
                             );
    shmemu_assert(s == UCS_OK, "can't pack rkey");

    snprintf(k1, PMIX_MAX_KEYLEN, rkey_exch_fmt, r, proc.rank);

    v.type = PMIX_BYTE_OBJECT;
    v.data.bo.bytes = (char *) packed_rkey;
    v.data.bo.size = rkey_len;

    ps = PMIx_Put(PMIX_GLOBAL, k1, &v);
    shmemu_assert(ps == PMIX_SUCCESS,
                  "can't publish rkey for memory region %lu",
                  r);

    ucp_rkey_buffer_release(packed_rkey);
}

#ifndef ENABLE_ALIGNED_ADDRESSES
static const char *region_base_fmt = "base:%lu:%d"; /* region, pe */
static const char *region_size_fmt = "size:%lu:%d"; /* region, pe */
#endif /* ! ENABLE_ALIGNED_ADDRESSES */

#ifndef ENABLE_ALIGNED_ADDRESSES
inline static void
publish_one_heap(size_t r)
{
    pmix_value_t vb, vs;

    snprintf(k1, PMIX_MAX_KEYLEN, region_base_fmt, r, proc.rank);
    snprintf(k2, PMIX_MAX_KEYLEN, region_size_fmt, r, proc.rank);

    vb.type = PMIX_UINT64;
    vb.data.uint64 = proc.comms.regions[r].minfo[proc.rank].base;
    vs.type = PMIX_SIZE;
    vs.data.size = proc.comms.regions[r].minfo[proc.rank].len;

    ps = PMIx_Put(PMIX_GLOBAL, k1, &vb);
    shmemu_assert(ps == PMIX_SUCCESS,
                  "can't publish heap base for memory region %lu",
                  r);
    ps = PMIx_Put(PMIX_GLOBAL, k2, &vs);
    shmemu_assert(ps == PMIX_SUCCESS,
                  "can't publish heap size for memory region %lu",
                  r);
}
#endif /* ! ENABLE_ALIGNED_ADDRESSES */

void
shmemc_pmi_publish_rkeys_and_heaps(void)
{
    size_t r;

    publish_one_rkeys(0);

    for (r = 1; r < proc.comms.nregions; ++r) {
        publish_one_rkeys(r);

#ifndef ENABLE_ALIGNED_ADDRESSES
        publish_one_heap(r);
#endif /* ! ENABLE_ALIGNED_ADDRESSES */
    }
}

/* -------------------------------------------------------------- */

/*
 * Get remote info out of PMIx
 */

void
shmemc_pmi_exchange_workers(void)
{
    pmix_value_t *vp = NULL;
    int pe;

    for (pe = 0; pe < proc.nranks; ++pe) {
        pmix_byte_object_t *bop;

        snprintf(k1, PMIX_MAX_KEYLEN, wrkr_exch_fmt, pe);
        ex_proc.rank = pe;

        ps = PMIx_Get(&ex_proc, k1, NULL, 0, &vp);
        shmemu_assert(ps == PMIX_SUCCESS,
                      "can't find remote worker blob for PE %d",
                      pe);

        bop = & vp->data.bo;

        /* save published worker */
        proc.comms.xchg_wrkr_info[pe].buf = (char *) malloc(bop->size);
        shmemu_assert(proc.comms.xchg_wrkr_info[pe].buf != NULL,
                      "can't allocate memory for remote workers for PE %d",
                      pe);
        memcpy(proc.comms.xchg_wrkr_info[pe].buf, bop->bytes, bop->size);
    }
    PMIX_VALUE_RELEASE(vp);
}

#ifndef ENABLE_ALIGNED_ADDRESSES
inline static void
exchange_one_heap(size_t r, int pe)
{
    pmix_value_t *vpb = NULL;
    pmix_value_t *vps = NULL;
    uint64_t base;
    size_t len;

    ex_proc.rank = pe;

    snprintf(k1, PMIX_MAX_KEYLEN, region_base_fmt, r, pe);
    snprintf(k2, PMIX_MAX_KEYLEN, region_size_fmt, r, pe);

    ps = PMIx_Get(&ex_proc, k1, NULL, 0, &vpb);
    shmemu_assert(ps == PMIX_SUCCESS,
                  "can't fetch heap base for memory region %lu from PE %d",
                  r, pe);

    ps = PMIx_Get(&ex_proc, k2, NULL, 0, &vps);
    shmemu_assert(ps == PMIX_SUCCESS,
                  "can't fetch heap size for memory region %lu from PE %d",
                  r, pe);

    base = vpb->data.uint64;
    len  = vps->data.size;

    proc.comms.regions[r].minfo[pe].base = base;
    proc.comms.regions[r].minfo[pe].len = len;
    /* slightly redundant storage, but useful */
    proc.comms.regions[r].minfo[pe].end = base + len;

    PMIX_VALUE_RELEASE(vps);
    PMIX_VALUE_RELEASE(vpb);
}
#endif /* ! ENABLE_ALIGNED_ADDRESSES */

inline static void
exchange_one_rkeys(size_t r, int pe)
{
    pmix_value_t *vp = NULL;
    pmix_byte_object_t *bop;

    ex_proc.rank = pe;

    snprintf(k1, PMIX_MAX_KEYLEN, rkey_exch_fmt, r, pe);

    ps = PMIx_Get(&ex_proc, k1, NULL, 0, &vp);
    shmemu_assert(ps == PMIX_SUCCESS,
                  "can't fetch remote rkey for memory region %lu from PE %d",
                  r, pe);

    bop = & vp->data.bo;

    /* opaque rkey */
    proc.comms.orks[r].rkeys[pe].data = malloc(bop->size);
    shmemu_assert(proc.comms.orks[r].rkeys[pe].data != NULL,
                  "can't allocate memory for rkey data"
                  " for memory region %lu from PE %d",
                  r, pe);

    memcpy(proc.comms.orks[r].rkeys[pe].data, bop->bytes, bop->size);

    PMIX_VALUE_RELEASE(vp);
}

inline static void
exchange_all_rkeys(size_t r)
{
    int pe;

    for (pe = 0; pe < proc.nranks; ++pe) {
        exchange_one_rkeys(r, pe);
    }
}

inline static void
exchange_one_rkeys_and_heaps(size_t r)
{
    int pe;

    for (pe = 0; pe < proc.nranks; ++pe) {
        exchange_one_rkeys(r, pe);

#ifndef ENABLE_ALIGNED_ADDRESSES
        exchange_one_heap(r, pe);
#endif /* ! ENABLE_ALIGNED_ADDRESSES */
    }  /* PE loop */
}

void
shmemc_pmi_exchange_rkeys_and_heaps(void)
{
    size_t r;

    /* global rkeys */
    exchange_all_rkeys(0);

    /* now everything else */
    for (r = 1; r < proc.comms.nregions; ++r) {
        exchange_one_rkeys_and_heaps(r);
    }
}

/* -------------------------------------------------------------- */

/*
 * this barrier is purely for internal use with PMIx, nothing to do
 * with SHMEM/UCX
 */
void
shmemc_pmi_barrier_all(bool collect_data)
{
    /* put all info out there */
    ps = PMIx_Commit();
    shmemu_assert(ps == PMIX_SUCCESS,
                  "PMIx_Commit() failed: %s",
                  PMIx_Error_string(ps));

    if (collect_data) {
        pmix_info_t cd;             /* collect data? */

        PMIX_INFO_CONSTRUCT(&cd);
        PMIX_INFO_LOAD(&cd, PMIX_COLLECT_DATA, &collect_data, PMIX_BOOL);
        ps = PMIx_Fence(NULL, 0, &cd, 1);
    }
    else {
        ps = PMIx_Fence(NULL, 0, NULL, 0);
    }

    shmemu_assert(ps == PMIX_SUCCESS,
                  "PMIx_Fence() [collect=%s] failed: %s",
                  collect_data ? "true" : "false",
                  PMIx_Error_string(ps));
}

/*
 * handle the different init/fini APIs
 */

inline static pmix_status_t
pmix_init_wrapper(pmix_proc_t *pp)
{
#ifdef HAVE_PMIX_NO_INIT_HINTS
    ps = PMIx_Init(pp);
#else
    ps = PMIx_Init(pp, NULL, 0);
#endif  /* HAVE_PMIX_NO_INIT_HINTS */

    return ps;
}

inline static pmix_status_t
pmix_finalize_wrapper(void)
{
#ifdef HAVE_PMIX_NO_INIT_HINTS
    ps = PMIx_Finalize();
#else
    ps = PMIx_Finalize(NULL, 0);
#endif  /* HAVE_PMIX_NO_INIT_HINTS */

    return ps;
}

/*
 * get the PMIx client-side up and running
 */

inline static void
init_ranks(void)
{
    pmix_value_t *vp;           /* holds things we get from PMIx */

    /* we can get our own rank immediately */
    proc.rank = (int) my_proc.rank;
    shmemu_assert(proc.rank >= 0,
                  "PMIx PE rank %d must be >= 0",
                  proc.rank);

    ps = PMIx_Get(&wc_proc, PMIX_JOB_SIZE, NULL, 0, &vp);
    shmemu_assert(ps == PMIX_SUCCESS,
                  "PMIx can't get program size: %s",
                  PMIx_Error_string(ps));

    proc.nranks = (int) vp->data.uint32; /* number of ranks/PEs */

    PMIX_VALUE_RELEASE(vp);

    /* this can fail on older PMIx setups, so be more careful */
    ps = PMIx_Get(&wc_proc, PMIX_NUM_NODES, NULL, 0, &vp);
    if (ps == PMIX_SUCCESS) {
        proc.nnodes = (int) vp->data.uint32; /* number of nodes */
        PMIX_VALUE_RELEASE(vp);
    }
    else {
        /* this seems somewhat sane if we can't query */
        proc.nnodes = 1;
    }

    ps = PMIx_Get(&wc_proc, PMIX_UNIV_SIZE, NULL, 0, &vp);
    shmemu_assert(ps == PMIX_SUCCESS,
                  "PMIx can't get universe size: %s",
                  PMIx_Error_string(ps));

    proc.maxranks = (int) vp->data.uint32; /* total ranks available */

    /* is the world a sane size? */
    shmemu_assert(proc.nranks > 0,
                  "PMIx PE count is %d, but must be > 0",
                  proc.nranks);
    shmemu_assert(proc.maxranks > 0,
                  "PMIx universe size is %d, but must be > 0",
                  proc.maxranks);
    shmemu_assert(shmemu_valid_pe_number(proc.rank),
                  "PMIx PE rank %d is not in range [0...%d)",
                  proc.rank, proc.nranks);

    PMIX_VALUE_RELEASE(vp);
}

inline static void
init_peers(void)
{
    pmix_value_t *vp = NULL;    /* holds things we get from PMIx */

    /* what's on this node? */
    ps = PMIx_Get(&wc_proc, PMIX_LOCAL_SIZE, NULL, 0, &vp);
    shmemu_assert(ps == PMIX_SUCCESS,
                  "PMIx can't look up PE's peers: %s",
                  PMIx_Error_string(ps));

    proc.npeers = (int) vp->data.uint32;
    /* how's the 'hood look? */
    shmemu_assert(proc.npeers > 0,
                  "PMIx PE's peer count %d must be > 0",
                  proc.npeers);
    shmemu_assert(proc.npeers <= proc.nranks,
                  "PMIx PE's peer count %d bigger than program size %d",
                  proc.npeers, proc.nranks);

    PMIX_VALUE_RELEASE(vp);

    /* look for peers; leave empty if can't find them */
    ps = PMIx_Get(&wc_proc, PMIX_LOCAL_PEERS, NULL, 0, &vp);
    if (ps == PMIX_SUCCESS) {
        if (vp->data.string != NULL) {
            size_t n;
            const int s =
                shmemu_parse_csv(vp->data.string, &proc.peers, &n);
            NO_WARN_UNUSED(n);
            shmemu_assert(s > 0,
                          "Unable to parse peer PE numbers \"%s\"",
                          vp->data.string);
        }
    }

    PMIX_VALUE_RELEASE(vp);

    /* am I first on a node/in a group? */
    proc.leader = (proc.rank == proc.peers[0]);
}

/*
 * -- register event handler for global exit --
 */

static const pmix_status_t ERROR_TO_REPORT =
    PMIX_ERR_PROC_REQUESTED_ABORT;

/*
 * track completion of registration
 */
static volatile int active;

#ifdef HAVE__EXIT
# define EXIT _exit
#elif defined(HAVE_EXIT)
# define EXIT exit
#else
# error "no exit() routine found"
#endif

/*
 * called when global exit happens
 */
static void
notification_fn(size_t evhdlr_registration_id,
                pmix_status_t status,
                const pmix_proc_t *source,
                pmix_info_t info[], size_t ninfo,
                pmix_info_t results[], size_t nresults,
                pmix_event_notification_cbfunc_fn_t cbfunc,
                void *cbdata)
{
    /* unpack passed PMIx info for exit code */
    const int ret = info[0].value.data.integer;

    NO_WARN_UNUSED(evhdlr_registration_id);
    NO_WARN_UNUSED(status);
    NO_WARN_UNUSED(source);
    NO_WARN_UNUSED(ninfo);
    NO_WARN_UNUSED(results); NO_WARN_UNUSED(nresults);

    ps = PMIx_Abort(ret, "global_exit", NULL, 0);
    shmemu_assert(ps == PMIX_SUCCESS,
                  "PMIx can't abort: %s",
                  PMIx_Error_string(ps));

    if (cbfunc != NULL) {
        cbfunc(PMIX_EVENT_ACTION_COMPLETE,
               NULL, 0,
               NULL, NULL,
               cbdata);
    }

    EXIT(ret);
}

/*
 * registration happened
 */
static void
evhandler_reg_callbk(pmix_status_t status,
                     size_t evhandler_ref,
                     void *cbdata)
{
    volatile int *act = (volatile int *) cbdata;

    NO_WARN_UNUSED(evhandler_ref);

    shmemu_assert(status == PMIX_SUCCESS,
                  "couldn't register event handler for global exit");

    *act = status;
}

static void
init_event_handler(void)
{
    pmix_status_t sp = ERROR_TO_REPORT;

    active = -1;
    PMIx_Register_event_handler(&sp, 1,
                                NULL, 0,
                                notification_fn,
                                evhandler_reg_callbk,
                                (void *) &active);

    while (active == -1) {}

    shmemu_assert(active == 0,
                  "global exit event handler registration failed");
}

/*
 * crunch out if something fatal happens
 */

void
shmemc_pmi_client_abort(const char *msg, int status)
{
    pmix_info_t si;

    NO_WARN_UNUSED(msg);

    PMIX_INFO_CONSTRUCT(&si);
    PMIX_INFO_LOAD(&si, PMIX_EXIT_CODE, &status, PMIX_INT);

    ps = PMIx_Notify_event(ERROR_TO_REPORT,
                           &my_proc,
                           PMIX_RANGE_NAMESPACE,
                           &si, 1,
                           NULL, NULL);

    shmemu_assert(ps == PMIX_SUCCESS,
                  "PMIx can't notify global exit: %s",
                  PMIx_Error_string(ps));
}

/*
 * shut down PMIx client-side cleanly
 */

void
shmemc_pmi_client_finalize(void)
{
    ps = pmix_finalize_wrapper();

    shmemu_assert(ps == PMIX_SUCCESS,
                  "PMIx can't finalize: %s",
                  PMIx_Error_string(ps));

    /* clean up memory recording peer PEs */
    free(proc.peers);
}

/*
 * fire up PMIx client layer
 */

void
shmemc_pmi_client_init(void)
{
    ps = pmix_init_wrapper(&my_proc);

    shmemu_assert(ps == PMIX_SUCCESS,
                  "PMIx can't initialize: %s",
                  PMIx_Error_string(ps));

    /* make a new proc to query things not linked to a specific rank */
    PMIX_PROC_CONSTRUCT(&wc_proc);
    STRNCPY_SAFE(wc_proc.nspace, my_proc.nspace, PMIX_MAX_NSLEN + 1);
    wc_proc.rank = PMIX_RANK_WILDCARD;

    /* init proc for exchange use, rank filled in as needed */
    PMIX_PROC_CONSTRUCT(&ex_proc);
    strncpy(ex_proc.nspace, my_proc.nspace, PMIX_MAX_NSLEN + 1);

    init_ranks();
    init_peers();

    init_event_handler();
}
