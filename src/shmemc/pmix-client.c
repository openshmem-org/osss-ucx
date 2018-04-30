/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "thispe.h"
#include "shmemu.h"
#include "state.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <pmix.h>

#include <ucp/api/ucp.h>

/*
 * read out the peer PE numbers
 */
inline static void
parse_peers(char *peerstr)
{
    int i = 0;
    char *next;
    const char *sep = ",";

    /* parse the PE #s out of the string */
    proc.peers = (int *) calloc(proc.npeers, sizeof(*proc.peers));
    shmemu_assert(proc.peers != NULL, "can't allocate memory for peer list");

    next = strtok(peerstr, sep);
    while (next != NULL) {
        proc.peers[i] = (int) strtol(next, NULL, 10);
        i += 1;
        next = strtok(NULL, sep);
    }
}

#ifndef ENABLE_ALIGNED_ADDRESSES

/*
 * formats are <pe>:<region-index>:region:<key>
 */
static const char *region_base_fmt = "%d:%d:mr:base";
static const char *region_size_fmt = "%d:%d:mr:size";

void
shmemc_pmi_publish_heap_info(void)
{
    const unsigned int nfields = 2;
    pmix_info_t *ia;
    pmix_status_t ps;
    size_t r;

    PMIX_INFO_CREATE(ia, nfields);    /* base, size */

    /* everyone publishes their info (except for globals) */
    for (r = 1; r < proc.comms.nregions; r += 1) {
        snprintf(ia[0].key, PMIX_MAX_KEYLEN, region_base_fmt, proc.rank, r);
        ia[0].value.type = PMIX_UINT64;
        ia[0].value.data.uint64 =
            (uint64_t) proc.comms.regions[r].minfo[proc.rank].base;

        snprintf(ia[1].key, PMIX_MAX_KEYLEN, region_size_fmt, proc.rank, r);
        ia[1].value.type = PMIX_SIZE;
        ia[1].value.data.size = proc.comms.regions[r].minfo[proc.rank].len;

        ps = PMIx_Publish(ia, nfields);
        shmemu_assert( ps == PMIX_SUCCESS, "can't publish heap");
    }

    PMIX_INFO_FREE(ia, nfields);
}

void
shmemc_pmi_exchange_heap_info(void)
{
    pmix_status_t ps;
    pmix_pdata_t fetch_base;
    pmix_pdata_t fetch_size;
    pmix_info_t waiter;
    int all = 0;
    size_t r;
    int pe;

    PMIX_INFO_CONSTRUCT(&waiter);
    PMIX_INFO_LOAD(&waiter, PMIX_WAIT, &all, PMIX_INT32);

    PMIX_PDATA_CONSTRUCT(&fetch_base);
    PMIX_PDATA_CONSTRUCT(&fetch_size);

    /* exchange regions (except for globals) */
    for (r = 1; r < proc.comms.nregions; r += 1) {

        for (pe = 0; pe < proc.nranks; pe += 1) {
            /* can I merge these?  No luck so far */
            snprintf(fetch_base.key, PMIX_MAX_KEYLEN, region_base_fmt, pe, r);
            snprintf(fetch_size.key, PMIX_MAX_KEYLEN, region_size_fmt, pe, r);

            ps = PMIx_Lookup(&fetch_base, 1, &waiter, 1);
            shmemu_assert(ps == PMIX_SUCCESS, "can't fetch heap base");

            ps = PMIx_Lookup(&fetch_size, 1, &waiter, 1);
            shmemu_assert(ps == PMIX_SUCCESS, "can't fetch heap size");

            /* slightly redundant storage, but useful */
            proc.comms.regions[r].minfo[pe].base =
                fetch_base.value.data.uint64;
            proc.comms.regions[r].minfo[pe].len =
                fetch_size.value.data.size;
            proc.comms.regions[r].minfo[pe].end =
                proc.comms.regions[r].minfo[pe].base +
                fetch_size.value.data.size;
        }

    }
}
#endif /* ! ENABLE_ALIGNED_ADDRESSES */

static const char *wrkr_exch_fmt = "%d:wrkr:addr";

