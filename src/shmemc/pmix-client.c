#include "thispe.h"
#include "shmemu.h"
#include "state.h"
#include "heapx.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <pmix.h>
#include <pmix/pmix_common.h>
#include <ucp/api/ucp.h>

inline static void
pmix_finalize_handler(void)
{
    pmix_status_t ps;
    int pe;

    ps = PMIx_Finalize();
    assert(ps == PMIX_SUCCESS);

    for (pe = 0; pe < proc.nranks; pe += 1) {
        if (proc.comms.wrkrs[pe].buf != NULL) {
            free(proc.comms.wrkrs[pe].buf);
        }
    }

    /* clean up memory recording peer PEs */
    if (proc.peers != NULL) {
        free(proc.peers);
    }
}

static void
pmix_finalize_atexit(void)
{
    pmix_finalize_handler();
}

/*
 * read out the peer PE numbers
 */
static void
parse_peers(char *peerstr)
{
    int i = 0;
    char *next;
    const char *sep = ",";

    /* parse the PE #s out of the string */
    proc.peers = (int *) calloc(proc.npeers, sizeof(*proc.peers));
    assert(proc.peers != NULL);

    next = strtok(peerstr, sep);
    while (next != NULL) {
        proc.peers[i] = atoi(next);
        i += 1;
        next = strtok(NULL, sep);
    }
}

/*
 * formats are <pe>:heap:<key>
 */
static const char *heap_base_fmt = "%d:heap:base";
static const char *heap_size_fmt = "%d:heap:size";

void
shmemc_pmix_publish_heap_info(void)
{
    const unsigned int nfields = 2;
    pmix_info_t *ia;
    pmix_status_t ps;

    PMIX_INFO_CREATE(ia, nfields);    /* base, size */

    /* everyone publishes their info */
    snprintf(ia[0].key, PMIX_MAX_KEYLEN, heap_base_fmt, proc.rank);
    ia[0].value.type = PMIX_UINT64;
    ia[0].value.data.uint64 = (uint64_t) proc.comms.heaps[proc.rank].base;

    snprintf(ia[1].key, PMIX_MAX_KEYLEN, heap_size_fmt, proc.rank);
    ia[1].value.type = PMIX_SIZE;
    ia[1].value.data.size = proc.comms.heaps[proc.rank].length;

    ps = PMIx_Publish(ia, nfields);
    assert(ps == PMIX_SUCCESS);

    PMIX_INFO_FREE(ia, nfields);
}

void
shmemc_pmix_exchange_heap_info(void)
{
    pmix_status_t ps;
    pmix_pdata_t fetch_base;
    pmix_pdata_t fetch_size;
    pmix_info_t waiter;
    int all = 0;
    int pe;

    PMIX_INFO_CONSTRUCT(&waiter);
    PMIX_INFO_LOAD(&waiter, PMIX_WAIT, &all, PMIX_INT32);

    PMIX_PDATA_CONSTRUCT(&fetch_base);
    PMIX_PDATA_CONSTRUCT(&fetch_size);

    for (pe = 0; pe < proc.nranks; pe += 1) {
        /* can I merge these?  No luck so far */
        snprintf(fetch_base.key, PMIX_MAX_KEYLEN, heap_base_fmt, pe);
        snprintf(fetch_size.key, PMIX_MAX_KEYLEN, heap_size_fmt, pe);

        ps = PMIx_Lookup(&fetch_base, 1, &waiter, 1);
        assert(ps == PMIX_SUCCESS);

        ps = PMIx_Lookup(&fetch_size, 1, &waiter, 1);
        assert(ps == PMIX_SUCCESS);

        proc.comms.heaps[pe].base =
            (void *) fetch_base.value.data.uint64;
        proc.comms.heaps[pe].length =
            fetch_size.value.data.size;
    }
}

static const char *wrkr_exch_fmt = "%d:wrkr:addr";

void
shmemc_pmix_publish_worker(void)
{
    pmix_status_t ps;
    pmix_info_t pi;
    pmix_byte_object_t *bop;

    PMIX_INFO_CONSTRUCT(&pi);

    /* everyone publishes their info */
    snprintf(pi.key, PMIX_MAX_KEYLEN, wrkr_exch_fmt, proc.rank);
    pi.value.type = PMIX_BYTE_OBJECT;
    bop = &pi.value.data.bo;
    bop->bytes = (char *) proc.comms.wrkrs[proc.rank].addr;
    bop->size = proc.comms.wrkrs[proc.rank].len;
    ps = PMIx_Publish(&pi, 1);
    assert(ps == PMIX_SUCCESS);
}

void
shmemc_pmix_exchange_workers(void)
{
    pmix_status_t ps;
    pmix_pdata_t fetch;
    pmix_byte_object_t *bop;
    pmix_info_t waiter;
    int all = 1;
    int pe;

    PMIX_INFO_CONSTRUCT(&waiter);
    PMIX_INFO_LOAD(&waiter, PMIX_WAIT, &all, PMIX_INT32);

    PMIX_PDATA_CONSTRUCT(&fetch);

    for (pe = 0; pe < proc.nranks; pe += 1) {
        const int i = (pe + proc.rank) % proc.nranks;

        snprintf(fetch.key, PMIX_MAX_KEYLEN, wrkr_exch_fmt, i);
        ps = PMIx_Lookup(&fetch, 1, &waiter, 1);
        assert(ps == PMIX_SUCCESS);
        bop = &fetch.value.data.bo;

        /* save published worker */
        proc.comms.wrkrs[i].buf = (char *) malloc(bop->size);
        assert(proc.comms.wrkrs[i].buf != NULL);
        memcpy(proc.comms.wrkrs[i].buf, bop->bytes, bop->size);
    }
}

