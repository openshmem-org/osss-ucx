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

#include "api.h"
#include "module.h"

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
        calloc(proc.li.nranks, sizeof(*(proc.comms.xchg_wrkr_info)));
    shmemu_assert(proc.comms.xchg_wrkr_info != NULL,
                  MODULE ": can't allocate memory for worker exchange");
}

inline static void
deallocate_xworkers_table(void)
{
    int pe;

    /* clean up worker exchange */
    for (pe = 0; pe < proc.li.nranks; ++pe) {
        free(proc.comms.xchg_wrkr_info[pe].buf);
    }
    free(proc.comms.xchg_wrkr_info);
}

/*
 * THIS NEXT BIT NOW BELONGS TO TEAMS
 */

#if 0
/*
 * Context management
 */
inline static void
contexts_table_init(void)
{
    /*
     * no new SHMEM contexts created yet
     */
    proc.comms.nctxts = 0;
    proc.comms.ctxts = NULL;
}

inline static void
teardown_context(shmemc_context_h ch)
{
    size_t r;
    int pe;

    if (! proc.env.teardown_kludge) {
        shmemc_ucx_disconnect_all_eps(ch);
    }
    /* release remote access memory */
    for (r = 0; r < proc.comms.nregions; ++r) {
        for (pe = 0; pe < proc.li.nranks; ++pe) {
            ucp_rkey_destroy(ch->racc[r].rinfo[pe].rkey);
        }
        free(ch->racc[r].rinfo);
    }
    free(ch->racc);

    shmemc_ucx_deallocate_eps_table(ch);
    ucp_worker_destroy(ch->w);
}

inline static void
contexts_table_finalize(void)
{
    size_t c;

    /*
     * clear up each allocated SHMEM context
     */
    for (c = 0; c < proc.comms.nctxts; ++c) {
        teardown_context(proc.comms.ctxts[c]);
    }

    /*
     * special release case for default context
     */
    ucp_worker_release_address(defcp->w,
                               proc.comms.xchg_wrkr_info[proc.li.rank].addr);
    teardown_context(defcp);

    free(proc.comms.ctxts);
}
#endif

/*
 * shortcut for global variables
 */
static mem_info_t *globals;

/*
 * from the executable
 *
 * N.B. this will be extended when FreeBSD support is on the table.
 */
extern char data_start;
extern char end;

inline static void
get_globals_address_range(uint64_t *base_p, uint64_t *end_p)
{
    *base_p = (uint64_t) &data_start;
    *end_p  = (uint64_t) &end;
}

inline static void
register_globals(void)
{
    uint64_t g_base;
    uint64_t g_end;
    size_t len;
    ucp_mem_map_params_t mp;
    ucs_status_t s;

    mp.field_mask =
        UCP_MEM_MAP_PARAM_FIELD_ADDRESS |
        UCP_MEM_MAP_PARAM_FIELD_LENGTH;

    get_globals_address_range(&g_base, &g_end);
    len = g_end - g_base;

    mp.address = (void *) g_base;
    mp.length = len;

    globals->base = g_base;
    globals->end  = g_end;
    globals->len  = len;

    s = ucp_mem_map(proc.comms.ucx_ctxt, &mp, &globals->mh);
    shmemu_assert(s == UCS_OK,
                  MODULE ": can't map global variables memory: %s",
                  ucs_status_string(s));

    /* don't need allocator, variables already there */
}

inline static void
deregister_globals(void)
{
    ucs_status_t s;

    s = ucp_mem_unmap(proc.comms.ucx_ctxt, globals->mh);
    shmemu_assert(s == UCS_OK,
                  MODULE ": can't unmap global memory: %s",
                  ucs_status_string(s));
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
                  MODULE ": cannot register empty symmetric heap #%lu",
                  hn);

    /* now register it with UCX */
    mp.field_mask =
        UCP_MEM_MAP_PARAM_FIELD_LENGTH |
        UCP_MEM_MAP_PARAM_FIELD_FLAGS;

    mp.length = proc.env.heaps.heapsize[heapno];

    mp.flags =
        UCP_MEM_MAP_NONBLOCK |
        UCP_MEM_MAP_ALLOCATE;

    s = ucp_mem_map(proc.comms.ucx_ctxt, &mp, &mip->mh);
    shmemu_assert(s == UCS_OK,
                  MODULE ": can't map memory for symmetric heap #%lu: %s",
                  hn, ucs_status_string(s));

    mip->id = hn;

    /*
     * query back to find where it is, and its actual size (might be
     * aligned/padded)
     */

    /* the attributes we want to inspect */
    attr.field_mask =
        UCP_MEM_ATTR_FIELD_ADDRESS |
        UCP_MEM_ATTR_FIELD_LENGTH;

    s = ucp_mem_query(mip->mh, &attr);
    shmemu_assert(s == UCS_OK,
                  MODULE ": can't query extent of memory for "
                  "symmetric heap #%lu: %s",
                  hn, ucs_status_string(s));

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

    s = ucp_mem_unmap(proc.comms.ucx_ctxt, mip->mh);
    shmemu_assert(s == UCS_OK,
                  MODULE ": can't unmap memory for "
                  "symmetric heap #%lu: %s",
                  hn, ucs_status_string(s));
}

/*
 * create backing for memory regions (heaps & globals)
 */

