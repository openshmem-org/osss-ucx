
/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"
#include "state.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void
shmemu_warn(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "*** PE %d: WARNING: ", proc.rank);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, " ***\n");
    fflush(stderr);
    va_end(ap);
}

void
shmemu_fatal(const char *fmt, ...)
{
    /* this test also handles an uninitialized state */
    if (proc.rank < 1) {
        va_list ap;

        va_start(ap, fmt);
        fprintf(stderr, "*** PE %d: FATAL: ", proc.rank);
        vfprintf(stderr, fmt, ap);
        fprintf(stderr, " ***\n");
        fflush(stderr);
        va_end(ap);
    }

    shmemc_global_exit(EXIT_FAILURE);
    /* NOT REACHED */
}
