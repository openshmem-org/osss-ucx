#include <stdio.h>

#include "shmemu.h"

/**
 * stub for the proposed UFL profiling (PSHMEM) interface
 *
 */

void
shmem_pcontrol(int level)
{
    char *msg = NULL;

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
}