inline static void
opaque_rkeys_init(void)
{
    size_t r;

    proc.comms.orks = (mem_opaque_t *)
        calloc(proc.comms.nregions, sizeof(mem_opaque_t));
    shmemu_assert(proc.comms.orks != NULL,
                  MODULE ": can't allocate memory for opaque rkeys");

    for (r = 0; r < proc.comms.nregions; ++r) {
        proc.comms.orks[r].rkeys =
            (mem_opaque_rkey_t *) calloc(proc.li.nranks,
                                         sizeof(mem_opaque_rkey_t));
        shmemu_assert(proc.comms.orks[r].rkeys != NULL,
                      MODULE ": can't allocate memory for opaque rkeys");
    }
}

inline static void
opaque_rkeys_finalize(void)
{
    size_t r;
    int pe;

    /* clear opaque rkeys */
    for (r = 0; r < proc.comms.nregions; ++r) {
        for (pe = 0; pe < proc.li.nranks; ++pe) {
            free(proc.comms.orks[r].rkeys[pe].data);
        }
    }
}

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
                  MODULE ": can't allocate memory for memory regions");

    /* now prep for all PEs to exchange */
    for (i = 0; i < proc.comms.nregions; ++i) {
        proc.comms.regions[i].minfo =
            (mem_info_t *) calloc(proc.li.nranks, sizeof(mem_info_t));
        shmemu_assert(proc.comms.regions[i].minfo != NULL,
                      MODULE ": can't allocate memory region metadata");
    }

    /* to access global variables */
    globals = & proc.comms.regions[0].minfo[proc.li.rank];
}

/*
 * register global variables (implicitly index 0), then all heaps
 */

inline static void
register_memory_regions(void)
{
    size_t hi;

    register_globals();

    for (hi = 1; hi < proc.comms.nregions; ++hi) {
        mem_info_t *shp = & proc.comms.regions[hi].minfo[proc.li.rank];

        register_symmetric_heap(hi - 1, shp);
    }
}

inline static void
deregister_memory_regions(void)
{
    size_t hi;

    /* deregister symmetric heaps, then globals (index 0) */
    for (hi = proc.comms.nregions - 1; hi >= 1; hi -= 1) {
        mem_info_t *shp = & proc.comms.regions[hi].minfo[proc.li.rank];

        deregister_symmetric_heap(shp);

        /* TODO: reclaim shmema_finalize(); */
    }

    deregister_globals();

    for (hi = 0; hi < proc.comms.nregions; ++hi) {
        free(proc.comms.regions[hi].minfo);
    }
    free(proc.comms.regions);
}

/**
 * API
 *
 **/


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
        for (i = 0; i < SHMEM_BARRIER_SYNC_SIZE; ++i) {                 \
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
    shmemu_assert(s == UCS_OK,
                  MODULE ": can't read UCX config: %s",
                  ucs_status_string(s));

    pm.field_mask =
#ifdef HAVE_UCP_PARAM_FIELD_NAME
        UCP_PARAM_FIELD_NAME              |
#endif /* HAVE_UCP_PARAM_FIELD_NAME */
        UCP_PARAM_FIELD_FEATURES          |
        UCP_PARAM_FIELD_MT_WORKERS_SHARED |
        UCP_PARAM_FIELD_ESTIMATED_NUM_EPS |
        UCP_PARAM_FIELD_ESTIMATED_NUM_PPN;

#ifdef HAVE_UCP_PARAM_FIELD_NAME
    pm.name = PACKAGE_NAME;
#endif /* HAVE_UCP_PARAM_FIELD_NAME */

    pm.features =
        UCP_FEATURE_RMA      |  /* put/get */
        UCP_FEATURE_AMO32    |  /* 32-bit atomics */
        UCP_FEATURE_AMO64;      /* 64-bit atomics */

    pm.mt_workers_shared = (proc.td.osh_tl > SHMEM_THREAD_SINGLE);

    /* estimated program size */
    pm.estimated_num_eps = proc.li.nranks;
    pm.estimated_num_ppn = proc.li.npeers;

    s = ucp_init(&pm, proc.comms.ucx_cfg, &proc.comms.ucx_ctxt);
    if (s != UCS_OK) {
        shmemu_fatal(MODULE ": can't initialize UCX: %s",
                     ucs_status_string(s));
    }

    /* sanity check that we got a context */
    if (proc.comms.ucx_ctxt == NULL) {
        shmemu_fatal(MODULE ": UCX was initialized, "
                     "but there is no communications context");
    }
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
    ucx_init_ready();

    /* user-supplied setup */
    shmemc_env_init();

    /* make remote memory usable */
    init_memory_regions();
    register_memory_regions();

    /* master copy of exchanged rkeys */
    opaque_rkeys_init();

    /* Create exchange workers and space for EPs */
    allocate_xworkers_table();

    shmemc_ucx_allocate_eps_table(defcp);

#if 0
    /* prep contexts, allocate first one (default) */
    contexts_table_init();
#endif

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

#if 0
    contexts_table_finalize();
#endif

    deallocate_xworkers_table();

    /* free up internal sync variables */
    FREE_INTERNAL_SYMM_VAR(shmemc_barrier_all_psync);
    FREE_INTERNAL_SYMM_VAR(shmemc_sync_all_psync);

    opaque_rkeys_finalize();

    deregister_memory_regions();

    shmemc_env_finalize();

    ucx_cleanup();
}
