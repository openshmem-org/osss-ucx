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

void shmemc_ucx_allocate_eps_table(void);
void shmemc_ucx_deallocate_eps_table(void);

int shmemc_ucx_context_progress(shmemc_context_h ch);
void shmemc_ucx_make_remote_eps(void);
void shmemc_ucx_disconnect_all_eps(void);


#endif /* ! _SHMEMC_UCX_H */
