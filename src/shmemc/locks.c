/* For license: see LICENSE file at top-level */

/*
 * Rewrite of an original MCS lock code by
 *
 *    Copyright (c) 1996-2002 by Quadrics Supercomputers World Ltd.
 *    Copyright (c) 2003-2005 by Quadrics Ltd.
 */

#include "shmemc.h"
#include "shmemu.h"
#include "state.h"

#include <sys/types.h>

/*
 * this overlays an opaque blob we can move around with AMOs, and the
 * signalling/PE parts.
 *
 * TODO "short" PE in this setup, should be "int"
 */

typedef union shmem_lock {
    struct data_split {
        int16_t locked;
        int16_t next;
    } d;
    int32_t blob;               /* for AMOs & owner transfer */
} shmem_lock_t;

enum shmem_lock_state {
    SHMEM_LOCK_FREE = -1,
    SHMEM_LOCK_RESET
};

/*
 * spread lock ownership around PEs
 */
inline static int
lock_owner(void *addr)
{
    const uintptr_t la = (uintptr_t) addr;

    return (la >> 3) % proc.nranks;
}

inline static void
set_lock(shmem_lock_t *node, shmem_lock_t *lock)
{
    shmem_lock_t t;
    int16_t prev, locked;

    node->d.next = SHMEM_LOCK_FREE;

    /* request for ownership */
    t.d.locked = 1;
    t.d.next = proc.rank;

    shmemc_swap(&(lock->blob),
                &(t.blob), sizeof(t.blob),
                lock_owner(lock),
                &(t.blob));

    if (t.blob == SHMEM_LOCK_FREE) {
        t.blob = SHMEM_LOCK_RESET;
    }

    prev = t.d.next;
    locked = t.d.locked;

    if (locked) {
        int16_t me = proc.rank;

        node->d.locked = 1;

        shmemc_put(&(node->d.next), &(me), sizeof(me), prev);

        /* sit here until unlocked */
        shmemc_wait_until_eq16(&(node->d.locked), 0);
    }
}

inline static void
clear_lock(shmem_lock_t *node, shmem_lock_t *lock)
{
    int16_t zero = 0;

    if (node->d.next == SHMEM_LOCK_FREE) {
        shmem_lock_t t;
        int32_t r = SHMEM_LOCK_RESET;

        t.d.locked = 1;
        t.d.next = proc.rank;

        shmemc_cswap(&(lock->blob),
                     &(t.blob),
                     &r, sizeof(r),
                     lock_owner(lock),
                     &(t.blob));

        if (t.d.next == proc.rank) {
            return;
            /* NOT REACHED */
        }

        /* wait for a chainer PE to appear */
        shmemc_wait_until_ne16(&(node->d.next), SHMEM_LOCK_FREE);
    }

    shmemc_put(&(node->d.locked), &zero, sizeof(zero), node->d.next);
    // fprintf(stderr, "DEBUG(%d): leave %s\n", proc.rank, __func__);
}

inline static int
test_lock(shmem_lock_t *node, shmem_lock_t *lock)
{
    shmem_lock_t t;
    int ret;

    shmemc_get(&(t.blob), &(lock->blob), sizeof(t.blob), lock_owner(lock));

    if (t.blob == SHMEM_LOCK_FREE) {
        t.blob = SHMEM_LOCK_RESET;
    }

    if (t.blob == SHMEM_LOCK_RESET) {
        set_lock(node, lock);
        ret = 0;
    }
    else {
        ret = 1;
    }

    return ret;
}

/**
 * API
 */

void
shmemc_set_lock(long *lock)
{
    shmem_lock_t *lp = (shmem_lock_t *) lock;

    set_lock(&(lp[1]), &(lp[0]));
}

void
shmemc_clear_lock(long *lock)
{
    shmem_lock_t *lp = (shmem_lock_t *) lock;

    /* flush before release */
    shmemc_quiet();

    clear_lock(&(lp[1]), &(lp[0]));
}

int
shmemc_test_lock(long *lock)
{
    shmem_lock_t *lp = (shmem_lock_t *) lock;

    return test_lock(&(lp[1]), &(lp[0]));
}
