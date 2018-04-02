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
#include "barrier.h"

#include "allocator/memalloc.h"

#include <stdlib.h>             /* getenv */
#include <string.h>
#include <strings.h>
#include <assert.h>

#include <ucp/api/ucp.h>

#define DUMP_DEBUG_INFO 0

#if DUMP_DEBUG_INFO
inline static void
check_version(void)
{
    unsigned int maj, min, rel;

    ucp_get_version(&maj, &min, &rel);

    fprintf(stderr, "Piecewise query:\n");
    fprintf(stderr, "    UCX version \"%u.%u.%u\"\n", maj, min, rel);
    fprintf(stderr, "String query\n");
    fprintf(stderr, "    UCX version \"%s\"\n", ucp_get_version_string());
    fprintf(stderr, "\n");
}

/*
 * debugging output
 */
inline static void
dump_mapped_mem_info(const char *name, const mem_info_t *mp)
{
    ucs_status_t s;
    ucp_mem_attr_t attr;

    /* the attributes we want to inspect */
    attr.field_mask =
        UCP_MEM_ATTR_FIELD_ADDRESS |
        UCP_MEM_ATTR_FIELD_LENGTH;

    s = ucp_mem_query(mp->racc.mh, &attr);
    assert(s == UCS_OK);
}

inline static void
dump(void)
{
    if (proc.rank == 0) {
        const ucs_config_print_flags_t flags =
            UCS_CONFIG_PRINT_CONFIG |
            UCS_CONFIG_PRINT_HEADER;

        ucp_config_print(proc.comms.ucx_cfg, stderr, "My config", flags);
        ucp_context_print_info(proc.comms.ucx_ctxt, stderr);
        /* ucp_worker_print_info(proc.comms.wrkr, stderr); */
        check_version();
        fprintf(stderr, "----------------------------------------------\n\n");
        fflush(stderr);
    }
    dump_mapped_mem_info("heap", def_symm_heap);
    dump_mapped_mem_info("globals", globals);
}
#endif /* DUMP_DEBUG_INFO */

/*
 * UCX config
 */
inline static void
make_init_params(ucp_params_t *pmp)
{
    pmp->field_mask =
        UCP_PARAM_FIELD_FEATURES |
        UCP_PARAM_FIELD_MT_WORKERS_SHARED |
        UCP_PARAM_FIELD_ESTIMATED_NUM_EPS;

    pmp->features =
        UCP_FEATURE_RMA |       /* put/get */
        UCP_FEATURE_AMO32 |     /* 32-bit atomics */
        UCP_FEATURE_AMO64 |     /* 64-bit atomics */
        UCP_FEATURE_WAKEUP;     /* events */

    pmp->mt_workers_shared = (proc.td.osh_tl > SHMEM_THREAD_SINGLE);

    pmp->estimated_num_eps = proc.nranks;
}

/*
 * worker tables
 */
inline static void
allocate_xworkers_table(void)
{
    proc.comms.xchg_wrkr_info = (worker_info_t *)
        calloc(proc.nranks, sizeof(*(proc.comms.xchg_wrkr_info)));
    assert(proc.comms.xchg_wrkr_info != NULL);
}

inline static void
deallocate_xworkers_table(void)
{
    if (proc.comms.xchg_wrkr_info != NULL) {
        free(proc.comms.xchg_wrkr_info);
    }
}

/*
 * endpoint tables
 */
inline static void
allocate_endpoints_table(void)
{
    proc.comms.eps = (ucp_ep_h *)
        calloc(proc.nranks, sizeof(*(proc.comms.eps)));
    assert(proc.comms.eps != NULL);
}

inline static void
deallocate_endpoints_table(void)
{
    if (proc.comms.eps != NULL) {
        free(proc.comms.eps);
    }
}

inline static void
allocate_contexts_table(void)
{
    /*
     * no SHMEM contexts created yet
     */
    proc.comms.nctxts = 0;
    proc.comms.ctxts = NULL;
}

inline static void
deallocate_contexts_table(void)
{
    size_t c;

    /*
     * special release case for default context
     */
    ucp_worker_release_address(proc.comms.ctxts[0]->w,
                               proc.comms.xchg_wrkr_info[proc.rank].addr);
    /*
     * clear up each SHMEM context
     */
    for (c = 0; c < proc.comms.nctxts; c += 1) {
        if (proc.comms.ctxts[c] != NULL) {
            ucp_worker_destroy(proc.comms.ctxts[c]->w);
            free(proc.comms.ctxts[c]);
        }
    }
}

/*
 * a couple of shortcuts
 */
static mem_info_t *globals;
static mem_info_t *def_symm_heap;

inline static void
register_symmetric_heap(void)
{
    ucs_status_t s;
    ucp_mem_map_params_t mp;
    ucp_mem_attr_t attr;

    /* now register it with UCX */
    mp.field_mask =
        UCP_MEM_MAP_PARAM_FIELD_LENGTH |
        UCP_MEM_MAP_PARAM_FIELD_FLAGS;

    mp.length = proc.env.def_heap_size;
    mp.flags =
        UCP_MEM_MAP_ALLOCATE;

    s = ucp_mem_map(proc.comms.ucx_ctxt, &mp, &def_symm_heap->racc.mh);
    assert(s == UCS_OK);

    /*
     * query back to find where it is, and its actual size (might be
     * aligned/padded)
     */

    /* the attributes we want to inspect */
    attr.field_mask =
        UCP_MEM_ATTR_FIELD_ADDRESS |
        UCP_MEM_ATTR_FIELD_LENGTH;

    s = ucp_mem_query(def_symm_heap->racc.mh, &attr);
    assert(s == UCS_OK);

    /* tell the PE what was given */
    def_symm_heap->base = (uint64_t) attr.address;
    def_symm_heap->end  = def_symm_heap->base + attr.length;
    def_symm_heap->len  = attr.length;

    /* initialize the heap allocator */
    shmema_init((void *) def_symm_heap->base, def_symm_heap->len);
}

