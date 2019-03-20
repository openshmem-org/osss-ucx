/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <pthread.h>

typedef pthread_mutex_t threadwrap_mutex_t;

int
threadwrap_mutex_init(threadwrap_mutex_t *mp)
{
    return pthread_mutex_init(mp, NULL);
}

int
threadwrap_mutex_destroy(threadwrap_mutex_t *mp)
{
    return pthread_mutex_destroy(mp);
}

int
threadwrap_mutex_lock(threadwrap_mutex_t *mp)
{
    return pthread_mutex_lock(mp);
}

int
threadwrap_mutex_unlock(threadwrap_mutex_t *mp)
{
    return pthread_mutex_unlock(mp);
}

int
threadwrap_mutex_trylock(threadwrap_mutex_t *mp)
{
    return pthread_mutex_trylock(mp);
}

typedef pthread_t threadwrap_thread_t;

int
threadwrap_thread_create(threadwrap_thread_t *threadp,
                         void *(*start_routine)(void *),
                         void *args)
{
    return pthread_create(threadp, NULL /* attr */, start_routine, args);
}

int
threadwrap_thread_join(threadwrap_thread_t thread,
                       void **retval)
{
    return pthread_join(thread, retval);
}

threadwrap_thread_t
threadwrap_thread_id(void)
{
    return (threadwrap_thread_t) pthread_self();
}

int
threadwrap_thread_equal(threadwrap_thread_t t1, threadwrap_thread_t t2)
{
    return pthread_equal(t1, t2);
}
