/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */


#include "state.h"
#include "boolean.h"

#include <string.h>

int
shmemc_get_children_info(int tree_size, int tree_degree, int node,
                         int *children_begin, int *children_end)
{
    *children_begin = node * tree_degree + 1;

    if (*children_begin >= tree_size) {
        *children_begin = *children_end = -1;
        return 0;
        /* NOT REACHED */
    }

    *children_end = *children_begin + tree_degree;

    if (*children_end > tree_size) {
        *children_end = tree_size;
    }

    return *children_end - *children_begin;
}

int
shmemc_get_children_info_binomial(int tree_size, int node, int *children)
{
    int children_num = 0;
    int mask;                   /* Lowest bit */

    if (node == 0) {
        mask = 1 << (sizeof(int) * 8 - 2);
    } else {
        mask = node & ~(node - 1);
        mask >>= 1;
    }

    while (mask != 0) {
        const int child = node | mask;

        if (child < tree_size) {
            children[children_num++] = child;
        }

        mask >>= 1;
    }

    return children_num;
}
