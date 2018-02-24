/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"
#include "shmemu.h"
#include "state.h"
#include "collalgo.h"

#include <sys/types.h>

/*
 * Degree that's used for tree barrier implementation.
 */
static const int broadcast_tree_degree = 2;

inline static int
get_parent_binomial(int node)
{
    return node == 0 ? -1 : node & (node - 1);
}

/*
 * stupid linear broadcast during development
 */

void
broadcast_helper_linear(void *target, const void *source, size_t nbytes,
                        int PE_root, int PE_start,
                        int logPE_stride, int PE_size, long *pSync)
{
    const int stride = 1 << logPE_stride;
    const int root = (PE_root * stride) + PE_start;
    const int me = proc.rank;

    shmemc_barrier(PE_start, logPE_stride, PE_size, pSync);
    if (me != root) {
        shmemc_get(target, source, nbytes, root);
    }
}

/*
 * Broadcast tree implemenation.
 * In the virtual tree, tree root and broadcast root are swapped.
 */

void
broadcast_helper_tree(void *target, const void *source, size_t nbytes,
                        int PE_root, int PE_start,
                        int logPE_stride, int PE_size, long *pSync)
{
    const int me = proc.rank;
    const int stride = 1 << logPE_stride;
    int me_as;
    int children_begin, children_end;
    int num_children;

    /* Get my index after tree root and broadcast root swap */
    me_as = (me - PE_start) / stride;

    if (me_as == PE_root) {
        me_as = 0;
    }
    else if (me_as == 0) {
        me_as = PE_root;
    }

    /* Get information about children */
    num_children =
        shmemc_get_children_info(PE_size, broadcast_tree_degree, me_as,
                                 &children_begin, &children_end);

    /* Wait for the data from the parent */
    if (me_as != 0) {
        int parent;

        shmemc_wait_ne_until64(pSync, SHMEM_SYNC_VALUE);
        source = target;
        /* Send ack */
        parent = (me_as - 1) / broadcast_tree_degree;
        if (parent == PE_root) {
            parent = 0;
        }
        else if (parent == 0) {
            parent = PE_root;
        }
        shmemc_inc64(pSync, PE_start + parent * stride);
    }

    /* Send data to children */
    if (num_children != 0) {
        int child;

        for (child = children_begin; child != children_end; child++) {
            const int dst =
                PE_start + (child == PE_root ? 0 : child) * stride;

            shmemc_put(target, source, nbytes, dst);
        }

        shmemc_fence();

        for (child = children_begin; child != children_end; child++) {
            const int dst =
                PE_start + (child == PE_root ? 0 : child) * stride;

            shmemc_inc64(pSync, dst);
        }

        shmemc_wait_eq_until64(pSync, SHMEM_SYNC_VALUE + num_children +
                                (me_as == 0 ? 0 : 1));
    }

    *pSync = SHMEM_SYNC_VALUE;
}

void
broadcast_helper_binomial(void *target, const void *source, size_t nbytes,
                          int PE_root, int PE_start,
                          int logPE_stride, int PE_size, long *pSync)
{
    static int children[sizeof(int) * 8];
    const int me = proc.rank;
    const int stride = 1 << logPE_stride;
    /* Get my index in the active set */
    int me_as;
    int num_children;

    /* Get my index after tree root and broadcast root swap */
    me_as = (me - PE_start) / stride;

    if (me_as == PE_root) {
        me_as = 0;
    }
    else if (me_as == 0) {
        me_as = PE_root;
    }

    /* Get information about children */
    num_children =
        shmemc_get_children_info_binomial(PE_size, me_as, children);

    /* Wait for the data from the parent */
    if (me_as != 0) {
        int parent;

        shmemc_wait_ne_until64(pSync, SHMEM_SYNC_VALUE);
        source = target;
        /* Send ack */
        parent = get_parent_binomial(me_as);
        if (parent == PE_root) {
            parent = 0;
        }
        else if (parent == 0) {
            parent = PE_root;
        }
        shmemc_inc64(pSync, PE_start + parent * stride);
    }

    /* Send data to children */
    if (num_children != 0) {
        int i;

        for (i = 0; i < num_children; i++) {
            const int dst =
                PE_start + (children[i] == PE_root ? 0 : children[i]) * stride;

            shmemc_put(target, source, nbytes, dst);
            shmemc_fence();
            shmemc_inc64(pSync, dst);
        }

        shmemc_wait_eq_until64(pSync, SHMEM_SYNC_VALUE + num_children +
                                (me_as == 0 ? 0 : 1));
    }

    *pSync = SHMEM_SYNC_VALUE;
}

