/* For license: see LICENSE file at top-level */

#ifndef _SHMEMC_CONTEXTS_H
#define _SHMEMC_CONTEXTS_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"

int shmemc_context_fill(long options, shmemc_context_h ch);

void shmemc_context_cleanup(shmemc_context_h ch);

int shmemc_context_default_set_info(shmemc_context_h ch);

#endif  /* ! _SHMEMC_CONTEXTS_H */
