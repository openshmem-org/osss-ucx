#ifndef _SHCOLL_ALLTOALL_H
#define _SHCOLL_ALLTOALL_H 1

#define SHCOLL_ALLTOALL_DECLARATION(_name, _size)               \
    void shcoll_alltoall##_size##_##_name(void *dest,           \
                                          const void *source,   \
                                          size_t nelems,        \
                                          int PE_start,         \
                                          int logPE_stride,     \
                                          int PE_size,          \
                                          long *pSync);

SHCOLL_ALLTOALL_DECLARATION(shift_exchange_barrier, 32)
SHCOLL_ALLTOALL_DECLARATION(shift_exchange_barrier, 64)

SHCOLL_ALLTOALL_DECLARATION(shift_exchange_counter, 32)
SHCOLL_ALLTOALL_DECLARATION(shift_exchange_counter, 64)

SHCOLL_ALLTOALL_DECLARATION(shift_exchange_signal, 32)
SHCOLL_ALLTOALL_DECLARATION(shift_exchange_signal, 64)


SHCOLL_ALLTOALL_DECLARATION(xor_pairwise_exchange_barrier, 32)
SHCOLL_ALLTOALL_DECLARATION(xor_pairwise_exchange_barrier, 64)

SHCOLL_ALLTOALL_DECLARATION(xor_pairwise_exchange_counter, 32)
SHCOLL_ALLTOALL_DECLARATION(xor_pairwise_exchange_counter, 64)

SHCOLL_ALLTOALL_DECLARATION(xor_pairwise_exchange_signal, 32)
SHCOLL_ALLTOALL_DECLARATION(xor_pairwise_exchange_signal, 64)


SHCOLL_ALLTOALL_DECLARATION(color_pairwise_exchange_barrier, 32)
SHCOLL_ALLTOALL_DECLARATION(color_pairwise_exchange_barrier, 64)

SHCOLL_ALLTOALL_DECLARATION(color_pairwise_exchange_counter, 32)
SHCOLL_ALLTOALL_DECLARATION(color_pairwise_exchange_counter, 64)

SHCOLL_ALLTOALL_DECLARATION(color_pairwise_exchange_signal, 32)
SHCOLL_ALLTOALL_DECLARATION(color_pairwise_exchange_signal, 64)

#endif /* ! _SHCOLL_ALLTOALL_H */
