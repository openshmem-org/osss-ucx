/* For license: see LICENSE file at top-level */

#ifndef _SHMEM_MUTEX_H
#define _SHMEM_MUTEX_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef ENABLE_THREADS

void shmemt_init(void);

void shmemt_mutex_init(void);
void shmemt_mutex_destroy(void);
void shmemt_mutex_lock(void);
void shmemt_mutex_unlock(void);

#define SHMEMT_MUTEX_PROTECT(_fn)               \
    do {                                        \
        shmemt_mutex_lock();                    \
        _fn;                                    \
        shmemt_mutex_unlock();                  \
    } while (0)

#define SHMEMT_MUTEX_NOPROTECT(_fn)             \
    do {                                        \
        _fn;                                    \
    } while (0)

#else

#define shmemt_init()

#define SHMEMT_MUTEX_PROTECT(_fn) _fn
#define SHMEMT_MUTEX_NOPROTECT(_fn) _fn

#endif  /* ENABLE_THREADS */

#endif /* ! _SHMEM_MUTEX_H */
