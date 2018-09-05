//
// Created by Srdan Milakovic on 5/17/18.
//

#include "shcoll.h"
#include "shcoll/compat.h"
#include "../tests/util/debug.h"
#include "util/rotate.h"

#include <limits.h>
#include <string.h>
#include <assert.h>


/**
 @param pSync pSync should have at least 2 elements
 */
inline static void
fcollect_helper_linear(void *dest, const void *source, size_t nbytes,
                       int PE_start, int logPE_stride, int PE_size,
                       long *pSync)
{
    const int stride = 1 << logPE_stride;
    const int me = shmem_my_pe();

    /* Get my index in the active set */
    int me_as = (me - PE_start) / stride;

    shcoll_barrier_linear(PE_start, logPE_stride, PE_size, pSync);
    if (me != PE_start) {
        shmem_putmem_nbi((char *) dest + me_as * nbytes, source, nbytes, PE_start);
    } else {
        memcpy(dest, source, nbytes);
    }
    shcoll_barrier_linear(PE_start, logPE_stride, PE_size, pSync);

    shcoll_broadcast8_linear(dest, dest, nbytes * shmem_n_pes(), PE_start, PE_start, logPE_stride, PE_size, pSync + 1);
}

inline static void
fcollect_helper_all_linear(void *dest, const void *source, size_t nbytes,
                           int PE_start, int logPE_stride, int PE_size,
                           long *pSync)
{
    const int stride = 1 << logPE_stride;
    const int me = shmem_my_pe();
    const int me_as = (me - PE_start) / stride;

    int i;
    int target;

    for (i = 1; i < PE_size; i++) {
        target = PE_start + ((i + me_as) % PE_size) * stride;
        shmem_putmem_nbi((char*) dest + me_as * nbytes, source, nbytes, target);
    }

    memcpy((char*) dest + me_as * nbytes, source, nbytes);

    shmem_fence();

    for (i = 1; i < PE_size; i++) {
        target = PE_start + ((i + me_as) % PE_size) * stride;
        shmem_long_atomic_inc(pSync, target);
    }

    shmem_long_wait_until(pSync, SHMEM_CMP_EQ, SHCOLL_SYNC_VALUE + PE_size - 1);
    shmem_long_p(pSync, SHCOLL_SYNC_VALUE, me);
}

inline static void
fcollect_helper_all_linear1(void *dest, const void *source, size_t nbytes,
                            int PE_start, int logPE_stride, int PE_size,
                            long *pSync)
{
    const int stride = 1 << logPE_stride;
    const int me = shmem_my_pe();
    const int me_as = (me - PE_start) / stride;

    int i;
    int target;

    for (i = 1; i < PE_size; i++) {
        target = PE_start + ((i + me_as) % PE_size) * stride;
        shmem_putmem_nbi((char*) dest + me_as * nbytes, source, nbytes, target);
    }

    memcpy((char*) dest + me_as * nbytes, source, nbytes);

    shcoll_barrier_binomial_tree(PE_start, logPE_stride, PE_size, pSync);
}

/**
 * @param pSync pSync should have at least ⌈log(max_rank)⌉ elements
 */
inline static void
fcollect_helper_rec_dbl(void *dest, const void *source, size_t nbytes,
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
    int data_block = me_as;

    assert(((PE_size - 1) & PE_size) == 0);

    memcpy((char*) dest + me_as * nbytes, source, nbytes);

    for (mask = 0x1, i = 0; mask < PE_size; mask <<= 1, i++) {
        peer = PE_start + (me_as ^ mask) * stride;

        shmem_putmem_nbi((char*) dest + data_block * nbytes, (char*) dest + data_block * nbytes, nbytes * mask, peer);
        shmem_fence();
        shmem_long_p(pSync + i, SHCOLL_SYNC_VALUE + 1, peer);

        data_block &= ~mask;

        shmem_long_wait_until(pSync + i, SHMEM_CMP_NE, SHCOLL_SYNC_VALUE);
        shmem_long_p(pSync + i, SHCOLL_SYNC_VALUE, me);
    }
}

/**
 * @param pSync pSync should have at least 1 element
 */
