#ifndef _THISPE_H
#define _THISPE_H 1

#include <sys/types.h>
#include <ucp/api/ucp.h>

typedef struct worker_info {
    ucp_address_t *addr;        /* worker address */
    char *buf;                  /* allocated to copy remote worker */
    size_t len;                 /* size of worker */
} worker_info_t;

/*
 * PEs have various exposed (symmetric) memory regions.  Currently,
 * this means the globals and a default symmetric heap.  But could
 * change in future.
 */
typedef struct heap_access {
    ucp_mem_h mh;               /* memory handle */
    ucp_rkey_h rkey;            /* remote key for this heap */
} heap_access_t;

typedef struct heapx {
    void *base;                 /* start of this heap */
    size_t length;              /* its size (b) */

    heap_access_t acc;          /* for remote comms */
} heapx_t;

void shmemc_heapx_init(void);
void shmemc_heapx_finalize(void);

typedef struct comms_info {
    ucp_context_h ctxt;         /* local communication context */
    ucp_config_t *cfg;          /* local config */
    ucp_worker_h wrkr;          /* local worker */
    worker_info_t *wrkrs;       /* nranks workers */
    ucp_ep_h *eps;              /* nranks endpoints (1 of which is mine) */
    heapx_t *heaps;             /* exchanged heap info */
} comms_info_t;

typedef enum shmem_status {
    SHMEM_PE_SHUTDOWN = 0,
    SHMEM_PE_RUNNING,
    SHMEM_PE_UNKNOWN
} shmem_status_t;

typedef struct thispe_info {
    comms_info_t comms;         /* per-comms layer info */
    int rank;                   /* rank info */
    int nranks;
    shmem_status_t status;      /* up, down, out to lunch etc */
    int refcount;               /* library initialization count */
    int *peers;                 /* PEs in a node group */
    int npeers;
} thispe_info_t;

/*
 * TODO: get remote address "a" on PE "pe"
 */
#define TRANSLATE_ADDR(a, pe) ((uint64_t) a)

#endif /* ! _THISPE_H */
