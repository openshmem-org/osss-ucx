#include "thispe.h"
#include "shmemu.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <pmix.h>
#include <ucp/api/ucp.h>

/*
 * if finalize called through atexit, force a barrier
 */

static void
pmix_finalize_handler(_Bool need_barrier)
{
    pmix_status_t ps;
    pmix_info_t *bar;

    if (need_barrier) {
        logger(LOG_FINALIZE,
               "PMIx adding internal barrier to finalize");
    }

    PMIX_INFO_CREATE(bar, 1);
    PMIX_INFO_LOAD(bar, PMIX_EMBED_BARRIER, &need_barrier, PMIX_BOOL);

    ps = PMIx_Finalize(bar, 1);
    assert(ps == PMIX_SUCCESS);

    PMIX_INFO_FREE(bar, 1);
}

static void
pmix_finalize_atexit(void)
{
    const _Bool needbar = (proc.status == SHMEM_PE_RUNNING);

    pmix_finalize_handler(needbar);
}

/*
 * formats are <pe>:heapx:<key>
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

    /*
     * everyone publishes their info
     */
    snprintf(ia[0].key, PMIX_MAX_KEYLEN, heap_base_fmt, proc.rank);
    ia[0].value.type = PMIX_UINT64;
    ia[0].value.data.uint64 = (uint64_t) proc.heaps[proc.rank].base;

    snprintf(ia[1].key, PMIX_MAX_KEYLEN, heap_size_fmt, proc.rank);
    ia[1].value.type = PMIX_SIZE;
    ia[1].value.data.size = proc.heaps[proc.rank].size;

    ps = PMIx_Publish(ia, nfields);
    assert(ps == PMIX_SUCCESS);

#if 0
    logger(LOG_HEAP, "PUBLISH: my heap @ %p, %lu bytes",
           proc.heaps[proc.rank].base,
           proc.heaps[proc.rank].size);
#endif
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

        proc.heaps[pe].base =
            (void *) fetch_base.value.data.uint64;
        proc.heaps[pe].size =
            fetch_size.value.data.size;
    }

#if 0
    /* debugging validation */
    for (pe = 0; pe < proc.nranks; pe += 1) {
        if (pe != proc.rank) {
            logger(LOG_HEAP, "FETCH: from PE %d, heap @ %p, %lu bytes",
                   pe,
                   proc.heaps[proc.rank].base,
                   proc.heaps[proc.rank].size);
        }
    }
#endif
}

/*
 * formats are <pe>:wrkr:addr, <pe>:wrkr:len
 */
static const char *wrkr_addr_fmt = "%d:wrkr:addr";
static const char *wrkr_len_fmt = "%d:wrkr:len";

void
shmemc_pmix_publish_worker(void)
{
    const unsigned int nfields = 2;
    pmix_info_t *ia;
    pmix_status_t ps;

    PMIX_INFO_CREATE(ia, nfields); /* base, size */

    /*
     * everyone publishes their info
     */
    snprintf(ia[0].key, PMIX_MAX_KEYLEN, wrkr_addr_fmt, proc.rank);
    ia[0].value.type = PMIX_UINT64;
    ia[0].value.data.uint64 = (uint64_t) proc.comms.wrkrs[proc.rank].addr;

    snprintf(ia[1].key, PMIX_MAX_KEYLEN, wrkr_len_fmt, proc.rank);
    ia[1].value.type = PMIX_SIZE;
    ia[1].value.data.size = proc.comms.wrkrs[proc.rank].len;

    ps = PMIx_Publish(ia, nfields);
    assert(ps == PMIX_SUCCESS);

    PMIX_INFO_FREE(ia, nfields);

    logger(LOG_WORKER, "PUBLISH: worker @ %p, %lu bytes",
           proc.comms.wrkrs[proc.rank].addr,
           proc.comms.wrkrs[proc.rank].len);
}

