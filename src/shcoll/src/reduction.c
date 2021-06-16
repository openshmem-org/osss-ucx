#include "shcoll.h"
#include "util/bithacks.h"
#include "../tests/util/debug.h"

#include "shmem.h"

#include <string.h>
#include <stdlib.h>
#include <limits.h>

/*
 * Local reduce helper
 */

#define REDUCE_HELPER_LOCAL(_name, _type, _op)                  \
    inline static void                                          \
    local_##_name##_reduce(_type *dest, const _type *src1,      \
                           const _type *src2, size_t nreduce)   \
    {                                                           \
        size_t i;                                               \
                                                                \
        for (i = 0; i < nreduce; i++) {                         \
            dest[i] = _op(src1[i], src2[i]);                    \
        }                                                       \
    }

/*
 * Linear reduction implementation
 */

#define REDUCE_HELPER_LINEAR(_name, _type, _op)                         \
    void                                                                \
    shcoll_##_name##_to_all_linear(_type *dest, const _type *source,    \
                                   int nreduce, int PE_start,           \
                                   int logPE_stride, int PE_size,       \
                                   _type *pWrk, long *pSync)            \
    {                                                                   \
        const int stride = 1 << logPE_stride;                           \
        const int me = shmem_my_pe();                                   \
        const int me_as = (me - PE_start) / stride;                     \
        const size_t nbytes = sizeof(_type) * nreduce;                  \
                                                                        \
        _type *tmp_array;                                               \
        int i;                                                          \
                                                                        \
        shcoll_barrier_linear(PE_start, logPE_stride, PE_size, pSync);  \
                                                                        \
        if (me_as == 0) {                                               \
            tmp_array = malloc(nbytes);                                 \
            if (!tmp_array) {                                           \
                /* TODO: raise error */                                 \
                exit(-1);                                               \
            }                                                           \
                                                                        \
            memcpy(tmp_array, source, nbytes);                          \
                                                                        \
            for (i = 1; i < PE_size; i++) {                             \
                shmem_getmem(dest, source, nbytes, PE_start + i * stride); \
                local_##_name##_reduce(tmp_array, tmp_array, dest, nreduce); \
            }                                                           \
                                                                        \
            memcpy(dest, tmp_array, nbytes);                            \
            free(tmp_array);                                            \
        }                                                               \
                                                                        \
        shcoll_barrier_linear(PE_start, logPE_stride, PE_size, pSync);  \
                                                                        \
        shcoll_broadcast8_linear(dest, dest, nreduce * sizeof(_type),   \
                                 PE_start, PE_start,                    \
                                 logPE_stride, PE_size,                 \
                                 pSync + 1);                            \
    }


    /*
     * Binomial reduction implementation
     */

#define REDUCE_HELPER_BINOMIAL(_name, _type, _op)                       \
    void                                                                \
    shcoll_##_name##_to_all_binomial(_type *dest, const _type *source,  \
                                     int nreduce,                       \
                                     int PE_start, int logPE_stride,    \
                                     int PE_size,                       \
                                     _type *pWrk, long *pSync)          \
    {                                                                   \
        const int stride = 1 << logPE_stride;                           \
        const int me = shmem_my_pe();                                   \
        int me_as = (me - PE_start) / stride;                           \
        int target_as;                                                  \
        size_t nbytes = sizeof(_type) * nreduce;                        \
        _type *tmp_array = NULL;                                        \
        unsigned mask = 0x1;                                            \
        long old_pSync = SHCOLL_SYNC_VALUE;                             \
        long to_receive = 0;                                            \
        long recv_mask;                                                 \
                                                                        \
        tmp_array = malloc(nbytes);                                     \
        if (!tmp_array) {                                               \
            /* TODO: raise error */                                     \
            fprintf(stderr, "PE %d: Cannot allocate memory!\n", me);    \
            exit(-1);                                                   \
        }                                                               \
                                                                        \
        if (source != dest) {                                           \
            memcpy(dest, source, nbytes);                               \
        }                                                               \
                                                                        \
        /* Stop if all messages are received or if there are no more PE on right */ \
        for (mask = 0x1;                                                \
             !(me_as & mask) && ((me_as | mask) < PE_size);             \
             mask <<= 1) {                                              \
            to_receive |= mask;                                         \
        }                                                               \
                                                                        \
        /* TODO: fix if SHCOLL_SYNC_VALUE not 0 */                      \
        /* Wait until all messages are received */                      \
        while (to_receive != 0) {                                       \
            memcpy(tmp_array, dest, nbytes);                            \
            shmem_long_wait_until(pSync, SHMEM_CMP_NE, old_pSync);      \
            recv_mask = shmem_long_atomic_fetch(pSync, me);             \
                                                                        \
            recv_mask &= to_receive;                                    \
            recv_mask ^= (recv_mask - 1) & recv_mask;                   \
                                                                        \
            /* Get array and reduce */                                  \
            target_as = (int) (me_as | recv_mask);                      \
            shmem_getmem(dest, dest, nbytes, PE_start + target_as * stride); \
                                                                        \
            local_##_name##_reduce(dest, dest, tmp_array, nreduce);     \
                                                                        \
            /* Mark as received */                                      \
            to_receive &= ~recv_mask;                                   \
            old_pSync |= recv_mask;                                     \
        }                                                               \
                                                                        \
        /* Notify parent */                                             \
        if (me_as != 0) {                                               \
            target_as = me_as & (me_as - 1);                            \
            shmem_long_atomic_add(pSync, me_as ^ target_as,             \
                                  PE_start + target_as * stride);       \
        }                                                               \
                                                                        \
        shmem_long_p(pSync, SHCOLL_SYNC_VALUE, me);                     \
        shcoll_barrier_linear(PE_start, logPE_stride, PE_size, pSync + 1); \
                                                                        \
        shcoll_broadcast8_binomial_tree(dest, dest,                     \
                                        nreduce * sizeof(_type),        \
                                        PE_start, PE_start,             \
                                        logPE_stride, PE_size,          \
                                        pSync + 2);                     \
                                                                        \
        free(tmp_array);                                                \
    }

