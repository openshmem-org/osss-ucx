/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */


#include "state.h"
#include "shmemu.h"
#include "boolean.h"

#include <string.h>


struct algo_desc {
    char *name;
    shmemc_coll_t algo;
} algo_desc_table[] = {
    { "linear",     SHMEMC_COLL_LINEAR    },
    { "tree",       SHMEMC_COLL_TREE      },
    { "dissem",     SHMEMC_COLL_DISSEM    },
    { NULL,         SHMEMC_COLL_UNKNOWN   }
};

shmemc_coll_t
parse_algo(char *str)
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
unparse_algo(shmemc_coll_t algo)
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
