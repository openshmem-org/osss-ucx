/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "state.h"
#include "shmemu.h"

#include <stdlib.h>             /* getenv */
#include <string.h>
#include <strings.h>

#define KB 1024L
#define MB (KB * KB)
#define GB (KB * MB)

/*
 * internal helpers
 */

inline static char *
shmemc_getenv(char *name)
{
    return getenv(name);
}

inline static int
option_enabled_test(char *str)
{
    int ret = 0;

    if ((strncasecmp(str, "y", 1) == 0) ||
        (strncasecmp(str, "on", 2) == 0) ||
        (atoi(str) > 0)) {
        ret = 1;
    }

    return ret;
}

/*
 * read & save all our environment variables
 */

#define CHECK_ENV(_e, _name)                        \
    do {                                            \
        (_e) = shmemc_getenv("SHMEM_" #_name);      \
        if ((_e) == NULL) {                         \
            (_e) = shmemc_getenv("SMA_" #_name);    \
        }                                           \
    } while (0)

void
read_environment(void)
{
    char *e;

    /*
     * defined in spec
     */

    proc.env.print_version = 0;
    proc.env.print_info = 0;
    proc.env.def_heap_size = 4 * MB;
    proc.env.debug = 0;

    CHECK_ENV(e, VERSION);
    if (e != NULL) {
        proc.env.print_version = 1;
    }
    CHECK_ENV(e, INFO);
    if (e != NULL) {
        proc.env.print_info = 1;
    }
    CHECK_ENV(e, SYMMETRIC_SIZE);
    if (e != NULL) {
        const int r = shmemu_parse_size(e, &proc.env.def_heap_size);

        if (r != 0) {
            shmemu_fatal("Couldn't work out requested heap size \"%s\"", e);
        }
    }
    CHECK_ENV(e, DEBUG);
    if (e != NULL) {
        proc.env.debug = option_enabled_test(e);
    }

    /*
     * this implementation also has...
     */

    proc.env.debug_file = NULL;

    CHECK_ENV(e, DEBUG_FILE);
    if (e != NULL) {
        proc.env.debug_file = strdup(e); /* free at end */
    }
}

#undef CHECK_ENV
