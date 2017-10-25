#include "thispe.h"
#include "shmemu.h"
#include "shmemc.h"
#include "state.h"
#include "progress.h"

#include <stdlib.h>             /* getenv */
#include <assert.h>

#include <ucp/api/ucp.h>

#define KB 1024L
#define MB (KB * KB)
#define GB (KB * MB)

/*
 * debugging output stream
 */
static FILE *say;

#if 0
/*
 * TODO: move to shmemu namespace in real implementation
 */
static void *
shmemu_round_down_address_to_pagesize(void *addr)
{
    const long psz = sysconf(_SC_PAGESIZE);
    /* make sure aligned to page size multiples */
    const size_t mod = (size_t) addr % psz;

    if (mod != 0) {
        const size_t div = (size_t) addr / psz;

        addr = (void *) ((div - 1) * psz);
    }

    return addr;
}
#endif

/**
 * no special treatment required here
 *
 * TODO put getenv calls all in 1 place
 */
char *
shmemc_getenv(const char *name)
{
    return getenv(name);
}

static void
check_version(void)
{
    unsigned int maj, min, rel;

    ucp_get_version(&maj, &min, &rel);

    fprintf(say, "Piecewise query:\n");
    fprintf(say, "    UCX version \"%u.%u.%u\"\n", maj, min, rel);
    fprintf(say, "String query\n");
    fprintf(say, "    UCX version \"%s\"\n", ucp_get_version_string());
    fprintf(say, "\n");
}

static void
make_init_params(ucp_params_t *p_p)
{
    p_p->field_mask =
        UCP_PARAM_FIELD_FEATURES |
        UCP_PARAM_FIELD_ESTIMATED_NUM_EPS;

    /*
     * we'll try at first to get both 32- and 64-bit direct AMO
     * support because OpenSHMEM wants them
     */
    p_p->features =
        UCP_FEATURE_RMA |
        UCP_FEATURE_WAKEUP |
        UCP_FEATURE_AMO32 |
        UCP_FEATURE_AMO64
        ;
    p_p->estimated_num_eps = proc.nranks;
}

/*
 * worker tables
 */
static void
allocate_workers(void)
{
    proc.comms.wrkrs = (worker_info_t *)
        calloc(proc.nranks, sizeof(*(proc.comms.wrkrs)));
    assert(proc.comms.wrkrs != NULL);
}

static void
deallocate_workers(void)
{
    if (proc.comms.wrkrs != NULL) {
        free(proc.comms.wrkrs);
    }
}

static void
make_local_worker(void)
{
    ucs_status_t s;
    ucp_worker_params_t wkpm;
    ucp_address_t *a;
    size_t l;

    wkpm.field_mask  = UCP_WORKER_PARAM_FIELD_THREAD_MODE;
    wkpm.thread_mode = UCS_THREAD_MODE_SINGLE;

    s = ucp_worker_create(proc.comms.ctxt, &wkpm, &proc.comms.wrkr);
    assert(s == UCS_OK);

    /* get address for remote access to worker */
    s = ucp_worker_get_address(proc.comms.wrkr, &a, &l);
    assert(s == UCS_OK);

    proc.comms.wrkrs[proc.rank].addr = a;
    proc.comms.wrkrs[proc.rank].len = l;
}

/*
 * endpoint tables
 */
static void
allocate_endpoints(void)
{
    proc.comms.eps = (ucp_ep_h *)
        calloc(proc.nranks, sizeof(*(proc.comms.eps)));
    assert(proc.comms.eps != NULL);
}

static void
deallocate_endpoints(void)
{
    if (proc.comms.eps != NULL) {
        free(proc.comms.eps);
    }
}

/*
 * a couple of shortcuts
 */
static mem_info_t *globals;
static mem_info_t *def_symm_heap;

/*
 * debugging output
 */
static void
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

#if 0
    logger(LOG_MEMORY,
           "%d: \"%s\" memory at %p, length %.2f MB (%lu bytes)",
           proc.rank, name,
           attr.address,
           (double) attr.length / MB,
           (unsigned long) attr.length
           );
#endif
}

static void
dump(void)
{
    if (proc.rank == 0) {
        const ucs_config_print_flags_t flags =
            UCS_CONFIG_PRINT_CONFIG |
            UCS_CONFIG_PRINT_HEADER;

        ucp_config_print(proc.comms.cfg, say, "My config", flags);
        ucp_context_print_info(proc.comms.ctxt, say);
        ucp_worker_print_info(proc.comms.wrkr, say);
        check_version();
        fprintf(say, "----------------------------------------------\n\n");
        fflush(say);
    }
    dump_mapped_mem_info("heap", def_symm_heap);
    dump_mapped_mem_info("globals", globals);
}

