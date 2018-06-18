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

/* -------------------------------------------------------------- */

/*
 * Persistent local state
 */

static pmix_info_t waiter;      /* control lookups */
static pmix_proc_t my_proc;     /* about me */
static pmix_proc_t wc_proc;     /* wildcard lookups */

/*
 * Make local info avaialable to PMIx
 */

static const char *wrkr_exch_fmt   = "wrkr:%d";     /* pe */

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

static const char *rkey_exch_fmt   = "rkey:%lu:%d"; /* region, pe */

#ifndef ENABLE_ALIGNED_ADDRESSES
static const char *region_base_fmt = "base:%lu:%d"; /* region, pe */
static const char *region_size_fmt = "size:%lu:%d"; /* region, pe */
#endif /* ! ENABLE_ALIGNED_ADDRESSES */

void
shmemc_pmi_publish_rkeys_and_heaps(void)
{
    pmix_status_t ps;
    pmix_info_t pi;
#ifndef ENABLE_ALIGNED_ADDRESSES
    pmix_info_t *hi;
#endif /* ! ENABLE_ALIGNED_ADDRESSES */
    void *packed_rkey;
    size_t rkey_len;
    size_t r;

    PMIX_INFO_CONSTRUCT(&pi);
#ifndef ENABLE_ALIGNED_ADDRESSES
    PMIX_INFO_CREATE(hi, 2);
#endif /* ! ENABLE_ALIGNED_ADDRESSES */

    for (r = 0; r < proc.comms.nregions; r += 1) {
        pmix_byte_object_t *bop = &pi.value.data.bo;
        const ucs_status_t s =
            ucp_rkey_pack(proc.comms.ucx_ctxt,
                          proc.comms.regions[r].minfo[proc.rank].racc.mh,
                          &packed_rkey, &rkey_len
                          );
        shmemu_assert(s == UCS_OK, "can't unpack rkey");

        snprintf(pi.key, PMIX_MAX_KEYLEN, rkey_exch_fmt, r, proc.rank);
        pi.value.type = PMIX_BYTE_OBJECT;
        bop->bytes = (char *) packed_rkey;
        bop->size = rkey_len;
        ps = PMIx_Publish(&pi, 1);
        shmemu_assert(ps == PMIX_SUCCESS, "can't publish rkey");

        ucp_rkey_buffer_release(packed_rkey);

#ifndef ENABLE_ALIGNED_ADDRESSES
        if (shmemu_likely(r > 0)) {
            snprintf(hi[0].key, PMIX_MAX_KEYLEN,
                     region_base_fmt, r, proc.rank);
            hi[0].value.type = PMIX_UINT64;
            hi[0].value.data.uint64 =
                (uint64_t) proc.comms.regions[r].minfo[proc.rank].base;

            snprintf(hi[1].key, PMIX_MAX_KEYLEN,
                     region_size_fmt, r, proc.rank);
            hi[1].value.type = PMIX_SIZE;
            hi[1].value.data.size = proc.comms.regions[r].minfo[proc.rank].len;

            /* newer PMIx should be able to do this at one go */
#if PMIX_VERSION_MAJOR > 2
            ps = PMIx_Publish(hi, 2);
            shmemu_assert(ps == PMIX_SUCCESS, "can't publish heap base/size");
#else /* PMIX_VERSION_MAJOR */
            ps = PMIx_Publish(&hi[0], 1);
            shmemu_assert(ps == PMIX_SUCCESS, "can't publish heap base");
            ps = PMIx_Publish(&hi[1], 1);
            shmemu_assert(ps == PMIX_SUCCESS, "can't publish heap size");
#endif  /* PMIX_VERSION_MAJOR */
        }
#endif /* ! ENABLE_ALIGNED_ADDRESSES */
    }

#ifndef ENABLE_ALIGNED_ADDRESSES
    PMIX_INFO_FREE(hi, 2);
#endif /* ! ENABLE_ALIGNED_ADDRESSES */
}

/* -------------------------------------------------------------- */

/*
 * Get remote info out of PMIx
 */

