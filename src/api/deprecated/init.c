/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"
#include "shmemu.h"

/*
 * deprecated
 */

static const shmemu_version_t v = { .major = 1, .minor = 2 };

#ifdef ENABLE_PSHMEM
#pragma weak start_pes = pstart_pes
#define start_pes pstart_pes
#endif /* ENABLE_PSHMEM */

void
start_pes(int n)
{
    NO_WARN_UNUSED(n);

    shmemc_init();

    /* can't tell anyone until init has really happened */
    deprecate(__func__, &v);
}
