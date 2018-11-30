//
// Created by Srdan Milakovic on 5/21/18.
//

#include "shcoll.h"
#include "shcoll/compat.h"

#include "shmem.h"

#include <limits.h>
#include <assert.h>


inline static int edge_color(int i, int me, int npes) {
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


#define SHCOLL_ALLTOALLS_BARRIER_DEFINITION(_name, _size, _peer, _cond, _nbi) \
    void shcoll_alltoalls##_size##_##_name##_barrier##_nbi(void *dest, const void *source, ptrdiff_t dst, \
                                                           ptrdiff_t sst, size_t nelems, int PE_start, \
                                                           int logPE_stride, int PE_size, long *pSync) {  \
        const int stride = 1 << logPE_stride;                           \
        const int me = shmem_my_pe();                                   \
                                                                        \
        /* Get my index in the active set */                            \
        /* const int me_as = (me - PE_start) / stride; */               \
                                                                        \
        void *const dest_ptr = ((uint8_t *) dest) + me * dst * nelems * ((_size) / CHAR_BIT); \
        void const *source_ptr = ((uint8_t *) source) + me * sst * nelems * ((_size) / CHAR_BIT); \
                                                                        \
        int i;                                                          \
        int peer_as;                                                    \
                                                                        \
        assert(_cond);                                                  \
                                                                        \
        for (i = 0; i < nelems; i++) {                                  \
            *(((uint##_size##_t*) dest_ptr) + i * dst) = *(((uint##_size##_t*) source_ptr) + i * sst); \
        }                                                               \
                                                                        \
        for (i = 1; i < PE_size; i++) {                                 \
            peer_as = _peer(i, me, PE_size);                            \
            source_ptr = ((uint8_t *) source) + peer_as * sst * nelems * ((_size) / CHAR_BIT); \
                                                                        \
            shmem_iput##_size##_nbi(dest_ptr, source_ptr, dst, sst, nelems, PE_start + peer_as * stride); \
        }                                                               \
                                                                        \
        /* TODO: change to auto shcoll barrier */                       \
        shcoll_barrier_binomial_tree(PE_start, logPE_stride, PE_size, pSync); \
    }                                                                   \

#define SHCOLL_ALLTOALLS_COUNTER_DEFINITION(_name, _size, _peer, _cond, _nbi) \
    void shcoll_alltoalls##_size##_##_name##_counter##_nbi(void *dest, const void *source, ptrdiff_t dst, \
                                                           ptrdiff_t sst, size_t nelems, int PE_start, \
                                                           int logPE_stride, int PE_size, long *pSync) {  \
        const int stride = 1 << logPE_stride;                           \
        const int me = shmem_my_pe();                                   \
                                                                        \
        /* Get my index in the active set */                            \
        const int me_as = (me - PE_start) / stride;                     \
                                                                        \
        void *const dest_ptr = ((uint8_t *) dest) + me * dst * nelems * ((_size) / CHAR_BIT); \
        void const *source_ptr = ((uint8_t *) source) + me * sst * nelems * ((_size) / CHAR_BIT); \
                                                                        \
        int i;                                                          \
        int peer_as;                                                    \
                                                                        \
        assert(_cond);                                                  \
                                                                        \
        for (i = 0; i < nelems; i++) {                                  \
            *(((uint##_size##_t*) dest_ptr) + i * dst) = *(((uint##_size##_t*) source_ptr) + i * sst); \
        }                                                               \
                                                                        \
        for (i = 1; i < PE_size; i++) {                                 \
            peer_as = _peer(i, me, PE_size);                            \
            source_ptr = ((uint8_t *) source) + peer_as * sst * nelems * ((_size) / CHAR_BIT); \
                                                                        \
            shmem_iput##_size##_nbi(dest_ptr, source_ptr, dst, sst, nelems, PE_start + peer_as * stride); \
        }                                                               \
                                                                        \
        shmem_fence();                                                  \
                                                                        \
        for (i = 1; i < PE_size; i++) {                                 \
            peer_as = _peer(i, me_as, PE_size);                         \
            shmem_long_atomic_inc(pSync, PE_start + peer_as * stride);  \
        }                                                               \
                                                                        \
        shmem_long_wait_until(pSync, SHMEM_CMP_EQ, SHCOLL_SYNC_VALUE + PE_size - 1); \
        shmem_long_p(pSync, SHCOLL_SYNC_VALUE, me);                     \
    }                                                                   \


#define SHCOLL_ALLTOALL_DEFINITION(_macro, _name, _peer, _cond, _nbi)       \
    _macro(_name, 32, _peer, _cond, _nbi)                                   \
    _macro(_name, 64, _peer, _cond, _nbi)                                   \


// @formatter:off

#define SHIFT_PEER(I, ME, NPES) (((ME) + (I)) % (NPES))

SHCOLL_ALLTOALL_DEFINITION(SHCOLL_ALLTOALLS_BARRIER_DEFINITION, shift_exchange, SHIFT_PEER, 1,)
SHCOLL_ALLTOALL_DEFINITION(SHCOLL_ALLTOALLS_BARRIER_DEFINITION, shift_exchange, SHIFT_PEER, 1, _nbi)

SHCOLL_ALLTOALL_DEFINITION(SHCOLL_ALLTOALLS_COUNTER_DEFINITION, shift_exchange, SHIFT_PEER, 1,)
SHCOLL_ALLTOALL_DEFINITION(SHCOLL_ALLTOALLS_COUNTER_DEFINITION, shift_exchange, SHIFT_PEER, 1, _nbi)


#define XOR_PEER(I, ME, NPES) ((I) ^ (ME))
#define XOR_COND (((PE_size - 1) & PE_size) == 0)

SHCOLL_ALLTOALL_DEFINITION(SHCOLL_ALLTOALLS_BARRIER_DEFINITION, xor_pairwise_exchange, SHIFT_PEER, XOR_COND,)
SHCOLL_ALLTOALL_DEFINITION(SHCOLL_ALLTOALLS_BARRIER_DEFINITION, xor_pairwise_exchange, SHIFT_PEER, XOR_COND, _nbi)

SHCOLL_ALLTOALL_DEFINITION(SHCOLL_ALLTOALLS_COUNTER_DEFINITION, xor_pairwise_exchange, SHIFT_PEER, XOR_COND,)
SHCOLL_ALLTOALL_DEFINITION(SHCOLL_ALLTOALLS_COUNTER_DEFINITION, xor_pairwise_exchange, SHIFT_PEER, XOR_COND, _nbi)


#define COLOR_PEER(I, ME, NPES) edge_color(I, ME, NPES)

SHCOLL_ALLTOALL_DEFINITION(SHCOLL_ALLTOALLS_BARRIER_DEFINITION, color_pairwise_exchange, SHIFT_PEER, 1,)
SHCOLL_ALLTOALL_DEFINITION(SHCOLL_ALLTOALLS_BARRIER_DEFINITION, color_pairwise_exchange, SHIFT_PEER, 1, _nbi)

SHCOLL_ALLTOALL_DEFINITION(SHCOLL_ALLTOALLS_COUNTER_DEFINITION, color_pairwise_exchange, SHIFT_PEER, 1,)
SHCOLL_ALLTOALL_DEFINITION(SHCOLL_ALLTOALLS_COUNTER_DEFINITION, color_pairwise_exchange, SHIFT_PEER, 1, _nbi)


// @formatter:on
