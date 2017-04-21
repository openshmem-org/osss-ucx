/*
 *    Copyright (c) 1996-2002 by Quadrics Supercomputers World Ltd.
 *    Copyright (c) 2003-2005 by Quadrics Ltd.
 *
 *    For licensing information please see the supplied COPYING file
 *
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc/shmemc.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_set_lock = pshmem_set_lock
#define shmem_set_lock pshmem_set_lock
#pragma weak shmem_test_lock = pshmem_test_lock
#define shmem_test_lock pshmem_test_lock
#pragma weak shmem_clear_lock = pshmem_clear_lock
#define shmem_clear_lock pshmem_clear_lock
#endif /* ENABLE_PSHMEM */

void
shmem_set_lock(volatile long *lock)
{
    shmemc_set_lock(lock);
}

void
shmem_clear_lock(volatile long *lock)
{
    /* The Cray man pages suggest we also need to do this (addy
       12.10.05) */
    shmemc_quiet();

    shmemc_clear_lock(lock);
}

int
shmem_test_lock(volatile long *lock)
{
    return shmemc_test_lock(lock);
}
