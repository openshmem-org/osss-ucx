/* For license: see LICENSE file at top-level */

#ifndef _SHMEMC_UCX_H
#define _SHMEMC_UCX_H 1

/*
 * UCX-specific context operations
 */

#include "pe.h"

int shmemc_context_fill(long options, shmemc_context_h ch);
void shmemc_context_cleanup(shmemc_context_h ch);
int shmemc_context_default_set_info(shmemc_context_h ch);

/*
 * setup and teardown
 */

void shmemc_ucx_init(void);
void shmemc_ucx_finalize(void);

/*
 * build endpoints from remotely exchanged worker info
 */

void shmemc_ucx_make_remote_endpoints(void);

#endif /* ! _SHMEMC_UCX_H */
