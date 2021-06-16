//
// Created by Srdan Milakovic on 5/15/18.
//

#include "shcoll.h"
#include "shcoll/compat.h"
#include "util/rotate.h"
#include "util/scan.h"
#include "util/broadcast-size.h"

#include <string.h>
#include <limits.h>
#include <assert.h>

inline static void
collect_helper_linear(void *dest, const void *source, size_t nbytes,
                      int PE_start, int logPE_stride, int PE_size,
                      long *pSync)
{
    /* pSync[0] is used for barrier
     * pSync[1] is used for broadcast
     * next sizeof(size_t) bytes are used for the offset */

    const int stride = 1 << logPE_stride;
    const int me = shmem_my_pe();
    const int me_as = (me - PE_start) / stride;
    size_t *offset = (size_t *) (pSync + 2);
    int i;

    /* set offset to 0 */
    shmem_size_p(offset, 0, me);
    shcoll_barrier_linear(PE_start, logPE_stride, PE_size, pSync);

    if (me_as == 0) {
        shmem_size_atomic_add(offset, nbytes + 1, me + stride);
        memcpy(dest, source, nbytes);

        /* Wait for the full array size and notify everybody */
        shmem_size_wait_until(offset, SHMEM_CMP_NE, 0);

        /* Send offset to everybody */
        for (i = 1; i < PE_size; i++) {
            shmem_size_p(offset, *offset, PE_start + i * stride);
        }
    } else {
        shmem_size_wait_until(offset, SHMEM_CMP_NE, 0);

        /* Write data to PE 0 */
        shmem_putmem_nbi((char *) dest + *offset - 1, source, nbytes, PE_start);

        /* Send offset to the next PE, PE_start will contain full array size */
        shmem_size_atomic_add(offset, nbytes + *offset, PE_start + ((me_as + 1) % PE_size) * stride);
    }

    /* Wait for all PEs to send the data to PE_start */
    shcoll_barrier_linear(PE_start, logPE_stride, PE_size, pSync);

    shcoll_broadcast8_linear(dest, dest, *offset - 1, PE_start, PE_start, logPE_stride, PE_size, pSync + 1);

    shmem_size_p(offset, SHCOLL_SYNC_VALUE, me);
}

inline static void
collect_helper_all_linear(void *dest, const void *source, size_t nbytes,
                          int PE_start, int logPE_stride, int PE_size,
                          long *pSync)
{
    /* pSync[0] is used for counting received messages
     * pSync[1..1+PREFIX_SUM_SYNC_SIZE) is used for prefix sum
     * next sizeof(size_t) bytes are used for the offset */

    const int stride = 1 << logPE_stride;
    const int me = shmem_my_pe();
    const int me_as = (me - PE_start) / stride;
    size_t block_offset;

    int i;
    int target;

    exclusive_prefix_sum(&block_offset, nbytes, PE_start, logPE_stride, PE_size, pSync + 1);

    for (i = 1; i < PE_size; i++) {
        target = PE_start + ((i + me_as) % PE_size) * stride;
        shmem_putmem_nbi((char*) dest + block_offset, source, nbytes, target);
    }

    memcpy((char*) dest + block_offset, source, nbytes);

    shmem_fence();

    for (i = 1; i < PE_size; i++) {
        target = PE_start + ((i + me_as) % PE_size) * stride;
        shmem_long_atomic_inc(pSync, target);
    }

    shmem_long_wait_until(pSync, SHMEM_CMP_EQ, SHCOLL_SYNC_VALUE + PE_size - 1);
    shmem_long_p(pSync, SHCOLL_SYNC_VALUE, me);
}

