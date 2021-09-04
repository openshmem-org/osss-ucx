/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"

#include <stdio.h>
#include <stdarg.h>

/*
 * stub for the profiling (PSHMEM) interface
 *
 * See OpenSHMEM 1.5 spec, p. 141.  Appears to be typo re. level 2,
 * assume last entry is > 2 instead of >= 2.
 */

static int profiling_level = 1; /* default */

void
shmem_pcontrol(const int level, ...)
{
    char *msg;

    if (level <= 0) {
        msg = "disabled";
    }
    else if (level == 1) {
        msg = "enabled "
            "(default detail)";
    }
    else if (level == 2) {
        msg = "enabled "
            "(profile buffers flushed)";
    }
    else {                      /* > 2 */
        msg = "enabled "
            "(profile library defined effects and additional arguments)";
    }

    profiling_level = level;

    logger(LOG_INFO,
           "shmem_pcontrol(level = %d) set to \"%s\"",
           level, msg);

#ifndef ENABLE_LOGGING
    NO_WARN_UNUSED(msg);
#endif /* ! ENABLE_LOGGING */
}
