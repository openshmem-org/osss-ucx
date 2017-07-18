#ifndef _SHMEMC_H
#define _SHMEMC_H 1

#include "shmemc.h"

#include <ucp/api/ucp.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * endpoints of all PEs
 */
typedef struct endpoint {
    ucp_ep_h remote_ep;
} endpoint_t;

extern endpoint_t *endpoints;

typedef struct comms_info {
    /* local endpoint */
    ucp_ep_h my_ep;
    /* local communication context */
    ucp_context_h ctxt;
    /* UCP config */
    ucp_config_t *cfg;
    /* local worker */
    ucp_worker_h wrkr;
    /* local worker address info to pass to remote */
    ucp_address_t *addr_p;
    size_t addr_len;
} comms_info_t;

#endif /* ! _SHMEMC_H */