void
broadcast_helper_binomial2(void *target, const void *source, size_t nbytes,
                          int PE_root, int PE_start,
                          int logPE_stride, int PE_size, long *pSync)
{
    static int children[sizeof(int) * 8];
    const int me = proc.rank;
    const int stride = 1 << logPE_stride;
    int me_as;
    int num_children;

    /* Get my index after tree root and broadcast root swap */
    me_as = (me - PE_start) / stride;

    if (me_as == PE_root) {
        me_as = 0;
    } else if (me_as == 0) {
        me_as = PE_root;
    }

    /* Get information about children */
    num_children =
        shmemc_get_children_info_binomial(PE_size, me_as, children);

    /* Wait for the data form the parent */
    if (me_as != 0) {
        int parent;

        shmemc_wait_ne_until64(pSync, SHMEM_SYNC_VALUE);
        source = target;
        /* Send ack */
        parent = get_parent_binomial(me_as);
        if (parent == PE_root) {
            parent = 0;
        }
        else if (parent == 0) {
            parent = PE_root;
        }
        shmemc_inc64(pSync, PE_start + parent * stride);
    }

    /* Send data to children */
    if (num_children != 0) {
        int i;

        for (i = 0; i < num_children; i++) {
            const int dst =
                PE_start + (children[i] == PE_root ? 0 : children[i]) * stride;

            shmemc_put(target, source, nbytes, dst);
        }

        shmemc_fence();

        for (i = 0; i < num_children; i++) {
            const int dst =
                PE_start + (children[i] == PE_root ? 0 : children[i]) * stride;

            shmemc_inc64(pSync, dst);
        }

        shmemc_wait_eq_until64(pSync, SHMEM_SYNC_VALUE + num_children +
                                (me_as == 0 ? 0 : 1));
    }

    *pSync = SHMEM_SYNC_VALUE;
}

/* -------------------------------------------------------------------- */

/*
 * chosen implementation
 */
static void (*broadcast_helper)(void *target, const void *source,
                                size_t nelems,
                                int PE_root, int PE_start,
                                int logPE_stride, int PE_size,
                                long *pSync);

/*
 * internal psyncs
 */
long shmemc_all_broadcast = SHMEM_SYNC_VALUE;

/*
 * API
 */

void
shmemc_broadcast_init(void)
{
    switch (proc.env.broadcast_algo) {
    case SHMEMC_COLL_LINEAR:
        broadcast_helper = broadcast_helper_linear;
        break;
    case SHMEMC_COLL_TREE:
        broadcast_helper = broadcast_helper_tree;
        break;
    case SHMEMC_COLL_BINOMIAL:
        broadcast_helper = broadcast_helper_binomial;
        break;
    case SHMEMC_COLL_BINOMIAL2:
        broadcast_helper = broadcast_helper_binomial2;
        break;
    default:
        /* error */
        break;
    }
}

void
shmemc_broadcast_finalize(void)
{
    /* nothing to do as of now */
}

/* -------------------------------------------------------------------- */

#define SHMEMC_BROADCAST_TYPE(_name, _size)                             \
    void                                                                \
    shmemc_broadcast##_name(void *target, const void *source,           \
                            size_t nelems,                              \
                            int PE_root, int PE_start,                  \
                            int logPE_stride, int PE_size,              \
                            long *pSync)                                \
    {                                                                   \
        const int sized_nelems = nelems * _size;                        \
                                                                        \
        broadcast_helper(target, source,                                \
                         sized_nelems,                                  \
                         PE_root, PE_start,                             \
                         logPE_stride, PE_size,                         \
                         pSync);                                        \
    }

SHMEMC_BROADCAST_TYPE(32, 4)
SHMEMC_BROADCAST_TYPE(64, 8)
