//
// Created by Srdan Milakovic on 6/11/18.
//

#include "broadcast-size.h"
#include "trees.h"
#include "../shcoll.h"

#include "shmem.h"

const int binomial_tree_radix = 8;

void broadcast_size(size_t *value, int PE_root, int PE_start, int logPE_stride, int PE_size, long *pSync) {
    const int me = shmem_my_pe();
    const int stride = 1 << logPE_stride;

    /* Get my index in the active set */
    const int me_as = (me - PE_start) / stride;

    int i;
    node_info_knomial_t node;

    /* Get node info */
    get_node_info_knomial_root(PE_size, PE_root, binomial_tree_radix, me_as, &node);

    /* Wait for the data from the parent */
    if (me != PE_root) {
        shmem_long_wait_until(pSync, SHMEM_CMP_NE, SHCOLL_SYNC_VALUE);
        *value = ((size_t) *pSync) - 1;
    }

    /* Send data to children */
    for (i = 0; i < node.children_num; i++) {
        shmem_size_p((size_t *) pSync, *value + 1, PE_start + node.children[i] * stride);
    }

    shmem_long_p(pSync, SHCOLL_SYNC_VALUE, me);
}
