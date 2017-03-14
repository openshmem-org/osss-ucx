#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "version.h"

static
void
output(const char *tag, const char *val)
{
    printf("%20s: %s\n", tag, val);
}

static
void
output_spec_version(void)
{
#if defined(SHMEM_MAJOR_VERSION) && defined(SHMEM_MINOR_VERSION)
    {
        char buf[8];

        snprintf(buf, 8, "%d.%d", SHMEM_MAJOR_VERSION, SHMEM_MINOR_VERSION);
        output("Specification", buf);
#endif /* spec. version check */
    }
}

static
void
output_date(void)
{
    const time_t t = time(NULL);
    char *now = ctime(&t);

    /* chomp */
    now[strlen(now) - 1] = '\0';

    output("Build date", now);
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

    output_date();

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

    return 0;
}
