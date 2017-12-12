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

static const int tag_width = 28;

#define UNKNOWN        "unknown"
#define INTERNAL_ERROR "not found [shouldn't happen]"

inline static void
output(const char *tag, const char *val)
{
    if (tag != NULL) {
#define STRMAX 64
        char buf[STRMAX];

        snprintf(buf, STRMAX, "%s:", tag);
        printf("# %-*s ", tag_width, buf);
#undef STRMAX
    }

    if (val != NULL) {
        printf("%s\n", val);
    }
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
output_package_name(void)
{
    output("OpenSHMEM Package name",
#ifdef PACKAGE_NAME
           PACKAGE_NAME
#else
           INTERNAL_ERROR
#endif /* PACKAGE_NAME */
           );
}

static void
output_package_contact(void)
{
    output("OpenSHMEM Package URL",
#ifdef PACKAGE_URL
            PACKAGE_URL
#else
           INTERNAL_ERROR
#endif /* PACKAGE_URL */
           );

    output("OpenSHMEM Bug Report",
#ifdef PACKAGE_BUGREPORT
           PACKAGE_BUGREPORT
#else
           INTERNAL_ERROR
#endif /* PACKAGE_BUGREPORT */
           );
}

static void
output_package_version(int terse)
{
    output(terse ? NULL : "OpenSHMEM Package version",
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

    output("Aligned symmetric addresses",
#ifdef ENABLE_ALIGNED_ADDRESSES
           "on"
#else
           "off"
#endif /* ENABLE_ALIGNED_ADDRESSES */
           );

    output("Thread support",
#ifdef ENABLE_THREADS
           "on"
#else
           "off"
#endif /* ENABLE_THREADS */
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

shmem_oshinfo_t
osh_info = {
    .spec_version = output_spec_version,
    .package_name = output_package_name,
    .package_contact = output_package_contact,
    .package_version = output_package_version,
    .buildenv = output_build_env,
    .features = output_features,
    .comms = output_comms,
};
