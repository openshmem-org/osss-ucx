/*
 * REMOVAL NOTICE: PMIx IS GETTING RID OF PMI{1,2} SUPPORT
 *
 */


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

#include <pmi.h>

#include <ucp/api/ucp.h>

/* -------------------------------------------------------------- */

/*
 * Persistent local state
 */

static char *kvs_name = NULL;   /* PMI namespace */

static int kvs_max_name_len;
static int kvs_max_key_len;
static int kvs_max_value_len;

/*
 * allocate state variables
 */

inline static char *
alloc_string(size_t n)
{
    return (char *) malloc(n);
}

/* -------------------------------------------------------------- */

static const char *wrkr_exch_fmt = "%d:wrkr:addr"; /* pe */

/*
 * PMI key/val lookups
 */
static char *key = NULL;
static char *val = NULL;

void
shmemc_pmi_publish_worker(void)
{
    int ps;

    /* everyone publishes their info */
    snprintf(key, kvs_max_key_len,
             wrkr_exch_fmt,
             proc.rank);
    memcpy(val,
           & proc.comms.xchg_wrkr_info[proc.rank],
           sizeof(proc.comms.xchg_wrkr_info[proc.rank]));

    ps = PMI_KVS_Put(kvs_name, key, val);
    shmemu_assert(ps == PMI_SUCCESS,
                  "shmemc/pmi1: put() of worker blob failed (status %d)",
                  ps);
}

static const char *rkey_exch_fmt   = "rkey:%lu:%d"; /* region, pe */

#ifndef ENABLE_ALIGNED_ADDRESSES
static const char *heap_exch_fmt = "heap:%lu:%d"; /* region, pe */
#endif /* ! ENABLE_ALIGNED_ADDRESSES */

inline static void
publish_one_rkeys(size_t r)
{
    int ps;
    int next;
    void *packed_rkey;
    size_t rkey_len;
    const ucs_status_t s =
        ucp_rkey_pack(proc.comms.ucx_ctxt,
                      proc.comms.regions[r].minfo[proc.rank].racc.mh,
                      &packed_rkey, &rkey_len
                      );
    shmemu_assert(s == UCS_OK, "shmemc/pmi1: can't pack rkey");

    snprintf(key, kvs_max_key_len, rkey_exch_fmt, proc.rank);

    next = snprintf(val, kvs_max_value_len,
                    "%8lu",
                    (unsigned long) rkey_len);
    memcpy(val + next, packed_rkey, rkey_len);

    ps = PMI_KVS_Put(kvs_name, key, val);
    shmemu_assert(ps == PMI_SUCCESS,
                  "shmemc/pmi1: put of worker blob failed (status %d)",
                  ps);

    ucp_rkey_buffer_release(packed_rkey);
}

#ifndef ENABLE_ALIGNED_ADDRESSES
inline static void
publish_one_heap(size_t r)
{
    int ps;
    const mem_info_t *mp = & proc.comms.regions[r].minfo[proc.rank];

    /* everyone publishes their info */
    snprintf(key, kvs_max_key_len,
             heap_exch_fmt,
             proc.rank);
    memcpy(val, mp, sizeof(*mp));

    ps = PMI_KVS_Put(kvs_name, key, val);
    shmemu_assert(ps == PMI_SUCCESS,
                  "shmemc/pmi1: put() of heap blob failed (status %d)",
                  ps);
}
#endif /* ! ENABLE_ALIGNED_ADDRESSES */

void
shmemc_pmi_publish_rkeys_and_heaps(void)
{
    size_t r;

    publish_one_rkeys(0);

    for (r = 1; r < proc.comms.nregions; ++r) {
        publish_one_rkeys(r);

#ifndef ENABLE_ALIGNED_ADDRESSES
        publish_one_heap(r);
#endif /* ! ENABLE_ALIGNED_ADDRESSES */
    }
}