void
shmemc_pmix_exchange_workers(void)
{
    pmix_status_t ps;
    pmix_pdata_t fetch_addr;
    pmix_pdata_t fetch_len;
    pmix_info_t waiter;
    int all = 1;
    int pe;

    PMIX_INFO_CONSTRUCT(&waiter);
    PMIX_INFO_LOAD(&waiter, PMIX_WAIT, &all, PMIX_INT32);

    PMIX_PDATA_CONSTRUCT(&fetch_addr);
    PMIX_PDATA_CONSTRUCT(&fetch_len);

    for (pe = 0; pe < proc.nranks; pe += 1) {
        const int i = (pe + proc.rank) % proc.nranks;

        /* can I merge these?  No luck so far */
        snprintf(fetch_addr.key, PMIX_MAX_KEYLEN, wrkr_addr_fmt, i);
        snprintf(fetch_len.key, PMIX_MAX_KEYLEN, wrkr_len_fmt, i);

        ps = PMIx_Lookup(&fetch_addr, 1, &waiter, 1);
        assert(ps == PMIX_SUCCESS);

        ps = PMIx_Lookup(&fetch_len, 1, &waiter, 1);
        assert(ps == PMIX_SUCCESS);

        proc.comms.wrkrs[i].addr =
            (ucp_address_t *) fetch_addr.value.data.uint64;
        proc.comms.wrkrs[i].len =
            fetch_len.value.data.size;
    }

    /* debugging validation */
    for (pe = 0; pe < proc.nranks; pe += 1) {
        logger(LOG_WORKER, "FETCH: from PE %d, worker @ %p, %lu bytes",
               pe,
               proc.comms.wrkrs[pe].addr,
               proc.comms.wrkrs[pe].len);
    }
}

/* -------------------------------------------------------------- */

/*
 * this is purely for internal use with PMIx,
 * nothing to do with SHMEM/UCX
 */
void
shmemc_pmix_barrier_all(void)
{
    PMIx_Fence(NULL, 0, NULL, 0);
}

void
shmemc_pmix_client_finalize(void)
{
    pmix_finalize_handler(false);
}

void
shmemc_pmix_client_init(void)
{
    pmix_proc_t my_proc;        /* about me */
    pmix_proc_t wc_proc;        /* wildcard lookups */
    pmix_value_t v;
    pmix_value_t *vp = &v;
    pmix_status_t ps;

    ps = PMIx_Init(&my_proc, NULL, 0);
    assert(ps == PMIX_SUCCESS);

    /*
     * we can get our rank immediately
     */
    proc.rank = (int) my_proc.rank;
    assert(proc.rank >= 0);

    /*
     * make a new proc to query things not linked to a specific rank
     */
    PMIX_PROC_CONSTRUCT(&wc_proc);
    strncpy(wc_proc.nspace, my_proc.nspace, PMIX_MAX_NSLEN);
    wc_proc.rank = PMIX_RANK_WILDCARD;

    ps = PMIx_Get(&wc_proc, PMIX_JOB_SIZE, NULL, 0, &vp);
    assert(ps == PMIX_SUCCESS);

    /*
     * this is the program size / number of ranks/PEs
     */
    proc.nranks = (int) vp->data.uint32;

    /*
     * is the world a sane size?
     */
    assert(proc.nranks > 0);
    assert(proc.rank < proc.nranks);

    /*
     * what's on this node?
     */
    ps = PMIx_Get(&wc_proc, PMIX_LOCAL_SIZE, NULL, 0, &vp);
    assert(ps == PMIX_SUCCESS);

    proc.npeers = (int) vp->data.uint32;
    assert(proc.npeers >= 0);

    ps = PMIx_Get(&wc_proc, PMIX_LOCAL_PEERS, NULL, 0, &vp);
    assert(ps == PMIX_SUCCESS);

    proc.peers = strdup(vp->data.string);
    assert(proc.peers != NULL);

    PMIX_VALUE_RELEASE(vp);

#if 0
    logger(LOG_INIT,
           "there %s %d peer%s on this node: \"%s\"",
           (proc.npeers > 1) ? "are" : "is",
           proc.npeers,
           (proc.npeers > 1) ? "s" : "",
           proc.peers
           );
#endif
}
