/* For license: see LICENSE file at top-level */

#ifndef _SHMEMC_UCX_H
#define _SHMEMC_UCX_H 1

/*
 * UCX-specific context operations
 */

#include "pe.h"

/*
 * setup and teardown
 */

void shmemc_ucx_init(void);
void shmemc_ucx_finalize(void);

/*
 * build context endpoints
 */

int shmemc_ucx_context_default_set_info(void);

void shmemc_ucx_allocate_eps_table(shmemc_context_h ch);
void shmemc_ucx_deallocate_eps_table(shmemc_context_h ch);

int shmemc_ucx_context_progress(shmemc_context_h ch);
void shmemc_ucx_make_remote_eps(shmemc_context_h ch);
void shmemc_ucx_disconnect_all_eps(shmemc_context_h ch);


#endif /* ! _SHMEMC_UCX_H */
