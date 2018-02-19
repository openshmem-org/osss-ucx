/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */


#include "state.h"
#include "shmemu.h"
#include "boolean.h"

#include <string.h>

int
shmemu_get_children_info(int tree_size, int tree_degree, int node,
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
shmemu_get_children_info_binomial(int tree_size, int node, int *children)
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

static struct algo_desc {
    char *name;
    shmemc_coll_t algo;
} algo_desc_table[] = {
    { "linear",     SHMEMC_COLL_LINEAR    },
    { "tree",       SHMEMC_COLL_TREE      },
    { "dissem",     SHMEMC_COLL_DISSEM    },
    { "binomial",   SHMEMC_COLL_BINOMIAL  },
    { "binomial2",  SHMEMC_COLL_BINOMIAL2  },
    { NULL,         SHMEMC_COLL_UNKNOWN   }
};

shmemc_coll_t
shmemu_parse_algo(char *str)
{
    struct algo_desc *adp = algo_desc_table;

    while (adp->name != NULL) {
        if (strncmp(str, adp->name, strlen(adp->name)) == 0) {
            return adp->algo;
            /* NOT REACHED */
        }
        adp += 1;
    }

    return SHMEMC_COLL_UNKNOWN;
}

char *
shmemu_unparse_algo(shmemc_coll_t algo)
{
    struct algo_desc *adp = algo_desc_table;

    while (adp->name != NULL) {
        if (algo == adp->algo) {
            return adp->name;
            /* NOT REACHED */
        }
        adp += 1;
    }

    return "unknown";
}
