/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "version.h"
#include "shmemu.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include <libgen.h>             /* basename */

static const int tag_width = 28;

#define UNKNOWN        "unknown"
#define INTERNAL_ERROR "not found [shouldn't happen]"

static void
output(const char *tag, const char *val)
{
#define STRMAX 64
    char buf[STRMAX];

    snprintf(buf, STRMAX, "%s:", tag);
    printf("%-*s %s\n", tag_width, buf, val);
#undef STRMAX
}

static void
output_spec_version(void)
{
#if defined(SHMEM_MAJOR_VERSION) && defined(SHMEM_MINOR_VERSION)

#define BUFMAX 16
    char buf[BUFMAX];

    snprintf(buf, BUFMAX,
             "%d.%d",
             SHMEM_MAJOR_VERSION, SHMEM_MINOR_VERSION);
    output("OpenSHMEM Specification", buf);
#undef BUFMAX
#endif /* spec. version check */
}

static void
output_package(void)
{
    output("OpenSHMEM Package name",
#ifdef PACKAGE_NAME
           PACKAGE_NAME
#else
           INTERNAL_ERROR
#endif /* PACKAGE_NAME */
           );

    output("OpenSHMEM Package URL",
#ifdef PACKAGE_URL
            PACKAGE_URL
#else
           INTERNAL_ERROR
#endif /* PACKAGE_URL */
           );

    output("OpenSHMEM bug report",
#ifdef PACKAGE_BUGREPORT
           PACKAGE_BUGREPORT
#else
           INTERNAL_ERROR
#endif /* PACKAGE_BUGREPORT */
           );
}

static void
output_version(void)
{
    output("OpenSHMEM Package version",
#ifdef PACKAGE_VERSION
            PACKAGE_VERSION
#else
           INTERNAL_ERROR
#endif /* PACKAGE_VERSION */
           );

}

static void
output_build_env(void)
{
    char *host;

    output("Build date",
#ifdef CONFIG_BUILD_DATE
           CONFIG_BUILD_DATE
#else
           UNKNOWN
#endif /* CONFIG_BUILD_DATE */
           );

    output("Build host",
#ifdef CONFIG_BUILD_HOST
           CONFIG_BUILD_HOST
#else
           UNKNOWN
#endif /* CONFIG_BUILD_HOST */
           );

    host = shmemu_gethostname();
    output("Execution host", (host != NULL) ? host : UNKNOWN);

    /* command-line that built the library */
#ifdef CONFIG_FLAGS
    output("Configure", CONFIG_FLAGS);
#endif /* CONFIG_FLAGS */
}

static void
output_features(void)
{
    output("Static libraries",
#ifdef ENABLE_STATIC
           "on"
#else
           "off"
#endif /* ENABLE_STATIC */
           );

    output("Shared libraries",
#ifdef ENABLE_SHARED
           "on"
#else
           "off"
#endif /* ENABLE_SHARED */
           );

    output("C++ compiler",
#ifdef ENABLE_CXX
           "on"
#else
           "off"
#endif /* ENABLE_CXX */
           );

    output("Fortran API",
#ifdef ENABLE_FORTRAN
           "on"
#else
           "off"
#endif /* ENABLE_FORTRAN */
           );

    output("Debug messages",
#ifdef ENABLE_DEBUG
           "on"
#else
           "off"
#endif /* ENABLE_DEBUG */
           );

    output("Fixed symmetric addresses",
#ifdef ENABLE_FIXED_ADDRESSES
           "on"
#else
           "off"
#endif /* ENABLE_FIXED_ADDRESSES */
           );

    output("Experimental API",
#ifdef ENABLE_EXPERIMENTAL
           "on"
#else
           "off"
#endif /* ENABLE_EXPERIMENTAL */
           );

    output("Profiling interface",
#ifdef ENABLE_PSHMEM
           "on"
#else
           "off"
#endif /* ENABLE_PSHMEM */
           );
}

static void
output_comms(void)
{
    output("UCX Install",
#ifdef HAVE_UCX
           UCX_DIR
#else
           INTERNAL_ERROR
#endif /* HAVE_UCX */
           );

    output("PMIx Install",
#ifdef HAVE_PMIX
           PMIX_DIR
#else
           INTERNAL_ERROR
#endif /* HAVE_PMIX */
           );
}

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
            "    [-V | --version]    only output OpenSHMEM version info\n");
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
    int version_only = 0;
    int help = 0;

    progname = basename(argv[0]);

    while (1) {
        int c = getopt_long(argc, argv, "hV", opts, NULL);

        if (c == -1) {
            break;
        }

        switch (c) {
        case 'h':
            help = 1;
            break;
        case 'V':
            version_only = 1;
            break;
        default:
            help = 1;
            break;
        }
    }

    /* usage info, and bail */
    if (help) {
        output_help();
        return 1;
    }

    output_version();
    if (version_only) {
        return 0;
    }

    /* we want all the rest of it */

    output_spec_version();
    output_package();
    output_build_env();
    output_features();
    output_comms();

    return 0;
}
