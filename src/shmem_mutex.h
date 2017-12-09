/* For license: see LICENSE file at top-level */

#ifndef _SHMEM_MUTEX_H
#define _SHMEM_MUTEX_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "state.h"

#include "shmem/defs.h"

#include <pthread.h>

#ifdef ENABLE_THREADS

inline static void
shmeml_mutex_init(pthread_mutex_t *mp)
{
    if (proc.thread_level > SHMEM_THREAD_SINGLE) {
        pthread_mutex_init(mp, NULL);
    }
}

inline static void
shmeml_mutex_destroy(pthread_mutex_t *mp)
{
    if (proc.thread_level > SHMEM_THREAD_SINGLE) {
        pthread_mutex_destroy(mp);
    }
}

inline static void
shmeml_mutex_lock(pthread_mutex_t *mp)
{
    if (proc.thread_level > SHMEM_THREAD_SINGLE) {
        pthread_mutex_lock(mp);
    }
}

inline static void
shmeml_mutex_unlock(pthread_mutex_t *mp)
{
    if (proc.thread_level > SHMEM_THREAD_SINGLE) {
        pthread_mutex_unlock(mp);
    }
}

#define SHMEML_MUTEX_PROTECT(_fn, _mp)            \
    do {                                          \
        shmeml_mutex_lock(_mp);                   \
        _fn;                                      \
        shmeml_mutex_unlock(_mp);                 \
    } while (0)

#else

#define shmeml_mutex_init(_mp)
#define shmeml_mutex_destroy(_mp)
#define shmeml_mutex_lock(_mp)
#define shmeml_mutex_unlock(_mp)

#define SHMEML_MUTEX_PROTECT(_fn, _mp) _fn

#endif  /* ENABLE_THREADS */

#endif /* ! _SHMEM_MUTEX_H */
