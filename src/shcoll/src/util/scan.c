//
// Created by Srdan Milakovic on 5/24/18.
//

#include "../shcoll.h"
#include "scan.h"

#include "shmem.h"

void exclusive_prefix_sum(size_t *dest, size_t value, int PE_start, int logPE_stride, int PE_size, long *pSync) {
    const int stride = 1 << logPE_stride;
    const int me = shmem_my_pe();
    const int me_as = (me - PE_start) / stride;

    size_t *scan_rounds = (size_t *) pSync;
    size_t partial_scan = value;
    int dist = 1;
    int round = 0;
    int parent;
    int mask = 0x1;

    parent = me_as + 1;

    if (parent < PE_size) {
        shmem_size_atomic_add(scan_rounds, value + 1, parent);
    }

    while (mask < PE_size) {
        if (me_as - dist >= 0) {
            shmem_size_wait_until(&scan_rounds[round], SHMEM_CMP_NE, SHCOLL_SYNC_VALUE);
            partial_scan += scan_rounds[round] - 1 - SHCOLL_SYNC_VALUE;

            shmem_size_p(scan_rounds + round, SHCOLL_SYNC_VALUE, me);
            shmem_size_wait_until(scan_rounds + round, SHMEM_CMP_EQ, SHCOLL_SYNC_VALUE);
        }

        dist <<= 1;
        round++;

        parent = me_as + dist;
        if (parent < PE_size) {
            shmem_size_atomic_add(&scan_rounds[round], partial_scan + 1, parent);
        }

        mask <<= 1;
    }

    *dest = partial_scan - value;
}
