/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>

#include "shmemu.h"

/**
 * stub for the proposed UFL profiling (PSHMEM) interface
 *
 */

void
shmemx_pcontrol(int level)
{
    NO_WARN_UNUSED(level);

#ifdef ENABLE_LOGGING
    char *msg;

    switch (level) {
    case 0:
        msg = "disabled";
        break;
    case 1:
        msg = "enabled (default detail)";
        break;
    default:
        msg = "tool-specific";
        break;
    }

    logger(LOG_INFO, "shmem_pcontrol(%d) is %s", level, msg);
#endif  /* ENABLE_LOGGING */
}
