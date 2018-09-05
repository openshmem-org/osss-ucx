#include "shcoll.h"
#include "shcoll/compat.h"
#include "util/trees.h"

#include <stdio.h>

static int tree_degree_broadcast = 2;
static int knomial_tree_radix_barrier = 2;

void
shcoll_set_broadcast_tree_degree(int tree_degree)
{
    tree_degree_broadcast = tree_degree;
}

void
shcoll_set_broadcast_knomial_tree_radix_barrier(int tree_radix)
{
    knomial_tree_radix_barrier = tree_radix;
}


inline static void
broadcast_helper_linear(void *target, const void *source, size_t nbytes,
                        int PE_root, int PE_start,
                        int logPE_stride, int PE_size,
                        long *pSync)
{
    const int stride = 1 << logPE_stride;
    const int root = (PE_root * stride) + PE_start;
    const int me = shmem_my_pe();

    shcoll_barrier_linear(PE_start, logPE_stride, PE_size, pSync);
    if (me != root) {
        shmem_getmem(target, source, nbytes, root);
    }
    shcoll_barrier_linear(PE_start, logPE_stride, PE_size, pSync);
}


inline static void
broadcast_helper_complete_tree(void *target, const void *source,
                               size_t nbytes,
                               int PE_root, int PE_start,
                               int logPE_stride, int PE_size,
                               long *pSync)
{
    const int me = shmem_my_pe();
    const int stride = 1 << logPE_stride;

    int child;
    int dst;
    node_info_complete_t node;

    /* Get my index in the active set */
    int me_as = (me - PE_start) / stride;

    /* Get information about children */
    get_node_info_complete_root(PE_size, PE_root,
                                tree_degree_broadcast,
                                me_as,
                                &node);

    /* Wait for the data form the parent */
    if (PE_root != me) {
        shmem_long_wait_until(pSync, SHMEM_CMP_NE, SHCOLL_SYNC_VALUE);
        source = target;

        /* Send ack */
        shmem_long_atomic_inc(pSync, PE_start + node.parent * stride);
    }

    /* Send data to children */
    if (node.children_num != 0) {
        for (child = node.children_begin;
             child != node.children_end;
             child = (child + 1) % PE_size) {
            dst = PE_start + child * stride;
            shmem_putmem_nbi(target, source, nbytes, dst);
        }

        shmem_fence();

        for (child = node.children_begin;
             child != node.children_end;
             child = (child + 1) % PE_size) {
            dst = PE_start + child * stride;
            shmem_long_atomic_inc(pSync, dst);
        }

        shmem_long_wait_until(pSync, SHMEM_CMP_EQ, SHCOLL_SYNC_VALUE + node.children_num + (PE_root == me ? 0 : 1));
    }

    shmem_long_p(pSync, SHCOLL_SYNC_VALUE, me);
}


inline static void
broadcast_helper_binomial_tree(void *target, const void *source,
                               size_t nbytes,
                               int PE_root, int PE_start,
                               int logPE_stride, int PE_size,
                               long *pSync)
{
    const int me = shmem_my_pe();
    const int stride = 1 << logPE_stride;
    int i;
    int parent;
    int dst;
    node_info_binomial_t node;
    /* Get my index in the active set */
    int me_as = (me - PE_start) / stride;

    /* Get information about children */
    get_node_info_binomial_root(PE_size, PE_root, me_as, &node);

    /* Wait for the data form the parent */
    if (me_as != PE_root) {
        shmem_long_wait_until(pSync, SHMEM_CMP_NE, SHCOLL_SYNC_VALUE);
        source = target;

        /* Send ack */
        parent = node.parent;
        shmem_long_atomic_inc(pSync, PE_start + parent * stride);
    }

    /* Send data to children */
    if (node.children_num != 0) {
        for (i = 0; i < node.children_num; i++) {
            dst = PE_start + node.children[i] * stride;
            shmem_putmem_nbi(target, source, nbytes, dst);
            shmem_fence();
            shmem_long_atomic_inc(pSync, dst);
        }

        shmem_long_wait_until(pSync, SHMEM_CMP_EQ, SHCOLL_SYNC_VALUE + node.children_num + (me_as == PE_root ? 0 : 1));
    }

    shmem_long_p(pSync, SHCOLL_SYNC_VALUE, me);
}

