/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "thispe.h"
#include "shmemu.h"
#include "shmemc.h"
#include "state.h"
#include "globalexit.h"
#include "readenv.h"

#include "allocator/memalloc.h"

#include <stdlib.h>             /* getenv */
#include <string.h>
#include <strings.h>

#include <ucp/api/ucp.h>

/*
 * worker tables
 */
inline static void
allocate_xworkers_table(void)
{
    proc.comms.xchg_wrkr_info = (worker_info_t *)
        calloc(proc.nranks, sizeof(*(proc.comms.xchg_wrkr_info)));
    shmemu_assert(proc.comms.xchg_wrkr_info != NULL,
                  "can't allocate memory for worker exchange");
}

inline static void
deallocate_xworkers_table(void)
{
    free(proc.comms.xchg_wrkr_info);
}

/*
 * endpoint tables
 */
inline static void
allocate_endpoints_table(void)
{
    proc.comms.eps = (ucp_ep_h *)
        calloc(proc.nranks, sizeof(*(proc.comms.eps)));
    shmemu_assert(proc.comms.eps != NULL,
                  "can't allocate memory for endpoints");
}

inline static void
deallocate_endpoints_table(void)
{
    free(proc.comms.eps);
}

/*
 * Context management
 */
inline static void
allocate_contexts_table(void)
{
    /*
     * no new SHMEM contexts created yet
     */
    proc.comms.nctxts = 0;
    proc.comms.ctxts = NULL;
}

inline static void
deallocate_contexts_table(void)
{
    shmemc_context_h def = (shmemc_context_h) SHMEM_CTX_DEFAULT;
    size_t c;

    /*
     * special release case for default context
     */
    ucp_worker_release_address(def->w,
                               proc.comms.xchg_wrkr_info[proc.rank].addr);
    ucp_worker_destroy(def->w);
    /*
     * clear up each allocated SHMEM context
     */
    for (c = 1; c < proc.comms.nctxts; c += 1) {
        if (proc.comms.ctxts[c] != NULL) {
            ucp_worker_destroy(proc.comms.ctxts[c]->w);
            free(proc.comms.ctxts[c]);
        }
    }
}

/*
 * shortcut for global variables
 */
static mem_info_t *globals;

inline static void
register_globals()
{
    extern char data_start; /* from the executable */
    extern char end; /* from the executable */
    uint64_t g_base = (uint64_t) &data_start;
    uint64_t g_end = (uint64_t) &end;
    const size_t len = g_end - g_base;
    ucp_mem_map_params_t mp;
    ucs_status_t s;

    mp.field_mask =
        UCP_MEM_MAP_PARAM_FIELD_ADDRESS |
        UCP_MEM_MAP_PARAM_FIELD_LENGTH;

    mp.address = (void *) g_base;
    mp.length = len;
    mp.flags =
        UCP_MEM_MAP_ALLOCATE |
        UCP_MEM_MAP_FIXED;

    globals->base = g_base;
    globals->end  = globals->base + len;
    globals->len  = len;

    s = ucp_mem_map(proc.comms.ucx_ctxt, &mp, &globals->racc.mh);
    shmemu_assert(s == UCS_OK, "can't map global memory");

    /* don't need allocator, variables already there */
}

inline static void
deregister_globals(void)
{
    ucs_status_t s;

    s = ucp_mem_unmap(proc.comms.ucx_ctxt, globals->racc.mh);
    shmemu_assert(s == UCS_OK, "can't unmap global memory");
}

/*
 * while there's only 1 globals area, we can theoretically have
 * multiple symmetric heaps
 */

inline static void
register_symmetric_heap(size_t heapno, mem_info_t *mip)
{
    ucs_status_t s;
    ucp_mem_map_params_t mp;
    ucp_mem_attr_t attr;
    const unsigned long hn = (unsigned long) heapno; /* printing */

    shmemu_assert(proc.env.heaps.heapsize[heapno] > 0,
                  "Cannot register empty symmetric heap #%lu",
                  hn);

    /* now register it with UCX */
    mp.field_mask =
        UCP_MEM_MAP_PARAM_FIELD_LENGTH |
        UCP_MEM_MAP_PARAM_FIELD_FLAGS;

    mp.length = proc.env.heaps.heapsize[heapno];

    mp.flags =
        UCP_MEM_MAP_ALLOCATE;

    s = ucp_mem_map(proc.comms.ucx_ctxt, &mp, &mip->racc.mh);
    shmemu_assert(s == UCS_OK,
                  "can't map memory for symmetric heap #%lu",
                  hn);

    mip->id = hn;

    /*
     * query back to find where it is, and its actual size (might be
     * aligned/padded)
     */

    /* the attributes we want to inspect */
    attr.field_mask =
        UCP_MEM_ATTR_FIELD_ADDRESS |
        UCP_MEM_ATTR_FIELD_LENGTH;

    s = ucp_mem_query(mip->racc.mh, &attr);
    shmemu_assert(s == UCS_OK,
                  "can't query extent of memory for symmetric heap #%lu",
                  hn);

    /* tell the PE what was given */
    mip->base = (uint64_t) attr.address;
    mip->end  = mip->base + attr.length;
    mip->len  = attr.length;

    /* initialize the heap allocator */
    shmema_init((void *) mip->base, mip->len);
}

