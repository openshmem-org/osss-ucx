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

#define DO_CARP(_type)                          \
    do {                                        \
        va_list ap;                             \
                                                \
        fprintf(stderr,                         \
                "*** PE %d: %s: ",              \
                proc.li.rank, #_type);          \
        va_start(ap, fmt);                      \
        vfprintf(stderr, fmt, ap);              \
        va_end(ap);                             \
        fprintf(stderr, " ***\n");              \
        fflush(stderr);                         \
    } while (0)

void
shmemu_warn(const char *fmt, ...)
{
    DO_CARP(WARNING);
}

void
shmemu_fatal(const char *fmt, ...)
{
    /* this test also handles an uninitialized state */
    if (proc.li.rank < 1) {
        DO_CARP(FATAL);
    }

    shmemc_global_exit(EXIT_FAILURE);
    /* NOT REACHED */
}