/*
 * Recursive doubling implementation
 */

#define REDUCE_HELPER_REC_DBL(_name, _type, _op)                        \
    void                                                                \
    shcoll_##_name##_to_all_rec_dbl(_type *dest, const _type *source,   \
                                    int nreduce, int PE_start,          \
                                    int logPE_stride, int PE_size,      \
                                    _type *pWrk, long *pSync)           \
    {                                                                   \
        const int stride = 1 << logPE_stride;                           \
        const int me = shmem_my_pe();                                   \
        int peer;                                                       \
                                                                        \
        size_t nbytes = nreduce * sizeof(_type);                        \
                                                                        \
        /* Get my index in the active set */                            \
        int me_as = (me - PE_start) / stride;                           \
        int mask;                                                       \
                                                                        \
        int xchg_peer_p2s;                                              \
        int xchg_peer_as;                                               \
        int xchg_peer_pe;                                               \
                                                                        \
        /* Power 2 set */                                               \
        int me_p2s;                                                     \
        int p2s_size;                                                   \
                                                                        \
        _type *tmp_array = NULL;                                        \
                                                                        \
        /* Find the greatest power of 2 lower than PE_size */           \
        for (p2s_size = 1; p2s_size * 2 <= PE_size; p2s_size *= 2);     \
                                                                        \
        /* Check if the current PE belongs to the power 2 set */        \
        me_p2s = me_as * p2s_size / PE_size;                            \
        if ((me_p2s * PE_size + p2s_size - 1) / p2s_size != me_as) {    \
            me_p2s = -1;                                                \
        }                                                               \
                                                                        \
        /* If current PE belongs to the power 2 set, it will need temporary buffer */ \
        if (me_p2s != -1) {                                             \
            tmp_array = malloc(nreduce * sizeof(_type));                \
            if (tmp_array == NULL) {                                    \
                /* TODO: raise error */                                 \
                fprintf(stderr, "PE %d: Cannot allocate memory!\n", me); \
                exit(-1);                                               \
            }                                                           \
        }                                                               \
                                                                        \
        /* Check if the current PE should wait/send data to the peer */ \
        if (me_p2s == -1) {                                             \
            /* Notify peer that the data is ready */                    \
            peer = PE_start + (me_as - 1) * stride;                     \
            shmem_long_p(pSync, SHCOLL_SYNC_VALUE + 1, peer);           \
        } else if ((me_as + 1) * p2s_size / PE_size == me_p2s) {        \
            /* We should wait for the data to be ready */               \
            peer = PE_start + (me_as + 1) * stride;                     \
                                                                        \
            shmem_long_wait_until(pSync, SHMEM_CMP_NE, SHCOLL_SYNC_VALUE); \
            shmem_long_p(pSync, SHCOLL_SYNC_VALUE, me);                 \
                                                                        \
            /* Get the array and reduce */                              \
            shmem_getmem(dest, source, nbytes, peer);                   \
            local_##_name##_reduce(tmp_array, dest, source, nreduce);   \
        } else {                                                        \
            memcpy(tmp_array, source, nbytes);                          \
        }                                                               \
                                                                        \
        /* If the current PE belongs to the power 2 set, do recursive doubling */ \
        if (me_p2s != -1) {                                             \
            int i;                                                      \
                                                                        \
            for (mask = 0x1, i = 1; mask < p2s_size; mask <<= 1, i++) { \
                xchg_peer_p2s = me_p2s ^ mask;                          \
                xchg_peer_as = (xchg_peer_p2s * PE_size + p2s_size - 1) / p2s_size; \
                xchg_peer_pe = PE_start + xchg_peer_as * stride;        \
                                                                        \
                /* Notify the peer PE that current PE is ready to accept the data */ \
                shmem_long_p(pSync + i, SHCOLL_SYNC_VALUE + 1, xchg_peer_pe); \
                                                                        \
                /* Wait until the peer PE is ready to accept the data */ \
                shmem_long_wait_until(pSync + i, SHMEM_CMP_GT, SHCOLL_SYNC_VALUE); \
                                                                        \
                /* Send the data to the peer */                         \
                shmem_putmem(dest, tmp_array, nbytes, xchg_peer_pe);    \
                shmem_fence();                                          \
                shmem_long_p(pSync + i, SHCOLL_SYNC_VALUE + 2, xchg_peer_pe); \
                                                                        \
                /* Wait until the data is received and do local reduce */ \
                shmem_long_wait_until(pSync + i, SHMEM_CMP_GT, SHCOLL_SYNC_VALUE + 1); \
                local_##_name##_reduce(tmp_array, tmp_array, dest, nreduce); \
                                                                        \
                /* Reset the pSync for the current round */             \
                shmem_long_p(pSync + i, SHCOLL_SYNC_VALUE, me);         \
            }                                                           \
                                                                        \
            memcpy(dest, tmp_array, nbytes);                            \
        }                                                               \
                                                                        \
        if (me_p2s == -1) {                                             \
            /* Wait to get the data from a PE that is in the power 2 set */ \
            shmem_long_wait_until(pSync, SHMEM_CMP_NE, SHCOLL_SYNC_VALUE); \
            shmem_long_p(pSync, SHCOLL_SYNC_VALUE, me);                 \
        } else if ((me_as + 1) * p2s_size / PE_size == me_p2s) {        \
            /* Send data to peer PE that is outside the power 2 set */  \
            peer = PE_start + (me_as + 1) * stride;                     \
                                                                        \
            shmem_putmem(dest, dest, nbytes, peer);                     \
            shmem_fence();                                              \
            shmem_long_p(pSync, SHCOLL_SYNC_VALUE + 1, peer);           \
        }                                                               \
                                                                        \
        if (tmp_array != NULL) {                                        \
            free(tmp_array);                                            \
        }                                                               \
    }