void
shmemc_pmi_exchange_workers(void)
{
    int pe;

    for (pe = 0; pe < proc.nranks; ++pe) {
        int ps;
        worker_info_t *wp;
        const int i = shmemu_shift(pe);

        snprintf(key, kvs_max_key_len,
                 wrkr_exch_fmt,
                 i);

        ps = PMI_KVS_Get(kvs_name, key, val, kvs_max_value_len);
        shmemu_assert(ps == PMI_SUCCESS,
                      "shmemc/pmi1: get() of remote worker blob failed (status %d)",
                      ps);

        wp = (worker_info_t *) val;

        proc.comms.xchg_wrkr_info[i].buf = (char *) malloc(wp->len);
        shmemu_assert(proc.comms.xchg_wrkr_info[i].buf != NULL,
                      "shmemc/pmi1: can't allocate memory for "
                      "remote worker blob");
        memcpy(proc.comms.xchg_wrkr_info[i].buf, wp->addr, wp->len);
    }
}

inline static void
exchange_one_rkeys(size_t r, int pe)
{
    const int i = shmemu_shift(pe);
    int ps;
    ucs_status_t s;
    size_t len;

    snprintf(key, kvs_max_key_len,
             rkey_exch_fmt,
             r, i);

    ps = PMI_KVS_Get(kvs_name, key, val, kvs_max_value_len);
    shmemu_assert(ps == PMI_SUCCESS,
                  "shmemc/pmi1: fetch of rkey from PE %d failed (status %d)",
                  i, ps);

    sscanf(val, "%8lu", (unsigned long *) &len);

    proc.comms.regions[r].minfo[i].racc.rkey =
        (ucp_rkey_h) malloc(len);
    shmemu_assert(proc.comms.regions[r].minfo[i].racc.rkey != NULL,
                  "shmemc/pmi1: can't allocate memory for remote rkey");

    s = ucp_ep_rkey_unpack(proc.comms.eps[i],
                           val + len,
                           &proc.comms.regions[r].minfo[i].racc.rkey
                           );
    shmemu_assert(s == UCS_OK,
                  "shmemc/pmi1: can't unpack remote rkey");
}

#ifndef ENABLE_ALIGNED_ADDRESSES
inline static void
exchange_one_heap(size_t r, int pe)
{
    int ps;
    mem_info_t *mp;

    snprintf(key, kvs_max_key_len,
             heap_exch_fmt,
             r, pe);

    ps = PMI_KVS_Get(kvs_name, key, val, kvs_max_value_len);
    shmemu_assert(ps == PMI_SUCCESS,
                  "shmemc/pmi1: fetch of rkey from PE %d failed (status %d)",
                  pe, ps);

    mp = (mem_info_t *) val;

    proc.comms.regions[r].minfo[pe].base =
        mp->base;
    proc.comms.regions[r].minfo[pe].len =
        mp->len;
    /* slightly redundant storage, but useful */
    proc.comms.regions[r].minfo[pe].end =
        proc.comms.regions[r].minfo[pe].base +
        mp->len;
}
#endif /* ! ENABLE_ALIGNED_ADDRESSES */

inline static void
exchange_all_rkeys(size_t r)
{
    int pe;

    for (pe = 0; pe < proc.nranks; ++pe) {
        const int i = shmemu_shift(pe);

        exchange_one_rkeys(r, i);
    }
}

inline static void
exchange_one_rkeys_and_heaps(size_t r)
{
    int pe;

    for (pe = 0; pe < proc.nranks; ++pe) {
        const int i = shmemu_shift(pe);

        exchange_one_rkeys(r, i);

#ifndef ENABLE_ALIGNED_ADDRESSES
        exchange_one_heap(r, i);
#endif /* ! ENABLE_ALIGNED_ADDRESSES */
    }  /* PE loop */
}

void
shmemc_pmi_exchange_rkeys_and_heaps(void)
{
    size_t r;

    /* global rkeys */
    exchange_all_rkeys(0);

    /* now everything else */
    for (r = 1; r < proc.comms.nregions; ++r) {
        exchange_one_rkeys_and_heaps(r);
    }
}

/* -------------------------------------------------------------- */

/*
 * this barrier is purely for internal use with PMI, nothing to do
 * with SHMEM/UCX
 */
