#ifndef _THISPE_H
#define _THISPE_H 1

#include "heapx.h"

#include <ucp/api/ucp.h>

typedef struct worker_info {
    ucp_address_t *addr_p;
    size_t len;
} worker_info_t;

typedef struct comms_info {
    ucp_context_h ctxt;         /* local communication context */
    ucp_config_t *cfg;          /* local config */
    ucp_worker_h wrkr;          /* local worker */
    worker_info_t *wrkrs;       /* nranks workers */
    ucp_ep_h *eps;              /* nranks endpoints (1 of which is mine) */

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

    char *peers;                /* PEs in a node group */
    int npeers;

    heapx_t *heaps;             /* exchanged symmetric heaps */

} thispe_info_t;

extern thispe_info_t proc;

/*
 * shortcut to look up the UCP endpoint
 */
#define LOOKUP_UCP_EP(rank) (proc.comms.eps[(rank)])

/*
 * TODO: shortcut to look up the remote key for address "a"
 */
#define LOOKUP_RKEY(a) NULL

/*
 * TODO: get remote address "a" on PE "pe"
 */
#define TRANSLATE_ADDR(a, pe) ((uint64_t) a)

#endif /* ! _THISPE_H */
