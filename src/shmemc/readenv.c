/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "state.h"
#include "shmemu.h"
#include "boolean.h"

#include <stdio.h>
#include <stdlib.h>             /* getenv */
#include <ctype.h>
#include <string.h>
#include <strings.h>

/*
 * internal helpers
 */

/*
 * for string formatting
 */

#define BUFSIZE 16

/*
 * return non-zero if option enabled, 0 if not
 */

static bool
option_enabled_test(char *str)
{
    if (str == NULL) {
        return false;
        /* NOT REACHED */
    }

    return
        (tolower(*str) == 'y') ||
        (strncasecmp(str, "on", 2) == 0) ||
        (strtol(str, NULL, 10) > 0L);
}

/*
 * read & save all our environment variables
 */

#define CHECK_ENV(_e, _name)                        \
    do {                                            \
        (_e) = getenv("SHMEM_" #_name);             \
        if ((_e) == NULL) {                         \
            (_e) = getenv("SMA_" #_name);           \
        }                                           \
    } while (0)

void
shmemc_env_init(void)
{
    char *e;
    int r;

    /*
     * defined in spec
     */

    proc.env.print_version = false;
    proc.env.print_info    = false;
    proc.env.debug         = false;

    CHECK_ENV(e, VERSION);
    if (e != NULL) {
        proc.env.print_version = option_enabled_test(e);
    }
    CHECK_ENV(e, INFO);
    if (e != NULL) {
        proc.env.print_info = option_enabled_test(e);
    }
    CHECK_ENV(e, DEBUG);
    if (e != NULL) {
        proc.env.debug = option_enabled_test(e);
    }

    /*
     * heaps need a bit more handling
     */

    proc.env.heaps.nheaps = 1;  /* TODO: will be parsed out of env */
    proc.env.heaps.heapsize =
        (size_t *) malloc(proc.env.heaps.nheaps *
                          sizeof(*proc.env.heaps.heapsize));
    shmemu_assert(proc.env.heaps.heapsize != NULL,
                  "can't allocate memory for heap size declaration");

    CHECK_ENV(e, SYMMETRIC_SIZE);
    r = shmemu_parse_size( (e != NULL) ? e : SHMEM_DEFAULT_HEAP_SIZE,
                           &proc.env.heaps.heapsize[0] );
    if (r != 0) {
        shmemu_fatal("Couldn't work out requested heap size \"%s\"", e);
    }

    /*
     * this implementation also has...
     */

    proc.env.logging        = false;
    proc.env.logging_events = NULL;
    proc.env.logging_file   = NULL;
    proc.env.xpmem_kludge   = false;

    CHECK_ENV(e, LOGGING);
    if (e != NULL) {
        proc.env.logging = option_enabled_test(e);
    }
    CHECK_ENV(e, LOGGING_FILE);
    if (e != NULL) {
        proc.env.logging_file = strdup(e);
    }
    CHECK_ENV(e, LOGGING_EVENTS);
    if (e != NULL) {
        proc.env.logging_events = strdup(e);
    }
    CHECK_ENV(e, XPMEM_KLUDGE);
    if (e != NULL) {
        proc.env.xpmem_kludge = option_enabled_test(e);
    }

    proc.env.coll.barrier   = NULL;
    proc.env.coll.broadcast = NULL;
    proc.env.coll.collect   = NULL;
    proc.env.coll.fcollect  = NULL;
    proc.env.coll.alltoall  = NULL;
    proc.env.coll.alltoalls = NULL;
    proc.env.coll.reduce    = NULL;

    CHECK_ENV(e, BARRIER_ALGO);
    if (e != NULL) {
        proc.env.coll.barrier = strdup(e);
    }
    CHECK_ENV(e, BROADCAST_ALGO);
    if (e != NULL) {
        proc.env.coll.broadcast = strdup(e);
    }
    CHECK_ENV(e, COLLLECT_ALGO);
    if (e != NULL) {
        proc.env.coll.collect = strdup(e);
    }
    CHECK_ENV(e, FCOLLLECT_ALGO);
    if (e != NULL) {
        proc.env.coll.fcollect = strdup(e);
    }
    CHECK_ENV(e, ALLTOALL_ALGO);
    if (e != NULL) {
        proc.env.coll.alltoall = strdup(e);
    }
    CHECK_ENV(e, ALLTOALLS_ALGO);
    if (e != NULL) {
        proc.env.coll.alltoalls = strdup(e);
    }
    CHECK_ENV(e, REDUCE_ALGO);
    if (e != NULL) {
        proc.env.coll.reduce = strdup(e);
    }

    proc.env.progress_threads = NULL;

    CHECK_ENV(e, PROGRESS_THREADS);
    if (e != NULL) {
        proc.env.progress_threads = strdup(e);
    }
}

#undef CHECK_ENV

void
shmemc_env_finalize(void)
{
    free(proc.env.logging_file);
    free(proc.env.logging_events);

    free(proc.env.coll.reduce);
    free(proc.env.coll.alltoalls);
    free(proc.env.coll.alltoall);
    free(proc.env.coll.fcollect);
    free(proc.env.coll.collect);
    free(proc.env.coll.barrier);
    free(proc.env.coll.broadcast);

    free(proc.env.progress_threads);
}

static const int var_width = 22;
static const int val_width = 10;
static const int hr_width  = 74;

inline static void
hr(FILE *stream, const char *prefix)
{
    int i;

    fprintf(stream, "%s", prefix);
    for (i = 0; i < hr_width; i += 1) {
        fprintf(stream, "-");
    }
    fprintf(stream, "\n");
}

void
shmemc_print_env_vars(FILE *stream, const char *prefix)
{
    fprintf(stream, "%sEnvironment Variable Information\n",
            prefix);
    fprintf(stream, "%s\n", prefix);
    fprintf(stream, "%s%-*s %-*s %s\n",
            prefix,
            var_width, "Variable",
            val_width, "Value",
            "Description");
    hr(stream, prefix);
    fprintf(stream, "%s\n", prefix);
    fprintf(stream, "%s%s\n",
            prefix,
            "From specification:");
    fprintf(stream, "%s\n", prefix);
    fprintf(stream, "%s%-*s %-*s %s\n",
            prefix,
            var_width, "SHMEM_VERSION",
            val_width, shmemu_human_option(proc.env.print_version),
            "print library version at start-up");
    fprintf(stream, "%s%-*s %-*s %s\n",
            prefix,
            var_width, "SHMEM_INFO",
            val_width, shmemu_human_option(proc.env.print_info),
            "print this information");
    {
        char buf[BUFSIZE];

        /* TODO hardwired index */
        (void) shmemu_human_number(proc.env.heaps.heapsize[0], buf, BUFSIZE);
        fprintf(stream, "%s%-*s %-*s %s\n",
                prefix,
                var_width, "SHMEM_SYMMETRIC_SIZE",
                val_width, buf,
                "requested size of the symmetric heap");
    }
    fprintf(stream, "%s%-*s %-*s %s\n",
            prefix,
            var_width, "SHMEM_DEBUG",
            val_width, shmemu_human_option(proc.env.debug),
            "enable sanity checking ("
#if ! defined(ENABLE_DEBUG)
            "not "
#endif /* ! ENABLE_DEBUG */
            "configured)"
            );

    fprintf(stream, "%s\n", prefix);
    fprintf(stream, "%s%s\n",
            prefix,
            "Specific to this implementation:");
    fprintf(stream, "%s\n", prefix);
    fprintf(stream, "%s%-*s %-*s %s\n",
            prefix,
            var_width, "SHMEM_LOGGING",
            val_width, shmemu_human_option(proc.env.logging),
            "enable logging messages ("
#if ! defined(ENABLE_LOGGING)
            "not "
#endif /* ! ENABLE_LOGGING */
            "configured)"
            );

    fprintf(stream, "%s%-*s %-*s %s\n",
            prefix,
            var_width, "SHMEM_LOGGING_EVENTS",
            val_width, "...",   /* could be far too long to show */
            "types of logging events to show");
    fprintf(stream, "%s%-*s %-*s %s\n",
            prefix,
            var_width, "SHMEM_LOGGING_FILE",
            val_width, proc.env.logging_file ? proc.env.logging_file : "unset",
            "file for logging information");
    fprintf(stream, "%s%-*s %-*s %s\n",
            prefix,
            var_width, "SHMEM_XPMEM_KLUDGE",
            val_width, shmemu_human_option(proc.env.xpmem_kludge),
            "avoid XPMEM tear-down bug (temporary)");

#define DESCRIBE_COLLECTIVE(_name, _envvar)                             \
    do {                                                                \
        fprintf(stream, "%s%-*s %-*s %s\n",                             \
                prefix,                                                 \
                var_width, "SHMEM_" #_envvar "_ALGO",                   \
                val_width,                                              \
                proc.env.coll._name ? proc.env.coll._name : "unset",    \
                "Algorithm for \"" #_name "\" routine");                \
    } while (0)

    DESCRIBE_COLLECTIVE(barrier, BARRIER);
    DESCRIBE_COLLECTIVE(broadcast, BROADCAST);
    DESCRIBE_COLLECTIVE(collect, COLLECT);
    DESCRIBE_COLLECTIVE(fcollect, FCOLLECT);
    DESCRIBE_COLLECTIVE(alltoall, ALLTOALL);
    DESCRIBE_COLLECTIVE(alltoalls, ALLTOALLS);
    DESCRIBE_COLLECTIVE(reduce, REDUCE);

    fprintf(stream, "%s%-*s %-*s %s\n",
            prefix,
            var_width, "SHMEM_PROGRESS_THREADS",
            val_width,
            proc.env.progress_threads ? proc.env.progress_threads : "no",
            "Do we manage our own progress?");

    fprintf(stream, "%s\n", prefix);
    hr(stream, prefix);
    fprintf(stream, "%s\n", prefix);
    fprintf(stream, "\n");

    fflush(stream);
}