/*
 * Rabenseifner reduction implementation
 */

#define REDUCE_HELPER_RABENSEIFNER(_name, _type, _op)                   \
    void                                                                \
    shcoll_##_name##_to_all_rabenseifner(_type *dest, const _type *source, \
                                         int nreduce, int PE_start,     \
                                         int logPE_stride, int PE_size, \
                                         _type *pWrk, long *pSync)      \
    {                                                                   \
        const int stride = 1 << logPE_stride;                           \
        const int me = shmem_my_pe();                                   \
                                                                        \
        int me_as = (me - PE_start) / stride;                           \
        int peer;                                                       \
        size_t i;                                                       \
        const size_t nelems = (const size_t) nreduce;                   \
                                                                        \
        int block_idx_begin;                                            \
        int block_idx_end;                                              \
                                                                        \
        ptrdiff_t block_offset;                                         \
        ptrdiff_t next_block_offset;                                    \
        size_t block_nelems;                                            \
                                                                        \
        int xchg_peer_p2s;                                              \
        int xchg_peer_as;                                               \
        int xchg_peer_pe;                                               \
                                                                        \
        /* Power 2 set */                                               \
        int me_p2s;                                                     \
        int p2s_size;                                                   \
        int log_p2s_size;                                               \
                                                                        \
        int distance;                                                   \
        _type *tmp_array = NULL;                                        \
                                                                        \
        /* Find the greatest power of 2 lower than PE_size */           \
        for (p2s_size = 1, log_p2s_size = 0; p2s_size * 2 <= PE_size; p2s_size *= 2, log_p2s_size++); \
                                                                        \
        /* Check if the current PE belongs to the power 2 set */        \
        me_p2s = me_as * p2s_size / PE_size;                            \
        if ((me_p2s * PE_size + p2s_size - 1) / p2s_size != me_as) {    \
            me_p2s = -1;                                                \
        }                                                               \
                                                                        \
        /* If current PE belongs to the power 2 set, it will need temporary buffer */ \
        if (me_p2s != -1) {                                             \
            tmp_array = malloc((nelems / 2 + 1) * sizeof(_type));       \
            if (tmp_array == NULL) {                                    \
                /* TODO: raise error */                                 \
                fprintf(stderr, "PE %d: Cannot allocate memory!\n", me); \
                exit(-1);                                               \
            }                                                           \
        }                                                               \
                                                                        \
        /* Check if the current PE should wait/send data to the peer */ \
        if (me_p2s == -1) {                                             \
            /* Notify peer that the data is ready */                    \
            peer = PE_start + (me_as - 1) * stride;                     \
            shmem_long_p(pSync, SHCOLL_SYNC_VALUE + 1, peer);           \
                                                                        \
            /* Wait until the data on peer node is ready and get the data (upper half of the array) */ \
            block_offset = nelems / 2;                                  \
            block_nelems = (size_t) (nelems - block_offset);            \
                                                                        \
            shmem_long_wait_until(pSync, SHMEM_CMP_NE, SHCOLL_SYNC_VALUE); \
            shmem_getmem(dest + block_offset, source + block_offset, block_nelems * sizeof(_type), peer); \
                                                                        \
            /* Reduce the upper half of the array */                    \
            local_##_name##_reduce(dest + block_offset, dest + block_offset, source + block_offset, block_nelems); \
                                                                        \
            /* Send the upper half of the array to peer */              \
            shmem_putmem(dest + block_offset, dest + block_offset, block_nelems * sizeof(_type), peer); \
            shmem_fence();                                              \
            shmem_long_p(pSync, SHCOLL_SYNC_VALUE + 2, peer);           \
        } else if ((me_as + 1) * p2s_size / PE_size == me_p2s) {        \
            /* Notify peer that the data is ready */                    \
            peer = PE_start + (me_as + 1) * stride;                     \
            shmem_long_p(pSync, SHCOLL_SYNC_VALUE + 1, peer);           \
                                                                        \
            /* Wait until the data on peer node is ready and get the data (lower half of the array) */ \
            block_offset = 0;                                           \
            block_nelems = (size_t) (nelems / 2 - block_offset);        \
                                                                        \
            shmem_long_wait_until(pSync, SHMEM_CMP_GT, SHCOLL_SYNC_VALUE); \
            shmem_getmem(dest, source, block_nelems * sizeof(_type), peer); \
                                                                        \
            /* Do local reduce */                                       \
            local_##_name##_reduce(dest, dest, source, block_nelems);   \
                                                                        \
            /* Wait until the upper half is received from peer */       \
            shmem_long_wait_until(pSync, SHMEM_CMP_GT, SHCOLL_SYNC_VALUE + 1); \
            shmem_long_p(pSync, SHCOLL_SYNC_VALUE, me);                 \
        } else {                                                        \
            memcpy(dest, source, nelems * sizeof(_type));               \
        }                                                               \
                                                                        \
        /* For nodes in the power 2 set, dest contains data that should be reduced */ \
                                                                        \
        /* Do reduce scatter with the nodes in power 2 set */           \
        if (me_p2s != -1) {                                             \
            block_idx_begin = 0;                                        \
            block_idx_end = p2s_size;                                   \
                                                                        \
            for (distance = 1, i = 1; distance < p2s_size; distance <<= 1, i++) { \
                xchg_peer_p2s = ((me_p2s & distance) == 0) ? me_p2s + distance : me_p2s - distance; \
                xchg_peer_as = (xchg_peer_p2s * PE_size + p2s_size - 1) / p2s_size; \
                xchg_peer_pe = PE_start + xchg_peer_as * stride;        \
                                                                        \
                /* Notify the peer PE that the data is ready to be read */ \
                shmem_long_p(pSync + i, SHCOLL_SYNC_VALUE + 1, xchg_peer_pe); \
                                                                        \
                /* Check if the current PE is responsible for lower half of upper half of the vector */ \
                if ((me_p2s & distance) == 0) {                         \
                    block_idx_end = (block_idx_begin + block_idx_end) / 2; \
                } else {                                                \
                    block_idx_begin = (block_idx_begin + block_idx_end) / 2; \
                }                                                       \
                                                                        \
                /* TODO: possible overflow */                           \
                block_offset = (block_idx_begin * nelems) / p2s_size;   \
                next_block_offset = (block_idx_end * nelems) / p2s_size; \
                block_nelems = (size_t) (next_block_offset - block_offset); \
                                                                        \
                /* Wait until the data on peer PE is ready to be read and get the data */ \
                shmem_long_wait_until(pSync + i, SHMEM_CMP_GE, SHCOLL_SYNC_VALUE + 1); \
                shmem_getmem(tmp_array, dest + block_offset, block_nelems * sizeof(_type), xchg_peer_pe); \
                                                                        \
                /* Notify the peer PE that the data transfer has completed successfully */ \
                shmem_fence();                                          \
                shmem_long_p(pSync + i, SHCOLL_SYNC_VALUE + 2, xchg_peer_pe); \
                                                                        \
                /* Do local reduce */                                   \
                local_##_name##_reduce(dest + block_offset, dest + block_offset, tmp_array, block_nelems); \
                                                                        \
                /* Wait until the peer PE has read the data */          \
                shmem_long_wait_until(pSync + i, SHMEM_CMP_GE, SHCOLL_SYNC_VALUE + 2); \
                shmem_long_p(pSync + i, SHCOLL_SYNC_VALUE, me);         \
            }                                                           \
        }                                                               \
                                                                        \
        /* For nodes in the power 2 set, destination will contain the reduced block */ \
                                                                        \
        /* Do collect with the nodes in power 2 set */                  \
        if (me_p2s != -1) {                                             \
            block_offset = 0;                                           \
            block_idx_begin = reverse_bits(me_p2s, log_p2s_size);       \
            block_idx_end = block_idx_begin + 1;                        \
                                                                        \
            for (distance = p2s_size / 2, i = sizeof(int) * CHAR_BIT + 1; distance > 0; distance >>= 1, i++) { \
                xchg_peer_p2s = ((me_p2s & distance) == 0) ? me_p2s + distance : me_p2s - distance; \
                xchg_peer_as = (xchg_peer_p2s * PE_size + p2s_size - 1) / p2s_size; \
                xchg_peer_pe = PE_start + xchg_peer_as * stride;        \
                                                                        \
                /* TODO: possible overflow */                           \
                block_offset = (block_idx_begin * nelems) / p2s_size;   \
                next_block_offset = (block_idx_end * nelems) / p2s_size; \
                block_nelems = (size_t) (next_block_offset - block_offset); \
                                                                        \
                shmem_putmem(dest + block_offset, dest + block_offset,  \
                             block_nelems * sizeof(_type), xchg_peer_pe); \
                shmem_fence();                                          \
                shmem_long_p(pSync + i, SHCOLL_SYNC_VALUE + 1, xchg_peer_pe); \
                                                                        \
                /* Wait until the data has arrived from exchange the peer PE */ \
                shmem_long_wait_until(pSync + i, SHMEM_CMP_GE, SHCOLL_SYNC_VALUE + 1); \
                shmem_long_p(pSync + i, SHCOLL_SYNC_VALUE, me);         \
                                                                        \
                /* Updated the block range */                           \
                if ((me_p2s & distance) == 0) {                         \
                    block_idx_end += (block_idx_end - block_idx_begin); \
                } else {                                                \
                    block_idx_begin -= (block_idx_end - block_idx_begin); \
                }                                                       \
            }                                                           \
        }                                                               \
                                                                        \
        /* Check if the current PE should wait/send data to the peer */ \
        if (me_p2s == -1) {                                             \
            /* Wait until the peer PE sends the data */                 \
            shmem_long_wait_until(pSync + 1, SHMEM_CMP_GE, SHCOLL_SYNC_VALUE + 1); \
            shmem_long_p(pSync + 1, SHCOLL_SYNC_VALUE, me);             \
        } else if ((me_as + 1) * p2s_size / PE_size == me_p2s) {        \
            peer = PE_start + (me_as + 1) * stride;                     \
            shmem_putmem(dest, dest, nelems * sizeof(_type), peer);     \
            shmem_fence();                                              \
            shmem_long_p(pSync + 1, SHCOLL_SYNC_VALUE + 1, peer);       \
        }                                                               \
                                                                        \
        if (tmp_array != NULL) {                                        \
            free(tmp_array);                                            \
        }                                                               \
    }