inline static void
collect_helper_all_linear1(void *dest, const void *source, size_t nbytes,
                           int PE_start, int logPE_stride, int PE_size,
                           long *pSync)
{
    /* pSync[0] is used for barrier
     * pSync[1..1+PREFIX_SUM_SYNC_SIZE) is used for prefix sum
     * next sizeof(size_t) bytes are used for the offset */

    const int stride = 1 << logPE_stride;
    const int me = shmem_my_pe();
    const int me_as = (me - PE_start) / stride;
    size_t block_offset;

    int i;
    int target;

    exclusive_prefix_sum(&block_offset, nbytes, PE_start, logPE_stride, PE_size, pSync + 1);

    for (i = 1; i < PE_size; i++) {
        target = PE_start + ((i + me_as) % PE_size) * stride;
        shmem_putmem_nbi((char*) dest + block_offset, source, nbytes, target);
    }

    memcpy((char*) dest + block_offset, source, nbytes);

    shcoll_barrier_binomial_tree(PE_start, logPE_stride, PE_size, pSync);
}

inline static void
collect_helper_rec_dbl(void *dest, const void *source, size_t nbytes,
                       int PE_start, int logPE_stride, int PE_size,
                       long *pSync)
{
    const int stride = 1 << logPE_stride;
    const int me = shmem_my_pe();

    /* Get my index in the active set */
    int me_as = (me - PE_start) / stride;
    int mask;
    int peer;
    int i;
    size_t round_block_size;
    size_t block_offset;
    size_t block_size = nbytes;

    /* pSync */
    long *prefix_sum_pSync = pSync;
    size_t *block_sizes = (size_t *) (prefix_sum_pSync + PREFIX_SUM_SYNC_SIZE);

    assert(((PE_size - 1) & PE_size) == 0);

    exclusive_prefix_sum(&block_offset, nbytes, PE_start, logPE_stride, PE_size, prefix_sum_pSync);

    memcpy((char*) dest + block_offset, source, nbytes);

    for (mask = 0x1, i = 0; mask < PE_size; mask <<= 1, i++) {
        peer = PE_start + (me_as ^ mask) * stride;

        shmem_putmem_nbi((char*) dest + block_offset, (char*) dest + block_offset, block_size, peer);
        shmem_fence();
        shmem_size_p(block_sizes + i, block_size + 1 + SHCOLL_SYNC_VALUE, peer);

        shmem_size_wait_until(block_sizes + i, SHMEM_CMP_NE, SHCOLL_SYNC_VALUE);
        round_block_size = *(block_sizes + i) - 1;
        shmem_size_p(block_sizes + i, SHCOLL_SYNC_VALUE, me);

        if (me > peer) {
            block_offset -= round_block_size;
        }
        block_size += round_block_size;
    }
}

inline static void
collect_helper_rec_dbl_signal(void *dest, const void *source, size_t nbytes,
                              int PE_start, int logPE_stride, int PE_size,
                              long *pSync)
{
    const int stride = 1 << logPE_stride;
    const int me = shmem_my_pe();

    /* Get my index in the active set */
    int me_as = (me - PE_start) / stride;
    int mask;
    int peer;
    int i;
    size_t round_block_size;
    size_t block_offset;
    size_t block_size = nbytes;

    /* pSync */
    long *prefix_sum_pSync = pSync;
    size_t *block_sizes = (size_t *) (prefix_sum_pSync + PREFIX_SUM_SYNC_SIZE);

    assert(((PE_size - 1) & PE_size) == 0);

    exclusive_prefix_sum(&block_offset, nbytes, PE_start, logPE_stride, PE_size, prefix_sum_pSync);

    memcpy((char*) dest + block_offset, source, nbytes);

    for (mask = 0x1, i = 0; mask < PE_size; mask <<= 1, i++) {
        peer = PE_start + (me_as ^ mask) * stride;

        shmem_putmem_signal_nb((char*) dest + block_offset, (char*) dest + block_offset, block_size,
                               (uint64_t *) (block_sizes + i), block_size + 1 + SHCOLL_SYNC_VALUE, peer, NULL);

        shmem_size_wait_until(block_sizes + i, SHMEM_CMP_NE, SHCOLL_SYNC_VALUE);
        round_block_size = *(block_sizes + i) - 1 - SHCOLL_SYNC_VALUE;
        shmem_size_p(block_sizes + i, SHCOLL_SYNC_VALUE, me);

        if (me > peer) {
            block_offset -= round_block_size;
        }
        block_size += round_block_size;
    }
}


