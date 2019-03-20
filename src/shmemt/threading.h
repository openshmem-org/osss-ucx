/* For license: see LICENSE file at top-level */

#ifndef _THREADWRAP_THREADING_H
#define _THREADWRAP_THREADING_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

typedef void *threadwrap_thread_t;
typedef void *threadwrap_mutex_t;

int threadwrap_mutex_init(threadwrap_mutex_t *tp);
int threadwrap_mutex_destroy(threadwrap_mutex_t *mp);
int threadwrap_mutex_lock(threadwrap_mutex_t *mp);
int threadwrap_mutex_unlock(threadwrap_mutex_t *mp);
int threadwrap_mutex_trylock(threadwrap_mutex_t *mp);
int threadwrap_thread_create(threadwrap_thread_t *threadp,
                             void *(*start_routine)(void *),
                             void *args);
int threadwrap_thread_join(threadwrap_thread_t thread,
                           void **retval);
threadwrap_thread_t threadwrap_thread_id(void);
int threadwrap_thread_equal(threadwrap_thread_t t1, threadwrap_thread_t t2);

#endif  /* ! _THREADWRAP_THREADING_H */
