#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "version.h"
#include "shmemu.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static const int tag_width = 24;
static char *unknown = "unknown";

inline static void
output(const char *tag, const char *val)
{
#define STRMAX 64
    char buf[STRMAX];

    snprintf(buf, STRMAX, "%s:", tag);
    printf("%-*s %s\n", tag_width, buf, val);
#undef STRMAX
}

static void
output_package(void)
{
    output("Package name",
#ifdef PACKAGE_NAME
           PACKAGE_NAME
#else
           unknown
#endif /* PACKAGE_NAME */
           );

    output("Package version",
#ifdef PACKAGE_VERSION
            PACKAGE_VERSION
#else
           unknown
#endif /* PACKAGE_VERSION */
           );

    output("Package URL",
#ifdef PACKAGE_URL
            PACKAGE_URL
#else
           unknown
#endif /* PACKAGE_URL */
           );

    output("Package bug report",
#ifdef PACKAGE_BUGREPORT
           PACKAGE_BUGREPORT
#else
           unknown
#endif /* PACKAGE_BUGREPORT */
           );
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
    output("Specification", buf);
#undef BUFMAX
#endif /* spec. version check */
}

static void
output_build_env(void)
{
    char *host;

    output("Build date",
#ifdef CONFIG_BUILD_DATE
           CONFIG_BUILD_DATE
#else
           unknown
#endif /* CONFIG_BUILD_DATE */
           );

    output("Build host",
#ifdef CONFIG_BUILD_HOST
           CONFIG_BUILD_HOST
#else
           unknown
#endif /* CONFIG_BUILD_HOST */
           );

    host = shmemu_gethostname();
    output("Execution host", (host != NULL) ? host : unknown);

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

    output("C++ support",
#ifdef ENABLE_CXX
           "on"
#else
           "off"
#endif /* ENABLE_CXX */
           );

    output("Debug messages",
#ifdef ENABLE_DEBUG
           "on"
#else
           "off"
#endif /* ENABLE_DEBUG */
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

    output("Fortran support",
#ifdef ENABLE_FORTRAN
           "on"
#else
           "off"
#endif /* ENABLE_FORTRAN */
           );
}

static void
output_comms(void)
{
#ifdef HAVE_UCX
    output("UCX", "on");
    output("UCX Install", UCX_DIR);
#else
    output("UCX", "off [shouldn't happen]");
#endif /* HAVE_UCX */

#ifdef HAVE_PMIX
    output("PMIx", "on");
    output("PMIx Install", PMIX_DIR);
#else
    output("PMIx", "off [shouldn't happen]");
#endif /* HAVE_PMIX */
}

int
main(void)
{
    output_package();

    output_spec_version();

    output_build_env();

    output_features();

    output_comms();

    return 0;
}