inline static void
fcollect_helper_ring(void *dest, const void *source, size_t nbytes,
                     int PE_start, int logPE_stride, int PE_size,
                     long *pSync)
{
    const int stride = 1 << logPE_stride;
    const int me = shmem_my_pe();

    /* Get my index in the active set */
    int me_as = (me - PE_start) / stride;
    int peer = PE_start + ((me_as + 1) % PE_size) * stride;
    int data_block = me_as;
    int i;

    memcpy((char *) dest + data_block * nbytes, source, nbytes);

    for (i = 1; i < PE_size; i++) {
        shmem_putmem_nbi((char*)dest + data_block * nbytes, (char*)dest + data_block * nbytes, nbytes, peer);
        shmem_fence();
        shmem_long_atomic_inc(pSync, peer);

        data_block = (data_block - 1 + PE_size) % PE_size;
        shmem_long_wait_until(pSync, SHMEM_CMP_GE, SHCOLL_SYNC_VALUE + i);
    }

    shmem_long_p(pSync, SHCOLL_SYNC_VALUE, me);
}

/**
 * @param pSync pSync should have at least ⌈log(max_rank)⌉ elements
 */
inline static void
fcollect_helper_bruck(void *dest, const void *source, size_t nbytes,
                      int PE_start, int logPE_stride, int PE_size,
                      long *pSync)
{
    const int stride = 1 << logPE_stride;
    const int me = shmem_my_pe();

    /* Get my index in the active set */
    int me_as = (me - PE_start) / stride;
    size_t distance;
    int round;
    int peer;
    size_t sent_bytes = nbytes;
    size_t total_nbytes = PE_size * nbytes;
    size_t to_send;

    memcpy(dest, source, nbytes);

    for (distance = 1, round = 0; distance < PE_size; distance <<= 1, round++) {
        peer = (int) (PE_start + ((me_as - distance + PE_size) % PE_size) * stride);
        to_send = (2 * sent_bytes <= total_nbytes) ? sent_bytes : total_nbytes - sent_bytes;

        shmem_putmem_nbi((char *) dest + sent_bytes, dest, to_send, peer);
        shmem_fence();
        shmem_long_p(pSync + round, SHCOLL_SYNC_VALUE + 1, peer);

        sent_bytes += distance * nbytes;
        shmem_long_wait_until(pSync + round, SHMEM_CMP_NE, SHCOLL_SYNC_VALUE);
        shmem_long_p(pSync + round, SHCOLL_SYNC_VALUE, me);
    }

    rotate(dest, total_nbytes, me_as * nbytes);
}

/**
* @param pSync pSync should have at least ⌈log(max_rank)⌉ elements
*/
inline static void
fcollect_helper_bruck_no_rotate(void *dest, const void *source, size_t nbytes,
                                int PE_start, int logPE_stride, int PE_size,
                                long *pSync)
{
    const int stride = 1 << logPE_stride;
    const int me = shmem_my_pe();

    /* Get my index in the active set */
    int me_as = (me - PE_start) / stride;
    size_t distance;
    int round;
    int peer;
    size_t sent_bytes = nbytes;
    size_t total_nbytes = PE_size * nbytes;
    size_t to_send;

    size_t my_offset_nbytes = nbytes * me_as;
    char *my_offset = (char *) dest + my_offset_nbytes;

    memcpy(my_offset, source, nbytes);

    for (distance = 1, round = 0; distance < PE_size; distance <<= 1, round++) {
        peer = (int) (PE_start + ((me_as - distance + PE_size) % PE_size) * stride);
        to_send = (2 * sent_bytes <= total_nbytes) ? sent_bytes : total_nbytes - sent_bytes;

        if (my_offset_nbytes + to_send <= total_nbytes) {
            shmem_putmem_nbi(my_offset, my_offset, to_send, peer);
        } else {
            shmem_putmem_nbi(my_offset, my_offset, total_nbytes - my_offset_nbytes, peer);
            shmem_putmem_nbi(dest, dest, to_send - (total_nbytes - my_offset_nbytes), peer);
        }

        shmem_fence();
        shmem_long_p(pSync + round, SHCOLL_SYNC_VALUE + 1, peer);

        sent_bytes += distance * nbytes;
        shmem_long_wait_until(pSync + round, SHMEM_CMP_NE, SHCOLL_SYNC_VALUE);
        shmem_long_p(pSync + round, SHCOLL_SYNC_VALUE, me);
    }
}

