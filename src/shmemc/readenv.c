/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "state.h"
#include "shmemu.h"
#include "boolean.h"

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

static bool
option_enabled_test(char *str)
{
    if (str == NULL) {
        return false;
        /* NOT REACHED */
    }
    if (*str == 'y') {
        return true;
        /* NOT REACHED */
    }
    if (strncasecmp(str, "on", 2) == 0) {
        return true;
        /* NOT REACHED */
    }
    if (atoi(str) > 0) {
        return true;
        /* NOT REACHED */
    }

    return false;
}

struct algo_desc {
    char *name;
    shmemc_coll_t algo;
} algo_desc_table[] = {
    { "linear",     SHMEMC_COLL_LINEAR    },
    { "tree",       SHMEMC_COLL_TREE      },
    { "dissem",     SHMEMC_COLL_DISSEM    },
    { NULL,         SHMEMC_COLL_UNKNOWN   }
};

static shmemc_coll_t
parse_algo(char *str)
{
    struct algo_desc *adp = algo_desc_table;

    while (adp->name != NULL) {
        if (strncmp(str, adp->name, strlen(adp->name)) == 0) {
            return adp->algo;
            /* NOT REACHED */
        }
        adp += 1;
    }

    return SHMEMC_COLL_UNKNOWN;
}

static char *
unparse_algo(shmemc_coll_t algo)
{
    struct algo_desc *adp = algo_desc_table;

    while (adp->name != NULL) {
        if (algo == adp->algo) {
            return adp->name;
            /* NOT REACHED */
        }
        adp += 1;
    }

    return "unknown";
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
shmemc_env_init(void)
{
    char *e;

    /*
     * defined in spec
     */

    proc.env.print_version = false;
    proc.env.print_info = false;
    proc.env.def_heap_size = 32 * SHMEMU_MB; /* arbitrary value */
    proc.env.debug = false;

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
    proc.env.xpmem_kludge = false;
    proc.env.barrier_algo = SHMEMC_COLL_LINEAR;

    CHECK_ENV(e, DEBUG_FILE);
    if (e != NULL) {
        proc.env.debug_file = strdup(e); /* free at end */
    }
    CHECK_ENV(e, XPMEM_KLUDGE);
    if (e != NULL) {
        proc.env.xpmem_kludge = option_enabled_test(e);
    }
    CHECK_ENV(e, BARRIER_ALGO);
    if (e != NULL) {
        proc.env.barrier_algo = parse_algo(e);
    }
}

#undef CHECK_ENV

void
shmemc_env_finalize(void)
{
    if (proc.env.debug_file != NULL) {
        free(proc.env.debug_file);
    }
}

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
#define BUFSIZE 16
        char buf[BUFSIZE];

        (void) shmemu_human_number(proc.env.def_heap_size, buf, BUFSIZE);
        fprintf(stream, "%s%-*s %-*s %s\n",
                prefix,
                var_width, "SHMEM_SYMMETRIC_SIZE",
                val_width, buf,
                "set the size of the symmetric heap");
#undef BUFSIZE
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
    fprintf(stream, "%s%-*s %-*s %s\n",
            prefix,
            var_width, "SHMEM_XPMEM_KLUDGE",
            val_width, shmemu_human_option(proc.env.xpmem_kludge),
            "avoid XPMEM tear-down bug (temporary)");
    fprintf(stream, "%s%-*s %-*s %s\n",
            prefix,
            var_width, "SHMEM_BARRIER_ALGO",
            val_width, unparse_algo(proc.env.barrier_algo),
            "algorithm to use for barrier");

    fprintf(stream, "%s\n",
            prefix);
    fprintf(stream, "\n");

    fflush(stream);
}
