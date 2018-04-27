/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"

#include "klib/khash.h"

KHASH_SET_INIT_STR(deprecations)

static khash_t(deprecations) *table;

inline static int
already_seen(const char *name)
{
    const khint_t k = kh_get(deprecations, table, name);

    return (k != kh_end(table));
}

inline static void
record(const char *name)
{
    int absent;

    (void) kh_put(deprecations, table, name, &absent);
}

/*
 * need to restrict this report to first usage only
 */
void
shmemu_deprecate(const char *fn_name, int maj, int min)
{
    if (already_seen(fn_name)) {
        return;
        /* NOT REACHED */
    }

    record(fn_name);

    logger(LOG_DEPRECATE,
           "\"%s\" is deprecated as of specification %d.%d",
           fn_name,
           maj, min
           );
}

void
shmemu_deprecate_init(void)
{
    table = kh_init(deprecations);
}

void
shmemu_deprecate_finalize(void)
{
    kh_destroy(deprecations, table);
}