static void
reg_symmetric_heap(void)
{
    ucs_status_t s;
    ucp_mem_map_params_t mp;
    ucp_mem_attr_t attr;
    char *hs_str = shmemc_getenv("SHMEM_SYMMETRIC_HEAP_SIZE");
    size_t len;

    /* first look to see if we request a certain size */
    if (hs_str != NULL) {
        const int r = shmemu_parse_size(hs_str, &len);
        assert(r == 0);
    }
    else {
        len = 4 * MB; /* just some usable value */
    }

    /* now register it with UCX */
    mp.field_mask =
        UCP_MEM_MAP_PARAM_FIELD_LENGTH |
        UCP_MEM_MAP_PARAM_FIELD_FLAGS;

    mp.length = len;
    mp.flags =
        UCP_MEM_MAP_ALLOCATE;

    s = ucp_mem_map(proc.comms.ctxt, &mp, &def_symm_heap->racc.mh);
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
    def_symm_heap->length = attr.length;

    /* initialize the heap allocator */
    shmemc_mem_init((void *) def_symm_heap->base,
                    def_symm_heap->length);

#if 0
    logger(LOG_MEMORY,
           "default symm heap @ %lu, size %lu",
           def_symm_heap->base, def_symm_heap->length);
#endif
}

static void
dereg_symmetric_heap(void)
{
    ucs_status_t s;

    s = ucp_mem_unmap(proc.comms.ctxt, def_symm_heap->racc.mh);
    assert(s == UCS_OK);
}

static void
reg_globals(void)
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
    globals->length = len;

    s = ucp_mem_map(proc.comms.ctxt, &mp, &globals->racc.mh);
    assert(s == UCS_OK);

#if 0
    logger(LOG_MEMORY,
           "globals @ %lu, size %lu",
           globals->base, globals->length);
#endif
}

static void
dereg_globals(void)
{
    ucs_status_t s;

    s = ucp_mem_unmap(proc.comms.ctxt, globals->racc.mh);
    assert(s == UCS_OK);
}

static void
disconnect_all_eps(void)
{
    int pe;

    for (pe = 0; pe < proc.nranks; pe += 1) {
        ucs_status_ptr_t req =
            ucp_ep_close_nb(proc.comms.eps[pe], UCP_EP_CLOSE_MODE_FLUSH);

        /* if not done immediately, wait */
        if (req != UCS_OK) {
            ucs_status_t s;

            do {
                (void) ucp_worker_progress(proc.comms.wrkr);
                s = ucp_request_check_status(req);
            } while (s == UCS_INPROGRESS);
            ucp_request_free(req);
        }
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
    ucp_ep_params_t epm;
    int pe;

    for (pe = 0; pe < proc.nranks; pe += 1) {
        const int i = (pe + proc.rank) % proc.nranks;

        epm.field_mask = UCP_EP_PARAM_FIELD_REMOTE_ADDRESS;
        epm.address = (ucp_address_t *) proc.comms.wrkrs[i].buf;

        s = ucp_ep_create(proc.comms.wrkr, &epm, &proc.comms.eps[i]);
        assert(s == UCS_OK);
    }
}

void
shmemc_ucx_init(void)
{
    ucs_status_t s;
    ucp_params_t pm;

    say = stderr;

    /* start initialization */
    s = ucp_config_read(NULL, NULL, &proc.comms.cfg);
    assert(s == UCS_OK);

    make_init_params(&pm);

    s = ucp_init(&pm, proc.comms.cfg, &proc.comms.ctxt);
    assert(s == UCS_OK);

    /* local shortcuts TODO: hardwired index */
    globals = & proc.comms.regions[0].minfo[proc.rank];
    def_symm_heap = & proc.comms.regions[1].minfo[proc.rank];

    reg_globals();
    reg_symmetric_heap();

    /*
     * Create workers and space for EPs
     */
    allocate_workers();
    allocate_endpoints();

    make_local_worker();

    // dump();

    /* don't need config info any more */
    ucp_config_release(proc.comms.cfg);

    shmemc_ucx_progress_init();
}

#define REAL_SHUTDOWN 1

void
shmemc_ucx_finalize(void)
{
    shmemc_ucx_progress_finalize();

#if REAL_SHUTDOWN
    disconnect_all_eps();
    deallocate_endpoints();
#endif

    if (proc.comms.wrkr) {
        ucp_worker_release_address(proc.comms.wrkr,
                                   proc.comms.wrkrs[proc.rank].addr);
        ucp_worker_destroy(proc.comms.wrkr);
    }
    deallocate_workers();

#if REAL_SHUTDOWN
    dereg_globals();
    dereg_symmetric_heap();

    ucp_cleanup(proc.comms.ctxt);
#endif
}
