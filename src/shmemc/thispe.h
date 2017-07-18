#ifndef _THISPE_H
#define _THISPE_H 1

#include <ucp/api/ucp.h>

typedef struct comms_info {

    ucp_ep_h *eps;              /* nranks * endpoints (1 is mine) */

    ucp_context_h ctxt;         /* local communication context */

    ucp_config_t *cfg;          /* local config */

    ucp_worker_h wrkr;          /* local worker and its addr info */
    ucp_address_t *addr_p;
    size_t addr_len;

} comms_info_t;

typedef enum shmem_status {
    SHMEM_PE_SHUTDOWN = 0,
    SHMEM_PE_UP,
    SHMEM_PE_UNKNOWN
} shmem_status_t;

typedef struct thispe_info {
    comms_info_t comms;         /* per-comms layer info */

    int rank;                   /* rank info */
    int nranks;

    shmem_status_t status;      /* up, down, out to lunch etc */

    int refcount;               /* library initialization count */

    int nheaps;                 /* number of symmetric heaps */

} thispe_info_t;

extern thispe_info_t proc;

#endif /* ! _THISPE_H */
