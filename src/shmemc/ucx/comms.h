#ifndef _SHMEMC_H
#define _SHMEMC_H 1

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

/**
 * API
 *
 **/

void shmemc_quiet(void);
void shmemc_init(void);
void shmemc_finalize(void);

void shmemc_put(void *dest, const void *src,
                size_t nbytes, int pe);
void shmemc_get(void *dest, const void *src,
                size_t nbytes, int pe);

void shmemc_add_int(int *t, int v, int pe);
void shmemc_add_long(long *t, long v, int pe);
void shmemc_add_longlong(long long *t, long long v, int pe);
void shmemc_inc_int(int *t, int pe);
void shmemc_inc_long(long *t, int pe);
void shmemc_inc_longlong(long long *t, int pe);
int shmemc_fadd_int(int *t, int v, int pe);
long shmemc_fadd_long(long *t, long v, int pe);
long long shmemc_fadd_longlong(long long *t, long long v, int pe);
int shmemc_finc_int(int *t, int pe);
long shmemc_finc_long(long *t, int pe);
long long shmemc_finc_longlong(long *t, int pe);
int shmemc_swap_int(int *t, int v, int pe);
long shmemc_swap_long(long *t, long v, int pe);
long long shmemc_swap_longlong(long long *t, long long v, int pe);
int shmemc_cswap_int(int *t, int c, int v, int pe);
long shmemc_cswap_long(long *t, long c, long v, int pe);
long long shmemc_cswap_longlong(long long *t, long long c, long long v, int pe);
int shmemc_fetch_int(int *t, int pe);
long shmemc_fetch_long(long *t, int pe);
long long shmemc_fetch_longlong(long long *t, int pe);
void shmemc_set_int(int *t, int v, int pe);
void shmemc_set_long(long *t, long v, int pe);
void shmemc_set_longlong(long long *t, long long v, int pe);

#endif /* ! _SHMEMC_H */
