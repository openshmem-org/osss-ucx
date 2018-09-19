
/* For license: see LICENSE file at top-level */

#ifndef _UCP_PE_H
#define _UCP_PE_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "boolean.h"
#include "threading.h"

#include <sys/types.h>
#include <ucp/api/ucp.h>

/*
 * exchanged at start-up
 */
typedef struct worker_info {
    ucp_address_t *addr;        /* worker address */
    char *buf;                  /* allocated to copy remote worker */
    size_t len;                 /* size of worker */
} worker_info_t;

/*
 * Encapsulate what UCX needs for remote access to a memory region
 */
typedef struct mem_region_access {
    ucp_mem_h mh;               /* memory handle */
    ucp_rkey_h rkey;            /* remote key for this heap */
} mem_region_access_t;

/*
 * each PE has a number of memory regions, which need the following
 * info:
 */
typedef struct mem_info {
    size_t id;                  /* number of this heap */
    uint64_t base;              /* start of this heap */
    uint64_t end;               /* end of this heap */
    size_t len;                 /* its size (b) */
    mem_region_access_t racc;   /* for remote access */
} mem_info_t;

/*
 * for PE exchange
 */
typedef struct mem_region {
    mem_info_t *minfo;          /* nranks mem info */
} mem_region_t;

/*
 * *Internal* OpenSMHEM context management handle
 *
 * NB difference between UCX context, and OpenSHMEM context.  Each
 * OpenSHMEM context requires...
 */

/*
 * context attributes, see OpenSMHEM 1.4 spec, sec. 9.4.1, pp. 30-31
 */
typedef struct shmemc_context_attr {
    bool serialized;
    bool private;
    bool nostore;
} shmemc_context_attr_t;

typedef struct shmemc_context {
    ucp_worker_h w;             /* for separate context progress */

    unsigned long id;           /* internal tracking */

    threadwrap_thread_t creator_thread; /* thread ID that created me */

    /*
     * parsed options during creation (defaults: no)
     */
    shmemc_context_attr_t attr;

    /*
     * possibly other things
     */
} shmemc_context_t;

typedef shmemc_context_t *shmemc_context_h;

/*
 * this comms-layer needs to know...
 */
typedef struct comms_info {
    ucp_context_h ucx_ctxt;     /* local communication context */
    ucp_config_t *ucx_cfg;      /* local config */
    ucp_ep_h *eps;              /* nranks endpoints (1 of which is mine) */

    worker_info_t *xchg_wrkr_info; /* nranks worker info exchanged */

    shmemc_context_h *ctxts;    /* PE's contexts */
    size_t nctxts;              /* how many contexts */

    mem_region_t *regions;      /* exchanged symmetric regions */
    size_t nregions;            /* how many regions */
} comms_info_t;

typedef struct thread_desc {
    ucs_thread_mode_t ucx_tl;   /* UCX thread level */
    int osh_tl;                 /* corresponding OpenSHMEM thread level */
    threadwrap_thread_t invoking_thread; /* thread that called shmem_init*() */
} thread_desc_t;

#endif /* ! _UCP_PE_H */
