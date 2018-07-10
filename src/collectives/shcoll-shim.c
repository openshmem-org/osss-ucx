#include "collectives/shcoll-shim.h"

SHIM_ALLTOALL(xor_pairwise_exchange_barrier, 32)
SHIM_ALLTOALL(xor_pairwise_exchange_barrier, 64)

SHIM_ALLTOALLS(xor_pairwise_exchange_barrier, 32)
SHIM_ALLTOALLS(xor_pairwise_exchange_barrier, 64)

SHIM_COLLECT(bruck, 32)
SHIM_COLLECT(bruck, 64)

SHIM_FCOLLECT(bruck_inplace, 32)
SHIM_FCOLLECT(bruck_inplace, 64)

SHIM_BARRIER(linear)
SHIM_SYNC(binomial_tree)

SHIM_BROADCAST(binomial_tree, 32)
SHIM_BROADCAST(binomial_tree, 64)

SHIM_REDUCE_ALL(rec_dbl)

