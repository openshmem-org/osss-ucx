/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "uthash.h"

#include <assert.h>

struct depr {
    const char *name;
    unsigned long count;
    UT_hash_handle hh;
};

static struct depr *table = NULL;

/*
 * need to restrict this report to first usage only
 */
void
deprecate(const char *fn)
{
    struct depr *lu;

    HASH_FIND_STR(table, fn, lu);

    if (lu == NULL) {
        struct depr *dp = (struct depr *) malloc(sizeof(*dp));

        assert(dp != NULL);

        dp->name = strdup(fn);  /* TODO: memleak */
        dp->count = 1;

        HASH_ADD_STR(table, name, dp);

        logger(LOG_DEPRECATE, "\"%s\" is deprecated", fn);
    }
}
