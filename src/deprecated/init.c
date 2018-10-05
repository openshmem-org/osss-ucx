/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"

#include "shmem/api.h"

/*
 * deprecated
 */

#ifdef ENABLE_PSHMEM
#pragma weak start_pes = pstart_pes
#define start_pes pstart_pes
#endif /* ENABLE_PSHMEM */

void
start_pes(int n)
{
    NO_WARN_UNUSED(n);

    shmem_init();

    /* can't tell anyone until init has really happened */
    deprecate(__func__, 1, 2);
}