/**
 * @param pSync pSync should have at least ⌈log(max_rank)⌉ elements
 */
inline static void
fcollect_helper_bruck_signal(void *dest, const void *source, size_t nbytes,
                             int PE_start, int logPE_stride, int PE_size,
                             long *pSync)
{
    const int stride = 1 << logPE_stride;
    const int me = shmem_my_pe();

    /* Get my index in the active set */
    int me_as = (me - PE_start) / stride;
    size_t distance;
    int round;
    int peer;
    size_t sent_bytes = nbytes;
    size_t total_nbytes = PE_size * nbytes;
    size_t to_send;

    memcpy(dest, source, nbytes);

    for (distance = 1, round = 0; distance < PE_size; distance <<= 1, round++) {
        peer = (int) (PE_start + ((me_as - distance + PE_size) % PE_size) * stride);
        to_send = (2 * sent_bytes <= total_nbytes) ? sent_bytes : total_nbytes - sent_bytes;

        shmem_putmem_signal_nb((char*) dest + sent_bytes, dest, to_send,
                               (uint64_t *) (pSync + round), SHCOLL_SYNC_VALUE + 1, peer, NULL);

        sent_bytes += distance * nbytes;
        shmem_long_wait_until(pSync + round, SHMEM_CMP_NE, SHCOLL_SYNC_VALUE);
        shmem_long_p(pSync + round, SHCOLL_SYNC_VALUE, me);
    }

    rotate(dest, total_nbytes, me_as * nbytes);
}

/**
 * @param pSync pSync should have at least ⌈log(max_rank)⌉ elements
 */
inline static void
fcollect_helper_bruck_inplace(void *dest, const void *source, size_t nbytes,
                              int PE_start, int logPE_stride, int PE_size,
                              long *pSync)
{
    const int stride = 1 << logPE_stride;
    const int me = shmem_my_pe();

    /* Get my index in the active set */
    int me_as = (me - PE_start) / stride;
    size_t distance;
    int round;
    int peer;
    size_t sent_bytes = nbytes;
    size_t total_nbytes = PE_size * nbytes;
    size_t to_send;

    memcpy(dest, source, nbytes);

    for (distance = 1, round = 0; distance < PE_size; distance <<= 1, round++) {
        peer = (int) (PE_start + ((me_as - distance + PE_size) % PE_size) * stride);
        to_send = (2 * sent_bytes <= total_nbytes) ? sent_bytes : total_nbytes - sent_bytes;

        shmem_putmem_nbi((char *) dest + sent_bytes, dest, to_send, peer);
        shmem_fence();
        shmem_long_p(pSync + round, SHCOLL_SYNC_VALUE + 1, peer);

        sent_bytes += distance * nbytes;
        shmem_long_wait_until(pSync + round, SHMEM_CMP_NE, SHCOLL_SYNC_VALUE);
        shmem_long_p(pSync + round, SHCOLL_SYNC_VALUE, me);
    }

    rotate_inplace(dest, total_nbytes, me_as * nbytes);
}

/**
 * @param pSync pSync should have at least 2 elements
 */
