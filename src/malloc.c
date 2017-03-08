#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

#include "logger.h"
#include "heapx.h"

static const int DEFAULT_HEAP = 0;

typedef struct malloc_api {
    void *(*malloc_fn)(int, size_t);
    void  (*free_fn)(int, void *);
    void *(*realloc_fn)(int, void *, size_t);
    void *(*align_fn)(int, size_t, size_t);
} malloc_api_t;

static malloc_api_t once_api, run_api;

static malloc_api_t *api;

/* ---------------------------------------------------------------- */

static
void *
shmemi_malloc_run(int heap_no, size_t s)
{
    return malloc(s);
}

static
void *
shmemi_malloc_once(int heap_no, size_t s)
{
    if (! heapx_initialized(heap_no)) {
        heapx_init();
        api = &run_api;
    }

    return api->malloc_fn(heap_no, s);
}

/* ---------------------------------------------------------------- */

static
void
shmemi_free_run(int heap_no, void *p)
{
    free(p);
}

static
void
shmemi_free_once(int heap_no, void *p)
{
    if (! heapx_initialized(heap_no)) {
        heapx_init();
        api = &run_api;
    }

    api->free_fn(heap_no, p);
}

/* ---------------------------------------------------------------- */

static
void *
shmemi_realloc_run(int heap_no, void *p, size_t s)
{
    return realloc(p, s);
}

static
void *
shmemi_realloc_once(int heap_no, void *p, size_t s)
{
    if (! heapx_initialized(heap_no)) {
        heapx_init();
        api = &run_api;
    }

    api->realloc_fn = shmemi_realloc_run;

    return api->realloc_fn(heap_no, p, s);
}

/* ---------------------------------------------------------------- */

static
void *
shmemi_align_run(int heap_no, size_t a, size_t s)
{
    void *p;
    int err;

    err = posix_memalign(&p, a, s);
    if (err != 0) {
        return NULL;
    }

    return p;
}

static
void *
shmemi_align_once(int heap_no, size_t a, size_t s)
{
    if (! heapx_initialized(heap_no)) {
        heapx_init();
        api = &run_api;
    }

    api->align_fn = shmemi_align_run;

    return api->align_fn(heap_no, a, s);
}

/* ---------------------------------------------------------------- */

void
shmemi_malloc_init(void)
{
    once_api.malloc_fn  = shmemi_malloc_once;
    once_api.free_fn    = shmemi_free_once;
    once_api.realloc_fn = shmemi_realloc_once;
    once_api.align_fn   = shmemi_align_once;

    run_api.malloc_fn   = shmemi_malloc_run;
    run_api.free_fn     = shmemi_free_run;
    run_api.realloc_fn  = shmemi_realloc_run;
    run_api.align_fn    = shmemi_align_run;

    api                 = &once_api;
}

void
shmemi_malloc_finalize(void)
{
    return;                     /* nothing to do */
}

/* ---------------------------------------------------------------- */

void *
shmem_malloc(size_t s)
{
    void *p = api->malloc_fn(DEFAULT_HEAP, s);

    logger(LOG_MEMORY, "leave %s(%lu) -> %p", __func__, s, p);

    return p;
}

void
shmem_free(void *p)
{
    api->free_fn(DEFAULT_HEAP, p);

    logger(LOG_MEMORY, "leave %s(%p)", __func__, p);
}

void *
shmem_realloc(void *p, size_t s)
{
    void *new_p = api->realloc_fn(DEFAULT_HEAP, p, s);

    logger(LOG_MEMORY, "leave %s(%p, %lu) -> %p", __func__, p, s, new_p);

    return new_p;
}

void *
shmem_align(size_t a, size_t s)
{
    void *p = api->align_fn(DEFAULT_HEAP, a, s);

    logger(LOG_MEMORY, "leave %s(%lu, %lu) -> %p", __func__, a, s, p);

    return p;
}