void
shmemc_pmi_publish_worker(void)
{
    pmix_status_t ps;
    pmix_info_t pi;
    pmix_byte_object_t *bop;    /* shortcut */

    PMIX_INFO_CONSTRUCT(&pi);

    /* everyone publishes their info */
    snprintf(pi.key, PMIX_MAX_KEYLEN, wrkr_exch_fmt, proc.rank);
    pi.value.type = PMIX_BYTE_OBJECT;
    bop = &pi.value.data.bo;
    bop->bytes = (char *) proc.comms.xchg_wrkr_info[proc.rank].addr;
    bop->size = proc.comms.xchg_wrkr_info[proc.rank].len;
    ps = PMIx_Publish(&pi, 1);
    shmemu_assert(ps == PMIX_SUCCESS, "can't publish worker blob");
}

void
shmemc_pmi_exchange_workers(void)
{
    pmix_status_t ps;
    pmix_pdata_t fetch;
    pmix_info_t waiter;
    int all = 1;
    int pe;

    PMIX_INFO_CONSTRUCT(&waiter);
    PMIX_INFO_LOAD(&waiter, PMIX_WAIT, &all, PMIX_INT32);

    PMIX_PDATA_CONSTRUCT(&fetch);

    for (pe = 0; pe < proc.nranks; pe += 1) {
        const pmix_byte_object_t *bop = &fetch.value.data.bo;
        const int i = (pe + proc.rank) % proc.nranks;

        snprintf(fetch.key, PMIX_MAX_KEYLEN, wrkr_exch_fmt, i);
        ps = PMIx_Lookup(&fetch, 1, &waiter, 1);
        shmemu_assert(ps == PMIX_SUCCESS, "can't find remote worker blob");

        /* save published worker */
        proc.comms.xchg_wrkr_info[i].buf = (char *) malloc(bop->size);
        shmemu_assert(proc.comms.xchg_wrkr_info[i].buf != NULL,
                      "can't allocate memory for remote workers");
        memcpy(proc.comms.xchg_wrkr_info[i].buf, bop->bytes, bop->size);
    }
}

/*
 * PE:rkey:HEAPNO
 */
static const char *rkey_exch_fmt = "%d:rkey:%d";

void
shmemc_pmi_publish_my_rkeys(void)
{
    pmix_status_t ps;
    pmix_info_t pi;
    void *packed_rkey;
    size_t rkey_len;
    size_t r;

    for (r = 0; r < proc.comms.nregions; r += 1) {
        pmix_byte_object_t *bop = &pi.value.data.bo;
        const ucs_status_t s =
            ucp_rkey_pack(proc.comms.ucx_ctxt,
                          proc.comms.regions[r].minfo[proc.rank].racc.mh,
                          &packed_rkey, &rkey_len
                          );
        shmemu_assert(s == UCS_OK, "can't unpack rkey");

        PMIX_INFO_CONSTRUCT(&pi);
        snprintf(pi.key, PMIX_MAX_KEYLEN, rkey_exch_fmt, proc.rank, r);
        pi.value.type = PMIX_BYTE_OBJECT;
        bop->bytes = (char *) packed_rkey;
        bop->size = rkey_len;
        ps = PMIx_Publish(&pi, 1);
        shmemu_assert(ps == PMIX_SUCCESS, "can't publish rkey");

        ucp_rkey_buffer_release(packed_rkey);
    }
}

void
shmemc_pmi_exchange_all_rkeys(void)
{
    pmix_status_t ps;
    pmix_pdata_t fetch;
    pmix_info_t waiter;
    int all = 1;
    int pe;
    size_t r;

    PMIX_INFO_CONSTRUCT(&waiter);
    PMIX_INFO_LOAD(&waiter, PMIX_WAIT, &all, PMIX_INT32);

    PMIX_PDATA_CONSTRUCT(&fetch);

    for (r = 0; r < proc.comms.nregions; r += 1) {

        for (pe = 0; pe < proc.nranks; pe += 1) {
            const pmix_byte_object_t *bop = &fetch.value.data.bo;
            const int i = (pe + proc.rank) % proc.nranks;
            ucs_status_t s;

            snprintf(fetch.key, PMIX_MAX_KEYLEN, rkey_exch_fmt, i, r);

            ps = PMIx_Lookup(&fetch, 1, &waiter, 1);
            shmemu_assert(ps == PMIX_SUCCESS, "can't fetch remote rkey");
            proc.comms.regions[r].minfo[i].racc.rkey =
                (ucp_rkey_h) malloc(bop->size);
            shmemu_assert(proc.comms.regions[r].minfo[i].racc.rkey != NULL,
                          "can't allocate memory for remote rkey");

            s = ucp_ep_rkey_unpack(proc.comms.eps[i],
                                   bop->bytes,
                                   &proc.comms.regions[r].minfo[i].racc.rkey
                                   );
            shmemu_assert(s == UCS_OK, "can't unpack remote rkey");
        }

    }
}

