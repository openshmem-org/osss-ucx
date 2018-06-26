/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"

#include "klib/khash.h"

KHASH_SET_INIT_STR(deprecations)

static khash_t(deprecations) *table;

/*
 * restrict report to first usage only or it's far too noisy
 *
 * (maj, min) is the spec version in which the routine fn_name was
 * first deprecated
 *
 */
void
shmemu_deprecate(const char *fn_name, int maj, int min)
{
    int absent;

    (void) kh_put(deprecations, table, fn_name, &absent);

    if (shmemu_likely(absent) == 0) {
        return;
        /* NOT REACHED */
    }

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
