/* For license: see LICENSE file at top-level */

#ifndef _THREADWRAP_THREADING_H
#define _THREADWRAP_THREADING_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <pthread.h>

typedef pthread_t threadwrap_thread_t;
typedef pthread_mutex_t threadwrap_mutex_t;

#define THREADWRAP_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER

inline static int
threadwrap_mutex_init(threadwrap_mutex_t *tp)
{
    return pthread_mutex_init(tp, NULL);
}

inline static int
threadwrap_mutex_destroy(threadwrap_mutex_t *mp)
{
    return pthread_mutex_destroy(mp);
}

inline static int
threadwrap_mutex_lock(threadwrap_mutex_t *mp)
{
    return pthread_mutex_lock(mp);
}

inline static int
threadwrap_mutex_unlock(threadwrap_mutex_t *mp)
{
    return pthread_mutex_unlock(mp);
}

inline static int
threadwrap_mutex_trylock(threadwrap_mutex_t *mp)
{
    return pthread_mutex_trylock(mp);
}

inline static int
threadwrap_thread_create(threadwrap_thread_t *threadp,
                     void *(*start_routine)(void *),
                     void *args)
{
    return pthread_create(threadp, NULL /* attr */, start_routine, args);
}

inline static int
threadwrap_thread_join(threadwrap_thread_t thread,
                   void **retval)
{
    return pthread_join(thread, retval);
}

inline static unsigned long
threadwrap_thread_id(void)
{
    return (unsigned long) pthread_self();
}

inline static int
threadwrap_thread_equal(threadwrap_thread_t t1, threadwrap_thread_t t2)
{
    return pthread_equal(t1, t2);
}

#endif  /* ! _THREADWRAP_THREADING_H */