inline static void
deregister_symmetric_heap(mem_info_t *mip)
{
    ucs_status_t s;
    const unsigned long hn = (unsigned long) mip->id; /* printing */

    NO_WARN_UNUSED(hn);

    s = ucp_mem_unmap(proc.comms.ucx_ctxt, mip->racc.mh);
    shmemu_assert(s == UCS_OK,
                  "can't unmap memory for symmetric heap #%lu",
                  hn);
}

/*
 * Fire off the EP disconnect process
 */

inline static ucs_status_ptr_t
ep_disconnect_nb(ucp_ep_h ep)
{
    ucs_status_ptr_t req;

    if (ep == NULL) {
        return NULL;
        /* NOT REACHED */
    }

#ifdef HAVE_UCP_EP_CLOSE_NB
    req = ucp_ep_close_nb(ep, UCP_EP_CLOSE_MODE_FLUSH);
#else
    req = ucp_disconnect_nb(ep);
#endif  /* HAVE_UCP_EP_CLOSE_NB */

    return req;
}

/*
 * Complete the EP disconnect process
 */

inline static void
ep_wait(ucs_status_ptr_t req)
{
    ucs_status_t s;

    if (req == UCS_OK || UCS_PTR_IS_ERR(req)) {
        return;
        /* NOT REACHED */
    }

    do {
        shmemc_progress();

#ifdef HAVE_UCP_REQUEST_CHECK_STATUS
        s = ucp_request_check_status(req);
#else
        s = ucp_request_test(req, NULL);
#endif  /* HAVE_UCP_REQUEST_CHECK_STATUS */
    } while (s == UCS_INPROGRESS);

    ucp_request_free(req);
}

/*
 * Start the disconnects for all PEs, and then wait for completion
 */

inline static void
disconnect_all_endpoints(void)
{
    ucs_status_ptr_t *req;
    int i;

    req = (ucs_status_ptr_t *) calloc(proc.nranks, sizeof(*req));
    shmemu_assert(req != NULL,
                  "failed to allocate memory for UCP endpoint disconnect");

    for (i = 0; i < proc.nranks; i += 1) {
        req[i] = ep_disconnect_nb(proc.comms.eps[i]);
    }

    for (i = 0; i < proc.nranks; i += 1) {
        ep_wait(req[i]);
    }

    free(req);
}

/*
 * create backing for memory regions (heaps & globals)
 */

inline static void
init_memory_regions(void)
{
    size_t i;

    /* 1 globals region, plus symmetric heaps */
    proc.comms.nregions = 1 + proc.env.heaps.nheaps;

    /* init that many regions on me */
    proc.comms.regions =
        (mem_region_t *) calloc(proc.comms.nregions, sizeof(mem_region_t));
    shmemu_assert(proc.comms.regions != NULL,
                  "can't allocate memory for memory regions");

    /* now prep for all PEs to exchange */
    for (i = 0; i < proc.comms.nregions; i += 1) {
        proc.comms.regions[i].minfo =
            (mem_info_t *) calloc(proc.nranks, sizeof(mem_info_t));
        shmemu_assert(proc.comms.regions[i].minfo != NULL,
                      "can't allocate memory region metadata");
    }

    /* to access global variables */
    globals = & proc.comms.regions[0].minfo[proc.rank];
}

/*
 * register global variables (implicitly index 0), then all heaps
 */

inline static void
register_memory_regions(void)
{
    size_t hi;

    register_globals();

    for (hi = 1; hi < proc.comms.nregions; hi += 1) {
        mem_info_t *shp = & proc.comms.regions[hi].minfo[proc.rank];

        register_symmetric_heap(hi - 1, shp);
    }
}

inline static void
deregister_memory_regions(void)
{
    size_t hi;

    /* deregister symmetric heaps, then globals (index 0) */
    for (hi = proc.comms.nregions - 1; hi >= 1; hi -= 1) {
        mem_info_t *shp = & proc.comms.regions[hi].minfo[proc.rank];

        deregister_symmetric_heap(shp);

        /* TODO: reclaim shmema_finalize(); */

    }

    deregister_globals();
}

/**
 * API
 *
 **/

