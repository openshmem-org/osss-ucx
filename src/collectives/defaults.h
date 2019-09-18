/* For license: see LICENSE file at top-level */

#ifndef _COLLECTIVES_DEFAULTS_H
#define _COLLECTIVES_DEFAULTS_H 1

#define COLLECTIVES_DEFAULT_BARRIER          "color_pairwise_exchange_counter"
#define COLLECTIVES_DEFAULT_SYNC             "color_pairwise_exchange_counter"
#define COLLECTIVES_DEFAULT_BARRIER_ALL      "bruck"
#define COLLECTIVES_DEFAULT_SYNC_ALL         "bruck_inplace"
#define COLLECTIVES_DEFAULT_BROADCAST        "binomial_tree"
#define COLLECTIVES_DEFAULT_COLLECT          "binomial_tree"
#define COLLECTIVES_DEFAULT_FCOLLECT         "binomial_tree"
#define COLLECTIVES_DEFAULT_ALLTOALL         "binomial_tree"
#define COLLECTIVES_DEFAULT_ALLTOALLS        "binomial_tree"
#define COLLECTIVES_DEFAULT_REDUCTIONS       "rec_dbl"

#endif /* ! _COLLECTIVES_DEFAULTS_H */
