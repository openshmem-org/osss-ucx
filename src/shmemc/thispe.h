/* For license: see LICENSE file at top-level */

#ifndef _THISPE_H
#define _THISPE_H 1

#include <sys/types.h>
#include <ucp/api/ucp.h>

/*
 * -- for UCX --------------------------------------------------------
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
    size_t length;              /* its size (b) */
    mem_region_access_t racc;   /* for remote access */
} mem_info_t;

/*
 * for PE exchange
 */
typedef struct mem_region {
    mem_info_t *minfo;          /* nranks mem info */
} mem_region_t;

typedef struct comms_info {
    ucp_context_h ctxt;         /* local communication context */
    ucp_config_t *cfg;          /* local config */
    ucp_worker_h wrkr;          /* local worker */
    worker_info_t *wrkrs;       /* nranks workers */
    ucp_ep_h *eps;              /* nranks endpoints (1 of which is mine) */
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
    /* required */
    int print_version;
    int print_info;
    size_t def_heap_size;       /* TODO: expand for multiple heaps */
    int debug;                  /* are we doing debugging? */

    /* this implementation */
    char *debug_file;           /* where does debugging output go? */
} env_info_t;

typedef enum shmem_status {
    SHMEM_PE_SHUTDOWN = 0,
    SHMEM_PE_RUNNING,
    SHMEM_PE_UNKNOWN
} shmem_status_t;

typedef struct thispe_info {
    comms_info_t comms;         /* per-comms layer info */
    env_info_t env;             /* environment vars */

    int rank;                   /* rank info */
    int nranks;
    shmem_status_t status;      /* up, down, out to lunch etc */
    int refcount;               /* library initialization count */
    int *peers;                 /* PEs in a node group */
    size_t npeers;
} thispe_info_t;

#endif /* ! _THISPE_H */
