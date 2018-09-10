/* For license: see LICENSE file at top-level */

#include "collectives/shcoll-shim.h"

/*
 * alltoall:
 *
 * shift_exchange_barrier
 * shift_exchange_counter
 * shift_exchange_signal
 * xor_pairwise_exchange_barrier
 * color_pairwise_exchange_signal
 * color_pairwise_exchange_barrier
 * color_pairwise_exchange_counter
 *
 */
SHIM_ALLTOALL(xor_pairwise_exchange_barrier, 32)
SHIM_ALLTOALL(xor_pairwise_exchange_barrier, 64)

/*
 * alltoalls:
 *
 * shift_exchange_barrier
 * shift_exchange_counter
 * shift_exchange_barrier_nbi
 * shift_exchange_counter_nbi
 * xor_pairwise_exchange_barrier
 * xor_pairwise_exchange_counter
 * xor_pairwise_exchange_barrier_nbi
 * xor_pairwise_exchange_counter_nbi
 * color_pairwise_exchange_barrier
 * color_pairwise_exchange_counter
 * color_pairwise_exchange_barrier_nbi
 * color_pairwise_exchange_counter_nbi
 *
 */
SHIM_ALLTOALLS(xor_pairwise_exchange_barrier, 32)
SHIM_ALLTOALLS(xor_pairwise_exchange_barrier, 64)

/*
 * collect:
 *
 * linear
 * all_linear
 * all_linear1
 * rec_dbl
 * rec_dbl_signal
 * ring
 * bruck
 * bruck_no_rotate
 *
 */
SHIM_COLLECT(bruck, 32)
SHIM_COLLECT(bruck, 64)

/*
 * fcollect:
 *
 * linear
 * all_linear
 * all_linear1
 * rec_dbl
 * rec_dbl_signal
 * ring
 * bruck
 * bruck_no_rotate
 * bruck_signal
 * bruck_inplace
 * neighbor_exchange
 *
 */
SHIM_FCOLLECT(bruck_inplace, 32)
SHIM_FCOLLECT(bruck_inplace, 64)

/*
 * barrier/sync:
 *
 * linear
 * complete_tree
 * binomial_tree
 * knomial_tree
 * dissemination
 *
 */
SHIM_BARRIER(binomial_tree)
SHIM_SYNC(binomial_tree)

/*
 * broadcast:
 *
 * linear
 * complete_tree
 * binomial_tree
 * knomial_tree
 * knomial_tree_signal
 * scatter_collect
 *
 */
SHIM_BROADCAST(binomial_tree, 32)
SHIM_BROADCAST(binomial_tree, 64)

/*
 * reductions:
 *
 * linear
 * binomial
 * rec_dbl
 * rabenseifner
 * rabenseifner2
 *
 */
SHIM_REDUCE_ALL(rec_dbl)