static void *global_packed_rkey;
static size_t global_rkey_len;

static void *symm_packed_rkey;
static size_t symm_rkey_len;

static const char *rkey_exch_fmt = "%d:rkey:%s";

void
shmemc_pmix_publish_rkey(void)
{
    pmix_status_t ps;
    pmix_info_t pi;
    pmix_byte_object_t *bop;
    ucs_status_t s;

    s = ucp_rkey_pack(proc.comms.ctxt,
                      global_segment.mh,
                      &global_packed_rkey, &global_rkey_len
                      );
    assert(s == UCS_OK);

    s = ucp_rkey_pack(proc.comms.ctxt,
                      symm_segment.mh,
                      &symm_packed_rkey, &symm_rkey_len
                      );
    assert(s == UCS_OK);

    PMIX_INFO_CONSTRUCT(&pi);
    snprintf(pi.key, PMIX_MAX_KEYLEN, rkey_exch_fmt, proc.rank, "global");
    pi.value.type = PMIX_BYTE_OBJECT;
    bop = &pi.value.data.bo;
    bop->bytes = (char *) global_packed_rkey;
    bop->size = global_rkey_len;
    ps = PMIx_Publish(&pi, 1);
    assert(ps == PMIX_SUCCESS);

    PMIX_INFO_CONSTRUCT(&pi);
    snprintf(pi.key, PMIX_MAX_KEYLEN, rkey_exch_fmt, proc.rank, "symm");
    pi.value.type = PMIX_BYTE_OBJECT;
    bop = &pi.value.data.bo;
    bop->bytes = (char *) symm_packed_rkey;
    bop->size = symm_rkey_len;
    ps = PMIx_Publish(&pi, 1);
    assert(ps == PMIX_SUCCESS);
}

void
shmemc_pmix_exchange_rkeys(void)
{
    pmix_status_t ps;
    pmix_pdata_t fetch;
    pmix_byte_object_t *bop;
    pmix_info_t waiter;
    int all = 1;
    int pe;
    ucs_status_t s;

    PMIX_INFO_CONSTRUCT(&waiter);
    PMIX_INFO_LOAD(&waiter, PMIX_WAIT, &all, PMIX_INT32);

    PMIX_PDATA_CONSTRUCT(&fetch);

    for (pe = 0; pe < proc.nranks; pe += 1) {
        const int i = (pe + proc.rank) % proc.nranks;

        snprintf(fetch.key, PMIX_MAX_KEYLEN, rkey_exch_fmt, i, "global");

        ps = PMIx_Lookup(&fetch, 1, &waiter, 1);
        assert(ps == PMIX_SUCCESS);
        bop = &fetch.value.data.bo; /* shortcut */
        global_segment.rkeys[i] = (ucp_rkey_h) malloc(bop->size);
        assert(global_segment.rkeys[i] != NULL);

        s = ucp_ep_rkey_unpack(proc.comms.eps[i],
                               bop->bytes,
                               &global_segment.rkeys[i]
                               );
        assert(s == UCS_OK);

        snprintf(fetch.key, PMIX_MAX_KEYLEN, rkey_exch_fmt, i, "symm");

        ps = PMIx_Lookup(&fetch, 1, &waiter, 1);
        assert(ps == PMIX_SUCCESS);
        bop = &fetch.value.data.bo;
        symm_segment.rkeys[i] = (ucp_rkey_h) malloc(bop->size);
        assert(symm_segment.rkeys[i] != NULL);

        s = ucp_ep_rkey_unpack(proc.comms.eps[i],
                               bop->bytes,
                               &symm_segment.rkeys[i]
                               );
        assert(s == UCS_OK);
    }
}

/* -------------------------------------------------------------- */

/*
 * this barrier is purely for internal use with PMIx, nothing to do
 * with SHMEM/UCX
 */
void
shmemc_pmix_barrier_all(void)
{
    PMIx_Fence(NULL, 0, NULL, 0);
}

void
shmemc_pmix_client_finalize(void)
{
    pmix_finalize_handler();
}

void
shmemc_pmix_client_init(void)
{
    pmix_proc_t my_proc;        /* about me */
    pmix_proc_t wc_proc;        /* wildcard lookups */
    pmix_value_t v;
    pmix_value_t *vp = &v;      /* holds things we get from PMIx */
    pmix_status_t ps;

    ps = PMIx_Init(&my_proc);
    assert(ps == PMIX_SUCCESS);

    /* we can get our own rank immediately */
    proc.rank = (int) my_proc.rank;
    assert(proc.rank >= 0);

    /* make a new proc to query things not linked to a specific rank */
    PMIX_PROC_CONSTRUCT(&wc_proc);
    strncpy(wc_proc.nspace, my_proc.nspace, PMIX_MAX_NSLEN);
    wc_proc.rank = PMIX_RANK_WILDCARD;

    ps = PMIx_Get(&wc_proc, PMIX_JOB_SIZE, NULL, 0, &vp);
    assert(ps == PMIX_SUCCESS);

    /* this is the program size / number of ranks/PEs */
    proc.nranks = (int) vp->data.uint32;

    /* is the world a sane size? */
    assert(proc.nranks > 0);
    assert(proc.rank < proc.nranks);

    /* what's on this node? */
    ps = PMIx_Get(&wc_proc, PMIX_LOCAL_SIZE, NULL, 0, &vp);
    assert(ps == PMIX_SUCCESS);

    proc.npeers = (int) vp->data.uint32;
    assert(proc.npeers >= 0);

    ps = PMIx_Get(&wc_proc, PMIX_LOCAL_PEERS, NULL, 0, &vp);
    assert(ps == PMIX_SUCCESS);

    parse_peers(vp->data.string);

    /* and done */
    PMIX_VALUE_RELEASE(vp);
}
