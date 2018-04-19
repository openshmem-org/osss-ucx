/* For license: see LICENSE file at top-level */

#ifndef _THISPE_H
#define _THISPE_H 1

#include "boolean.h"
#include "threading.h"

#include <sys/types.h>
#include <ucp/api/ucp.h>

/*
 * -- for UCX --------------------------------------------------------
 */

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

typedef struct shmemc_context_attr {
    bool serialized;
    bool private;
    bool nostore;
} shmemc_context_attr_t;

typedef struct shmemc_context {
    ucp_worker_h w;             /* for separate context progress */

    unsigned long id;           /* internal tracking */

    shmemc_thread_t creator_thread; /* thread ID that created me */

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
    shmemc_thread_t invoking_thread; /* thread that called shmem_init*() */
} thread_desc_t;

/*
 * -- General --------------------------------------------------------
 */

typedef enum shmemc_status {
    SHMEMC_PE_SHUTDOWN = 0,
    SHMEMC_PE_RUNNING,
    SHMEMC_PE_FAILED,
    SHMEMC_PE_UNKNOWN
} shmemc_status_t;

/*
 * which collectives we're using.  might want to move this per-team at
 * some point so we can optimize through locality-awareness
 */
typedef enum shmemc_coll {
    SHMEMC_COLL_LINEAR = 0,
    SHMEMC_COLL_TREE,
    SHMEMC_COLL_DISSEM,
    SHMEMC_COLL_BINOMIAL,
    SHMEMC_COLL_BINOMIAL2,
    SHMEMC_COLL_UNKNOWN
} shmemc_coll_t;

#define SHMEMC_COLL_DEFAULT SHMEMC_COLL_TREE

/*
 * typedef struct shmemc_coll_algo {
 *    shmemc_coll_t barrier;
 *    shmemc_coll_t broadcast;
 *    shmemc_coll_t collect;
 *    shmemc_coll_t alltoall;
 * } shmemc_coll_algo_t;
 */

typedef struct heapinfo {
    size_t nheaps;              /* how many heaps reuqested */
    size_t *heapsize;           /* array of their sizes */
} heapinfo_t;

/*
 * implementations support some environment variables
 */
typedef struct env_info {
    /*
     * required
     */
    bool print_version;         /* produce info output? */
    bool print_info;
    heapinfo_t heaps;
    bool debug;                 /* are we doing sanity debugging? */

    /*
     * this implementation
     */
    bool logging;            /* turn on message logging? */
    char *logging_file;      /* where does logging output go? */
    char *logging_events;    /* show only these types of messages */
    bool xpmem_kludge;       /* protect against UCX bug temporarily */
    shmemc_coll_t barrier_algo;
    shmemc_coll_t broadcast_algo;
} env_info_t;

/*
 * PEs can belong to teams
 */
typedef struct shmemc_team {
    unsigned long id;           /* team ID */
    int *members;               /* list of PEs in the team */
    size_t nmembers;
} shmemc_team_t;

/*
 * each PE has this state info
 */
typedef struct thispe_info {
    comms_info_t comms;         /* per-comms layer info */
    env_info_t env;             /* environment vars */
    thread_desc_t td;           /* threading model invoked */
    int rank;                   /* rank info */
    int nranks;                 /* how many ranks */
    shmemc_status_t status;     /* up, down, out to lunch etc */
    int refcount;               /* library initialization count */
    int *peers;                 /* peer PEs in a node group */
    int npeers;                 /* how many peers */
    shmemc_team_t *teams;       /* PE teams we belong to */
    size_t nteams;              /* how many teams */
} thispe_info_t;

#endif /* ! _THISPE_H */
