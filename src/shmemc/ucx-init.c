/* For license: see LICENSE file at top-level */

#include "thispe.h"
#include "shmemu.h"
#include "shmemc.h"
#include "state.h"
#include "globalexit.h"
#include "memalloc.h"

#include <stdlib.h>             /* getenv */
#include <string.h>
#include <strings.h>
#include <assert.h>

#include <ucp/api/ucp.h>

#define KB 1024L
#define MB (KB * KB)
#define GB (KB * MB)

/*
 * debugging output stream
 */
static FILE *say;

/**
 * no special treatment required here
 *
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

inline static int
option_enabled_test(const char *str)
{
    int ret = 0;

    if ((strncasecmp(str, "y", 1) == 0) ||
        (strncasecmp(str, "on", 2) == 0) ||
        (atoi(str) > 0)) {
        ret = 1;
    }

    return ret;
}

/*
 * backward-compatible environment variables
 */
inline static void
backcompat_environment(void)
{
    int overwrite = 1;
    char *e;

    /*
     * defined in spec
     */
    e = shmemc_getenv("SMA_VERSION");
    if (e != NULL) {
        (void) setenv("SHMEM_VERSION", "1", overwrite);
    }
    e = shmemc_getenv("SMA_INFO");
    if (e != NULL) {
        (void) setenv("SHMEM_INFO", "1", overwrite);
    }
    e = shmemc_getenv("SMA_SYMMETRIC_SIZE");
    if (e != NULL) {
        (void) setenv("SHMEM_SYMMETRIC_SIZE", e, overwrite);
    }
    e = shmemc_getenv("SMA_DEBUG");
    if (e != NULL) {
        (void) setenv("SHMEM_DEBUG", "y", overwrite);
    }
}

/*
 * read & save all our environment variables
 */
inline static void
read_environment(void)
{
    char *e;

    /* init environment */
    proc.env.print_version = 0;
    proc.env.print_info = 0;
    proc.env.def_heap_size = 4 * MB;
    proc.env.debug = 0;

    backcompat_environment();

    /*
     * defined in spec
     */
    e = shmemc_getenv("SHMEM_VERSION");
    if (e != NULL) {
        proc.env.print_version = 1;
    }
    e = shmemc_getenv("SHMEM_INFO");
    if (e != NULL) {
        proc.env.print_info = 1;
    }

    e = shmemc_getenv("SHMEM_SYMMETRIC_SIZE");
    if (e != NULL) {
        const int r = shmemu_parse_size(e, &proc.env.def_heap_size);

        assert(r == 0);
    }

    e = shmemc_getenv("SHMEM_DEBUG");
    if (e != NULL) {
        proc.env.debug = option_enabled_test(e);
    }

    /*
     * this implementation also has...
     */

    e = shmemc_getenv("SHMEM_DEBUG_FILE");
    if (e != NULL) {
        proc.env.debug_file = strdup(e); /* free at end */
    }
    else {
        proc.env.debug_file = NULL;
    }
}

inline static void
make_init_params(ucp_params_t *p_p)
{
    p_p->field_mask =
        UCP_PARAM_FIELD_FEATURES |
        UCP_PARAM_FIELD_ESTIMATED_NUM_EPS;

    /*
     * we'll try at first to get both 32- and 64-bit direct AMO
     * support because OpenSHMEM wants them
     */
    p_p->features  = UCP_FEATURE_RMA;
    p_p->features |= UCP_FEATURE_WAKEUP; /* for events */
    p_p->features |= UCP_FEATURE_AMO32;
    p_p->features |= UCP_FEATURE_AMO64;

    p_p->estimated_num_eps = proc.nranks;
}

/*
 * worker tables
 */
inline static void
allocate_workers(void)
{
    proc.comms.xchg_wrkr_info = (worker_info_t *)
        calloc(proc.nranks, sizeof(*(proc.comms.xchg_wrkr_info)));
    assert(proc.comms.xchg_wrkr_info != NULL);
}

