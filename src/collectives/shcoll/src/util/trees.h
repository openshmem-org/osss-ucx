#ifndef OPENSHMEM_COLLECTIVE_ROUTINES_TREES_H
#define OPENSHMEM_COLLECTIVE_ROUTINES_TREES_H

#include <limits.h>

#define MAX_KNOMIAL_RADIX 32

typedef struct {
    int parent;
    int children_num;
    int children[sizeof(int) * CHAR_BIT];
} node_info_binomial_t;

void get_node_info_binomial(int tree_size, int node, node_info_binomial_t *node_info);

void get_node_info_binomial_root(int tree_size, int root, int node, node_info_binomial_t *node_info);


typedef struct {
    int parent;
    int children_num;
    /* (max_radix - 1) * log_(max_radix) */
    int children[(MAX_KNOMIAL_RADIX-1) * 6];
    int groups_num;
    int groups_sizes[sizeof(int) * CHAR_BIT];
} node_info_knomial_t;

void get_node_info_knomial(int tree_size, int k, int node, node_info_knomial_t *node_info);

void get_node_info_knomial_root(int tree_size, int root, int k, int node, node_info_knomial_t *node_info);


typedef struct _node_info_complete {
    int parent;
    int children_num;
    int children_begin;
    int children_end;
} node_info_complete_t;

void get_node_info_complete(int tree_size, int tree_degree, int node, node_info_complete_t *node_info);

void get_node_info_complete_root(int tree_size, int root, int tree_degree, int node, node_info_complete_t *node_info);

#endif /* OPENSHMEM_COLLECTIVE_ROUTINES_TREES_H */
