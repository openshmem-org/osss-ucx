/* For license: see LICENSE file at top-level */

#include "shcoll.h"
#include "table.h"

#include <stdio.h>
#include <string.h>

/*
 * construct table of known algorithms
 */

#define SIZED_REG(_type, _name)                 \
    { #_name,                                   \
            shcoll_##_type##32##_##_name,       \
            shcoll_##_type##64##_##_name }
#define SIZED_LAST                              \
    { "", NULL, NULL }
#define UNSIZED_REG(_type, _name)               \
    { #_name,                                   \
            shcoll_##_type##_##_name }
#define UNSIZED_LAST                            \
    { "", NULL }

/*
 * known implementations from SHCOLL
 */

static sized_op_t
broadcast_tab[] = {
    SIZED_REG(broadcast, linear),
    SIZED_REG(broadcast, complete_tree),
    SIZED_REG(broadcast, binomial_tree),
    SIZED_REG(broadcast, knomial_tree),
    SIZED_REG(broadcast, knomial_tree_signal),
    SIZED_REG(broadcast, scatter_collect),
    SIZED_LAST
};

static sized_op_t
alltoall_tab[] = {
    SIZED_REG(alltoall, shift_exchange_barrier),
    SIZED_REG(alltoall, shift_exchange_counter),
    SIZED_REG(alltoall, shift_exchange_signal),
    SIZED_REG(alltoall, xor_pairwise_exchange_barrier),
    SIZED_REG(alltoall, color_pairwise_exchange_signal),
    SIZED_REG(alltoall, color_pairwise_exchange_barrier),
    SIZED_REG(alltoall, color_pairwise_exchange_counter),
    SIZED_LAST
};

static sized_op_t
alltoalls_tab[] = {
    SIZED_REG(alltoalls, shift_exchange_barrier),
    SIZED_REG(alltoalls, shift_exchange_counter),
    SIZED_REG(alltoalls, shift_exchange_barrier_nbi),
    SIZED_REG(alltoalls, shift_exchange_counter_nbi),
    SIZED_REG(alltoalls, xor_pairwise_exchange_barrier),
    SIZED_REG(alltoalls, xor_pairwise_exchange_counter),
    SIZED_REG(alltoalls, xor_pairwise_exchange_barrier_nbi),
    SIZED_REG(alltoalls, xor_pairwise_exchange_counter_nbi),
    SIZED_REG(alltoalls, color_pairwise_exchange_barrier),
    SIZED_REG(alltoalls, color_pairwise_exchange_counter),
    SIZED_REG(alltoalls, color_pairwise_exchange_barrier_nbi),
    SIZED_REG(alltoalls, color_pairwise_exchange_counter_nbi),
    SIZED_LAST
};

static sized_op_t
collect_tab[] = {
    SIZED_REG(collect, linear),
    SIZED_REG(collect, all_linear),
    SIZED_REG(collect, all_linear1),
    SIZED_REG(collect, rec_dbl),
    SIZED_REG(collect, rec_dbl_signal),
    SIZED_REG(collect, ring),
    SIZED_REG(collect, bruck),
    SIZED_REG(collect, bruck_no_rotate),
    SIZED_LAST
};

static sized_op_t
fcollect_tab[] = {
    SIZED_REG(fcollect, linear),
    SIZED_REG(fcollect, all_linear),
    SIZED_REG(fcollect, all_linear1),
    SIZED_REG(fcollect, rec_dbl),
    SIZED_REG(fcollect, ring),
    SIZED_REG(fcollect, bruck),
    SIZED_REG(fcollect, bruck_no_rotate),
    SIZED_REG(fcollect, bruck_signal),
    SIZED_REG(fcollect, bruck_inplace),
    SIZED_REG(fcollect, neighbor_exchange),
    SIZED_LAST
};

static unsized_op_t
barrier_all_tab[] = {
    UNSIZED_REG(barrier_all, linear),
    UNSIZED_REG(barrier_all, complete_tree),
    UNSIZED_REG(barrier_all, binomial_tree),
    UNSIZED_REG(barrier_all, knomial_tree),
    UNSIZED_REG(barrier_all, dissemination),
    UNSIZED_LAST
};

static unsized_op_t
sync_all_tab[] = {
    UNSIZED_REG(sync_all, linear),
    UNSIZED_REG(sync_all, complete_tree),
    UNSIZED_REG(sync_all, binomial_tree),
    UNSIZED_REG(sync_all, knomial_tree),
    UNSIZED_REG(sync_all, dissemination),
    UNSIZED_LAST
};

static unsized_op_t
barrier_tab[] = {
    UNSIZED_REG(barrier, linear),
    UNSIZED_REG(barrier, complete_tree),
    UNSIZED_REG(barrier, binomial_tree),
    UNSIZED_REG(barrier, knomial_tree),
    UNSIZED_REG(barrier, dissemination),
    UNSIZED_LAST
};

static unsized_op_t
sync_tab[] = {
    UNSIZED_REG(sync, linear),
    UNSIZED_REG(sync, complete_tree),
    UNSIZED_REG(sync, binomial_tree),
    UNSIZED_REG(sync, knomial_tree),
    UNSIZED_REG(sync, dissemination),
    UNSIZED_LAST
};

/*
 * find the function(s) corresponding to the requested name.
 *
 * return 0 if found, non-zero otherwise
 *
 */

static int
register_sized(sized_op_t *tabp,
               const char *op,
               coll_fn_t *fn32, coll_fn_t *fn64)
{
    sized_op_t *p;

    for (p = tabp; p->f32 != NULL; ++p) {
        if (strncmp(op, p->op, COLL_NAME_MAX) == 0) {
            *fn32 = p->f32;
            *fn64 = p->f64;
            return 0;
            /* NOT REACHED */
        }
    }
    return -1;
}

static int
register_unsized(unsized_op_t *tabp,
                 const char *op,
                 coll_fn_t *fn)
{
    unsized_op_t *p;

    for (p = tabp; p->f != NULL; ++p) {
        if (strncmp(op, p->op, COLL_NAME_MAX) == 0) {
            *fn = p->f;
            return 0;
            /* NOT REACHED */
        }
    }
    return -1;
}

/*
 * global registry
 */
coll_ops_t colls;

/*
 * given name, set up functions
 */

#define REGISTER_SIZED(_coll)                       \
    int                                             \
    register_##_coll(const char *name)              \
    {                                               \
        return register_sized(_coll##_tab, name,    \
                              &colls._coll.f32,     \
                              &colls._coll.f64);    \
    }

#define REGISTER_UNSIZED(_coll)                          \
    int                                                  \
    register_##_coll(const char *name)                   \
    {                                                    \
        return register_unsized(_coll##_tab, name,       \
                                &colls._coll.f);         \
    }

REGISTER_SIZED(alltoall)
REGISTER_SIZED(alltoalls)
REGISTER_SIZED(broadcast)
REGISTER_SIZED(collect)
REGISTER_SIZED(fcollect)

REGISTER_UNSIZED(barrier)
REGISTER_UNSIZED(barrier_all)
REGISTER_UNSIZED(sync)
REGISTER_UNSIZED(sync_all)

/*
 * TODO reductions
 */
