/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"
#include "state.h"
#include "memfence.h"

#include "shmem/defs.h"

/*
 * just play with a simple linear barrier for now
 */

inline static void
barrier_sync_helper_linear(int start, int log2stride, int size, long *pSync)
{
    const int me = proc.rank;
    const int stride = 1 << log2stride;

    if (start == me) {
        const int npokes = size - 1;
        long poke = *pSync + 1;
        int pe;
        int i;

        /* wait for the rest of the AS to poke me */
        shmemc_wait_eq_until64(pSync, npokes + SHMEM_SYNC_VALUE);
        *pSync = SHMEM_SYNC_VALUE;

        /* send acks out */
        pe = start + stride;
        for (i = 1; i < size; i += 1) {
            shmemc_put(pSync, &poke, sizeof(poke), pe);
            pe += stride;
        }
    }
    else {
        /* poke root */
        shmemc_inc64(pSync, start);

        /* get ack */
        shmemc_wait_ne_until64(pSync, SHMEM_SYNC_VALUE);
        *pSync = SHMEM_SYNC_VALUE;
    }
}

/* -------------------------------------------------------------------- */

/*
 * Tree-based barrier
 */

/*
 * tree radix
 */
static const int tree_degree = 2;

inline static void
barrier_sync_helper_tree(int start, int log2stride, int size, long *pSync)
{
    const int me = proc.rank;
    const int stride = 1 << log2stride;
    const int me_as = (me - start) / stride; /* Get my index in the active set */
    /* Calculate parent's index in the active set */
    const int parent_idx = me_as != 0 ? (me_as - 1) / tree_degree : -1;
    /* Get information about children */
    int children_begin, children_end;
    long npokes;
    int child;

    npokes = shmemu_get_children_info(size, tree_degree, me_as,
                                      &children_begin, &children_end);

    /* Wait for pokes from the children */
    if (npokes != 0) {
        shmemc_wait_eq_until64(pSync, SHMEM_SYNC_VALUE + npokes);
    }

    if (parent_idx != -1) {
        /* Poke the parent exists */
        (void) shmemc_finc64(pSync, start + parent_idx * stride);

        /* Wait for the poke from parent */
        shmemc_wait_eq_until64(pSync, SHMEM_SYNC_VALUE + npokes + 1);
    }

    /* Clear pSync and poke the children */
    *pSync = SHMEM_SYNC_VALUE;
    for (child = children_begin; child != children_end; child++) {
        shmemc_inc64(pSync, start + child * stride);
    }
}

/* -------------------------------------------------------------------- */

/*
 * Dissemination barrier implementation
 */

inline static void
barrier_sync_helper_dissemination(int start, int log2stride,
                                  int size, long *pSync)
{
    const int me = proc.rank;
    const int stride = 1 << log2stride;
    const int me_as = (me - start) / stride; /* my index in active set */
    int round, distance;

    for (round = 0, distance = 1; distance < size; round++, distance <<= 1) {
        const int target_as = (me_as + distance) % size;

        /* Poke the target for the current round */
        shmemc_inc64(&pSync[round], start + target_as * stride);

        /* Wait until poked in this round */
        shmemc_wait_ne_until64(&pSync[round], SHMEM_SYNC_VALUE);

        /* Reset pSync element, fadd is used instead of add because we have to */
        /* be sure that reset happens before next invocation of barrier */
        (void) shmemc_fadd64(&pSync[round], -1, me);
    }
}

/* -------------------------------------------------------------------- */

/*
 * chosen implementation.  Later can be selected through e.g. env var
 */
static void (*barrier_sync_helper)(int start, int log2stride,
                                   int size, long *pSync);

/*
 * internal psyncs
 */
long shmemc_all_barrier = SHMEM_SYNC_VALUE;
long shmemc_all_sync = SHMEM_SYNC_VALUE;

/*
 * API
 */

void
shmemc_barrier_init(void)
{
    switch (proc.env.barrier_algo) {
    case SHMEMC_COLL_LINEAR:
        barrier_sync_helper = barrier_sync_helper_linear;
        break;
    case SHMEMC_COLL_TREE:
        barrier_sync_helper = barrier_sync_helper_tree;
        break;
    case SHMEMC_COLL_DISSEM:
        barrier_sync_helper = barrier_sync_helper_dissemination;
        break;
    default:
        /* error */
        break;
    }
}

void
shmemc_barrier_finalize(void)
{
    /* nothing to do as of now */
}

void
shmemc_barrier(int start, int log2stride, int size, long *pSync)
{
    shmemc_quiet();

    barrier_sync_helper(start, log2stride, size, pSync);
}

void
shmemc_barrier_all(void)
{
    shmemc_barrier(0, 0, proc.nranks, &shmemc_all_barrier);
}

void
shmemc_sync(int start, int log2stride, int size, long *pSync)
{
    LOAD_STORE_FENCE();

    barrier_sync_helper(start, log2stride, size, pSync);
}

void
shmemc_sync_all(void)
{
    shmemc_sync(0, 0, proc.nranks, &shmemc_all_sync);
}
