/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"

#include "uthash.h"

#include <stdlib.h>
#include <assert.h>

struct depr {
    char *name;
    UT_hash_handle hh;
};

static struct depr *table = NULL;

inline static int
already_seen(const char *name)
{
    struct depr *lu = NULL;

    HASH_FIND_STR(table, name, lu);

    return (lu != NULL) ? 1 : 0;
}

inline static void
record(const char *name)
{
    struct depr *dp = (struct depr *) malloc(sizeof(*dp));

    assert(dp != NULL);

    dp->name = strdup(name);

    HASH_ADD_STR(table, name, dp);
}

/*
 * need to restrict this report to first usage only
 */
void
shmemu_deprecate(const char *fn_name)
{
    if (already_seen(fn_name)) {
        return;
    }

    record(fn_name);

    logger(LOG_DEPRECATE, "\"%s\" is deprecated", fn_name);
}

void
shmemu_deprecate_init(void)
{
    return;
}

void
shmemu_deprecate_finalize(void)
{
    struct depr *cur = NULL;
    struct depr *tmp = NULL;

    HASH_ITER(hh, table, cur, tmp) {
        free(cur->name);   /* was strdup'ed above */
        HASH_DEL(table, cur);
    }
}
