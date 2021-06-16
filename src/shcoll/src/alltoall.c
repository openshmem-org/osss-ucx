/*
 * For license: see LICENSE file at top-level
 */

#include "shcoll.h"
#include "shcoll/compat.h"

#include <string.h>
#include <limits.h>
#include <assert.h>


inline static int
edge_color(int i, int me, int npes)
{
    int chr_idx;
    int v;

    chr_idx = npes % 2 == 1 ? npes : npes - 1;
    if (me < chr_idx) {
        v = (i + chr_idx - me) % chr_idx;
    } else {
        v = i % 2 == 1 ? (((i + chr_idx) / 2) % chr_idx) : i / 2;
    }

    if (npes % 2 == 1 && v == me) {
        return -1;
    } else if (v == me) {
        return chr_idx;
    } else {
        return v;
    }
}

static int alltoall_rounds_sync = INT32_MAX;

void
shcoll_set_alltoalls_round_sync(int rounds_sync)
{
    alltoall_rounds_sync = rounds_sync;
}

#define ALLTOALL_HELPER_BARRIER_DEFINITION(_name, _peer, _cond)             \
    inline static void                                                      \
    alltoall_helper_##_name##_barrier(void *dest, const void *source,       \
                                      size_t nelems, int PE_start,          \
                                      int logPE_stride, int PE_size,        \
                                      long *pSync) {                        \
        const int stride = 1 << logPE_stride;                               \
        const int me = shmem_my_pe();                                       \
                                                                            \
        /* Get my index in the active set */                                \
        const int me_as = (me - PE_start) / stride;                         \
                                                                            \
        void *const dest_ptr = ((uint8_t *) dest) + me_as * nelems;         \
        void const *source_ptr = ((uint8_t *) source) + me_as * nelems;     \
                                                                            \
        int i;                                                              \
        int peer_as;                                                        \
                                                                            \
        assert(_cond);                                                      \
                                                                            \
        memcpy(dest_ptr, source_ptr, nelems);                               \
                                                                            \
        for (i = 1; i < PE_size; i++) {                                     \
            peer_as = _peer(i, me_as, PE_size);                             \
            source_ptr = ((uint8_t *) source) + peer_as * nelems;           \
                                                                            \
            shmem_putmem_nbi(dest_ptr, source_ptr, nelems,                  \
                             PE_start + peer_as * stride);                  \
                                                                            \
            if (i % alltoall_rounds_sync == 0) {                            \
                /* TODO: change to auto shcoll barrier */                   \
                shcoll_barrier_binomial_tree(PE_start, logPE_stride,        \
                                PE_size, pSync);                            \
            }                                                               \
        }                                                                   \
                                                                            \
        /* TODO: change to auto shcoll barrier */                           \
        shcoll_barrier_binomial_tree(PE_start, logPE_stride, PE_size,       \
                                     pSync);                                \
    }                                                                       \

#define ALLTOALL_HELPER_COUNTER_DEFINITION(_name, _peer, _cond)             \
    inline static void                                                      \
    alltoall_helper_##_name##_counter(void *dest, const void *source,       \
                                      size_t nelems, int PE_start,          \
                                      int logPE_stride, int PE_size,        \
                                      long *pSync) {                        \
        const int stride = 1 << logPE_stride;                               \
        const int me = shmem_my_pe();                                       \
                                                                            \
        /* Get my index in the active set */                                \
        const int me_as = (me - PE_start) / stride;                         \
                                                                            \
        void *const dest_ptr = ((uint8_t *) dest) + me_as * nelems;         \
        void const *source_ptr;                                             \
                                                                            \
        int i;                                                              \
        int peer_as;                                                        \
                                                                            \
        assert(_cond);                                                      \
                                                                            \
        for (i = 1; i < PE_size; i++) {                                     \
            peer_as = _peer(i, me_as, PE_size);                             \
            source_ptr = ((uint8_t *) source) + peer_as * nelems;           \
                                                                            \
            shmem_putmem_nbi(dest_ptr, source_ptr, nelems,                  \
                             PE_start + peer_as * stride);                  \
        }                                                                   \
                                                                            \
        source_ptr = ((uint8_t *) source) + me_as * nelems;                 \
        memcpy(dest_ptr, source_ptr, nelems);                               \
                                                                            \
        shmem_fence();                                                      \
                                                                            \
        for (i = 1; i < PE_size; i++) {                                     \
            peer_as = _peer(i, me_as, PE_size);                             \
            shmem_long_atomic_inc(pSync, PE_start + peer_as * stride);      \
        }                                                                   \
                                                                            \
        shmem_long_wait_until(pSync, SHMEM_CMP_EQ,                          \
                              SHCOLL_SYNC_VALUE + PE_size - 1);             \
        shmem_long_p(pSync, SHCOLL_SYNC_VALUE, me);                         \
    }                                                                       \

