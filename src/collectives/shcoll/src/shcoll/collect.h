#ifndef _SHCOLL_COLLECT_H
#define _SHCOLL_COLLECT_H 1

#define SHCOLL_COLLECT_DECLARATION(_name, _size)                \
    void shcoll_collect##_size##_##_name(void *dest,            \
                                         const void *source,    \
                                         size_t nelems,         \
                                         int PE_start,          \
                                         int logPE_stride,      \
                                         int PE_size,           \
                                         long *pSync);

SHCOLL_COLLECT_DECLARATION(linear, 32)
SHCOLL_COLLECT_DECLARATION(linear, 64)

SHCOLL_COLLECT_DECLARATION(all_linear, 32)
SHCOLL_COLLECT_DECLARATION(all_linear, 64)

SHCOLL_COLLECT_DECLARATION(all_linear1, 32)
SHCOLL_COLLECT_DECLARATION(all_linear1, 64)

SHCOLL_COLLECT_DECLARATION(rec_dbl, 32)
SHCOLL_COLLECT_DECLARATION(rec_dbl, 64)

SHCOLL_COLLECT_DECLARATION(rec_dbl_signal, 32)
SHCOLL_COLLECT_DECLARATION(rec_dbl_signal, 64)

SHCOLL_COLLECT_DECLARATION(ring, 32)
SHCOLL_COLLECT_DECLARATION(ring, 64)

SHCOLL_COLLECT_DECLARATION(bruck, 32)
SHCOLL_COLLECT_DECLARATION(bruck, 64)

SHCOLL_COLLECT_DECLARATION(bruck_no_rotate, 32)
SHCOLL_COLLECT_DECLARATION(bruck_no_rotate, 64)

#endif /* ! _SHCOLL_COLLECT_H */
