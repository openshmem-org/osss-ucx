#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#ifdef HAVE_UNAME
# include <sys/utsname.h>
#endif /* HAVE_UNAME */

#include "version.h"
#include "shmemu/shmemu.h"

static const int tag_width = 20;
static char *unknown = "unknown";

static
void
output(const char *tag, const char *val)
{
    printf("%*s: %s\n", tag_width, tag, val);
}

static
void
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

    output("Package bug report",
#ifdef PACKAGE_BUGREPORT
           PACKAGE_BUGREPORT
#else
           unknown
#endif /* PACKAGE_BUGREPORT */
           );
}

static
void
output_spec_version(void)
{
#if defined(SHMEM_MAJOR_VERSION) && defined(SHMEM_MINOR_VERSION)

#define BUFMAX 8
    char buf[BUFMAX];

    snprintf(buf, BUFMAX,
             "%d.%d",
             SHMEM_MAJOR_VERSION, SHMEM_MINOR_VERSION);
    output("Specification", buf);
#endif /* spec. version check */
}

static
void
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

static
void
output_features(void)
{
    output("Run debugging",
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

    output("C++ support",
#ifdef ENABLE_CXX
           "on"
#else
           "off"
#endif /* ENABLE_CXX */
           );

    output("Fortran support",
#ifdef ENABLE_FORTRAN
           "on"
#else
           "off"
#endif /* ENABLE_FORTRAN */
           );
}

int
main(void)
{
    output_package();

    output_spec_version();

    output_build_env();

    output_features();

    return 0;
}
