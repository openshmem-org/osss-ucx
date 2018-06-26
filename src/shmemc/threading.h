/* For license: see LICENSE file at top-level */

#ifndef _SHMEMC_THREADING_H
#define _SHMEMC_THREADING_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <pthread.h>

typedef pthread_t shmemc_thread_t;
typedef pthread_mutex_t shmemc_mutex_t;

#define SHMEMC_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER

inline static int
shmemc_mutex_init(shmemc_mutex_t *tp)
{
    return pthread_mutex_init(tp, NULL);
}

inline static int
shmemc_mutex_destroy(shmemc_mutex_t *mp)
{
    return pthread_mutex_destroy(mp);
}

inline static int
shmemc_mutex_lock(shmemc_mutex_t *mp)
{
    return pthread_mutex_lock(mp);
}

inline static int
shmemc_mutex_unlock(shmemc_mutex_t *mp)
{
    return pthread_mutex_unlock(mp);
}

inline static int
shmemc_mutex_trylock(shmemc_mutex_t *mp)
{
    return pthread_mutex_trylock(mp);
}

inline static unsigned long
shmemc_thread_id(void)
{
    return (unsigned long) pthread_self();
}

inline static int
shmemc_thread_equal(shmemc_thread_t t1, shmemc_thread_t t2)
{
    return pthread_equal(t1, t2);
}

#endif  /* ! _SHMEMC_THREADING_H */
