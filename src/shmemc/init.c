#include "thispe.h"
#include "shmemc.h"

// #include <unistd.h>
#include <stdlib.h>             /* getenv */
// #include <string.h>
#include <assert.h>

#include <ucp/api/ucp.h>

#define KB 1024L
#define MB (KB * KB)
#define GB (KB * MB)

/*
 * the PE's state
 */
thispe_info_t proc = {
    .status = SHMEM_PE_UNKNOWN,
    .refcount = 0
};

/*
 * where globals/statics/commons/saves live
 *
 */
static ucp_mem_h global_segment;

/*
 * some memory to play with registering
 *
 * TODO: multiple heap support
 */
static ucp_mem_h symm_heap;

/*
 * debugging output stream
 */
static FILE *say;

/*
 * private shortcut to communications structure
 */
static comms_info_t *cp = & proc.comms;

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
make_worker(void)
{
    ucs_status_t s;
    ucp_worker_params_t wkpm;

    wkpm.field_mask  = UCP_WORKER_PARAM_FIELD_THREAD_MODE;
    wkpm.thread_mode = UCS_THREAD_MODE_SINGLE;

    s = ucp_worker_create(cp->ctxt, &wkpm, &cp->wrkr);
    assert(s == UCS_OK);
}

static void
make_init_params(ucp_params_t *p_p)
{
    p_p->field_mask =
        UCP_PARAM_FIELD_FEATURES |
        UCP_PARAM_FIELD_ESTIMATED_NUM_EPS;

    p_p->estimated_num_eps = proc.nranks;

    /*
     * we'll try at first to get both 32- and 64-bit direct AMO
     * support because OpenSHMEM wants them
     */
    p_p->features =
        UCP_FEATURE_WAKEUP |
        UCP_FEATURE_RMA
#if 1
    /* while testing so we can play on mlx4 card */
    |
        UCP_FEATURE_AMO32 |
        UCP_FEATURE_AMO64
#endif
        ;
}

/*
 * create remote endpoint table
 */
static void
make_remote_endpoints(void)
{
    cp->eps = (ucp_ep_h *) calloc(proc.nranks, sizeof(*cp->eps));
    assert(cp->eps != NULL);
}

static void
make_local_endpoint(void)
{
    ucs_status_t s;
    ucp_ep_params_t epm;

    epm.field_mask = UCP_EP_PARAM_FIELD_REMOTE_ADDRESS;
    epm.address = proc.comms.addr_p;

    s = ucp_ep_create(cp->wrkr, &epm, &cp->eps[proc.rank]);
    assert(s == UCS_OK);
}

/*
 * private helper
 */
inline static void
do_flush(void)
{
    ucs_status_t s;

    s = ucp_worker_flush(cp->wrkr);
    assert(s == UCS_OK);
}

static void
dump_mapped_mem_info(const char *name, const ucp_mem_h *m)
{
    ucs_status_t s;
    ucp_mem_attr_t attr;

    /* the attributes we want to inspect */
    attr.field_mask =
        UCP_MEM_ATTR_FIELD_ADDRESS |
        UCP_MEM_ATTR_FIELD_LENGTH;

    s = ucp_mem_query(*m, &attr);
    assert(s == UCS_OK);

    fprintf(say,
            "%d: \"%s\" memory at %p, length %.2f MB (%lu bytes)\n",
            proc.rank, name,
            attr.address,
            (double) attr.length / MB,
            (unsigned long) attr.length
            );
}

extern int shmemu_parse_size(char *size_str, size_t *bytes_p);

static void
reg_symmetric_heap(void)
{
    ucs_status_t s;
    ucp_mem_map_params_t mp;
    char *hs_str = shmemc_getenv("SHMEM_SYMMETRIC_HEAP_SIZE");
    size_t len;

    if (hs_str != NULL) {
        const int r = shmemu_parse_size(hs_str, &len);
        assert(r == 0);
    }
    else {
        len = 4 * MB; /* just some usable value */
    }

    mp.field_mask =
        UCP_MEM_MAP_PARAM_FIELD_LENGTH |
        UCP_MEM_MAP_PARAM_FIELD_FLAGS;

    mp.length = len;
    mp.flags =
        UCP_MEM_MAP_ALLOCATE;

    s = ucp_mem_map(cp->ctxt, &mp, &symm_heap);
    assert(s == UCS_OK);
}

extern char data_start;
extern char end;

static void
reg_globals(void)
{
    ucs_status_t s;
    ucp_mem_map_params_t mp;
    void *base = &data_start;
    const size_t len = (size_t) &end - (size_t) base;

    mp.field_mask =
        UCP_MEM_MAP_PARAM_FIELD_ADDRESS |
        UCP_MEM_MAP_PARAM_FIELD_LENGTH;

    mp.address = base;
    mp.length = len;

    s = ucp_mem_map(cp->ctxt, &mp, &global_segment);
    assert(s == UCS_OK);
}

/**
 * API
 *
 **/

void
shmemc_quiet(void)
{
    do_flush();
}

void
shmemc_init(void)
{
    ucs_status_t s;
    ucp_params_t pm;

    say = stderr;

    proc.refcount += 1;

    /* no re-init */
    if (proc.status == SHMEM_PE_RUNNING) {
        return;
    }

    /* start initialization */
    s = ucp_config_read(NULL, NULL, &cp->cfg);
    assert(s == UCS_OK);

    make_init_params(&pm);

    s = ucp_init(&pm, cp->cfg, &cp->ctxt);
    assert(s == UCS_OK);

    make_worker();

    /* get address for remote access to worker */
    s = ucp_worker_get_address(cp->wrkr, &cp->addr_p, &cp->addr_len);
    assert(s == UCS_OK);

    /*
     * try registering some implicit memory as symmetric heap
     *
     */
    reg_symmetric_heap();

    /*
     * try registering the data and bss segments
     *
     */
    reg_globals();

    make_local_endpoint();
    make_remote_endpoints();

#ifdef DEBUG
    if (proc.rank == 0) {
        const ucs_config_print_flags_t flags =
            UCS_CONFIG_PRINT_CONFIG |
            UCS_CONFIG_PRINT_HEADER;

        ucp_config_print(cp->cfg, say, "My config", flags);
        ucp_context_print_info(cp->ctxt, say);
        check_version();
        fprintf(say, "----------------------------------------------\n\n");
        fflush(say);
    }
    ucp_worker_print_info(cp->wrkr, say);
    dump_mapped_mem_info("heap", &symm_heap);
    dump_mapped_mem_info("globals", &global_segment);
#endif
    /* don't need config info any more */
    ucp_config_release(cp->cfg);

    proc.status = SHMEM_PE_RUNNING;
}

void
shmemc_finalize(void)
{
    ucs_status_t s;

    proc.refcount -= 1;

    if (proc.status != SHMEM_PE_RUNNING) {
        return;
    }

    /* really want a global barrier */
    shmemc_quiet();

    /* and clean up */

    ucp_disconnect_nb(cp->eps[proc.rank]);

    free(cp->eps);

    ucp_worker_release_address(cp->wrkr, cp->addr_p);

    ucp_worker_destroy(cp->wrkr);

    s = ucp_mem_unmap(cp->ctxt, symm_heap);
    assert(s == UCS_OK);
    s = ucp_mem_unmap(cp->ctxt, global_segment);
    assert(s == UCS_OK);

    ucp_cleanup(cp->ctxt);

    proc.status = SHMEM_PE_SHUTDOWN;
}
