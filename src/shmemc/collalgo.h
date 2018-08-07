/* For license: see LICENSE file at top-level */

#ifndef _SHMEMC_COLLALGO_H
#define _SHMEMC_COLLALGO_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "thispe.h"

int shmemc_get_children_info(int tree_size, int tree_degree, int node,
                             int *children_begin, int *children_end);
int shmemc_get_children_info_binomial(int tree_size, int node, int *children);

#endif /* ! _SHMEMC_COLLALGO_H */
