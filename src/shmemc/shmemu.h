#ifndef _SHMEM_SHEMU_H
#define _SHMEM_SHEMU_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <sys/types.h>

/*
 * how many elements in array T?
 *
 */
#define TABLE_SIZE(_t) ( sizeof(_t) / sizeof((_t)[0]) )

void shmemu_init(void);
void shmemu_finalize(void);

void shmemu_timer_init(void);
void shmemu_timer_finalize(void);
double shmemu_timer(void);

char *shmemu_gethostname(void);

int shmemu_parse_size(char *size_str, size_t *bytes_p);

/*
 * message logging
 */

void shmemu_logger_init(void);
void shmemu_logger_finalize(void);

#define SHMEMU_BIT_SET(_level) (1 << (_level))

typedef enum shmemu_log {
    LOG_FATAL       = SHMEMU_BIT_SET(0),
    LOG_INIT        = SHMEMU_BIT_SET(1),
    LOG_FINALIZE    = SHMEMU_BIT_SET(2),
    LOG_MEMORY      = SHMEMU_BIT_SET(3),
    LOG_WORKER      = SHMEMU_BIT_SET(4),
    LOG_HEAP        = SHMEMU_BIT_SET(5),
    LOG_INFO        = SHMEMU_BIT_SET(6),
    LOG_REDUCTION   = SHMEMU_BIT_SET(7),
    LOG_ALL         = LOG_FATAL | LOG_INIT | LOG_FINALIZE
                         | LOG_MEMORY | LOG_HEAP | LOG_WORKER | LOG_INFO,
    LOG_UNKNOWN     = -1
} shmemu_log_t;

void shmemu_logger(shmemu_log_t level, const char *fmt, ...);

#ifdef ENABLE_DEBUG
# define logger(...) shmemu_logger(__VA_ARGS__)
#else
# define logger(...)
#endif /* ENABLE_DEBUG */

inline static void
deprecate(const char *fn)
{
    logger(LOG_INFO, "\"%s\" is deprecated", fn);
}

#endif /* ! _SHMEM_SHEMU_H */
