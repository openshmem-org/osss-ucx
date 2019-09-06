/* For license: see LICENSE file at top-level */

#include "shcoll.h"

#include <stdio.h>
#include <string.h>

/*
 * how to register the collectives with their names
 */

typedef struct unsized_table {
    const char *name;
    void *f;
} unsized_table_t;

typedef struct sized_table {
    const char *name;
    void *f32;
    void *f64;
} sized_table_t;

#define UNSIZED_REG(_type, _name)               \
    { #_name, shcoll_##_type##_##_name }
#define UNSIZED_LAST                             \
    { NULL, NULL }
#define SIZED_REG(_type, _name)                                         \
    { #_name, shcoll_##_type##32##_##_name, shcoll_##_type##64##_##_name }
#define SIZED_LAST                               \
    { NULL, NULL, NULL }

/*
 *
 */

static unsized_table_t
barrier_all_tab[] = {
     UNSIZED_REG(barrier_all, linear),
     UNSIZED_REG(barrier_all, complete_tree),
     UNSIZED_REG(barrier_all, binomial_tree),
     UNSIZED_REG(barrier_all, knomial_tree),
     UNSIZED_REG(barrier_all, dissemination),
     UNSIZED_LAST
};

static unsized_table_t
sync_all_tab[] = {
     UNSIZED_REG(sync_all, linear),
     UNSIZED_REG(sync_all, complete_tree),
     UNSIZED_REG(sync_all, binomial_tree),
     UNSIZED_REG(sync_all, knomial_tree),
     UNSIZED_REG(sync_all, dissemination),
     UNSIZED_LAST
};

static unsized_table_t
barrier_tab[] = {
     UNSIZED_REG(barrier, linear),
     UNSIZED_REG(barrier, complete_tree),
     UNSIZED_REG(barrier, binomial_tree),
     UNSIZED_REG(barrier, knomial_tree),
     UNSIZED_REG(barrier, dissemination),
     UNSIZED_LAST
};

static unsized_table_t
sync_tab[] = {
     UNSIZED_REG(sync, linear),
     UNSIZED_REG(sync, complete_tree),
     UNSIZED_REG(sync, binomial_tree),
     UNSIZED_REG(sync, knomial_tree),
     UNSIZED_REG(sync, dissemination),
     UNSIZED_LAST
};

static sized_table_t
broadcast_tab[] = {
     SIZED_REG(broadcast, linear),
     SIZED_REG(broadcast, complete_tree),
     SIZED_REG(broadcast, binomial_tree),
     SIZED_REG(broadcast, knomial_tree),
     SIZED_REG(broadcast, knomial_tree_signal),
     SIZED_REG(broadcast, scatter_collect),
     SIZED_LAST
};

static sized_table_t
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

static sized_table_t
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

static sized_table_t
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

static sized_table_t
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

/*
 *
 */

inline static void
unsized_lookup(unsized_table_t *tabp,
               const char *name,
               void **fn)
{
    unsized_table_t *up;

    for (up = tabp; up->name != NULL; ++up) {
        if (strncmp(up->name, name, strlen(up->name)) == 0) {
            *fn = (void *) up->f;
            return;
            /* NOT REACHED */
        }
    }
    *fn = NULL;
}

inline static void
sized_lookup(sized_table_t *tabp,
             const char *name,
             void **fn32, void **fn64)
{
    sized_table_t *sp;

    for (sp = tabp; sp->name != NULL; ++sp) {
        if (strncmp(sp->name, name, strlen(sp->name)) == 0) {
            *fn32 = sp->f32;
            *fn64 = sp->f64;
            return;
            /* NOT REACHED */
        }
    }
    *fn32 = *fn64 = NULL;
}

/*
 *
 */

#define UNSIZED_LOOKUP(_coll)                   \
    void                                        \
    _coll##_lookup(const char *name, void **fn) \
    {                                           \
        unsized_lookup(_coll##_tab, name, fn);  \
    }
#define SIZED_LOOKUP(_coll)                                    \
    void                                                       \
    _coll##_lookup(const char *name, void **fn32, void **fn64) \
    {                                                          \
        sized_lookup(_coll##_tab, name, fn32, fn64);           \
    }

UNSIZED_LOOKUP(barrier_all)
UNSIZED_LOOKUP(sync_all)
UNSIZED_LOOKUP(barrier)
UNSIZED_LOOKUP(sync)
SIZED_LOOKUP(broadcast)
SIZED_LOOKUP(alltoall)
SIZED_LOOKUP(alltoalls)
SIZED_LOOKUP(collect)
SIZED_LOOKUP(fcollect)
