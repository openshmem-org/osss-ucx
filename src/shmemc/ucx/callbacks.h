/* For license: see LICENSE file at top-level */

#ifndef _SHMEMC_UCX_CALLBACKS_H
#define _SHMEMC_UCX_CALLBACKS_H 1

#include <ucp/api/ucp.h>

/*
 * callbacks for non-blocking ops.
 */

void nb_callback(void *request, ucs_status_t status);
void nb_callbackx(void *request, ucs_status_t status, void *user_data);

void noop_callback(void *request, ucs_status_t status);
void noop_callbackx(void *request, ucs_status_t status, void *user_data);

#endif /* ! _SHMEMC_UCX_CALLBACKS_HH */
