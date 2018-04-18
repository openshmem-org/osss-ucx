/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"

#include "shmemu.h"
#include "shmemc.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_set_lock = pshmem_set_lock
#define shmem_set_lock pshmem_set_lock
#pragma weak shmem_test_lock = pshmem_test_lock
#define shmem_test_lock pshmem_test_lock
#pragma weak shmem_clear_lock = pshmem_clear_lock
#define shmem_clear_lock pshmem_clear_lock
#endif /* ENABLE_PSHMEM */

void
shmem_set_lock(long *lock)
{
    logger(LOG_LOCKS,
           "%s(lock=%p)",
           __func__,
           lock
           );

    SHMEMT_MUTEX_PROTECT(shmemc_set_lock(lock));
}

void
shmem_clear_lock(long *lock)
{
    logger(LOG_LOCKS,
           "%s(lock=%p)",
           __func__,
           lock
           );

    SHMEMT_MUTEX_PROTECT(shmemc_clear_lock(lock));
}

int
shmem_test_lock(long *lock)
{
    int s;

    logger(LOG_LOCKS,
           "%s(lock=%p)",
           __func__,
           lock
           );

    SHMEMT_MUTEX_PROTECT(s = shmemc_test_lock(lock));

    return s;
}
