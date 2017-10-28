/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef ENABLE_DEBUG

#include "shmemu.h"
#include "uthash.h"

#include <stdlib.h>
#include <assert.h>

struct depr {
    char *name;
    unsigned long count;
    UT_hash_handle hh;
};

static struct depr *table;

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

void
shmemu_deprecate_init(void)
{
    table = NULL;
}

void
shmemu_deprecate_finalize(void)
{
    struct depr *cur;
    struct depr *tmp;

    HASH_ITER(hh, table, cur, tmp) {
        free(cur->name);   /* was strdup'ed above */
        HASH_DEL(table, cur);
    }
}

#endif /* ENABLE_DEBUG */