#define REDUCE_HELPER_RABENSEIFNER2(_name, _type, _op)                  \
    void                                                                \
    shcoll_##_name##_to_all_rabenseifner2(_type *dest, const _type *source, \
                                          int nreduce, int PE_start,    \
                                          int logPE_stride, int PE_size, \
                                          _type *pWrk, long *pSync)     \
    {                                                                   \
        const int stride = 1 << logPE_stride;                           \
        const int me = shmem_my_pe();                                   \
                                                                        \
        int me_as = (me - PE_start) / stride;                           \
        int peer;                                                       \
        size_t i;                                                       \
                                                                        \
        const size_t nelems = (const size_t) nreduce;                   \
        int block_idx_begin;                                            \
        int block_idx_end;                                              \
                                                                        \
        ptrdiff_t block_offset;                                         \
        ptrdiff_t next_block_offset;                                    \
        size_t block_nelems;                                            \
                                                                        \
        int xchg_peer_p2s;                                              \
        int xchg_peer_as;                                               \
        int xchg_peer_pe;                                               \
                                                                        \
        int ring_peer_p2s;                                              \
        int ring_peer_as;                                               \
        int ring_peer_pe;                                               \
                                                                        \
        /* Power 2 set */                                               \
        int me_p2s;                                                     \
        int p2s_size;                                                   \
        int log_p2s_size;                                               \
                                                                        \
        int distance;                                                   \
        _type *tmp_array = NULL;                                        \
                                                                        \
        long *collect_pSync = pSync + (1 + sizeof(int) * CHAR_BIT);     \
                                                                        \
        /* Find the greatest power of 2 lower than PE_size */           \
        for (p2s_size = 1, log_p2s_size = 0; p2s_size * 2 <= PE_size; p2s_size *= 2, log_p2s_size++); \
                                                                        \
        /* Check if the current PE belongs to the power 2 set */        \
        me_p2s = me_as * p2s_size / PE_size;                            \
        if ((me_p2s * PE_size + p2s_size - 1) / p2s_size != me_as) {    \
            me_p2s = -1;                                                \
        }                                                               \
                                                                        \
        /* If current PE belongs to the power 2 set, it will need temporary buffer */ \
        if (me_p2s != -1) {                                             \
            tmp_array = malloc((nelems / 2 + 1) * sizeof(_type));       \
            if (tmp_array == NULL) {                                    \
                /* TODO: raise error */                                 \
                fprintf(stderr, "PE %d: Cannot allocate memory!\n", me); \
                exit(-1);                                               \
            }                                                           \
        }                                                               \
                                                                        \
        /* Check if the current PE should wait/send data to the peer */ \
        if (me_p2s == -1) {                                             \
            /* Notify peer that the data is ready */                    \
            peer = PE_start + (me_as - 1) * stride;                     \
            shmem_long_p(pSync, SHCOLL_SYNC_VALUE + 1, peer);           \
                                                                        \
            /* Wait until the data on peer node is ready and get the data (upper half of the array) */ \
            block_offset = nelems / 2;                                  \
            block_nelems = (size_t) (nelems - block_offset);            \
                                                                        \
            shmem_long_wait_until(pSync, SHMEM_CMP_NE, SHCOLL_SYNC_VALUE); \
            shmem_getmem(dest + block_offset, source + block_offset, block_nelems * sizeof(_type), peer); \
                                                                        \
            /* Reduce the upper half of the array */                    \
            local_##_name##_reduce(dest + block_offset, dest + block_offset, source + block_offset, block_nelems); \
                                                                        \
            /* Send the upper half of the array to peer */              \
            shmem_putmem(dest + block_offset, dest + block_offset, block_nelems * sizeof(_type), peer); \
            shmem_fence();                                              \
            shmem_long_p(pSync, SHCOLL_SYNC_VALUE + 2, peer);           \
        } else if ((me_as + 1) * p2s_size / PE_size == me_p2s) {        \
            /* Notify peer that the data is ready */                    \
            peer = PE_start + (me_as + 1) * stride;                     \
            shmem_long_p(pSync, SHCOLL_SYNC_VALUE + 1, peer);           \
                                                                        \
            /* Wait until the data on peer node is ready and get the data (lower half of the array) */ \
            block_offset = 0;                                           \
            block_nelems = (size_t) (nelems / 2 - block_offset);        \
                                                                        \
            shmem_long_wait_until(pSync, SHMEM_CMP_GT, SHCOLL_SYNC_VALUE); \
            shmem_getmem(dest, source, block_nelems * sizeof(_type), peer); \
                                                                        \
            /* Do local reduce */                                       \
            local_##_name##_reduce(dest, dest, source, block_nelems);   \
                                                                        \
            /* Wait until the upper half is received from peer */       \
            shmem_long_wait_until(pSync, SHMEM_CMP_GT, SHCOLL_SYNC_VALUE + 1); \
            shmem_long_p(pSync, SHCOLL_SYNC_VALUE, me);                 \
        } else {                                                        \
            memcpy(dest, source, nelems * sizeof(_type));               \
        }                                                               \
                                                                        \
        /* For nodes in the power 2 set, dest contains data that should be reduced */ \
                                                                        \
        /* Do reduce scatter with the nodes in power 2 set */           \
        if (me_p2s != -1) {                                             \
            block_idx_begin = 0;                                        \
            block_idx_end = p2s_size;                                   \
                                                                        \
            for (distance = 1, i = 1; distance < p2s_size; distance <<= 1, i++) { \
                xchg_peer_p2s = ((me_p2s & distance) == 0) ? me_p2s + distance : me_p2s - distance; \
                xchg_peer_as = (xchg_peer_p2s * PE_size + p2s_size - 1) / p2s_size; \
                xchg_peer_pe = PE_start + xchg_peer_as * stride;        \
                                                                        \
                /* Notify the peer PE that the data is ready to be read */ \
                shmem_long_p(pSync + i, SHCOLL_SYNC_VALUE + 1, xchg_peer_pe); \
                                                                        \
                /* Check if the current PE is responsible for lower half of upper half of the vector */ \
                if ((me_p2s & distance) == 0) {                         \
                    block_idx_end = (block_idx_begin + block_idx_end) / 2; \
                } else {                                                \
                    block_idx_begin = (block_idx_begin + block_idx_end) / 2; \
                }                                                       \
                                                                        \
                /* TODO: possible overflow */                           \
                block_offset = (block_idx_begin * nelems) / p2s_size;   \
                next_block_offset = (block_idx_end * nelems) / p2s_size; \
                block_nelems = (size_t) (next_block_offset - block_offset); \
                                                                        \
                /* Wait until the data on peer PE is ready to be read and get the data */ \
                shmem_long_wait_until(pSync + i, SHMEM_CMP_GE, SHCOLL_SYNC_VALUE + 1); \
                shmem_getmem(tmp_array, dest + block_offset, block_nelems * sizeof(_type), xchg_peer_pe); \
                                                                        \
                /* Notify the peer PE that the data transfer has completed successfully */ \
                shmem_fence();                                          \
                shmem_long_p(pSync + i, SHCOLL_SYNC_VALUE + 2, xchg_peer_pe); \
                                                                        \
                /* Do local reduce */                                   \
                local_##_name##_reduce(dest + block_offset, dest + block_offset, tmp_array, block_nelems); \
                                                                        \
                /* Wait until the peer PE has read the data */          \
                shmem_long_wait_until(pSync + i, SHMEM_CMP_GE, SHCOLL_SYNC_VALUE + 2); \
                shmem_long_p(pSync + i, SHCOLL_SYNC_VALUE, me);         \
            }                                                           \
        }                                                               \
                                                                        \
        /* For nodes in the power 2 set, destination will contain the reduced block */ \
                                                                        \
        /* Do collect with the nodes in power 2 set */                  \
        if (me_p2s != -1) {                                             \
            ring_peer_p2s = (me_p2s + 1) % p2s_size;                    \
            ring_peer_as = (ring_peer_p2s * PE_size + p2s_size - 1) / p2s_size; \
            ring_peer_pe = PE_start + ring_peer_as * stride;            \
                                                                        \
            for (i = 0; i < p2s_size; i++) {                            \
                block_idx_begin = reverse_bits((int) ((me_p2s - i + p2s_size) % p2s_size), log_p2s_size); \
                block_idx_end = block_idx_begin + 1;                    \
                                                                        \
                /* TODO: possible overflow */                           \
                block_offset = (block_idx_begin * nelems) / p2s_size;   \
                next_block_offset = (block_idx_end * nelems) / p2s_size; \
                block_nelems = (size_t) (next_block_offset - block_offset); \
                                                                        \
                shmem_putmem_nbi(dest + block_offset, dest + block_offset, \
                                 block_nelems * sizeof(_type), ring_peer_pe); \
                shmem_fence();                                          \
                shmem_long_p(collect_pSync, SHCOLL_SYNC_VALUE + i + 1, ring_peer_pe); \
                                                                        \
                shmem_long_wait_until(collect_pSync, SHMEM_CMP_GT, SHCOLL_SYNC_VALUE + i); \
            }                                                           \
                                                                        \
            shmem_long_p(collect_pSync, SHCOLL_SYNC_VALUE, me);         \
        }                                                               \
                                                                        \
        /* Check if the current PE should wait/send data to the peer */ \
        if (me_p2s == -1) {                                             \
            /* Wait until the peer PE sends the data */                 \
            shmem_long_wait_until(pSync + 1, SHMEM_CMP_GE, SHCOLL_SYNC_VALUE + 1); \
            shmem_long_p(pSync + 1, SHCOLL_SYNC_VALUE, me);             \
        } else if ((me_as + 1) * p2s_size / PE_size == me_p2s) {        \
            peer = PE_start + (me_as + 1) * stride;                     \
            shmem_putmem(dest, dest, nelems * sizeof(_type), peer);     \
            shmem_fence();                                              \
            shmem_long_p(pSync + 1, SHCOLL_SYNC_VALUE + 1, peer);       \
        }                                                               \
                                                                        \
        if (tmp_array != NULL) {                                        \
            free(tmp_array);                                            \
        }                                                               \
    }


