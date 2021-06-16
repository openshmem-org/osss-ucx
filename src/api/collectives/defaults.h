/* For license: see LICENSE file at top-level */

#ifndef _COLLECTIVES_DEFAULTS_H
#define _COLLECTIVES_DEFAULTS_H 1

#define COLLECTIVES_DEFAULT_ALLTOALL         "shift_exchange_barrier"
#define COLLECTIVES_DEFAULT_ALLTOALLS        "shift_exchange_barrier"
#define COLLECTIVES_DEFAULT_BARRIER          "binomial_tree"
#define COLLECTIVES_DEFAULT_BARRIER_ALL      "binomial_tree"
#define COLLECTIVES_DEFAULT_SYNC             "binomial_tree"
#define COLLECTIVES_DEFAULT_SYNC_ALL         "binomial_tree"
#define COLLECTIVES_DEFAULT_BROADCAST        "binomial_tree"
#define COLLECTIVES_DEFAULT_COLLECT          "bruck"
#define COLLECTIVES_DEFAULT_FCOLLECT         "bruck_inplace"
#define COLLECTIVES_DEFAULT_REDUCTIONS       "rec_dbl"

#endif /* ! _COLLECTIVES_DEFAULTS_H */
