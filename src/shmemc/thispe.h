/* For license: see LICENSE file at top-level */

#ifndef _THISPE_H
#define _THISPE_H 1

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
struct shmemc_context {
    ucp_ep_h *eps;              /* nranks endpoints (1 of which is mine) */

    /*
     * parsed options during creation
     */
    long serialized;
    long private;               /* default: no */
    long nostore;

    unsigned long id;           /* internal tracking */

    /*
     * possibly other things
     */
};
typedef struct shmemc_context *shmemc_context_h;

/*
 * this comms-layer needs to know...
 */
typedef struct comms_info {
    ucp_context_h ctxt;         /* local communication context */
    ucp_config_t *cfg;          /* local config */
    ucp_worker_h wrkr;          /* local worker */
    worker_info_t *xchg_wrkr_info; /* nranks worker info exchanged */
    ucp_ep_h *eps;              /* nranks endpoints (1 of which is mine) */
    shmemc_context_h *ctxts;    /* PE's contexts (replaces EPs above). */
                                /* Created on demand */
    mem_region_t *regions;      /* exchanged symmetric regions */
    size_t nregions;            /* number of symmetric regions per PE */
} comms_info_t;

/*
 * -- General --------------------------------------------------------
 */

/*
 * implementations support some environment variables
 */
typedef struct env_info {
    /*
     * required
     */
    int print_version;          /* produce info output? */
    int print_info;
    size_t def_heap_size;       /* TODO: expand for multiple heaps */
    int debug;                  /* are we doing debugging? */

    /*
     * this implementation
     */
    char *debug_file;           /* where does debugging output go? */
    int xpmem_kludge;           /* protect against UCX bug temporarily */
} env_info_t;

typedef enum shmem_status {
    SHMEM_PE_SHUTDOWN = 0,
    SHMEM_PE_RUNNING,
    SHMEM_PE_FAILED,
    SHMEM_PE_UNKNOWN
} shmem_status_t;

/*
 * PEs can belong to teams
 */
typedef struct shmem_team {
    unsigned long id;           /* team ID */
    int *members;               /* list of PEs in the team */
} shmem_team_t;

/*
 * each PE has this state info
 */
typedef struct thispe_info {
    comms_info_t comms;         /* per-comms layer info */
    env_info_t env;             /* environment vars */

    int thread_level;           /* current thread support */
    pthread_t invoking_thread;  /* thread that called shmem_init*() */

    int rank;                   /* rank info */
    int nranks;
    shmem_status_t status;      /* up, down, out to lunch etc */
    int refcount;               /* library initialization count */
    int *peers;                 /* # PEs in a node group */
    int npeers;
    shmem_team_t *teams;        /* PE teams we belong to */
} thispe_info_t;

#endif /* ! _THISPE_H */
