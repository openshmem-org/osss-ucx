/* For license: see LICENSE file at top-level */

#ifndef _SHMEMC_THREADING_H
#define _SHMEMC_THREADING_H 1

#include <pthread.h>

typedef pthread_t shmemc_thread_t;
typedef pthread_mutex_t shmemc_mutex_t;

#define SHMEMC_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER

int shmemc_mutex_init(shmemc_mutex_t *tp);

int shmemc_mutex_destroy(shmemc_mutex_t *mp);
int shmemc_mutex_lock(shmemc_mutex_t *mp);
int shmemc_mutex_unlock(shmemc_mutex_t *mp);
int shmemc_mutex_trylock(shmemc_mutex_t *mp);

unsigned long shmemc_thread_id(void);
int shmemc_thread_equal(shmemc_thread_t t1, shmemc_thread_t t2);

#endif  /* ! _SHMEMC_THREADING_H */
