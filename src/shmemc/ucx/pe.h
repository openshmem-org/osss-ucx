/* For license: see LICENSE file at top-level */

#ifndef _UCP_PE_H
#define _UCP_PE_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "boolean.h"
#include "threading.h"
#include "shmem/teams.h"
#include "../../klib/khash.h"      /* TODO */
#include "shmem/defs.h"

#include <sys/types.h>
#include <ucp/api/ucp.h>

/*
 * exchanged at start-up
 */
typedef struct worker_info {
    ucp_address_t *addr;        /**< worker address */
    char *buf;                  /**< allocated to copy remote worker */
    size_t len;                 /**< size of worker */
} worker_info_t;

/*
 * Encapsulate what UCX needs for remote access to a memory region
 */
typedef struct mem_opaque_rkey {
    void *data;
} mem_opaque_rkey_t;

typedef struct mem_opaque {
    mem_opaque_rkey_t *rkeys;   /* per PE */
} mem_opaque_t;

typedef struct mem_access {
    ucp_rkey_h rkey;            /* remote key for this heap */
} mem_access_t;

typedef struct mem_region_access {
    mem_access_t *rinfo;        /* nranks remote access info */
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
    ucp_mem_h mh;               /* memory handle */
} mem_info_t;

/*
 * for PE exchange
 */
typedef struct mem_region {
    mem_info_t *minfo;          /**< nranks mem info */
} mem_region_t;

/*
 * *Internal* OpenSMHEM context management handle
 *
 * NB difference between UCX context, and OpenSHMEM context.  Each
 * OpenSHMEM context requires...
 */

typedef struct shmemc_context *shmemc_context_h;

/*
 * PEs belong to teams
 */
typedef struct shmemc_team *shmemc_team_h;

KHASH_MAP_INIT_INT(map, int)

typedef struct shmemc_team {
    int rank;
    int nranks;                 /**< identity */

    khash_t(map) *fwd;
    khash_t(map) *rev;      /**< forward and reverse PE number maps */

    shmem_team_config_t cfg;    /**< team configuration */

    shmemc_context_h *ctxts;    /**< array of contexts in this team */
    size_t nctxts;              /**< how many contexts allocated */

    const char *name;           /**< if predef, who we are (else NULL) */
    shmemc_team_h parent;       /**< parent team we split from,
                                   NULL if predef */

    /* now need to add pSync arrays for collectives */
#define SHMEMC_NUM_PSYNCS 2
    long *pSyncs[SHMEMC_NUM_PSYNCS];
} shmemc_team_t;


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
    ucp_ep_h *eps;              /* endpoints */
    unsigned long id;           /* internal tracking */
    threadwrap_thread_t creator_thread; /* thread ID that created me */
    /*
     * parsed options during creation (defaults: no)
     */
    shmemc_context_attr_t attr;

    mem_region_access_t *racc;  /* for endpoint remote access */

    shmemc_team_h team;         /* team we belong to */

    /*
     * possibly other things
     */
} shmemc_context_t;

/*
 * this comms-layer needs to know...
 */
typedef struct comms_info {
    ucp_context_h ucx_ctxt;     /* local communication context */
    ucp_config_t *ucx_cfg;      /* local config */
    worker_info_t *xchg_wrkr_info; /* nranks worker info exchanged */

#if 0
    shmemc_context_h *ctxts;    /**< PE's contexts */
    size_t nctxts;              /**< how many contexts */
#endif

    mem_region_t *regions;      /**< exchanged symmetric regions */
    size_t nregions;            /**< how many regions */

    mem_opaque_t *orks;         /* opaque rkeys (nregions * PEs) */
} comms_info_t;

typedef struct thread_desc {
    ucs_thread_mode_t ucx_tl;   /**< UCX thread level */
    int osh_tl;                 /**< corresponding OpenSHMEM thread level */
    threadwrap_thread_t invoking_thread; /**< who called shmem_init*()? */
} thread_desc_t;

#endif /* ! _UCP_PE_H */
