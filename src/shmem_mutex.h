/* For license: see LICENSE file at top-level */

#ifndef _SHMEM_MUTEX_H
#define _SHMEM_MUTEX_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef ENABLE_THREADS

#include "state.h"

#include "shmem/defs.h"

#include <pthread.h>

extern pthread_mutex_t comms_mutex;

inline static void
shmemt_mutex_init(void)
{
    if (proc.thread_level == SHMEM_THREAD_MULTIPLE) {
        pthread_mutex_init(&comms_mutex, NULL);
    }
}

inline static void
shmemt_mutex_destroy(void)
{
    if (proc.thread_level == SHMEM_THREAD_MULTIPLE) {
        pthread_mutex_destroy(&comms_mutex);
    }
}

inline static void
shmemt_mutex_lock(void)
{
    if (proc.thread_level == SHMEM_THREAD_MULTIPLE) {
        pthread_mutex_lock(&comms_mutex);
    }
}

inline static void
shmemt_mutex_unlock(void)
{
    if (proc.thread_level == SHMEM_THREAD_MULTIPLE) {
        pthread_mutex_unlock(&comms_mutex);
    }
}

#define SHMEMT_MUTEX_PROTECT(_fn)                 \
    do {                                          \
        shmemt_mutex_lock();                      \
        _fn;                                      \
        shmemt_mutex_unlock();                    \
    } while (0)

#else

#define shmemt_mutex_init()
#define shmemt_mutex_destroy()
#define shmemt_mutex_lock()
#define shmemt_mutex_unlock()

#define SHMEMT_MUTEX_PROTECT(_fn) _fn

#endif  /* ENABLE_THREADS */

#endif /* ! _SHMEM_MUTEX_H */
