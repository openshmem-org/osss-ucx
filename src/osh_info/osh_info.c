/* For license: see LICENSE file at top-level */

/* no config.h */

#include "info.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include <libgen.h>             /* basename */

static char *progname;

static char *default_prefix = "# ";
static char *default_suffix = "";

static void
output_help(void)
{
    fprintf(stderr,
            "\n");
    fprintf(stderr,
            "Usage: %s [options]\n\n",
            progname);
    fprintf(stderr,
            "    -V   | --version    "
            "only output OpenSHMEM package version info\n");
    fprintf(stderr,
            "    -p S | --prefix=S   "
            "string to print at the beginning of output lines\n"
            "                        (default \"%s\")\n",
            default_prefix);
    fprintf(stderr,
            "    -s S | --suffix=S   "
            "string to print at the end of output lines\n"
            "                        (default \"%s\")\n",
            default_suffix);
    fprintf(stderr,
            "    -h   | --help       "
            "show this help message\n");
    fprintf(stderr,
            "\n");
}

static struct option opts[] = {
    { "version", no_argument,       NULL, 'V' },
    { "help",    no_argument,       NULL, 'h' },
    { "prefix",  required_argument, NULL, 'p' },
    { "suffix",  required_argument, NULL, 's' },
    { NULL,      no_argument,       NULL, 0   }
};


int
main(int argc, char *argv[])
{
    FILE *out = stdout;
    int just_version = 0;
    int help = 0;
    char *prefix = NULL;
    char *suffix = NULL;

    progname = basename(argv[0]);

    opterr = 0;                 /* no err msg, just my output */

    while (1) {
        const int c = getopt_long(argc, argv, "hVp:s:", opts, NULL);

        if (c == -1) {
            break;
            /* NOT REACHED */
        }

        switch ((char) c) {
        case 'h':
            help = 1;
            break;
        case 'V':
            just_version = 1;
            break;
        case 'p':
            prefix = optarg;
            break;
        case 's':
            suffix = optarg;
            break;
        default:
            help = 1;
            break;
        }
    }

    /* usage info, and bail */
    if (help) {
        output_help();
        return EXIT_FAILURE;
        /* NOT REACHED */
    }

    if (prefix == NULL) {
        prefix = default_prefix;
    }
    if (suffix == NULL) {
        suffix = default_suffix;
    }

    if (just_version) {
        info_output_package_version(out, prefix, suffix, just_version);
        return EXIT_SUCCESS;
        /* NOT REACHED */
    }

    info_output_package_name(out, prefix, suffix);
    info_output_package_version(out, prefix, suffix, just_version);
    info_output_package_contact(out, prefix, suffix);
    info_output_spec_version(out, prefix, suffix);
    info_output_build_env(out, prefix, suffix);
    info_output_features(out, prefix, suffix);
    info_output_comms(out, prefix, suffix);

    return EXIT_SUCCESS;
}
