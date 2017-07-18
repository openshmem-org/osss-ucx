#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <pmix.h>

#include "thispe.h"

#include "shmemu.h"

/*
 * if finalize called through atexit, force a barrier
 */

static void
pmix_finalize_handler(_Bool need_barrier)
{
    if (need_barrier) {
        pmix_info_t *bar;
        pmix_status_t ps;

        if (need_barrier) {
            logger(LOG_FINALIZE,
                   "PMIx adding intenral barrier to finalize");
        }

        PMIX_INFO_CREATE(bar, 1);
        PMIX_INFO_LOAD(bar, PMIX_EMBED_BARRIER, &need_barrier, PMIX_BOOL);
        ps = PMIx_Finalize(bar, 1);

        assert(ps == PMIX_SUCCESS);

        PMIX_INFO_FREE(bar, 1);

    }
}

static void
pmix_finalize_atexit(void)
{
    pmix_finalize_handler(proc.status == SHMEM_PE_RUNNING);
}

/*
 * this is purely for internal use with PMIx,
 * nothing to do with SHMEM/UCX
 */
static void
pmix_barrier_all(void)
{
    PMIx_Fence(NULL, 0, NULL, 0);
}

/*
 * formats are <pe>:heapx:<heap-index>:<key>
 */
static const char *base_fmt = "%d:heapx:%d:base";
static const char *size_fmt = "%d:heapx:%d:size";

static void
publish_heap_info(void)
{
    /* only publish if multiple PEs */

    if (proc.nranks > 1) {
        pmix_info_t *ia;
        pmix_status_t ps;
        int i;

        PMIX_INFO_CREATE(ia, 2);    /* base, size */

        /*
         * everyone publishes their info
         */
        for (i = 0; i < proc.nheaps; i += 1) {
            snprintf(ia[0].key, PMIX_MAX_KEYLEN, base_fmt, proc.rank, i);
            ia[0].value.type = PMIX_UINT64;
            ia[0].value.data.uint64 = (uint64_t) proc.heaps[proc.rank][i]->base;

            snprintf(ia[1].key, PMIX_MAX_KEYLEN, size_fmt, proc.rank, i);
            ia[1].value.type = PMIX_SIZE;
            ia[1].value.data.size = proc.heaps[proc.rank][i]->size;

            ps = PMIx_Publish(ia, 2);
            assert(ps == PMIX_SUCCESS);

            logger(LOG_HEAP, "PUBLISH: my heap #%d @ %p, %lu bytes",
                   i,
                   proc.heaps[proc.rank][i]->base,
                   proc.heaps[proc.rank][i]->size);
        }

        PMIX_INFO_FREE(ia, 2);
    }
}

static void
exchange_heap_info(void)
{
    pmix_status_t ps;
    pmix_pdata_t fetch_base;
    pmix_pdata_t fetch_size;
    pmix_info_t waiter;
    int all = 0;
    int pn;
    int i;

    PMIX_INFO_CONSTRUCT(&waiter);
    PMIX_INFO_LOAD(&waiter, PMIX_WAIT, &all, PMIX_INT32);

    PMIX_PDATA_CONSTRUCT(&fetch_base);
    PMIX_PDATA_CONSTRUCT(&fetch_size);

    for (pn = 0; pn < proc.nranks; pn += 1) {
        if (pn != proc.rank) {
            for (i = 0; i < proc.nheaps; i += 1) {

                /* can I merge these?  No luck so far */
                snprintf(fetch_base.key, PMIX_MAX_KEYLEN, base_fmt, pn, i);
                snprintf(fetch_size.key, PMIX_MAX_KEYLEN, size_fmt, pn, i);

                ps = PMIx_Lookup(&fetch_base, 1, &waiter, 1);
                assert(ps == PMIX_SUCCESS);

                ps = PMIx_Lookup(&fetch_size, 1, &waiter, 1);
                assert(ps == PMIX_SUCCESS);

                proc.heaps[pn][i]->base = (void *) fetch_base.value.data.uint64;
                proc.heaps[pn][i]->size = fetch_size.value.data.size;
            }
        }
    }

    for (pn = 0; pn < proc.nranks; pn += 1) {
        if (pn != proc.rank) {
            for (i = 0; i < proc.nheaps; i += 1) {
                logger(LOG_HEAP, "FETCH: from PE %d, heap #%d @ %p, %lu bytes",
                       pn, i,
                       proc.heaps[proc.rank][i]->base,
                       proc.heaps[proc.rank][i]->size);
            }
        }
    }
}

/* -------------------------------------------------------------- */

void
pmix_client_finalize(void)
{
    pmix_finalize_handler(false);
}

void
pmix_client_init(void)
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

    logger(LOG_INIT,
           "there %s %d peer%s on this node: \"%s\"",
           (proc.npeers > 1) ? "are" : "is",
           proc.npeers,
           (proc.npeers > 1) ? "s" : "",
           proc.peers
           );

    PMIX_VALUE_RELEASE(vp);

    pmix_barrier_all();
}