#define ALLTOALL_HELPER_SIGNAL_DEFINITION(_name, _peer, _cond)              \
    inline static void                                                      \
    alltoall_helper_##_name##_signal(void *dest, const void *source,        \
                                     size_t nelems, int PE_start,           \
                                     int logPE_stride, int PE_size,         \
                                     long *pSync) {                         \
        const int stride = 1 << logPE_stride;                               \
        const int me = shmem_my_pe();                                       \
                                                                            \
        /* Get my index in the active set */                                \
        const int me_as = (me - PE_start) / stride;                         \
                                                                            \
        void *const dest_ptr = ((uint8_t *) dest) + me_as * nelems;         \
        void const *source_ptr;                                             \
                                                                            \
        assert(_cond);                                                      \
                                                                            \
        int i;                                                              \
        int peer_as;                                                        \
                                                                            \
        for (i = 1; i < PE_size; i++) {                                     \
            peer_as = _peer(i, me_as, PE_size);                             \
            source_ptr = ((uint8_t *) source) + peer_as * nelems;           \
                                                                            \
            shmem_putmem_signal_nb(dest_ptr, source_ptr, nelems,            \
                                   pSync + i - 1, SHCOLL_SYNC_VALUE + 1,    \
                                   PE_start + peer_as * stride, NULL);      \
        }                                                                   \
                                                                            \
        source_ptr = ((uint8_t *) source) + me_as * nelems;                 \
        memcpy(dest_ptr, source_ptr, nelems);                               \
                                                                            \
        for (i = 1; i < PE_size; i++) {                                     \
            shmem_long_wait_until(pSync + i - 1, SHMEM_CMP_GT,              \
                                  SHCOLL_SYNC_VALUE);                       \
            shmem_long_p(pSync + i - 1, SHCOLL_SYNC_VALUE, me);             \
        }                                                                   \
    }                                                                       \

// @formatter:off

#define SHIFT_PEER(I, ME, NPES) (((ME) + (I)) % (NPES))
ALLTOALL_HELPER_BARRIER_DEFINITION(shift_exchange, SHIFT_PEER, 1)
ALLTOALL_HELPER_COUNTER_DEFINITION(shift_exchange, SHIFT_PEER, 1)
ALLTOALL_HELPER_SIGNAL_DEFINITION(shift_exchange, SHIFT_PEER, PE_size - 1 <= SHCOLL_ALLTOALL_SYNC_SIZE)


#define XOR_PEER(I, ME, NPES) ((I) ^ (ME))
#define XOR_COND (((PE_size - 1) & PE_size) == 0)

ALLTOALL_HELPER_BARRIER_DEFINITION(xor_pairwise_exchange, XOR_PEER, XOR_COND)
ALLTOALL_HELPER_COUNTER_DEFINITION(xor_pairwise_exchange, XOR_PEER, XOR_COND)
ALLTOALL_HELPER_SIGNAL_DEFINITION(xor_pairwise_exchange, XOR_PEER, XOR_COND && PE_size - 1 <= SHCOLL_ALLTOALL_SYNC_SIZE)


#define COLOR_PEER(I, ME, NPES) edge_color(I, ME, NPES)
#define COLOR_COND (PE_size % 2 == 0)

ALLTOALL_HELPER_BARRIER_DEFINITION(color_pairwise_exchange, COLOR_PEER, COLOR_COND)
ALLTOALL_HELPER_COUNTER_DEFINITION(color_pairwise_exchange, COLOR_PEER, COLOR_COND)
ALLTOALL_HELPER_SIGNAL_DEFINITION(color_pairwise_exchange, COLOR_PEER, (PE_size - 1 <= SHCOLL_ALLTOALL_SYNC_SIZE) && COLOR_COND)

// @formatter:on


#define SHCOLL_ALLTOALL_DEFINITION(_name, _size)                            \
    void                                                                    \
    shcoll_alltoall##_size##_##_name(void *dest, const void *source,        \
                                     size_t nelems, int PE_start,           \
                                     int logPE_stride, int PE_size,         \
                                     long *pSync) {                         \
        alltoall_helper_##_name(dest, source, (_size) / (CHAR_BIT) * nelems,\
                                PE_start, logPE_stride, PE_size, pSync);    \
}                                                                           \


// @formatter:off

SHCOLL_ALLTOALL_DEFINITION(shift_exchange_barrier, 32)
SHCOLL_ALLTOALL_DEFINITION(shift_exchange_barrier, 64)

SHCOLL_ALLTOALL_DEFINITION(shift_exchange_counter, 32)
SHCOLL_ALLTOALL_DEFINITION(shift_exchange_counter, 64)

SHCOLL_ALLTOALL_DEFINITION(shift_exchange_signal, 32)
SHCOLL_ALLTOALL_DEFINITION(shift_exchange_signal, 64)


SHCOLL_ALLTOALL_DEFINITION(xor_pairwise_exchange_barrier, 32)
SHCOLL_ALLTOALL_DEFINITION(xor_pairwise_exchange_barrier, 64)

SHCOLL_ALLTOALL_DEFINITION(xor_pairwise_exchange_counter, 32)
SHCOLL_ALLTOALL_DEFINITION(xor_pairwise_exchange_counter, 64)

SHCOLL_ALLTOALL_DEFINITION(xor_pairwise_exchange_signal, 32)
SHCOLL_ALLTOALL_DEFINITION(xor_pairwise_exchange_signal, 64)


SHCOLL_ALLTOALL_DEFINITION(color_pairwise_exchange_counter, 32)
SHCOLL_ALLTOALL_DEFINITION(color_pairwise_exchange_counter, 64)

SHCOLL_ALLTOALL_DEFINITION(color_pairwise_exchange_barrier, 32)
SHCOLL_ALLTOALL_DEFINITION(color_pairwise_exchange_barrier, 64)

SHCOLL_ALLTOALL_DEFINITION(color_pairwise_exchange_signal, 32)
SHCOLL_ALLTOALL_DEFINITION(color_pairwise_exchange_signal, 64)

// @formatter:on
