#include "threading.h"

int
shmemc_mutex_init(shmemc_mutex_t *tp)
{
    return pthread_mutex_init(tp, NULL);
}

unsigned long
shmemc_thread_id(void)
{
    return pthread_self();
}

int
shmemc_mutex_destroy(shmemc_mutex_t *mp)
{
    return pthread_mutex_destroy(mp);
}

int
shmemc_mutex_lock(shmemc_mutex_t *mp)
{
    return pthread_mutex_lock(mp);
}

int
shmemc_mutex_unlock(shmemc_mutex_t *mp)
{
    return pthread_mutex_unlock(mp);
}

int
shmemc_mutex_trylock(shmemc_mutex_t *mp)
{
    return pthread_mutex_trylock(mp);
}
