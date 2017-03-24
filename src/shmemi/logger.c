#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/time.h>
#include <assert.h>
#include <ctype.h>

#include "shmemi.h"
#include "shmemu.h"

#define TRACE_MSG_BUF_SIZE 256

static FILE *log_stream;
static bool logging = false;
static shmemi_log_t levels;

typedef struct shmemi_log_table {
    shmemi_log_t level;
    char *name;
} shmemi_log_table_t;

#define LOG_LEVEL_EMIT(_level) { LOG_##_level, #_level }

static shmemi_log_table_t table[] =
    {
        LOG_LEVEL_EMIT(INIT),
        LOG_LEVEL_EMIT(FINALIZE),
        LOG_LEVEL_EMIT(MEMORY),
        LOG_LEVEL_EMIT(HEAP),
        LOG_LEVEL_EMIT(FATAL),
        LOG_LEVEL_EMIT(UNKNOWN)
    };

static
char *
level_to_name(shmemi_log_t level)
{
    shmemi_log_table_t *tp = table;

    while (tp->level != LOG_UNKNOWN) {
        if (level == tp->level) {
            return tp->name;
            /* NOT REACHED */
        }
        tp += 1;
    }

    return "UNKNOWN";
}

static
shmemi_log_t
name_to_level(const char *name)
{
    shmemi_log_table_t *tp = table;

    while (tp->level != LOG_UNKNOWN) {
        if (strncmp(name, tp->name, strlen(tp->name)) == 0) {
            return tp->level;
            /* NOT REACHED */
        }
        tp += 1;
    }

    return LOG_UNKNOWN;
}

/*
 * for private use before up and running
 */
static
void
fatal(const char *fmt, ...)
{
        va_list ap;

        va_start(ap, fmt);
        {
            fprintf(stderr, "FATAL: ");
            vfprintf(stderr, fmt, ap);
            fprintf(stderr, "\n");
            fflush(stderr);
        }
        va_end(ap);

        exit(EXIT_FAILURE);
}

void
shmemi_logger_init(void)
{
    char *e;

    e = getenv("SHMEM_LOGGING");
    if (e == NULL) {
        /* nothing to do */
        return;
    }
    if ((tolower(e[0]) == 'n') || (atoi(e) == 0)) {
        /* nothing to do */
        return;
    }

    logging = true;

    /* TODO "%" modifiers for extra info */
    e = getenv("SHMEM_LOG_FILE");
    if (e != NULL) {
        log_stream = fopen(e, "a");
        if (log_stream == NULL) {
            fatal("can't open log file \"%s\"", e);
            /* NOT REACHED */
        }
    }
    else {
        log_stream = stderr;
    }
}

void
shmemi_logger_finalize(void)
{
    if (! logging) {
        return;
    }

    if (log_stream != NULL) {
        fclose(log_stream);
    }
}

void
shmemi_logger(shmemi_log_t level, const char *fmt, ...)
{
    if (logging) {
        char *tmp1;
        char *tmp2;
        va_list ap;

        tmp1 = (char *) malloc(TRACE_MSG_BUF_SIZE * sizeof(*tmp1));
        assert(tmp1 != NULL);
        tmp2 = (char *) malloc(TRACE_MSG_BUF_SIZE * sizeof(*tmp2));
        assert(tmp2 != NULL);

        snprintf(tmp1, TRACE_MSG_BUF_SIZE,
                 "[%4d:%6.6f] %10s: ",
                 p.me,
                 shmemu_timer(),
                 level_to_name(level)
                 );

        va_start(ap, fmt);
        vsnprintf(tmp2, TRACE_MSG_BUF_SIZE, fmt, ap);
        va_end(ap);

        strncat(tmp1, tmp2, strlen(tmp2));
        strncat(tmp1, "\n", 1);

        fputs(tmp1, log_stream);
        /* make sure this all goes out in 1 burst */
        fflush(log_stream);

        free(tmp2);
        free(tmp1);

        if (level == LOG_FATAL) {
            exit(1);            /* maybe not just exit... */
        }
    }
}