void
shmemc_pmi_exchange_workers(void)
{
    pmix_status_t ps;
    pmix_pdata_t fetch;
    int pe;

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

void
shmemc_pmi_exchange_rkeys_and_heaps(void)
{
    pmix_status_t ps;
    pmix_pdata_t rd;
#ifndef ENABLE_ALIGNED_ADDRESSES
    pmix_pdata_t *hd;
#endif /* ! ENABLE_ALIGNED_ADDRESSES */
    int pe;
    size_t r;

    PMIX_PDATA_CONSTRUCT(&rd);
#ifndef ENABLE_ALIGNED_ADDRESSES
    PMIX_PDATA_CREATE(hd, 2);
#endif /* ! ENABLE_ALIGNED_ADDRESSES */

    for (r = 0; r < proc.comms.nregions; r += 1) {

        for (pe = 0; pe < proc.nranks; pe += 1) {
            const pmix_byte_object_t *bop = &rd.value.data.bo;
            const int i = (pe + proc.rank) % proc.nranks;
            ucs_status_t s;

            /* rkey first */
            snprintf(rd.key, PMIX_MAX_KEYLEN, rkey_exch_fmt, r, i);

            ps = PMIx_Lookup(&rd, 1, &waiter, 1);
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

#ifndef ENABLE_ALIGNED_ADDRESSES
            /* now heaps, but skip globals */
            if (shmemu_likely(r > 0)) {
                snprintf(hd[0].key, PMIX_MAX_KEYLEN,
                         region_base_fmt, r, i);
                snprintf(hd[1].key, PMIX_MAX_KEYLEN,
                         region_size_fmt, r, i);

#if PMIX_VERSION_MAJOR > 2
                ps = PMIx_Lookup(hd, 2, &waiter, 1);
                shmemu_assert(ps == PMIX_SUCCESS,
                              "can't fetch heap base/size");
#else /* PMIX_VERSION_MAJOR */
                ps = PMIx_Lookup(&hd[0], 1, &waiter, 1);
                shmemu_assert(ps == PMIX_SUCCESS,
                              "can't fetch heap base");
                ps = PMIx_Lookup(&hd[1], 1, &waiter, 1);
                shmemu_assert(ps == PMIX_SUCCESS,
                              "can't fetch heap size");
#endif /* PMIX_VERSION_MAJOR */
                proc.comms.regions[r].minfo[i].base =
                    hd[0].value.data.uint64;
                proc.comms.regions[r].minfo[i].len =
                    hd[1].value.data.size;
                /* slightly redundant storage, but useful */
                proc.comms.regions[r].minfo[i].end =
                    proc.comms.regions[r].minfo[i].base +
                    hd[1].value.data.size;
            }
#endif /* ! ENABLE_ALIGNED_ADDRESSES */
        }
    }
#ifndef ENABLE_ALIGNED_ADDRESSES
    PMIX_PDATA_FREE(hd, 2);
#endif /* ! ENABLE_ALIGNED_ADDRESSES */
}

/* -------------------------------------------------------------- */

/*
 * read out the peer PE numbers
 */
inline static void
parse_peers(char *peerstr)
{
    size_t i = 0;
    char *next;
    const char *sep = ",";

    /* parse the PE #s out of the string */
    proc.peers = (int *) calloc(proc.npeers,
                                sizeof(*proc.peers)); /* free at end */
    shmemu_assert(proc.peers != NULL,
                  "can't allocate memory for peer list");

    next = strtok(peerstr, sep);
    while (next != NULL) {
        proc.peers[i] = (int) strtol(next, NULL, 10);
        i += 1;
        next = strtok(NULL, sep);
    }
}

/* -------------------------------------------------------------- */

inline static void
make_waiter(void)
{
    int all = 0;

    PMIX_INFO_CONSTRUCT(&waiter);
    PMIX_INFO_LOAD(&waiter, PMIX_WAIT, &all, PMIX_INT);
}

inline static void
release_waiter(void)
{
    return;                     /* nothing needed */
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

/*
 * handle the different init/fini APIs
 */

inline static pmix_status_t
pmix_init_wrapper(pmix_proc_t *pp)
{
    pmix_status_t ps;

#ifdef HAVE_PMIX_NO_INIT_HINTS
    ps = PMIx_Init(pp);
#else
    ps = PMIx_Init(pp, NULL, 0);
#endif  /* HAVE_PMIX_NO_INIT_HINTS */

    return ps;
}

inline static pmix_status_t
pmix_finalize_wrapper(void)
{
    pmix_status_t ps;

#ifdef HAVE_PMIX_NO_INIT_HINTS
    ps = PMIx_Finalize();
#else
    ps = PMIx_Finalize(NULL, 0);
#endif  /* HAVE_PMIX_NO_INIT_HINTS */

    return ps;
}

/*
 * get the PMIx client-side up and running
 */

void
shmemc_pmi_client_init(void)
{
    pmix_value_t v;
    pmix_value_t *vp = &v;      /* holds things we get from PMIx */
    pmix_status_t ps;

    ps = pmix_init_wrapper(&my_proc);

    shmemu_assert(ps == PMIX_SUCCESS,
                  "PMIx can't initialize (%s)",
                  PMIx_Error_string(ps));

    /* we can get our own rank immediately */
    proc.rank = (int) my_proc.rank;
    shmemu_assert(proc.rank >= 0,
                  "PMIx PE rank %d is not valid (%s)",
                  proc.rank,
                  PMIx_Error_string(ps));

    /* make a new proc to query things not linked to a specific rank */
    PMIX_PROC_CONSTRUCT(&wc_proc);
    strncpy(wc_proc.nspace, my_proc.nspace, PMIX_MAX_NSLEN + 1);
    wc_proc.rank = PMIX_RANK_WILDCARD;

    ps = PMIx_Get(&wc_proc, PMIX_JOB_SIZE, NULL, 0, &vp);
    shmemu_assert(ps == PMIX_SUCCESS,
                  "PMIx can't get program size (%s)",
                  PMIx_Error_string(ps));

    proc.nranks = (int) vp->data.uint32; /* number of ranks/PEs */

    ps = PMIx_Get(&wc_proc, PMIX_UNIV_SIZE, NULL, 0, &vp);
    shmemu_assert(ps == PMIX_SUCCESS,
                  "PMIx can't get universe size (%s)",
                  PMIx_Error_string(ps));

    proc.maxranks = (int) vp->data.uint32; /* total ranks available */

    /* is the world a sane size? */
    shmemu_assert(proc.nranks > 0,
                  "PMIx count of PE ranks %d is not valid",
                  proc.nranks);
    shmemu_assert(proc.maxranks > 0,
                  "PMIx PE universe size %d is not valid",
                  proc.maxranks);
    shmemu_assert(IS_VALID_PE_NUMBER(proc.rank),
                  "PMIx PE rank %d is not valid",
                  proc.rank);

    /* what's on this node? */
    ps = PMIx_Get(&wc_proc, PMIX_LOCAL_SIZE, NULL, 0, &vp);
    shmemu_assert(ps == PMIX_SUCCESS,
                  "PMIx can't find PE's peers (%s)",
                  PMIx_Error_string(ps));

    proc.npeers = (int) vp->data.uint32;
    /* how's the 'hood look? */
    shmemu_assert(proc.npeers >= 0,
                  "PMIx PE's peer count %d must be >= 0",
                  proc.npeers);
    shmemu_assert(proc.npeers <= proc.nranks,
                  "PMIx PE's peer count %d bigger than program %d",
                  proc.npeers, proc.nranks);

    ps = PMIx_Get(&wc_proc, PMIX_LOCAL_PEERS, NULL, 0, &vp);
    shmemu_assert(ps == PMIX_SUCCESS,
                  "PMIx can't find PE's peer list (s)",
                  PMIx_Error_string(ps));

    parse_peers(vp->data.string);

    make_waiter();
}

/*
 * shut down PMIx client-side
 */

void
shmemc_pmi_client_finalize(void)
{
    pmix_status_t ps;
    int pe;

    ps = pmix_finalize_wrapper();

    shmemu_assert(ps == PMIX_SUCCESS,
                  "PMIx can't finalize (%s)",
                  PMIx_Error_string(ps));

    for (pe = 0; pe < proc.nranks; pe += 1) {
        size_t r;

        /* clean up allocations for exchanged buffers */
        free(proc.comms.xchg_wrkr_info[pe].buf);
        for (r = 0; r < proc.comms.nregions; r += 1) {
            ucp_rkey_destroy(proc.comms.regions[r].minfo[pe].racc.rkey);
        }
    }

    /* clean up memory recording peer PEs */

    release_waiter();

    free(proc.peers);
}

/*
 * crunch out if fatal error
 */

void
shmemc_pmi_client_abort(const char *msg, int status)
{
    pmix_status_t ps;

    ps = PMIx_Abort(status, msg, NULL, 0);
    shmemu_assert(ps == PMIX_SUCCESS,
                  "PMIx can't abort (%s)",
                  PMIx_Error_string(ps));
}
