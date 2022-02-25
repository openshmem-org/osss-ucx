/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "boolean.h"

#include "../klib/khash.h"

KHASH_MAP_INIT_STR(deprecations, bool)

static khash_t(deprecations) *table;

/*
 * restrict report to first usage only or it's far too noisy
 *
 * (maj, min) is the spec version in which the routine fn_name was
 * first deprecated
 *
 */
void
shmemu_deprecate(const char *fn_name, const shmemu_version_t *vp)
{
    khiter_t k;
    int ret;

    /* already there? */
    k = kh_get(deprecations, table, fn_name);
    if (k != kh_end(table)) {
        return;
        /* NOT REACHED */
    }

    k = kh_put(deprecations, table, fn_name, &ret);
    /* ignore return status */

    kh_value(table, k) = true;

    logger(LOG_DEPRECATE,
           "\"%s\" is deprecated as of specification %d.%d",
           fn_name,
           vp->major, vp->minor
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