inline static void
broadcast_helper_knomial_tree(void *target, const void *source,
                              size_t nbytes,
                              int PE_root, int PE_start,
                              int logPE_stride, int PE_size,
                              long *pSync)
{
    const int me = shmem_my_pe();
    const int stride = 1 << logPE_stride;
    int i, j;
    int parent;
    int child_offset;
    int dst_pe;
    node_info_knomial_t node;
    /* Get my index in the active set */
    int me_as = (me - PE_start) / stride;

    /* Get information about children */
    get_node_info_knomial_root(PE_size, PE_root,
                               knomial_tree_radix_barrier,
                               me_as,
                               &node);

    /* Wait for the data form the parent */
    if (me_as != PE_root) {
        shmem_long_wait_until(pSync, SHMEM_CMP_NE, SHCOLL_SYNC_VALUE);
        source = target;

        /* Send ack */
        parent = node.parent;
        shmem_long_atomic_inc(pSync, PE_start + parent * stride);
    }

    /* Send data to children */
    if (node.children_num != 0) {
        child_offset = 0;

        for (i = 0; i < node.groups_num; i++) {
            for (j = 0; j < node.groups_sizes[i]; j++) {
                dst_pe = PE_start + node.children[child_offset + j] * stride;
                shmem_putmem_nbi(target, source, nbytes, dst_pe);
            }

            shmem_fence();

            for (j = 0; j < node.groups_sizes[i]; j++) {
                dst_pe = PE_start + node.children[child_offset + j] * stride;
                shmem_long_atomic_inc(pSync, dst_pe);
            }

            child_offset += node.groups_sizes[i];
        }

        shmem_long_wait_until(pSync, SHMEM_CMP_EQ, SHCOLL_SYNC_VALUE + node.children_num + (me_as == PE_root ? 0 : 1));
    }

    shmem_long_p(pSync, SHCOLL_SYNC_VALUE, me);
}

inline static void
broadcast_helper_knomial_tree_signal(void *target, const void *source,
                                     size_t nbytes,
                                     int PE_root,int PE_start,
                                     int logPE_stride, int PE_size,
                                     long *pSync)
{
    const int me = shmem_my_pe();
    const int stride = 1 << logPE_stride;
    int i, j;
    int parent;
    int child_offset;
    int dest_pe;
    node_info_knomial_t node;
    /* Get my index in the active set */
    int me_as = (me - PE_start) / stride;

    /* Get information about children */
    get_node_info_knomial_root(PE_size, PE_root,
                               knomial_tree_radix_barrier,
                               me_as,
                               &node);

    /* Wait for the data form the parent */
    if (me_as != PE_root) {
        shmem_long_wait_until(pSync, SHMEM_CMP_NE, SHCOLL_SYNC_VALUE);
        source = target;

        /* Send ack */
        parent = node.parent;
        shmem_long_atomic_inc(pSync, PE_start + parent * stride);
    }

    /* Send data to children */
    if (node.children_num != 0) {
        child_offset = 0;

        for (i = 0; i < node.groups_num; i++) {
            for (j = 0; j < node.groups_sizes[i]; j++) {
                dest_pe = PE_start + node.children[child_offset + j] * stride;

                shmem_putmem_signal_nb(target, source, nbytes,
                                       (uint64_t *) pSync,
                                       SHCOLL_SYNC_VALUE + 1, dest_pe, NULL);
            }

            child_offset += node.groups_sizes[i];
        }

        shmem_long_wait_until(pSync, SHMEM_CMP_EQ, SHCOLL_SYNC_VALUE + node.children_num + (me_as == PE_root ? 0 : 1));
    }

    shmem_long_p(pSync, SHCOLL_SYNC_VALUE, me);
}

