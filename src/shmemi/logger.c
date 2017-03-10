#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/time.h>
#include <stdbool.h>

#include "shmemi.h"

#define TRACE_MSG_BUF_SIZE 256

static FILE *log_stream;
static bool logging = false;
static shmem_log_t levels = 0;

static
inline
char *
level_to_string(shmem_log_t level)
{
    switch (level) {
    case LOG_FATAL:
        return "FATAL";
    case LOG_INIT:
        return "INIT";
    case LOG_FINALIZE:
        return "FINALIZE";
    case LOG_MEMORY:
        return "MEMORY";
    case LOG_ALL:
        return "ALL";
    default:
        return "UNKNOWN";
    }
}

void
shmemi_logger_init(void)
{
    char *e;

    e = getenv("SHMEM_LOG_LEVEL");
    if (e == NULL) {
        return;
    }

    logging = true;
    levels = LOG_ALL;
    /* TODO parse string for which levels to set */

    /* TODO "%" modifiers for extra info */
    e = getenv("SHMEM_LOG_FILE");
    if (e != NULL) {
        log_stream = fopen(e, "a");
    }
    else {
        log_stream = stderr;
    }
}

void
shmemi_logger_finalize(void)
{
    if (log_stream != NULL) {
        fclose(log_stream);
    }
}

#define SHMEMI_BIT_TEST(n) (levels & SHMEMI_BIT_SET(n))

void
shmemi_logger(shmem_log_t level, const char *fmt, ...)
{
    /* TODO just do all logging for now */
#if 1
    if (logging) {
#else
    if (logging && SHMEMI_BIT_TEST(level)) {
#endif
        char *tmp1;
        char *tmp2;
        va_list ap;

        tmp1 = (char *) malloc(TRACE_MSG_BUF_SIZE * sizeof(*tmp1));
        tmp2 = (char *) malloc(TRACE_MSG_BUF_SIZE * sizeof(*tmp2));

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
    }

    if (level == LOG_FATAL) {
        exit(1);
    }
}