/* TODO Find a better way to choose this value */
#define RING_DIFF 10

inline static void
collect_helper_ring(void *dest, const void *source, size_t nbytes,
                    int PE_start, int logPE_stride, int PE_size,
                    long *pSync)
{
    /*
     * pSync[0] is to track the progress of the left PE
     * pSync[1..RING_DIFF] is used to receive block sizes
     * pSync[RING_DIFF..] is used for exclusive prefix sum
     */
    const int stride = 1 << logPE_stride;
    const int me = shmem_my_pe();

    int me_as = (me - PE_start) / stride;
    int recv_from_pe = PE_start + ((me_as + 1) % PE_size) * stride;
    int send_to_pe = PE_start + ((me_as - 1 + PE_size) % PE_size) * stride;

    int round;
    long *receiver_progress = pSync;
    size_t *block_sizes = (size_t *) (pSync + 1);
    size_t *block_size_round;
    size_t nbytes_round = nbytes;

    size_t block_offset;

    exclusive_prefix_sum(&block_offset, nbytes, PE_start, logPE_stride, PE_size, pSync + 1 + RING_DIFF);

    memcpy(((char *) dest) + block_offset, source, nbytes_round);

    for (round = 0; round < PE_size - 1; round++) {

        shmem_putmem_nbi(((char *) dest) + block_offset, ((char *) dest) + block_offset, nbytes_round, send_to_pe);
        shmem_fence();

        /* Wait until it's safe to use block_size buffer */
        shmem_long_wait_until(receiver_progress, SHMEM_CMP_GT, round - RING_DIFF + SHCOLL_SYNC_VALUE);
        block_size_round = block_sizes + (round % RING_DIFF);

        // TODO: fix -> shmem_size_p(block_size_round, nbytes_round + 1 + SHCOLL_SYNC_VALUE, send_to_pe);
        shmem_size_atomic_set(block_size_round, nbytes_round + 1 + SHCOLL_SYNC_VALUE, send_to_pe);

        /* If writing block 0, reset offset to 0 */
        block_offset = (me_as + round + 1 == PE_size) ? 0 : block_offset + nbytes_round;

        /* Wait to receive the data in this round */
        shmem_size_wait_until(block_size_round, SHMEM_CMP_NE, SHCOLL_SYNC_VALUE);
        nbytes_round = *block_size_round - 1 - SHCOLL_SYNC_VALUE;

        /* Reset the block size from the current round */
        shmem_size_p(block_size_round, SHCOLL_SYNC_VALUE, me);
        shmem_size_wait_until(block_size_round, SHMEM_CMP_EQ, SHCOLL_SYNC_VALUE);

        /* Notify sender that one counter is freed */
        shmem_long_atomic_inc(receiver_progress, recv_from_pe);
    }

    /* Must be atomic fadd because there may be some PE that did not finish with sends */
    shmem_long_atomic_add(receiver_progress, -round, me);
}