/*
 * Supported reduction operations
 */

#define AND_OP(A, B)  ((A) & (B))
#define MAX_OP(A, B)  ((A) > (B) ? (A) : (B))
#define MIN_OP(A, B)  ((A) < (B) ? (A) : (B))
#define SUM_OP(A, B)  ((A) + (B))
#define PROD_OP(A, B) ((A) * (B))
#define OR_OP(A, B)   ((A) | (B))
#define XOR_OP(A, B)  ((A) ^ (B))

/*
 * Definitions for all reductions
 */

#define SHCOLL_REDUCE_DEFINE(_name)                             \
    /* AND operation */                                         \
    _name(short_and,        short,      AND_OP)                 \
        _name(int_and,          int,        AND_OP)             \
        _name(long_and,         long,       AND_OP)             \
        _name(longlong_and,     long long,  AND_OP)             \
                                                                \
        /* MAX operation */                                     \
        _name(short_max,        short,          MAX_OP)         \
        _name(int_max,          int,            MAX_OP)         \
        _name(double_max,       double,         MAX_OP)         \
        _name(float_max,        float,          MAX_OP)         \
        _name(long_max,         long,           MAX_OP)         \
        _name(longdouble_max,   long double,    MAX_OP)         \
        _name(longlong_max,     long long,      MAX_OP)         \
                                                                \
        /* MIN operation */                                     \
        _name(short_min,        short,          MIN_OP)         \
        _name(int_min,          int,            MIN_OP)         \
        _name(double_min,       double,         MIN_OP)         \
        _name(float_min,        float,          MIN_OP)         \
        _name(long_min,         long,           MIN_OP)         \
        _name(longdouble_min,   long double,    MIN_OP)         \
        _name(longlong_min,     long long,      MIN_OP)         \
                                                                \
        /* SUM operation */                                     \
        _name(complexd_sum,     double _Complex,    SUM_OP)     \
        _name(complexf_sum,     float _Complex,     SUM_OP)     \
        _name(short_sum,        short,              SUM_OP)     \
        _name(int_sum,          int,                SUM_OP)     \
        _name(double_sum,       double,             SUM_OP)     \
        _name(float_sum,        float,              SUM_OP)     \
        _name(long_sum,         long,               SUM_OP)     \
        _name(longdouble_sum,   long double,        SUM_OP)     \
        _name(longlong_sum,     long long,          SUM_OP)     \
                                                                \
        /* PROD operation */                                    \
        _name(complexd_prod,    double _Complex,    PROD_OP)    \
        _name(complexf_prod,    float _Complex,     PROD_OP)    \
        _name(short_prod,       short,              PROD_OP)    \
        _name(int_prod,         int,                PROD_OP)    \
        _name(double_prod,      double,             PROD_OP)    \
        _name(float_prod,       float,              PROD_OP)    \
        _name(long_prod,        long,               PROD_OP)    \
        _name(longdouble_prod,  long double,        PROD_OP)    \
        _name(longlong_prod,    long long,          PROD_OP)    \
                                                                \
        /* OR operation */                                      \
        _name(short_or,         short,      OR_OP)              \
        _name(int_or,           int,        OR_OP)              \
        _name(long_or,          long,       OR_OP)              \
        _name(longlong_or,      long long,  OR_OP)              \
                                                                \
        /* XOR operation */                                     \
        _name(short_xor,        short,      XOR_OP)             \
        _name(int_xor,          int,        XOR_OP)             \
        _name(long_xor,         long,       XOR_OP)             \
        _name(longlong_xor,     long long,  XOR_OP)


/* @formatter:off */

#ifndef CMAKE
        SHCOLL_REDUCE_DEFINE(REDUCE_HELPER_LOCAL)
        SHCOLL_REDUCE_DEFINE(REDUCE_HELPER_LINEAR)
        SHCOLL_REDUCE_DEFINE(REDUCE_HELPER_BINOMIAL)
        SHCOLL_REDUCE_DEFINE(REDUCE_HELPER_REC_DBL)
        SHCOLL_REDUCE_DEFINE(REDUCE_HELPER_RABENSEIFNER)
        SHCOLL_REDUCE_DEFINE(REDUCE_HELPER_RABENSEIFNER2)
#else
        REDUCE_HELPER_LOCAL(int_sum, int, SUM_OP)
        REDUCE_HELPER_LINEAR(int_sum, int, SUM_OP)
        REDUCE_HELPER_BINOMIAL(int_sum, int, SUM_OP)
        REDUCE_HELPER_REC_DBL(int_sum, int, SUM_OP)
        REDUCE_HELPER_RABENSEIFNER(int_sum, int, SUM_OP)
        REDUCE_HELPER_RABENSEIFNER2(int_sum, int, SUM_OP)
#endif

/* @formatter:on */
