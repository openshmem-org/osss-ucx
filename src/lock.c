/*
 *    Copyright (c) 1996-2002 by Quadrics Supercomputers World Ltd.
 *    Copyright (c) 2003-2005 by Quadrics Ltd.
 *
 *    For licensing information please see the supplied COPYING file
 *
 */

#include "shmemc/shmemc.h"

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
