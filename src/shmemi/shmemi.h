#ifndef _SHMEMI_H
#define _SHMEMI_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

/*
 * setup/teardown
 */
void shmemi_finalize(void);
void shmemi_init(void);

/*
 * heap creation
 */
void shmemi_setup_heaps(void); /* although maybe int:nheaps? */

#include <sys/types.h>

typedef struct heap_exchange {
    void *base;                 /* start address */
    size_t size;                /* bytes in this heap */
    short filled;               /* has this been exchanged yet? */
} heap_exchange_t;

extern heap_exchange_t *heapx;

void shmemi_heapx_init(void);
void shmemi_heapx_finalize(void);
void shmemi_heapx_set_pe(int pe, void *p, size_t s);
int shmemi_heapx_initialized(int n);

#define HEAP_SIZE (size_t) (4096 * 1024)

void shmemi_heapx_create(size_t s);

/*
 * API heap management
 */
void shmemi_malloc_init(void);
void shmemi_malloc_finalize(void);
void *shmemi_malloc(size_t s);
void shmemi_free(void *p);
void *shmemi_realloc(void *p, size_t s);
void *shmemi_align(size_t a, size_t s);

/*
 * timers
 */
void shmemi_timer_init(void);
void shmemi_timer_finalize(void);
double shmemi_timer_get_elapsed(void);

/*
 * message logging
 */

void shmemi_logger_init(void);
void shmemi_logger_finalize(void);

#define SHMEMI_BIT_SET(_level) (1 << (_level))

typedef enum shmem_log {
    LOG_FATAL       = SHMEMI_BIT_SET(0),
    LOG_INIT        = SHMEMI_BIT_SET(1),
    LOG_FINALIZE    = SHMEMI_BIT_SET(2),
    LOG_MEMORY      = SHMEMI_BIT_SET(3),
    LOG_HEAP        = SHMEMI_BIT_SET(4),
    LOG_ALL         = LOG_FATAL|LOG_INIT|LOG_FINALIZE|LOG_MEMORY|LOG_HEAP
} shmem_log_t;

void shmemi_logger(shmem_log_t level, const char *fmt, ...);

#ifdef ENABLE_DEBUG
# define logger(...) shmemi_logger(__VA_ARGS__)
#else
# define logger(...)
#endif /* ENABLE_DEBUG */

/*
 * PE management
 */

#include <stdbool.h>

typedef struct pe {
    int me;
    int npes;
    bool running;
} pe_t;

extern pe_t p;

/*
 * Ordering
 */
void shmemi_quiet(void);
void shmemi_fence(void);

#endif /* ! _SHMEMI_H */
