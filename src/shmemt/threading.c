/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "threading.h"

#include <pthread.h>

typedef pthread_mutex_t thr_mutex_t;

int
threadwrap_mutex_init(threadwrap_mutex_t *mp)
{
    thr_mutex_t *tp = (thr_mutex_t *) mp;

    return pthread_mutex_init(tp, NULL);
}

int
threadwrap_mutex_destroy(threadwrap_mutex_t *mp)
{
    thr_mutex_t *tp = (thr_mutex_t *) mp;

    return pthread_mutex_destroy(tp);
}

int
threadwrap_mutex_lock(threadwrap_mutex_t *mp)
{
    thr_mutex_t *tp = (thr_mutex_t *) mp;

    return pthread_mutex_lock(tp);
}

int
threadwrap_mutex_unlock(threadwrap_mutex_t *mp)
{
    thr_mutex_t *tp = (thr_mutex_t *) mp;

    return pthread_mutex_unlock(tp);
}

int
threadwrap_mutex_trylock(threadwrap_mutex_t *mp)
{
    thr_mutex_t *tp = (thr_mutex_t *) mp;

    return pthread_mutex_trylock(tp);
}

typedef pthread_t thr_thread_t;

int
threadwrap_thread_create(threadwrap_thread_t *threadp,
                         void *(*start_routine)(void *),
                         void *args)
{
    thr_thread_t *tp = (thr_thread_t *) threadp;

    return pthread_create(tp, NULL /* attr */, start_routine, args);
}

int
threadwrap_thread_join(threadwrap_thread_t thread,
                       void **retval)
{
    thr_thread_t t = (thr_thread_t) thread;

    return pthread_join(t, retval);
}

threadwrap_thread_t
threadwrap_thread_id(void)
{
    thr_thread_t id = (thr_thread_t) pthread_self();

    return (threadwrap_thread_t) id;
}

int
threadwrap_thread_equal(threadwrap_thread_t t1, threadwrap_thread_t t2)
{
    thr_thread_t tt1 = (thr_thread_t) t1;
    thr_thread_t tt2 = (thr_thread_t) t2;

    return pthread_equal(tt1, tt2);
}
