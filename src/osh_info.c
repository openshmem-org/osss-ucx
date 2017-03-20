#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/param.h>

#include "version.h"

static const int tag_width = 20;

static
void
output(const char *tag, const char *val)
{
    printf("%*s: %s\n", tag_width, tag, val);
}

static
void
output_spec_version(void)
{
#if defined(SHMEM_MAJOR_VERSION) && defined(SHMEM_MINOR_VERSION)
#define BUFMAX 8
    {
        char buf[BUFMAX];

        snprintf(buf, BUFMAX,
                 "%d.%d",
                 SHMEM_MAJOR_VERSION, SHMEM_MINOR_VERSION);
        output("Specification", buf);
    }
#endif /* spec. version check */
}

static
void
output_build_env(void)
{
    int s;
    const time_t t = time(NULL);
    char *now;
    char host[MAXHOSTNAMELEN];

    now = ctime(&t);
    if (now != NULL) {
        now[strlen(now) - 1] = '\0'; /* chomp */
        output("Build date", now);
    }

    s = gethostname(host, MAXHOSTNAMELEN);
    if (s == 0) {
        output("Build host", host);
    }
}

int
main(void)
{

#ifdef PACKAGE_NAME
    output("Package name", PACKAGE_NAME);
#endif /* PACKAGE_NAME */

#ifdef PACKAGE_VERSION
    output("Package version", PACKAGE_VERSION);
#endif /* PACKAGE_VERSION */

#ifdef PACKAGE_URL
    output("Package URL", PACKAGE_URL);
#endif /* PACKAGE_URL */

#ifdef PACKAGE_BUGREPORT
    output("Package bug report", PACKAGE_BUGREPORT);
#endif /* PACKAGE_BUGREPORT */

    output_spec_version();

    output_build_env();

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

    return 0;
}
