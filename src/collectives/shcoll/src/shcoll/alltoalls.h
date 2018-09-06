#ifndef _SHCOLL_ALLTOALLS_H
#define _SHCOLL_ALLTOALLS_H 1

#define SHCOLL_ALLTOALLS_DECLARATION(_name, _size)              \
    void shcoll_alltoalls##_size##_##_name(void *dest,          \
                                           const void *source,  \
                                           ptrdiff_t dst,       \
                                           ptrdiff_t sst,       \
                                           size_t nelems,       \
                                           int PE_start,        \
                                           int logPE_stride,    \
                                           int PE_size,         \
                                           long *pSync);

SHCOLL_ALLTOALLS_DECLARATION(shift_exchange_barrier, 32)
SHCOLL_ALLTOALLS_DECLARATION(shift_exchange_barrier, 64)

SHCOLL_ALLTOALLS_DECLARATION(shift_exchange_counter, 32)
SHCOLL_ALLTOALLS_DECLARATION(shift_exchange_counter, 64)

SHCOLL_ALLTOALLS_DECLARATION(shift_exchange_barrier_nbi, 32)
SHCOLL_ALLTOALLS_DECLARATION(shift_exchange_barrier_nbi, 64)

SHCOLL_ALLTOALLS_DECLARATION(shift_exchange_counter_nbi, 32)
SHCOLL_ALLTOALLS_DECLARATION(shift_exchange_counter_nbi, 64)


SHCOLL_ALLTOALLS_DECLARATION(xor_pairwise_exchange_barrier, 32)
SHCOLL_ALLTOALLS_DECLARATION(xor_pairwise_exchange_barrier, 64)

SHCOLL_ALLTOALLS_DECLARATION(xor_pairwise_exchange_counter, 32)
SHCOLL_ALLTOALLS_DECLARATION(xor_pairwise_exchange_counter, 64)

SHCOLL_ALLTOALLS_DECLARATION(xor_pairwise_exchange_barrier_nbi, 32)
SHCOLL_ALLTOALLS_DECLARATION(xor_pairwise_exchange_barrier_nbi, 64)

SHCOLL_ALLTOALLS_DECLARATION(xor_pairwise_exchange_counter_nbi, 32)
SHCOLL_ALLTOALLS_DECLARATION(xor_pairwise_exchange_counter_nbi, 64)


SHCOLL_ALLTOALLS_DECLARATION(color_pairwise_exchange_barrier, 32)
SHCOLL_ALLTOALLS_DECLARATION(color_pairwise_exchange_barrier, 64)

SHCOLL_ALLTOALLS_DECLARATION(color_pairwise_exchange_counter, 32)
SHCOLL_ALLTOALLS_DECLARATION(color_pairwise_exchange_counter, 64)

SHCOLL_ALLTOALLS_DECLARATION(color_pairwise_exchange_barrier_nbi, 32)
SHCOLL_ALLTOALLS_DECLARATION(color_pairwise_exchange_barrier_nbi, 64)

SHCOLL_ALLTOALLS_DECLARATION(color_pairwise_exchange_counter_nbi, 32)
SHCOLL_ALLTOALLS_DECLARATION(color_pairwise_exchange_counter_nbi, 64)

#endif /* ! _SHCOLL_ALLTOALLS_H */
