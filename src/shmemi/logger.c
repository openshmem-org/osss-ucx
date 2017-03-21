#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/time.h>
#include <stdbool.h>
#include <assert.h>

#include "shmemi.h"

#define TRACE_MSG_BUF_SIZE 256

static FILE *log_stream;
static bool logging = false;
static shmem_log_t levels;

static
char *
level_to_string(shmem_log_t level)
{
    switch (level) {
    case LOG_INIT:
        return "INIT";
    case LOG_FINALIZE:
        return "FINALIZE";
    case LOG_MEMORY:
        return "MEMORY";
    case LOG_HEAP:
        return "HEAP";
    case LOG_ALL:
        return "ALL";
    case LOG_FATAL:
        return "FATAL";
    default:
        return "UNKNOWN";
    }
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

    e = getenv("SHMEM_LOG_LEVEL");
    if (e == NULL) {
        /* nothing to do */
        return;
    }

    logging = true;
    levels = LOG_INIT | LOG_FINALIZE | LOG_MEMORY | LOG_HEAP;
    /* TODO parse string for which levels to set */

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

#define SHMEMI_BIT_IS_SET(_level) \
    (((_level) == LOG_FATAL) || (levels & SHMEMI_BIT_SET(_level)))

void
shmemi_logger(shmem_log_t level, const char *fmt, ...)
{
    if (! logging) {
        return;
    }

    /* TODO if (SHMEMI_BIT_IS_SET(level)) { */
    if (1) {
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
                 shmemi_timer_get_elapsed(),
                 level_to_string(level)
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