inline static void
fcollect_helper_neighbor_exchange(void *dest, const void *source,
                                  size_t nbytes,
                                  int PE_start, int logPE_stride, int PE_size,
                                  long *pSync)
{
    assert(PE_size % 2 == 0);

    const int stride = 1 << logPE_stride;
    const int me = shmem_my_pe();

    int neighbor_pe[2];
    int send_offset[2];
    int send_offset_diff;

    int i, parity;
    void *data;

    /* Get my index in the active set */
    int me_as = (me - PE_start) / stride;

    if (me_as % 2 == 0) {
        neighbor_pe[0] = PE_start + ((me_as + 1) % PE_size) * stride;
        neighbor_pe[1] = PE_start + ((me_as - 1 + PE_size) % PE_size) * stride;

        send_offset[0] = (me_as - 2 + PE_size) % PE_size & ~0x1;
        send_offset[1] = me_as & ~0x1;

        send_offset_diff = 2;
    } else {
        neighbor_pe[0] = PE_start + ((me_as - 1 + PE_size) % PE_size) * stride;
        neighbor_pe[1] = PE_start + ((me_as + 1) % PE_size) * stride;

        send_offset[0] = (me_as + 2) % PE_size & ~0x1;
        send_offset[1] = me_as & ~0x1;

        send_offset_diff = -2 + PE_size;
    }

    /* First round */
    data = (char *) dest + me_as * nbytes;

    memcpy(data, source, nbytes);

    shmem_putmem_nbi(data, data, nbytes, neighbor_pe[0]);
    shmem_fence();
    shmem_long_atomic_inc(pSync, neighbor_pe[0]);

    shmem_long_wait_until(pSync, SHMEM_CMP_GE, 1);

    /* Remaining npes/2 - 1 rounds */
    for (i = 1; i < PE_size / 2; i++) {
        parity = (i % 2) ? 1 : 0;
        data = ((char *) dest) + send_offset[parity] * nbytes;

        /* Send data */
        shmem_putmem_nbi(data, data, 2 * nbytes, neighbor_pe[parity]);
        shmem_fence();
        shmem_long_atomic_inc(pSync + parity, neighbor_pe[parity]);

        /* Calculate offset for the next round */
        send_offset[parity] = (send_offset[parity] + send_offset_diff) % PE_size;
        send_offset_diff = PE_size - send_offset_diff;

        /* Wait for the data from the neighbor */
        shmem_long_wait_until(pSync + parity, SHMEM_CMP_GT, i / 2);
    }

    pSync[0] = SHCOLL_SYNC_VALUE;
    pSync[1] = SHCOLL_SYNC_VALUE;
}

#define SHCOLL_FCOLLECT_DEFINITION(_name, _size)                        \
    void                                                                \
    shcoll_fcollect##_size##_##_name(void *dest, const void *source,    \
                                     size_t nelems,                     \
                                     int PE_start, int logPE_stride,    \
                                     int PE_size,                       \
                                     long *pSync)                       \
    {                                                                   \
        fcollect_helper_##_name(dest, source,                           \
                                (_size) / CHAR_BIT * nelems,            \
                                PE_start, logPE_stride, PE_size,        \
                                pSync);                                 \
    }

/* @formatter:off */

SHCOLL_FCOLLECT_DEFINITION(linear, 32)
SHCOLL_FCOLLECT_DEFINITION(linear, 64)

SHCOLL_FCOLLECT_DEFINITION(all_linear, 32)
SHCOLL_FCOLLECT_DEFINITION(all_linear, 64)

SHCOLL_FCOLLECT_DEFINITION(all_linear1, 32)
SHCOLL_FCOLLECT_DEFINITION(all_linear1, 64)

SHCOLL_FCOLLECT_DEFINITION(rec_dbl, 32)
SHCOLL_FCOLLECT_DEFINITION(rec_dbl, 64)

SHCOLL_FCOLLECT_DEFINITION(ring, 32)
SHCOLL_FCOLLECT_DEFINITION(ring, 64)

SHCOLL_FCOLLECT_DEFINITION(bruck, 32)
SHCOLL_FCOLLECT_DEFINITION(bruck, 64)

SHCOLL_FCOLLECT_DEFINITION(bruck_no_rotate, 32)
SHCOLL_FCOLLECT_DEFINITION(bruck_no_rotate, 64)

SHCOLL_FCOLLECT_DEFINITION(bruck_signal, 32)
SHCOLL_FCOLLECT_DEFINITION(bruck_signal, 64)

SHCOLL_FCOLLECT_DEFINITION(bruck_inplace, 32)
SHCOLL_FCOLLECT_DEFINITION(bruck_inplace, 64)

SHCOLL_FCOLLECT_DEFINITION(neighbor_exchange, 32)
SHCOLL_FCOLLECT_DEFINITION(neighbor_exchange, 64)

/* @formatter:on */