void
shmemc_ucx_make_remote_endpoints(void)
{
    ucs_status_t s;
    shmemc_context_h ch = (shmemc_context_h) SHMEM_CTX_DEFAULT;
    ucp_ep_params_t epm;
    int i;

    for (i = 0; i < proc.nranks; i += 1) {
        const int pe = SHIFT(i);

        epm.field_mask = UCP_EP_PARAM_FIELD_REMOTE_ADDRESS;
        epm.address = (ucp_address_t *) proc.comms.xchg_wrkr_info[pe].buf;

        s = ucp_ep_create(ch->w, &epm, &proc.comms.eps[pe]);

        /*
         * this can fail if we have e.g. mlx4 and not mlx5 infiniband.
         * I can tell it's failed, but I don't really know why...
         */
        if (s != UCS_OK) {
            shmemu_fatal("Unable to create remote endpoints: %s",
                         ucs_status_string(s)
                         );
            /* NOT REACHED */
        }
    }
}

/*
 * Barrier config
 */

long *shmemc_barrier_all_psync;
long *shmemc_sync_all_psync;

#define ALLOC_INTERNAL_SYMM_VAR(_var)                                   \
    do {                                                                \
        int i;                                                          \
        const size_t nbytes                                             \
            = sizeof(*(_var)) * SHMEM_BARRIER_SYNC_SIZE;                \
                                                                        \
        (_var) = (long *) shmema_malloc(nbytes);                        \
                                                                        \
        for (i = 0; i < SHMEM_BARRIER_SYNC_SIZE; i += 1) {              \
            (_var)[i] = SHMEM_SYNC_VALUE;                               \
        }                                                               \
    } while (0)

#define FREE_INTERNAL_SYMM_VAR(_var)                                    \
    do {                                                                \
        shmema_free(_var);                                              \
    } while (0)

/*
 * UCX initialize and finalize
 */

inline static void
ucx_init_ready(void)
{
    ucs_status_t s;
    ucp_params_t pm;

    s = ucp_config_read(NULL, NULL, &proc.comms.ucx_cfg);
    shmemu_assert(s == UCS_OK, "can't read UCX config");

    pm.field_mask =
        UCP_PARAM_FIELD_FEATURES |
        UCP_PARAM_FIELD_MT_WORKERS_SHARED |
        UCP_PARAM_FIELD_ESTIMATED_NUM_EPS;

    pm.features =
        UCP_FEATURE_RMA      |  /* put/get */
        UCP_FEATURE_AMO32    |  /* 32-bit atomics */
        UCP_FEATURE_AMO64    |  /* 64-bit atomics */
        UCP_FEATURE_WAKEUP;     /* events (not used, but looking ahead) */

    pm.mt_workers_shared = (proc.td.osh_tl > SHMEM_THREAD_SINGLE);

    pm.estimated_num_eps = proc.nranks;

    s = ucp_init(&pm, proc.comms.ucx_cfg, &proc.comms.ucx_ctxt);
    shmemu_assert(s == UCS_OK, "can't initialize UCX");
}

inline static void
ucx_ready(void)
{
    /* don't need config info any more */
    ucp_config_release(proc.comms.ucx_cfg);
}

inline static void
ucx_cleanup(void)
{
    ucp_cleanup(proc.comms.ucx_ctxt);
}

void
shmemc_ucx_init(void)
{
    int n;

    ucx_init_ready();

    /* user-supplied setup */
    shmemc_env_init();

    /* make remote memory usable */
    init_memory_regions();
    register_memory_regions();

    /* Create exchange workers and space for EPs */
    allocate_xworkers_table();
    allocate_endpoints_table();

    /* prep contexts, allocate first one (default) */
    allocate_contexts_table();

    n = shmemc_init_default_context();
    shmemu_assert(n == 0, "couldn't initialize default context");

    /* pre-allocate internal sync variables */
    ALLOC_INTERNAL_SYMM_VAR(shmemc_barrier_all_psync);
    ALLOC_INTERNAL_SYMM_VAR(shmemc_sync_all_psync);

    ucx_ready();

    /* set up globalexit handler */
    shmemc_globalexit_init();
}

/*
 * ucx_finalize basically just undoes ucx_init
 */

void
shmemc_ucx_finalize(void)
{
    shmemc_globalexit_finalize();

    if (! proc.env.xpmem_kludge) {
        disconnect_all_endpoints();
    }
    deallocate_endpoints_table();

    deallocate_contexts_table();
    deallocate_xworkers_table();

    /* free up internal sync variables */
    FREE_INTERNAL_SYMM_VAR(shmemc_barrier_all_psync);
    FREE_INTERNAL_SYMM_VAR(shmemc_sync_all_psync);

    deregister_memory_regions();

    shmemc_env_finalize();

    ucx_cleanup();
}
