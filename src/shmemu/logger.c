/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "state.h"
#include "shmemu.h"
#include "shmemc.h"
#include "boolean.h"
#include "module.h"

#include "../klib/khash.h"

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

/*
 * output formatting
 */
static int pe_width;
static int stamp_width;

/*
 * cache process ID
 */
static int mypid;

/*
 * keep track of trace events
 */

KHASH_MAP_INIT_STR(events_hash, bool)

static khash_t(events_hash) *events;

inline static void
event_set(shmemu_log_t name, bool state)
{
    khiter_t k;
    int ret;

    k = kh_put(events_hash, events, name, &ret);
    kh_value(events, k) = state;
}

inline static bool
event_enabled(shmemu_log_t name)
{
    const khiter_t k = kh_get(events_hash, events, name);

    return (k != kh_end(events)) ? kh_value(events, k) : false;
}

inline static void
upperize(char *str)
{
    char *q;

    for (q = str; *q != '\0'; ++q) {
        *q = toupper(*q);
    }
}

inline static void
parse_log_events(void)
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
        event_set(opt, true);
        opt = strtok(NULL, delims);
    }
}

void
shmemu_logger_init(void)
{
    double wd;

    if (! proc.env.logging) {
        return;
    }

    /* TODO "%" modifiers for extra info */
    if (proc.env.logging_file != NULL) {
        log_stream = fopen(proc.env.logging_file, "a");
        if (log_stream == NULL) {
            shmemu_fatal(MODULE ": can't append to log file \"%s\"",
                         proc.env.logging_file);
            /* NOT REACHED */
        }
    }
    else {
        log_stream = stderr;
    }

    /* how wide to display things */
    wd = ceil(log10((double) proc.li.nranks));
    pe_width = (int) wd;
    stamp_width = 30 - pe_width;
    if (stamp_width < 1) {
        stamp_width = 1;
    }

    events = kh_init(events_hash);

    event_set(LOG_INIT,       false);
    event_set(LOG_FINALIZE,   false);
    event_set(LOG_MEMORY,     false);
    event_set(LOG_FENCE,      false);
    event_set(LOG_QUIET,      false);
    event_set(LOG_HEAPS,      false);
    event_set(LOG_RMA,        false);
    event_set(LOG_CONTEXTS,   false);
    event_set(LOG_RANKS,      false);
    event_set(LOG_INFO,       false);
    event_set(LOG_REDUCTIONS, false);
    event_set(LOG_BARRIERS,   false);
    event_set(LOG_DEPRECATE,  false);
    event_set(LOG_LOCKS,      false);
    event_set(LOG_ATOMICS,    false);

    parse_log_events();

    mypid = (int) getpid();
}

void
shmemu_logger_finalize(void)
{
    if (! proc.env.logging) {
        return;
    }

    fclose(log_stream);

    kh_destroy(events_hash, events);
}

#define TRACE_MSG_BUF_SIZE_1 256
#define TRACE_MSG_BUF_SIZE_2 (TRACE_MSG_BUF_SIZE_1 * 2)

static char tmp1[TRACE_MSG_BUF_SIZE_1];
static char tmp2[TRACE_MSG_BUF_SIZE_2];

void
shmemu_logger(shmemu_log_t evt, const char *fmt, ...)
{
    if (! proc.env.logging) {
        return;
    }

    if (event_enabled(evt) || event_enabled(LOG_ALL)) {
        va_list ap;

        snprintf(tmp1, TRACE_MSG_BUF_SIZE_1,
                 "[%*d:%s:%d:%6.6f]",
                 pe_width, proc.li.rank,
                 proc.nodename,
                 mypid,
                 shmemu_timer()
                 );

        snprintf(tmp2, TRACE_MSG_BUF_SIZE_2,
                 "%-*s %s: ",
                 stamp_width, tmp1,
                 evt
                 );

        va_start(ap, fmt);
        vsnprintf(tmp1, TRACE_MSG_BUF_SIZE_1, fmt, ap);
        va_end(ap);

        STRNCAT_SAFE(tmp2, tmp1, TRACE_MSG_BUF_SIZE_1);
        STRNCAT_SAFE(tmp2, "\n", 2);

        fputs(tmp2, log_stream);
        /* make sure this all goes out in 1 burst */
        fflush(log_stream);
    }
}