inline static void
collect_helper_bruck(void *dest, const void *source, size_t nbytes,
                     int PE_start, int logPE_stride, int PE_size,
                     long *pSync)
{
    /* pSync[0] is used for barrier
     * pSync[1] is used for broadcast
     * pSync[2..2+PREFIX_SUM_SYNC_SIZE) bytes are used for the prefix sum
     * pSync[2+PREFIX_SUM_SYNC_SIZE..2+PREFIX_SUM_SYNC_SIZE+32) bytes are used for the Bruck's algorithm, block sizes */
    /* TODO change 32 with a constant */

    const int stride = 1 << logPE_stride;
    const int me = shmem_my_pe();

    /* Get my index in the active set */
    int me_as = (me - PE_start) / stride;
    size_t distance;
    int round;
    int send_to;
    int recv_from;
    size_t recv_nbytes = nbytes;
    size_t round_nbytes;

    /* pSyncs */
    long *barrier_pSync = pSync;
    long *broadcast_pSync = barrier_pSync + 1;
    long *prefix_sum_pSync = (broadcast_pSync + 1);
    size_t *block_sizes = (size_t *) (prefix_sum_pSync + PREFIX_SUM_SYNC_SIZE);

    size_t block_offset;
    size_t total_nbytes;

    /* Calculate prefix sum */
    exclusive_prefix_sum(&block_offset, nbytes, PE_start, logPE_stride, PE_size, prefix_sum_pSync);

    /* Broadcast the total size */
    if (me_as == PE_size - 1) {
        total_nbytes = block_offset + nbytes;
    }

    broadcast_size(&total_nbytes, PE_start + (PE_size - 1) * stride, PE_start, logPE_stride, PE_size, broadcast_pSync);

    /* Copy the local block to the destination */
    memcpy(dest, source, nbytes);

    for (distance = 1, round = 0; distance < PE_size; distance <<= 1, round++) {
        send_to = (int) (PE_start + ((me_as - distance + PE_size) % PE_size) * stride);
        recv_from = (int) (PE_start + ((me_as + distance) % PE_size) * stride);

        /* Notify partner that the data is ready */
        shmem_size_atomic_set(block_sizes + round, recv_nbytes + 1 + SHCOLL_SYNC_VALUE, send_to);

        /* Wait until the data is ready to be read */
        shmem_size_wait_until(block_sizes + round, SHMEM_CMP_NE, SHCOLL_SYNC_VALUE);
        round_nbytes = *(block_sizes + round) - 1 - SHCOLL_SYNC_VALUE;

        round_nbytes = recv_nbytes + round_nbytes < total_nbytes ? round_nbytes : total_nbytes - recv_nbytes;

        shmem_getmem(((char *) dest) + recv_nbytes, dest,
                     round_nbytes, recv_from);
        recv_nbytes += round_nbytes;

        /* Reset the block size from the current round */
        shmem_size_p(block_sizes + round, SHCOLL_SYNC_VALUE, me);
        shmem_size_wait_until(block_sizes + round, SHMEM_CMP_EQ, SHCOLL_SYNC_VALUE);
    }

    shcoll_barrier_binomial_tree(PE_start, logPE_stride, PE_size, barrier_pSync);

    rotate(dest, total_nbytes, block_offset);
}