inline static void
deregister_symmetric_heap(void)
{
    ucs_status_t s;

    shmema_finalize();

    s = ucp_mem_unmap(proc.comms.ucx_ctxt, def_symm_heap->racc.mh);
    assert(s == UCS_OK);
}

inline static void
register_globals(void)
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
    assert(s == UCS_OK);
}

inline static void
deregister_globals(void)
{
    ucs_status_t s;

    s = ucp_mem_unmap(proc.comms.ucx_ctxt, globals->racc.mh);
    assert(s == UCS_OK);
}

inline static void
blocking_ep_disconnect(ucp_ep_h ep)
{
    ucs_status_ptr_t req;

    if (ep == NULL) {
        return;
        /* NOT REACHED */
    }

#ifdef HAVE_UCP_EP_CLOSE_NB
    req = ucp_ep_close_nb(ep, UCP_EP_CLOSE_MODE_FLUSH);
#else
    req = ucp_disconnect_nb(ep);
#endif  /* HAVE_UCP_EP_CLOSE_NB */

    if (req == UCS_OK) {
        return;
        /* NOT REACHED */
    }
    else if (UCS_PTR_IS_ERR(req)) {
        // ucp_request_cancel(wrkr, req);
        return;
        /* NOT REACHED */
    }
    else {
        ucs_status_t s;

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
}

inline static void
disconnect_all_endpoints(void)
{
    int i;

    for (i = 0; i < proc.nranks; i += 1) {
        const int pe = (i + proc.rank) % proc.nranks;

        blocking_ep_disconnect(proc.comms.eps[pe]);
    }
}

inline static void
init_memory_regions(void)
{
    size_t i;

    /* TODO: hardwire for now: globals + default symmetric heap */
    proc.comms.nregions = 2;

    /* init that many regions on me */
    proc.comms.regions =
        (mem_region_t *) malloc(proc.comms.nregions * sizeof(mem_region_t));
    assert(proc.comms.regions != NULL);

    /* now prep for all PEs to exchange */
    for (i = 0; i < proc.comms.nregions; i += 1) {
        proc.comms.regions[i].minfo =
            (mem_info_t *) malloc(proc.nranks * sizeof(mem_info_t));
        assert(proc.comms.regions[i].minfo != NULL);
    }
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
        const int pe = (i + proc.rank) % proc.nranks;

        epm.field_mask = UCP_EP_PARAM_FIELD_REMOTE_ADDRESS;
        epm.address = (ucp_address_t *) proc.comms.xchg_wrkr_info[pe].buf;

        s = ucp_ep_create(ch->w, &epm, &proc.comms.eps[pe]);

        /*
         * this can fail if we have e.g. mlx4 and not mlx5 infiniband
         */
        if (s != UCS_OK) {
            shmemu_fatal("Unable to create remote endpoints: %s",
                         ucs_status_string(s)
                         );
            /* NOT REACHED */
        }
    }
}

extern long *shmemc_barrier_all_psync;
extern long *shmemc_sync_all_psync;

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

void
shmemc_ucx_init(void)
{
    ucs_status_t s;
    ucp_params_t pm;

    /* start initialization */
    s = ucp_config_read(NULL, NULL, &proc.comms.ucx_cfg);
    assert(s == UCS_OK);

    make_init_params(&pm);

    s = ucp_init(&pm, proc.comms.ucx_cfg, &proc.comms.ucx_ctxt);
    assert(s == UCS_OK);

    shmemc_env_init();

    /* collectives are go */
    shmemc_barrier_init();
    shmemc_broadcast_init();

    init_memory_regions();

    /* local shortcuts TODO: hardwired index */
    globals = & proc.comms.regions[0].minfo[proc.rank];
    def_symm_heap = & proc.comms.regions[1].minfo[proc.rank];

    /* TODO: generalize for multiple heaps */
    register_globals();
    register_symmetric_heap();

    /* pre-allocate internal sync variables */
    ALLOC_INTERNAL_SYMM_VAR(shmemc_barrier_all_psync);
    ALLOC_INTERNAL_SYMM_VAR(shmemc_sync_all_psync);

    /* Create exchange workers and space for EPs */
    allocate_xworkers_table();
    allocate_endpoints_table();

    /* prep contexts, allocate first one (default) */
    allocate_contexts_table();
    shmemc_create_default_context(&SHMEM_CTX_DEFAULT);

#if DUMP_DEBUG_INFO
    dump();
#endif /* DUMP_DEBUG_INFO */

    /* don't need config info any more */
    ucp_config_release(proc.comms.ucx_cfg);

    shmemc_globalexit_init();
}

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

    /* TODO: generalize for multiple heaps */
    deregister_symmetric_heap();
    deregister_globals();

    shmemc_broadcast_finalize();
    shmemc_barrier_finalize();

    shmemc_env_finalize();

    ucp_cleanup(proc.comms.ucx_ctxt);
}
