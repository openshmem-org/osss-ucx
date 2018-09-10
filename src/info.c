/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static const int tag_width = 28;

#define BUFMAX 64

#define UNKNOWN        "unknown"
#define INTERNAL_ERROR "not found [shouldn't happen]"

inline static void
output(FILE *strm, const char *tag, const char *val)
{
    if (tag != NULL) {
        char buf[BUFMAX];

        snprintf(buf, BUFMAX, "%s:", tag);
        fprintf(strm, "# %-*s ", tag_width, buf);
    }

    if (val != NULL) {
        fprintf(strm, "%s\n", val);
    }
}

void
info_output_spec_version(FILE *strm)
{
#if defined(SHMEM_MAJOR_VERSION) && defined(SHMEM_MINOR_VERSION)
    char buf[BUFMAX];

    snprintf(buf, BUFMAX,
             "%d.%d",
             SHMEM_MAJOR_VERSION, SHMEM_MINOR_VERSION);
    output(strm, "OpenSHMEM Specification", buf);
#endif /* spec. version check */
}

void
info_output_package_name(FILE *strm)
{
    output(strm, "OpenSHMEM Package name",
#ifdef PACKAGE_NAME
           PACKAGE_NAME
#else
           INTERNAL_ERROR
#endif /* PACKAGE_NAME */
           );
}

void
info_output_package_contact(FILE *strm)
{
    output(strm, "OpenSHMEM Package URL",
#ifdef PACKAGE_URL
           PACKAGE_URL
#else
           INTERNAL_ERROR
#endif /* PACKAGE_URL */
           );

    output(strm, "OpenSHMEM Bug Report",
#ifdef PACKAGE_BUGREPORT
           PACKAGE_BUGREPORT
#else
           INTERNAL_ERROR
#endif /* PACKAGE_BUGREPORT */
           );
}

void
info_output_package_version(FILE *strm, int terse)
{
    output(strm, terse ? NULL : "OpenSHMEM Package version",
#ifdef PACKAGE_VERSION
           PACKAGE_VERSION
#else
           INTERNAL_ERROR
#endif /* PACKAGE_VERSION */
           );

}

void
info_output_build_env(FILE *strm)
{
    int s;
    char host[BUFMAX];

    output(strm, "Configured as",
#ifdef CONFIG_FLAGS
           CONFIG_FLAGS
#else
           UNKNOWN
#endif /* CONFIG_FLAGS */
           );

    output(strm, "Configure date",
#ifdef CONFIG_BUILD_DATE
           CONFIG_BUILD_DATE
#else
           UNKNOWN
#endif /* CONFIG_BUILD_DATE */
           );

    output(strm, "Configure host",
#ifdef CONFIG_BUILD_HOST
           CONFIG_BUILD_HOST
#else
           UNKNOWN
#endif /* CONFIG_BUILD_HOST */
           );

    s = gethostname(host, BUFMAX);
    output(strm, "Execution host", (s == 0) ? host : UNKNOWN);
}

void
info_output_features(FILE *strm)
{
    output(strm, "Static libraries",
#ifdef ENABLE_STATIC
           "on"
#else
           "off"
#endif /* ENABLE_STATIC */
           );

    output(strm, "Shared libraries",
#ifdef ENABLE_SHARED
           "on"
#else
           "off"
#endif /* ENABLE_SHARED */
           );

    output(strm, "C++ compiler",
#ifdef ENABLE_CXX
           "on"
#else
           "off"
#endif /* ENABLE_CXX */
           );

    output(strm, "Fortran API and compiler",
#ifdef ENABLE_FORTRAN
           "on"
#else
           "off"
#endif /* ENABLE_FORTRAN */
           );

    output(strm, "Debug checks",
#ifdef ENABLE_DEBUG
           "on"
#else
           "off"
#endif /* ENABLE_DEBUG */
           );

    output(strm, "Logging messages",
#ifdef ENABLE_LOGGING
           "on"
#else
           "off"
#endif /* ENABLE_LOGGING */
           );

    output(strm, "Aligned symmetric addresses",
#ifdef ENABLE_ALIGNED_ADDRESSES
           "on"
#else
           "off"
#endif /* ENABLE_ALIGNED_ADDRESSES */
           );

    output(strm, "Thread support",
#ifdef ENABLE_THREADS
           "on"
#else
           "off"
#endif /* ENABLE_THREADS */
           );

    output(strm, "Experimental API",
#ifdef ENABLE_EXPERIMENTAL
           "on"
#else
           "off"
#endif /* ENABLE_EXPERIMENTAL */
           );

    output(strm, "Profiling interface",
#ifdef ENABLE_PSHMEM
           "on"
#else
           "off"
#endif /* ENABLE_PSHMEM */
           );
}

void
info_output_comms(FILE *strm)
{
    output(strm, "Using UCX from",
#ifdef HAVE_UCX
           UCX_DIR
#else
           INTERNAL_ERROR
#endif /* HAVE_UCX install */
           );

    output(strm, "UCX Version",
#ifdef HAVE_UCX
           UCX_VERSION_STRING
#else
           INTERNAL_ERROR
#endif /* HAVE_UCX version */
           );

    output(strm, "Using PMIx from",
#ifdef HAVE_PMIX
           PMIX_DIR
#else
           INTERNAL_ERROR
#endif /* HAVE_PMIX install */
           );
    output(strm, "PMIx Version",
#ifdef HAVE_PMIX
           PMIX_VERSION_STRING
#else
           INTERNAL_ERROR
#endif /* HAVE_PMIX version */
           );

    output(strm, "Using SHCOLL from",
#ifdef HAVE_SHCOLL_INTERNAL
           PACKAGE_STRING
#else
           SHCOLL_DIR
#endif
           );
}
