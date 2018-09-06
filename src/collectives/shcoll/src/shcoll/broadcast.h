#ifndef _SHCOLL_BROADCAST_H
#define _SHCOLL_BROADCAST_H 1

void shcoll_set_broadcast_tree_degree(int tree_degree);
void shcoll_set_broadcast_knomial_tree_radix_barrier(int tree_radix);

#define SHCOLL_BROADCAST_DECLARATION(_name, _size)              \
    void shcoll_broadcast##_size##_##_name(void *dest,          \
                                           const void *source,  \
                                           size_t nelems,       \
                                           int PE_root,         \
                                           int PE_start,        \
                                           int logPE_stride,    \
                                           int PE_size,         \
                                           long *pSync);

SHCOLL_BROADCAST_DECLARATION(linear, 8)
SHCOLL_BROADCAST_DECLARATION(linear, 16)
SHCOLL_BROADCAST_DECLARATION(linear, 32)
SHCOLL_BROADCAST_DECLARATION(linear, 64)

SHCOLL_BROADCAST_DECLARATION(complete_tree, 8)
SHCOLL_BROADCAST_DECLARATION(complete_tree, 16)
SHCOLL_BROADCAST_DECLARATION(complete_tree, 32)
SHCOLL_BROADCAST_DECLARATION(complete_tree, 64)

SHCOLL_BROADCAST_DECLARATION(binomial_tree, 8)
SHCOLL_BROADCAST_DECLARATION(binomial_tree, 16)
SHCOLL_BROADCAST_DECLARATION(binomial_tree, 32)
SHCOLL_BROADCAST_DECLARATION(binomial_tree, 64)

SHCOLL_BROADCAST_DECLARATION(knomial_tree, 8)
SHCOLL_BROADCAST_DECLARATION(knomial_tree, 16)
SHCOLL_BROADCAST_DECLARATION(knomial_tree, 32)
SHCOLL_BROADCAST_DECLARATION(knomial_tree, 64)

SHCOLL_BROADCAST_DECLARATION(knomial_tree_signal, 8)
SHCOLL_BROADCAST_DECLARATION(knomial_tree_signal, 16)
SHCOLL_BROADCAST_DECLARATION(knomial_tree_signal, 32)
SHCOLL_BROADCAST_DECLARATION(knomial_tree_signal, 64)

SHCOLL_BROADCAST_DECLARATION(scatter_collect, 8)
SHCOLL_BROADCAST_DECLARATION(scatter_collect, 16)
SHCOLL_BROADCAST_DECLARATION(scatter_collect, 32)
SHCOLL_BROADCAST_DECLARATION(scatter_collect, 64)

#endif /* ! _SHCOLL_BROADCAST_H */