inline static void
broadcast_helper_scatter_collect(void *target, const void *source,
                                 size_t nbytes,
                                 int PE_root, int PE_start,
                                 int logPE_stride, int PE_size,
                                 long *pSync)
{
    /* TODO: Optimize cases where data_start == data_end (block has size 0) */

    const int me = shmem_my_pe();
    const int stride = 1 << logPE_stride;
    int root_as = (PE_root - PE_start) / stride;
    /* Get my index in the active set */
    int me_as = (me - PE_start) / stride;

    /* Shift me_as so that me_as for PE_root is 0 */
    me_as = (me_as - root_as + PE_size) % PE_size;

    /* The number of received blocks (scatter + collect) */
    int total_received = me_as == 0 ? PE_size : 0;

    int target_pe;
    int next_as = (me_as + 1) % PE_size;
    int next_pe = PE_start + (root_as + next_as) % PE_size * stride;

    /* The index of the block that should be send to next_pe */
    int next_block = me_as;

    /* The number of blocks that next received */
    int next_pe_nblocks = next_as == 0 ? PE_size : 0;

    int left = 0;
    int right = PE_size;
    int mid;
    int dist;

    size_t data_start;
    size_t data_end;

    /* Used in the collect part to wait for new blocks */
    long ring_received = SHCOLL_SYNC_VALUE;

    if (me_as != 0) {
        source = target;
    }

    /* Scatter data to other PEs using binomial tree */
    while (right - left > 1) {
        /* dist = ceil((right - let) / 2) */
        dist = ((right - left) >> 1) + ((right - left) & 0x1);
        mid = left + dist;

        /* Send (right - mid) elements starting with mid to pe + dist */
        if (me_as == left && me_as + dist < right) {
            /* TODO: possible overflow */
            data_start = (mid * nbytes + PE_size - 1) / PE_size;
            data_end = (right * nbytes + PE_size - 1) / PE_size;
            target_pe = PE_start + (root_as + me_as + dist) % PE_size * stride;

            shmem_putmem_nbi((char *) target + data_start,
                             (char *) source + data_start,
                             data_end - data_start, target_pe);
            shmem_fence();
            shmem_long_atomic_inc(pSync, target_pe);
        }

        /* Send (right - mid) elements starting with mid from (me_as - dist) */
        if (me_as - dist == left) {
            shmem_long_wait_until(pSync, SHMEM_CMP_NE, SHCOLL_SYNC_VALUE);
            total_received = right - mid;
        }

        if (next_as - dist == left) {
            next_pe_nblocks = right - mid;
        }

        if (me_as < mid) {
            right = mid;
        } else {
            left = mid;
        }
    }

    /* Do collect using (modified) ring algorithm */
    while (next_pe_nblocks != PE_size) {
        data_start = (next_block * nbytes + PE_size - 1) / PE_size;
        data_end = ((next_block + 1) * nbytes + PE_size - 1) / PE_size;

        shmem_putmem_nbi((char *) target + data_start,
                         (char *) source + data_start,
                         data_end - data_start, next_pe);
        shmem_fence();
        shmem_long_atomic_inc(pSync + 1, next_pe);


        next_pe_nblocks++;
        next_block = (next_block - 1 + PE_size) % PE_size;

        /*
         * If we did not receive all blocks, we must wait for the next
         * block we want to send
         */
        if (total_received != PE_size) {
            shmem_long_wait_until(pSync + 1, SHMEM_CMP_GT, ring_received);
            ring_received++;
            total_received++;
        }
    }

    while (total_received != PE_size) {
        shmem_long_wait_until(pSync + 1, SHMEM_CMP_GT, ring_received);
        ring_received++;
        total_received++;
    }

    /* TODO: maybe only one pSync is enough */
    shmem_long_p(pSync + 0, SHCOLL_SYNC_VALUE, me);
    shmem_long_p(pSync + 1, SHCOLL_SYNC_VALUE, me);
}

#define SHCOLL_BROADCAST_DEFINITION(_name, _size)                       \
    void                                                                \
    shcoll_broadcast##_size##_##_name(void *dest, const void *source,   \
                                      size_t nelems,                    \
                                      int PE_root, int PE_start,        \
                                      int logPE_stride, int PE_size,    \
                                      long *pSync)                      \
    {                                                                   \
        broadcast_helper_##_name(dest, source,                          \
                                 (_size) / CHAR_BIT * nelems,           \
                                 PE_root, PE_start,                     \
                                 logPE_stride, PE_size,                 \
                                 pSync);                                \
    }                                                                   \

/* @formatter:off */

SHCOLL_BROADCAST_DEFINITION(linear, 8)
SHCOLL_BROADCAST_DEFINITION(linear, 16)
SHCOLL_BROADCAST_DEFINITION(linear, 32)
SHCOLL_BROADCAST_DEFINITION(linear, 64)

SHCOLL_BROADCAST_DEFINITION(complete_tree, 8)
SHCOLL_BROADCAST_DEFINITION(complete_tree, 16)
SHCOLL_BROADCAST_DEFINITION(complete_tree, 32)
SHCOLL_BROADCAST_DEFINITION(complete_tree, 64)

SHCOLL_BROADCAST_DEFINITION(binomial_tree, 8)
SHCOLL_BROADCAST_DEFINITION(binomial_tree, 16)
SHCOLL_BROADCAST_DEFINITION(binomial_tree, 32)
SHCOLL_BROADCAST_DEFINITION(binomial_tree, 64)

SHCOLL_BROADCAST_DEFINITION(knomial_tree, 8)
SHCOLL_BROADCAST_DEFINITION(knomial_tree, 16)
SHCOLL_BROADCAST_DEFINITION(knomial_tree, 32)
SHCOLL_BROADCAST_DEFINITION(knomial_tree, 64)

SHCOLL_BROADCAST_DEFINITION(knomial_tree_signal, 8)
SHCOLL_BROADCAST_DEFINITION(knomial_tree_signal, 16)
SHCOLL_BROADCAST_DEFINITION(knomial_tree_signal, 32)
SHCOLL_BROADCAST_DEFINITION(knomial_tree_signal, 64)

SHCOLL_BROADCAST_DEFINITION(scatter_collect, 8)
SHCOLL_BROADCAST_DEFINITION(scatter_collect, 16)
SHCOLL_BROADCAST_DEFINITION(scatter_collect, 32)
SHCOLL_BROADCAST_DEFINITION(scatter_collect, 64)

/* @formatter:on */
