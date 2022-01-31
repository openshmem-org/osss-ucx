/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "info.h"

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
output(FILE *strm, const char *prefix, const char *tag, const char *val)
{
    if (tag != NULL) {
        char buf[BUFMAX];

        snprintf(buf, BUFMAX, "%s:", tag);
        fprintf(strm, "%s%-*s ", prefix, tag_width, buf);
    }

    if (val != NULL) {
        fprintf(strm, "%s\n", val);
    }
}

void
info_output_spec_version(FILE *strm, const char *prefix)
{
#if defined(SHMEM_MAJOR_VERSION) && defined(SHMEM_MINOR_VERSION)
    char buf[BUFMAX];

    snprintf(buf, BUFMAX,
             "%d.%d",
             SHMEM_MAJOR_VERSION, SHMEM_MINOR_VERSION);
#else
    strncpy(buf, UNKNOWN, BUFMAX);
#endif /* spec. version check */
    output(strm, prefix, "OpenSHMEM Specification", buf);
}

void
info_output_package_name(FILE *strm, const char *prefix)
{
    output(strm, prefix, "OpenSHMEM Package name",
#ifdef PACKAGE_NAME
           PACKAGE_NAME
#else
           INTERNAL_ERROR
#endif /* PACKAGE_NAME */
           );
}

void
info_output_package_contact(FILE *strm, const char *prefix)
{
    output(strm, prefix, "OpenSHMEM Package URL",
#ifdef PACKAGE_URL
           PACKAGE_URL
#else
           INTERNAL_ERROR
#endif /* PACKAGE_URL */
           );

    output(strm, prefix, "OpenSHMEM Bug Report",
#ifdef PACKAGE_BUGREPORT
           PACKAGE_BUGREPORT
#else
           INTERNAL_ERROR
#endif /* PACKAGE_BUGREPORT */
           );
}

void
info_output_package_version(FILE *strm, const char *prefix, int terse)
{
    output(strm, prefix, terse ? NULL : "OpenSHMEM Package version",
#ifdef PACKAGE_VERSION
           PACKAGE_VERSION
#else
           INTERNAL_ERROR
#endif /* PACKAGE_VERSION */
           );

}

void
info_output_build_env(FILE *strm, const char *prefix)
{
    int s;
    char host[BUFMAX];

    output(strm, prefix, "Configured as",
#ifdef CONFIG_FLAGS
           CONFIG_FLAGS
#else
           UNKNOWN
#endif /* CONFIG_FLAGS */
           );

    output(strm, prefix, "Configure date",
#ifdef CONFIG_BUILD_DATE
           CONFIG_BUILD_DATE
#else
           UNKNOWN
#endif /* CONFIG_BUILD_DATE */
           );

    output(strm, prefix, "Configure host",
#ifdef CONFIG_BUILD_HOST
           CONFIG_BUILD_HOST
#else
           UNKNOWN
#endif /* CONFIG_BUILD_HOST */
           );

    s = gethostname(host, BUFMAX);
    output(strm, prefix, "Execution host", (s == 0) ? host : UNKNOWN);
}

void
info_output_features(FILE *strm, const char *prefix)
{
    output(strm, prefix, "Static libraries",
#ifdef ENABLE_STATIC
           "on"
#else
           "off"
#endif /* ENABLE_STATIC */
           );

    output(strm, prefix, "Shared libraries",
#ifdef ENABLE_SHARED
           "on"
#else
           "off"
#endif /* ENABLE_SHARED */
           );

    output(strm, prefix, "C++ support",
#ifdef ENABLE_CXX
           "on"
#else
           "off"
#endif /* ENABLE_CXX */
           );

    output(strm, prefix, "Debug checks",
#ifdef ENABLE_DEBUG
           "on"
#else
           "off"
#endif /* ENABLE_DEBUG */
           );

    output(strm, prefix, "Logging messages",
#ifdef ENABLE_LOGGING
           "on"
#else
           "off"
#endif /* ENABLE_LOGGING */
           );

    output(strm, prefix, "Aligned symmetric addresses",
#ifdef ENABLE_ALIGNED_ADDRESSES
           "on"
#else
           "off"
#endif /* ENABLE_ALIGNED_ADDRESSES */
           );

    output(strm, prefix, "Thread support",
#ifdef ENABLE_THREADS
           "on"
#else
           "off"
#endif /* ENABLE_THREADS */
           );

    output(strm, prefix, "Experimental API",
#ifdef ENABLE_EXPERIMENTAL
           "on"
#else
           "off"
#endif /* ENABLE_EXPERIMENTAL */
           );

    output(strm, prefix, "Profiling interface",
#ifdef ENABLE_PSHMEM
           "on"
#else
           "off"
#endif /* ENABLE_PSHMEM */
           );

#ifdef SHMEM_DEFAULT_HEAP_SIZE
    output(strm, prefix, "Default symmetric heap size",
           SHMEM_DEFAULT_HEAP_SIZE);
#endif /* SHMEM_DEFAULT_HEAP_SIZE */
}

void
info_output_comms(FILE *strm, const char *prefix)
{
    output(strm, prefix, "UCX Build Version",
#ifdef HAVE_UCX
           UCX_VERSION_STRING
#else
           INTERNAL_ERROR
#endif /* HAVE_UCX version */
           );

    output(strm, prefix, "PMIx Build Version",
#ifdef HAVE_PMIX
           PMIX_VERSION_STRING
#else
           INTERNAL_ERROR
#endif /* HAVE_PMIX version */
           );

    output(strm, prefix, "Using SHCOLL from",
#ifdef HAVE_SHCOLL
# ifdef HAVE_SHCOLL_INTERNAL
           PACKAGE_STRING " [internal]"
# else
           SHCOLL_DIR
# endif  /* HAVE_SHCOLL_INTERNAL */
#else
           INTERNAL_ERROR
#endif  /* HAVE_SHCOLL */
           );

    output(strm, prefix, "Using launcher",
#ifdef SHMEM_LAUNCHER
           SHMEM_LAUNCHER
#else
           "default"
#endif /* SHMEM_LAUNCHER */
           );
}