void
shmemc_pmi_barrier_all(void)
{
    int ps;

    ps = PMI_KVS_Commit(kvs_name);
    shmemu_assert(ps == PMI_SUCCESS,
                  "shmemc/pmi1: worker blob commit failed (status %d)",
                  ps);

    ps = PMI_Barrier();
    shmemu_assert(ps == PMI_SUCCESS,
                  "shmemc/pmi1: PMI barrier failed (status %d)",
                  ps);
}

/*
 * find my rank, program size, peer PEs
 */

void
shmemc_pmi_client_init(void)
{
    int ps;
    int spawned;

    ps = PMI_Init(&spawned);
    shmemu_assert(ps == PMI_SUCCESS,
                  "shmemc/pmi1: can't initialize PMI (status %d)",
                  ps);

    ps = PMI_Get_rank(&proc.rank);
    shmemu_assert(ps == PMI_SUCCESS,
                  "shmemc/pmi1: PMI can't get PE rank (status %d)",
                  ps);

    shmemu_assert(proc.rank >= 0,
                  "shmemc/pmi1: PMI PE rank is not valid (status %d)",
                  ps);

    ps = PMI_Get_size(&proc.nranks);
    shmemu_assert(ps == PMI_SUCCESS,
                  "shmemc/pmi1: PMI can't get program size (status %d)",
                  ps);

    /* is the world a sane size? */
    shmemu_assert(proc.nranks > 0,
                  "shmemc/pmi1: PMI count of PE ranks %d is not valid",
                  proc.nranks);
    shmemu_assert(shmemu_valid_pe_number(proc.rank),
                  "shmemc/pmi1: PMI PE rank %d is not valid",
                  proc.rank);

    /* I have no peer info (at present) */
    proc.npeers = 0;
    proc.peers = NULL;

    ps = PMI_KVS_Get_name_length_max(&kvs_max_name_len);
    shmemu_assert(ps == PMI_SUCCESS,
                  "shmemc/pmi1: PMI can't get max name length (status %d)",
                  ps);

    ps = PMI_KVS_Get_key_length_max(&kvs_max_key_len);
    shmemu_assert(ps == PMI_SUCCESS,
                  "shmemc/pmi1: PMI can't get max key length (status %d)",
                  ps);

    ps = PMI_KVS_Get_value_length_max(&kvs_max_value_len);
    shmemu_assert(ps == PMI_SUCCESS,
                  "shmemc/pmi1: PMI can't get max value length (status %d)",
                  ps);

    kvs_name = alloc_string(kvs_max_name_len);
    shmemu_assert(kvs_name != NULL,
                  "shmemc/pmi1: PMI can't allocate name string");

    ps = PMI_KVS_Get_my_name(kvs_name, kvs_max_name_len);
    shmemu_assert(ps == PMI_SUCCESS,
                  "shmemc/pmi1: PMI can't find its name (status %d)",
                  ps);

    key = alloc_string(kvs_max_key_len);
    shmemu_assert(key != NULL,
                  "shmemc/pmi1: PMI can't allocate memory for key holder");
    val = alloc_string(kvs_max_value_len);
    shmemu_assert(val != NULL,
                  "shmemc/pmi1: PMI can't allocate memory for value holder");
}

void
shmemc_pmi_client_finalize(void)
{
    int ps;
    int pe;

    ps = PMI_Finalize();
    shmemu_assert(ps == PMI_SUCCESS,
                  "shmemc/pmi1: PMI can't finalize (status %d)",
                  ps);

    /* TODO: PMI client common code below */

    for (pe = 0; pe < proc.nranks; ++pe) {
        size_t r;

        /* clean up allocations for exchanged buffers */
        free(proc.comms.xchg_wrkr_info[pe].buf);
        for (r = 0; r < proc.comms.nregions; ++r) {
            ucp_rkey_destroy(proc.comms.regions[r].minfo[pe].racc.rkey);
        }
    }

    /* clean up locally allocated memory */
    free(proc.peers);
    free(kvs_name);
    free(key);
    free(val);
}

void
shmemc_pmi_client_abort(const char *msg, int status)
{
    int ps;

    ps = PMI_Abort(status, msg);
    shmemu_assert(ps == PMI_SUCCESS,
                  "shmemc/pmi1: PMI can't abort (status %d)",
                  ps);
}
