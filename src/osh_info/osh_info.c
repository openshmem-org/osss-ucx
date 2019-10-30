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

static void
output_help(void)
{
    fprintf(stderr,
            "\n");
    fprintf(stderr,
            "Usage: %s [options]\n\n",
            progname);
    fprintf(stderr,
            "    -V | --version    only output OpenSHMEM"
            " package version info\n");
    fprintf(stderr,
            "    -h | --help       show this help message\n");
    fprintf(stderr,
            "\n");
}

static struct option opts[] = {
    { "version", no_argument, NULL, 'V' },
    { "help",    no_argument, NULL, 'h' },
    { NULL,      no_argument, NULL, 0   }
};


int
main(int argc, char *argv[])
{
    FILE *out = stdout;
    int just_version = 0;
    int help = 0;

    progname = basename(argv[0]);

    while (1) {
        const int c = getopt_long(argc, argv, "hV", opts, NULL);

        if (c == -1) {
            break;
            /* NOT REACHED */
        }

        switch (c) {
        case 'h':
            help = 1;
            break;
        case 'V':
            just_version = 1;
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

    if (just_version) {
        info_output_package_version(out, just_version);
        return EXIT_SUCCESS;
        /* NOT REACHED */
    }

    info_output_package_name(out);
    info_output_package_version(out, just_version);
    info_output_package_contact(out);
    info_output_spec_version(out);
    info_output_build_env(out);
    info_output_features(out);
    info_output_comms(out);

    return EXIT_SUCCESS;
}
