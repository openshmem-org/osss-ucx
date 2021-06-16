#ifndef _SHCOLL_BARRIER_H
#define _SHCOLL_BARRIER_H 1

void shcoll_set_tree_degree(int tree_degree);
void shcoll_set_knomial_tree_radix_barrier(int tree_radix);

#define SHCOLL_BARRIER_SYNC_DECLARATION(_name)                  \
    void shcoll_barrier_##_name(int PE_start, int logPE_stride, \
                                int PE_size, long *pSync);      \
                                                                \
    void shcoll_barrier_all_##_name(long *pSync);               \
                                                                \
    void shcoll_sync_##_name(int PE_start, int logPE_stride,    \
                             int PE_size, long *pSync);         \
                                                                \
    void shcoll_sync_all_##_name(long *pSync);

SHCOLL_BARRIER_SYNC_DECLARATION(linear)
SHCOLL_BARRIER_SYNC_DECLARATION(complete_tree)
SHCOLL_BARRIER_SYNC_DECLARATION(binomial_tree)
SHCOLL_BARRIER_SYNC_DECLARATION(knomial_tree)
SHCOLL_BARRIER_SYNC_DECLARATION(dissemination)

#endif /* ! _SHCOLL_BARRIER_H */
