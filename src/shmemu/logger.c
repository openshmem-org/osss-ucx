/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "state.h"
#include "shmemu.h"
#include "shmemc.h"

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
static int pe_width;
static int stamp_width;

typedef struct shmemu_log_table {
    shmemu_log_t level;
    char *name;
} shmemu_log_table_t;

#define LOG_LEVEL_EMIT(_level) { LOG_##_level, #_level }

static shmemu_log_table_t table[] =
    {
        LOG_LEVEL_EMIT(FATAL),
        LOG_LEVEL_EMIT(INIT),
        LOG_LEVEL_EMIT(FINALIZE),
        LOG_LEVEL_EMIT(MEMORY),
        LOG_LEVEL_EMIT(HEAP),
        LOG_LEVEL_EMIT(CONTEXT),
        LOG_LEVEL_EMIT(INFO),
        LOG_LEVEL_EMIT(REDUCTION),
        LOG_LEVEL_EMIT(BARRIER),
        LOG_LEVEL_EMIT(DEPRECATE),
        LOG_LEVEL_EMIT(LOCK),
        LOG_LEVEL_EMIT(ATOMICS),
        LOG_LEVEL_EMIT(UNKNOWN)
    };

static char *
level_to_name(shmemu_log_t level)
{
    shmemu_log_table_t *tp = table;

    while (tp->level != LOG_UNKNOWN) {
        if (level == tp->level) {
            return tp->name;
            /* NOT REACHED */
        }
        tp += 1;
    }

    return "UNKNOWN";
}

#if 0
static shmemu_log_t
name_to_level(const char *name)
{
    shmemu_log_table_t *tp = table;

    while (tp->level != LOG_UNKNOWN) {
        if (strncmp(name, tp->name, strlen(tp->name)) == 0) {
            return tp->level;
            /* NOT REACHED */
        }
        tp += 1;
    }

    return LOG_UNKNOWN;
}
#endif

void
shmemu_logger_init(void)
{
    if (proc.env.debug) {

        host = shmemu_gethostname();
        if (host == NULL) {
            host = "unknown";
        }

        /* TODO "%" modifiers for extra info */
        if (proc.env.debug_file != NULL) {
            log_stream = fopen(proc.env.debug_file, "a");
            if (log_stream == NULL) {
                shmemu_fatal("can't append to debug log file \"%s\"",
                             proc.env.debug_file);
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
    }
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
shmemu_logger(shmemu_log_t level, const char *fmt, ...)
{
    if (proc.env.debug) {
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
                 level_to_name(level)
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

        if (level == LOG_FATAL) {
            shmemc_global_exit(1);
        }
    }
}
