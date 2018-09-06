#ifndef _SHCOLL_FCOLLECT_H
#define _SHCOLL_FCOLLECT_H 1

#define SHCOLL_FCOLLECT_DECLARATION(_name, _size)               \
    void shcoll_fcollect##_size##_##_name(void *dest,           \
                                          const void *source,   \
                                          size_t nelems,        \
                                          int PE_start,         \
                                          int logPE_stride,     \
                                          int PE_size,          \
                                          long *pSync);

SHCOLL_FCOLLECT_DECLARATION(linear, 32)
SHCOLL_FCOLLECT_DECLARATION(linear, 64)

SHCOLL_FCOLLECT_DECLARATION(all_linear, 32)
SHCOLL_FCOLLECT_DECLARATION(all_linear, 64)

SHCOLL_FCOLLECT_DECLARATION(all_linear1, 32)
SHCOLL_FCOLLECT_DECLARATION(all_linear1, 64)

SHCOLL_FCOLLECT_DECLARATION(rec_dbl, 32)
SHCOLL_FCOLLECT_DECLARATION(rec_dbl, 64)

SHCOLL_FCOLLECT_DECLARATION(ring, 32)
SHCOLL_FCOLLECT_DECLARATION(ring, 64)

SHCOLL_FCOLLECT_DECLARATION(bruck, 32)
SHCOLL_FCOLLECT_DECLARATION(bruck, 64)

SHCOLL_FCOLLECT_DECLARATION(bruck_no_rotate, 32)
SHCOLL_FCOLLECT_DECLARATION(bruck_no_rotate, 64)

SHCOLL_FCOLLECT_DECLARATION(bruck_signal, 32)
SHCOLL_FCOLLECT_DECLARATION(bruck_signal, 64)

SHCOLL_FCOLLECT_DECLARATION(bruck_inplace, 32)
SHCOLL_FCOLLECT_DECLARATION(bruck_inplace, 64)

SHCOLL_FCOLLECT_DECLARATION(neighbor_exchange, 32)
SHCOLL_FCOLLECT_DECLARATION(neighbor_exchange, 64)

#endif /* ! _SHCOLL_FCOLLECT_H */
