/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "version.h"
#include "shmemu.h"
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
            "    [-V | --version]    only output OpenSHMEM"
            " package version info\n");
    fprintf(stderr,
            "    [-h | --help]       show this help message\n");
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
    int just_version = 0;
    int help = 0;

    progname = basename(argv[0]);

    while (1) {
        const int c = getopt_long(argc, argv, "hV", opts, NULL);

        if (c == -1) {
            break;
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
        osh_info.package_version(just_version);
    }
    else {
        osh_info.package_name();
        osh_info.package_version(just_version);
        osh_info.package_contact();
        osh_info.spec_version();
        osh_info.buildenv();
        osh_info.features();
        osh_info.comms();
    }

    return EXIT_SUCCESS;
}