/* -------------------------------------------------------------- */

/*
 * this barrier is purely for internal use with PMIx, nothing to do
 * with SHMEM/UCX
 */
void
shmemc_pmi_barrier_all(void)
{
    PMIx_Fence(NULL, 0, NULL, 0);
}

void
shmemc_pmi_client_init(void)
{
    pmix_proc_t my_proc;        /* about me */
    pmix_proc_t wc_proc;        /* wildcard lookups */
    pmix_value_t v;
    pmix_value_t *vp = &v;      /* holds things we get from PMIx */
    pmix_status_t ps;

#ifdef HAVE_PMIX_NO_INIT_HINTS
    ps = PMIx_Init(&my_proc);
#else
    ps = PMIx_Init(&my_proc, NULL, 0);
#endif  /* HAVE_PMIX_NO_INIT_HINTS */

    shmemu_assert(ps == PMIX_SUCCESS, "can't initialize PMIx");

    /* we can get our own rank immediately */
    proc.rank = (int) my_proc.rank;
    shmemu_assert(proc.rank >= 0, "PMIx PE rank is not valid");

    /* make a new proc to query things not linked to a specific rank */
    PMIX_PROC_CONSTRUCT(&wc_proc);
    strncpy(wc_proc.nspace, my_proc.nspace, PMIX_MAX_NSLEN);
    wc_proc.rank = PMIX_RANK_WILDCARD;

    ps = PMIx_Get(&wc_proc, PMIX_JOB_SIZE, NULL, 0, &vp);
    shmemu_assert(ps == PMIX_SUCCESS, "PMIx can't get program size");

    /* this is the program size / number of ranks/PEs */
    proc.nranks = (int) vp->data.uint32;

    /* is the world a sane size? */
    shmemu_assert(proc.nranks > 0, "PMIx count of PE ranks is not valid");
    shmemu_assert(IS_VALID_PE_NUMBER(proc.rank), "PMIx PE rank is not valid");

    /* what's on this node? */
    ps = PMIx_Get(&wc_proc, PMIX_LOCAL_SIZE, NULL, 0, &vp);
    shmemu_assert(ps == PMIX_SUCCESS, "PMIx can't find PE's peers");

    proc.npeers = (int) vp->data.uint32;
    /* how's the 'hood look? */
    shmemu_assert(proc.npeers >= 0, "PMIx PE's peer count");
    shmemu_assert(proc.npeers <= proc.nranks, "PMIx PE's peer count too high");

    ps = PMIx_Get(&wc_proc, PMIX_LOCAL_PEERS, NULL, 0, &vp);
    shmemu_assert(ps == PMIX_SUCCESS, "PMIx can't find PE's peer list");

    parse_peers(vp->data.string);
}

void
shmemc_pmi_client_finalize(void)
{
    pmix_status_t ps;
    int pe;

#ifdef HAVE_PMIX_NO_INIT_HINTS
    ps = PMIx_Finalize();
#else
    ps = PMIx_Finalize(NULL, 0);
#endif  /* HAVE_PMIX_NO_INIT_HINTS */

    shmemu_assert(ps == PMIX_SUCCESS, "PMIx can't finalize");

    for (pe = 0; pe < proc.nranks; pe += 1) {
        size_t r;

        /* clean up allocations for exchanged buffers */
        if (proc.comms.xchg_wrkr_info[pe].buf != NULL) {
            free(proc.comms.xchg_wrkr_info[pe].buf);
        }
        for (r = 0; r < proc.comms.nregions; r += 1) {
            ucp_rkey_destroy(proc.comms.regions[r].minfo[pe].racc.rkey);
        }
    }

    /* clean up memory recording peer PEs */
    if (proc.peers != NULL) {
        free(proc.peers);
    }
}

void
shmemc_pmi_client_abort(const char *msg, int status)
{
    pmix_status_t ps;

    ps = PMIx_Abort(status, msg, NULL, 0);
    shmemu_assert(ps == PMIX_SUCCESS, "PMIx can't abort");
}
