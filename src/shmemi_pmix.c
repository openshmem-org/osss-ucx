#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>
#include <pmix.h>

#include "pe.h"
#include "logger.h"
#include "heapx.h"

/*
 * if finalize called through atexit, force a barrier
 */

static
void
shmemi_finalize_handler_pmix(bool need_barrier)
{
    if (p.running) {
        pmix_info_t *info;
        pmix_status_t ps;

        if (need_barrier) {
            logger(LOG_FINALIZE, "still alive, add barrier to finalize");
        }

        PMIX_INFO_CREATE(info, 1);
        PMIX_INFO_LOAD(info, PMIX_EMBED_BARRIER, &need_barrier, PMIX_BOOL);

        ps = PMIx_Finalize(info, 1);
        assert(ps == PMIX_SUCCESS);

        PMIX_INFO_FREE(info, 1);

        heapx_finalize();

        p.running = false;

        logger(LOG_FINALIZE, "shut down complete");
    }
}

static
void
shmemi_finalize_atexit_pmix(void)
{
    shmemi_finalize_handler_pmix(p.running);
}

void
shmemi_finalize_pmix(void)
{
    shmemi_finalize_handler_pmix(false);
}

/*
 * this is purely for internal use with PMIx,
 * nothing to do with SHMEM/UCX
 */
static
inline
void
barrier_all_pmix(void)
{
    PMIx_Fence(NULL, 0, NULL, 0);
}

static const char *base_fmt = "heap:base:%d";
static const char *size_fmt = "heap:size:%d";

static
void
publish_heap_info(void)
{
    pmix_info_t *ia;
    pmix_status_t ps;

    PMIX_INFO_CREATE(ia, 2);    /* base, size */

    /*
     * everyone publishes their info
     */
    snprintf(ia[0].key, PMIX_MAX_KEYLEN, base_fmt, p.me);
    ia[0].value.type = PMIX_SIZE;
    ia[0].value.data.size = (size_t) heapx[p.me].base;

    snprintf(ia[1].key, PMIX_MAX_KEYLEN, size_fmt, p.me);
    ia[1].value.type = PMIX_SIZE;
    ia[1].value.data.size = heapx[p.me].size;

    ps = PMIx_Publish(ia, 2);
     assert(ps == PMIX_SUCCESS);

    logger(LOG_INIT, "PUBLISH: my heap @ %p, %lu bytes",
           heapx[p.me].base, heapx[p.me].size);

    PMIX_INFO_FREE(ia, 2);
}

static
void
exchange_heap_info(void)
{
    pmix_status_t ps;
    pmix_pdata_t *fetch;
    pmix_info_t *waiter;
    int all = 0;
    int i;

    PMIX_INFO_CREATE(waiter, 1);
    PMIX_INFO_LOAD(waiter, PMIX_WAIT, &all, PMIX_INT);

    PMIX_PDATA_CREATE(fetch, 2); /* base, size */

    for (i = 0; i < p.npes; i += 1) {
        if (i != p.me) {

            snprintf(fetch[0].key, PMIX_MAX_KEYLEN, base_fmt, i);
            snprintf(fetch[1].key, PMIX_MAX_KEYLEN, size_fmt, i);

            /* TODO can we merge these lookups?  No luck so far */

#if 1
            ps = PMIx_Lookup(&fetch[0], 1, waiter, 1);
            assert(ps == PMIX_SUCCESS);

            ps = PMIx_Lookup(&fetch[1], 1, waiter, 1);
            assert(ps == PMIX_SUCCESS);
#else
            ps = PMIx_Lookup(fetch, 2, waiter, 1);
            assert(ps == PMIX_SUCCESS);
#endif

            heapx_set_pe(i,
                         (void *) fetch[0].value.data.size,
                         fetch[1].value.data.size
                         );

        }
    }

    PMIX_PDATA_FREE(fetch, 2);
    PMIX_INFO_FREE(waiter, 1);

    for (i = 0; i < p.npes; i += 1) {
        logger(LOG_INIT, "FETCH: heap %d @ %p, %lu bytes",
               i, heapx[i].base, heapx[i].size);
    }
}

void
shmemi_setup_heaps_pmix(void)
{
    /*
     * defer until needed:
     */

    publish_heap_info();

    exchange_heap_info();
}

void
shmemi_init_pmix(void)
{
    pmix_proc_t my_proc;        /* about me */
    pmix_proc_t wc_proc;        /* wildcard lookups */
    pmix_value_t v;
    pmix_value_t *vp = &v;
    pmix_status_t ps;
    int s;

    ps = PMIx_Init(&my_proc, NULL, 0);
    assert(ps == PMIX_SUCCESS);

    /*
     * we can get our rank immediately
     */
    ps = PMIx_Get(&my_proc, PMIX_RANK, NULL, 0, &vp);
    assert(ps == PMIX_SUCCESS);

    p.me = (int) vp->data.rank;

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
    p.npes = (int) vp->data.uint32;

    /*
     * is the world a sane size?
     */
    assert(p.npes > 0);
    assert(p.me >= 0);
    assert(p.me < p.npes);

    ps = PMIx_Get(&wc_proc, PMIX_LOCAL_SIZE, NULL, 0, &vp);
    assert(ps == PMIX_SUCCESS);

    s = atexit(shmemi_finalize_atexit_pmix);
    assert(s == 0);

    barrier_all_pmix();

    p.running = true;
}

int
shmemi_my_pe_pmix(void)
{
    return p.me;
}

int
shmemi_n_pes_pmix(void)
{
    return p.npes;
}
