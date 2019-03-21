/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

/*
 * This only gets used if threading enabled
 */

#ifdef ENABLE_THREADS

#include "state.h"
#include "shmem_mutex.h"
#include "shmem/defs.h"
#include "threading.h"

static threadwrap_mutex_t comms_mutex;

void
shmemt_init(void)
{
    threadwrap_mutex_init(&comms_mutex);
}

void
shmemt_mutex_init(void)
{
    if (proc.td.osh_tl == SHMEM_THREAD_MULTIPLE) {
        threadwrap_mutex_init(&comms_mutex);
    }
}

void
shmemt_mutex_destroy(void)
{
    if (proc.td.osh_tl == SHMEM_THREAD_MULTIPLE) {
        threadwrap_mutex_destroy(&comms_mutex);
    }
}

void
shmemt_mutex_lock(void)
{
    if (proc.td.osh_tl == SHMEM_THREAD_MULTIPLE) {
        threadwrap_mutex_lock(&comms_mutex);
    }
}

void
shmemt_mutex_unlock(void)
{
    if (proc.td.osh_tl == SHMEM_THREAD_MULTIPLE) {
        threadwrap_mutex_unlock(&comms_mutex);
    }
}

#endif  /* ENABLE_THREADS */
