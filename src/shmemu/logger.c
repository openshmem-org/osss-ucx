/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "state.h"
#include "shmemu.h"
#include "shmemc.h"
#include "boolean.h"

#include "klib/khash.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/time.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

static FILE *log_stream = NULL;
static char *host = NULL;
static int fatal_len;           /* speed up strncmp */

/*
 * output formatting
 */
static int pe_width;
static int stamp_width;

/*
 * keep track of trace category events
 */

KHASH_MAP_INIT_STR(categories, bool)

static khash_t(categories) *cats;

inline static void
category_set(shmemu_log_t name, bool state)
{
    int nocheck;
    khiter_t k;

    k = kh_put(categories, cats, name, &nocheck);
    kh_value(cats, k) = state;
}

inline static bool
category_enabled(shmemu_log_t name)
{
    const khiter_t k = kh_get(categories, cats, name);

    return (k != kh_end(cats)) ? kh_value(cats, k) : false;
}

inline static void
upperize(char *str)
{
    char *q;

    for (q = str; *q != '\0'; q += 1) {
        *q = toupper(*q);
    }
}

inline static void
parse_log_categories(void)
{
    const char *delims = ",:;";
    char *cp = proc.env.logging_events;
    char *opt;

    if (cp == NULL) {
        return;
        /* NOT REACHED */
    }

    opt = strtok(cp, delims);

    while (opt != NULL) {
        upperize(opt);
        category_set(opt, true);
        opt = strtok(NULL, delims);
    }
}

void
shmemu_logger_init(void)
{
    if (! proc.env.debug) {
        return;
    }

    host = shmemu_gethostname();
    if (host == NULL) {
        host = "unknown";
    }

    /* TODO "%" modifiers for extra info */
    if (proc.env.logging_file != NULL) {
        log_stream = fopen(proc.env.logging_file, "a");
        if (log_stream == NULL) {
            shmemu_fatal("can't append to log file \"%s\"",
                         proc.env.logging_file);
            /* NOT REACHED */
        }
    }
    else {
        log_stream = stderr;
    }

    /* how wide to display things */
    pe_width = (int) ceil(log10((double) proc.nranks));
    stamp_width = 30 - pe_width;
    if (stamp_width < 1) {
        stamp_width = 1;
    }

    cats = kh_init(categories);

    category_set(LOG_FATAL,      true);
    category_set(LOG_INIT,       false);
    category_set(LOG_FINALIZE,   false);
    category_set(LOG_MEMORY,     false);
    category_set(LOG_HEAPS,      false);
    category_set(LOG_CONTEXTS,   false);
    category_set(LOG_INFO,       false);
    category_set(LOG_REDUCTIONS, false);
    category_set(LOG_BARRIERS,   false);
    category_set(LOG_DEPRECATE,  false);
    category_set(LOG_LOCKS,      false);
    category_set(LOG_ATOMICS,    false);

    parse_log_categories();

    fatal_len = strlen(LOG_FATAL);
}

void
shmemu_logger_finalize(void)
{
    if (! proc.env.debug) {
        return;
    }

    if (log_stream != NULL) {
        fclose(log_stream);
    }
}

#define TRACE_MSG_BUF_SIZE 256

void
shmemu_logger(shmemu_log_t cat, const char *fmt, ...)
{
    if (proc.env.debug) {
        if (category_enabled(cat) || category_enabled(LOG_ALL)) {
            char tmp1[TRACE_MSG_BUF_SIZE];
            char tmp2[TRACE_MSG_BUF_SIZE];
            va_list ap;

            snprintf(tmp1, TRACE_MSG_BUF_SIZE,
                     "[%*d:%s:%d:%6.6f]",
                     pe_width, proc.rank,
                     host,
                     (int) getpid(),
                     shmemu_timer()
                     );

            snprintf(tmp2, TRACE_MSG_BUF_SIZE,
                     "%-*s %8s: ",
                     stamp_width, tmp1,
                     cat
                     );

            va_start(ap, fmt);
            vsnprintf(tmp1, TRACE_MSG_BUF_SIZE, fmt, ap);
            va_end(ap);

#ifdef HAVE_STRLCAT
            strlcat(tmp2, tmp1, strlen(tmp1));
            strlcat(tmp2, "\n", 1);
#else
            strncat(tmp2, tmp1, strlen(tmp1));
            strncat(tmp2, "\n", 1);
#endif /* HAVE_STRLCAT */

            fputs(tmp2, log_stream);
            /* make sure this all goes out in 1 burst */
            fflush(log_stream);

            if (strncmp(cat, LOG_FATAL, fatal_len) == 0) {
                shmemc_global_exit(1);
            }
        }
    }
}