inline static void
collect_helper_bruck_no_rotate(void *dest, const void *source, size_t nbytes,
                               int PE_start, int logPE_stride, int PE_size,
                               long *pSync)
{
    /* pSync[0] is used for barrier
     * pSync[1] is used for broadcast
     * pSync[2..2+PREFIX_SUM_SYNC_SIZE) bytes are used for the prefix sum
     * pSync[2+PREFIX_SUM_SYNC_SIZE..2+PREFIX_SUM_SYNC_SIZE+32) bytes are used for the Bruck's algorithm, block sizes */
    /* TODO change 32 with a constant */

    const int stride = 1 << logPE_stride;
    const int me = shmem_my_pe();

    /* Get my index in the active set */
    int me_as = (me - PE_start) / stride;
    size_t distance;
    int round;
    int send_to;
    int recv_from;
    size_t recv_nbytes = nbytes;
    size_t round_nbytes;

    /* pSyncs */
    long *barrier_pSync = pSync;
    long *broadcast_pSync = barrier_pSync + 1;
    long *prefix_sum_pSync = (broadcast_pSync + 1);
    size_t *block_sizes = (size_t *) (prefix_sum_pSync + PREFIX_SUM_SYNC_SIZE);

    size_t block_offset;
    size_t total_nbytes;

    size_t next_block_start;

    /* Calculate prefix sum */
    exclusive_prefix_sum(&block_offset, nbytes, PE_start, logPE_stride, PE_size, prefix_sum_pSync);

    /* Broadcast the total size */
    if (me_as == PE_size - 1) {
        total_nbytes = block_offset + nbytes;
    }

    broadcast_size(&total_nbytes, PE_start + (PE_size - 1) * stride, PE_start, logPE_stride, PE_size, broadcast_pSync);

    /* Copy the local block to the destination */
    memcpy((char*) dest + block_offset, source, nbytes);

    for (distance = 1, round = 0; distance < PE_size; distance <<= 1, round++) {
        send_to = (int) (PE_start + ((me_as - distance + PE_size) % PE_size) * stride);
        recv_from = (int) (PE_start + ((me_as + distance) % PE_size) * stride);

        /* Notify partner that the data is ready */
        shmem_size_atomic_set(block_sizes + round, recv_nbytes + 1 + SHCOLL_SYNC_VALUE, send_to);

        /* Wait until the data is ready to be read */
        shmem_size_wait_until(block_sizes + round, SHMEM_CMP_NE, SHCOLL_SYNC_VALUE);
        round_nbytes = *(block_sizes + round) - 1 - SHCOLL_SYNC_VALUE;

        round_nbytes = recv_nbytes + round_nbytes < total_nbytes ? round_nbytes : total_nbytes - recv_nbytes;

        next_block_start = block_offset + recv_nbytes < total_nbytes ? block_offset + recv_nbytes :
                           block_offset + recv_nbytes - total_nbytes;

        if (next_block_start + round_nbytes <= total_nbytes) {
            shmem_getmem((char*) dest + next_block_start, (char*) dest + next_block_start, round_nbytes, recv_from);
        } else {
            shmem_getmem_nbi((char*) dest + next_block_start, (char*) dest + next_block_start,
                             total_nbytes - next_block_start, recv_from);

            shmem_getmem_nbi(dest, dest, round_nbytes - (total_nbytes - next_block_start), recv_from);

            shmem_quiet();
        }

        recv_nbytes += round_nbytes;

        /* Reset the block size from the current round */
        shmem_size_p(block_sizes + round, SHCOLL_SYNC_VALUE, me);
        shmem_size_wait_until(block_sizes + round, SHMEM_CMP_EQ, SHCOLL_SYNC_VALUE);
    }

    shcoll_barrier_binomial_tree(PE_start, logPE_stride, PE_size, barrier_pSync);
}

#define SHCOLL_COLLECT_DEFINITION(_name, _size)                         \
    void                                                                \
    shcoll_collect##_size##_##_name(void *dest, const void *source,     \
                                    size_t nelems,                      \
                                    int PE_start, int logPE_stride,     \
                                    int PE_size,                        \
                                    long *pSync)                        \
    {                                                                   \
        collect_helper_##_name(dest, source,                            \
                               (_size) / CHAR_BIT * nelems,             \
                               PE_start, logPE_stride, PE_size,         \
                               pSync);                                  \
    }                                                                   \


/* @formatter:off */

SHCOLL_COLLECT_DEFINITION(linear, 32)
SHCOLL_COLLECT_DEFINITION(linear, 64)

SHCOLL_COLLECT_DEFINITION(all_linear, 32)
SHCOLL_COLLECT_DEFINITION(all_linear, 64)

SHCOLL_COLLECT_DEFINITION(all_linear1, 32)
SHCOLL_COLLECT_DEFINITION(all_linear1, 64)

SHCOLL_COLLECT_DEFINITION(rec_dbl, 32)
SHCOLL_COLLECT_DEFINITION(rec_dbl, 64)

SHCOLL_COLLECT_DEFINITION(rec_dbl_signal, 32)
SHCOLL_COLLECT_DEFINITION(rec_dbl_signal, 64)

SHCOLL_COLLECT_DEFINITION(ring, 32)
SHCOLL_COLLECT_DEFINITION(ring, 64)

SHCOLL_COLLECT_DEFINITION(bruck, 32)
SHCOLL_COLLECT_DEFINITION(bruck, 64)

SHCOLL_COLLECT_DEFINITION(bruck_no_rotate, 32)
SHCOLL_COLLECT_DEFINITION(bruck_no_rotate, 64)

/* @formatter:on */
