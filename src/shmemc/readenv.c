/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "state.h"
#include "shmemu.h"

#include <stdio.h>
#include <stdlib.h>             /* getenv */
#include <string.h>
#include <strings.h>

/*
 * internal helpers
 */

/*
 * return non-zero if option enabled, 0 if not
 */

inline static int
option_enabled_test(char *str)
{
    if (strncasecmp(str, "y", 1) == 0) {
        return 1;
        /* NOT REACHED */
    }
    if (strncasecmp(str, "on", 2) == 0) {
        return 1;
        /* NOT REACHED */
    }
    if (atoi(str) > 0) {
        return 1;
        /* NOT REACHED */
    }

    return 0;
}

/*
 * read & save all our environment variables
 */

#define CHECK_ENV(_e, _name)                        \
    do {                                            \
        (_e) = getenv("SHMEM_" #_name);             \
        if ((_e) == NULL) {                         \
            (_e) = getenv("SMA_" #_name);           \
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
    proc.env.def_heap_size = 32 * SHMEMU_MB; /* arbitrary value */
    proc.env.debug = 0;

    CHECK_ENV(e, VERSION);
    if (e != NULL) {
        proc.env.print_version = option_enabled_test(e);
    }
    CHECK_ENV(e, INFO);
    if (e != NULL) {
        proc.env.print_info = option_enabled_test(e);
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
    proc.env.xpmem_kludge = 0;

    CHECK_ENV(e, DEBUG_FILE);
    if (e != NULL) {
        proc.env.debug_file = strdup(e); /* free at end */
    }
    CHECK_ENV(e, XPMEM_KLUDGE);
    if (e != NULL) {
        proc.env.xpmem_kludge = option_enabled_test(e);
    }
}

#undef CHECK_ENV

static const int var_width = 22;
static const int val_width = 10;

void
shmemc_print_env_vars(FILE *stream, const char *prefix)
{
    fprintf(stream, "%sEnvironment Variable Information\n",
            prefix);
    fprintf(stream, "%s\n",
            prefix);
    fprintf(stream, "%s%-*s %-*s %s\n",
            prefix,
            var_width, "Variable",
            val_width, "Value",
            "Description");
    fprintf(stream, "%s------------------------------------------------------------------------\n",
            prefix);
    fprintf(stream, "%s\n",
            prefix);
    fprintf(stream, "%s%s\n",
            prefix,
            "From specification:");
    fprintf(stream, "%s\n",
            prefix);
    fprintf(stream, "%s%-*s %-*s %s\n",
            prefix,
            var_width, "SHMEM_VERSION",
            val_width, shmemu_human_option(proc.env.print_version),
            "print library version at start-up");
    fprintf(stream, "%s%-*s %-*s %s\n",
            prefix,
            var_width, "SHMEM_INFO",
            val_width, shmemu_human_option(proc.env.print_info),
            "print this information");
    {
        char buf[64];

        (void) shmemu_human_number(proc.env.def_heap_size, buf, 64);
        fprintf(stream, "%s%-*s %-*s %s\n",
                prefix,
                var_width, "SHMEM_SYMMETRIC_SIZE",
                val_width, buf,
                "set the size of the symmetric heap");
    }
    fprintf(stream, "%s%-*s %-*s %s\n",
            prefix,
            var_width, "SHMEM_DEBUG",
            val_width, shmemu_human_option(proc.env.debug),
            "enable run debugging (if configured)");

    fprintf(stream, "%s\n",
            prefix);
    fprintf(stream, "%s%s\n",
            prefix,
            "Specific to this implementation:");
    fprintf(stream, "%s\n",
            prefix);
    fprintf(stream, "%s%-*s %-*s %s\n",
            prefix,
            var_width, "SHMEM_DEBUG_FILE",
            val_width, proc.env.debug_file ? proc.env.debug_file : "none",
            "file to receive debugging information");

    fprintf(stream, "%s\n",
            prefix);
    fprintf(stream, "\n");

    fflush(stream);
}