inline static void
deallocate_workers(void)
{
    if (proc.comms.xchg_wrkr_info != NULL) {
        free(proc.comms.xchg_wrkr_info);
    }
}

/*
 * TODO note the below needs to be replaced with the default context
 * creation process.  Do we need to exchange more info for newly
 * created workers?  Sounds painful.  Maybe there's another way.
 */

inline static void
make_local_worker(void)
{
    ucs_status_t s;
    ucp_worker_params_t wkpm;
    ucp_address_t *addr;
    size_t len;

    wkpm.field_mask  = UCP_WORKER_PARAM_FIELD_THREAD_MODE;
    wkpm.thread_mode = UCS_THREAD_MODE_SINGLE;

    s = ucp_worker_create(proc.comms.ctxt, &wkpm, &proc.comms.wrkr);
    assert(s == UCS_OK);

    /* get address for remote access to worker */
    s = ucp_worker_get_address(proc.comms.wrkr, &addr, &len);
    assert(s == UCS_OK);

    proc.comms.xchg_wrkr_info[proc.rank].addr = addr;
    proc.comms.xchg_wrkr_info[proc.rank].len = len;
}

/*
 * endpoint tables
 */
inline static void
allocate_endpoints(void)
{
    proc.comms.eps = (ucp_ep_h *)
        calloc(proc.nranks, sizeof(*(proc.comms.eps)));
    assert(proc.comms.eps != NULL);
}

inline static void
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

inline static void
reg_symmetric_heap(void)
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
    def_symm_heap->len  = attr.length;

    /* initialize the heap allocator */
    shmemm_mem_init((void *) def_symm_heap->base,
                    def_symm_heap->len);
}

inline static void
dereg_symmetric_heap(void)
{
    ucs_status_t s;

    s = ucp_mem_unmap(proc.comms.ctxt, def_symm_heap->racc.mh);
    assert(s == UCS_OK);
}

inline static void
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
    globals->len  = len;

    s = ucp_mem_map(proc.comms.ctxt, &mp, &globals->racc.mh);
    assert(s == UCS_OK);
}

inline static void
dereg_globals(void)
{
    ucs_status_t s;

    s = ucp_mem_unmap(proc.comms.ctxt, globals->racc.mh);
    assert(s == UCS_OK);
}

inline static void
disconnect_all_eps(void)
{
    int pe;

    for (pe = 0; pe < proc.nranks; pe += 1) {
        ucs_status_ptr_t req =
#ifdef HAVE_UCP_EP_CLOSE_NB
            ucp_ep_close_nb(proc.comms.eps[pe], UCP_EP_CLOSE_MODE_FLUSH);
#else
        ucp_disconnect_nb(proc.comms.eps[pe]);
#endif  /* HAVE_UCP_EP_CLOSE_NB*/

        /* if not done immediately, wait */
        if (req != UCS_OK) {
            ucs_status_t s;

            do {
                (void) ucp_worker_progress(proc.comms.wrkr);
#ifdef HAVE_UCP_REQUEST_CHECK_STATUS
                s = ucp_request_check_status(req);
#else
                s = ucp_request_test(req, NULL);
#endif  /* HAVE_UCP_REQUEST_CHECK_STATUS */
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
        epm.address = (ucp_address_t *) proc.comms.xchg_wrkr_info[i].buf;

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

    read_environment();

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

#if 0
    dump();
#endif

    /* don't need config info any more */
    ucp_config_release(proc.comms.cfg);

    shmemc_globalexit_init();
}

void
shmemc_ucx_finalize(void)
{
    shmemc_globalexit_finalize();

    disconnect_all_eps();
    deallocate_endpoints();

    if (proc.comms.wrkr) {
        ucp_worker_release_address(proc.comms.wrkr,
                                   proc.comms.xchg_wrkr_info[proc.rank].addr);
        ucp_worker_destroy(proc.comms.wrkr);
    }
    deallocate_workers();

    dereg_globals();
    dereg_symmetric_heap();

    ucp_cleanup(proc.comms.ctxt);
}
