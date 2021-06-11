/* For license: see LICENSE file at top-level */

/*
 * Rewrite of an original MCS lock code by
 *
 *    Copyright (c) 1996-2002 by Quadrics Supercomputers World Ltd.
 *    Copyright (c) 2003-2005 by Quadrics Ltd.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"
#include "shmem.h"

#include <stdint.h>
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

/*
 * spread lock ownership around PEs
 */

inline static int
lock_owner(void *addr)
{
    /*
     * can only agree on distributed owners if we all agree on aligned
     * addresses
     */
#ifdef ENABLE_ALIGNED_ADDRESSES
    const uintptr_t la = ( (uintptr_t) addr ) >> 3;
    const int owner = la % shmem_n_pes();

    // logger(LOG_LOCKS, "addr = %ld, owner = %d", la, owner);
#else
    const int owner = shmem_n_pes() - 1;

    NO_WARN_UNUSED(addr);
#endif /* ENABLE_ALIGNED_ADDRESSES */

    return owner;
}

/*
 * split the lock claim into request + execute.  "cmp" contains the
 * claim and connects the 2 phases
 */

static inline void
set_lock_request(shmem_lock_t *node, shmem_lock_t *lock,
                 int me,
                 shmem_lock_t *cmp)
{
    NO_WARN_UNUSED(node);

    /* request ownership */
    cmp->d.locked = 1;
    cmp->d.next = me;

    /* push my claim into the owner */
    cmp->blob = shmem_int_atomic_swap(&(lock->blob),
                                      cmp->blob,
                                      lock_owner(lock));
}

static inline void
set_lock_execute(shmem_lock_t *node, shmem_lock_t *lock,
                 int me,
                 shmem_lock_t *cmp)
{
    NO_WARN_UNUSED(lock);

    /* tail */
    node->d.next = -1;

    if (cmp->d.locked) {
        node->d.locked = 1;

        /* chain me on */
        shmem_short_p(&(node->d.next), me, cmp->d.next);
        shmem_quiet();

        /* sit here until unlocked */
        while (node->d.locked) {
            shmemc_progress();
        }
    }
}

static void
set_lock(shmem_lock_t *node, shmem_lock_t *lock)
{
    const int me = shmem_my_pe();
    shmem_lock_t t;

    set_lock_request(node, lock, me, &t);
    set_lock_execute(node, lock, me, &t);
}

static void
clear_lock(shmem_lock_t *node, shmem_lock_t *lock)
{
    const int me = shmem_my_pe();

    if (node->d.next < 0) {
        shmem_lock_t t;

        /* request ownership */
        t.d.locked = 1;
        t.d.next = me;

        /* onwer can reset */
        t.blob = shmem_int_atomic_compare_swap(&(lock->blob),
                                               t.blob,
                                               0,
                                               lock_owner(lock));

        /* any more chainers? */
        if (t.d.next == me) {
            return;
            /* NOT REACHED */
        }

        /* otherwise, wait for a chainer PE to appear */
        while (node->d.next < 0) {
            shmemc_progress();
        }
    }

    /* tell next pe about release */
    shmem_short_p(&(node->d.locked), 0, node->d.next);
    shmem_quiet();
}

static int
test_lock(shmem_lock_t *node, shmem_lock_t *lock)
{
    const int me = shmem_my_pe();
    shmem_lock_t t;

    /* request ownership */
    t.d.locked = 1;
    t.d.next = me;

    /* if owner is unset, grab the lock */
    t.blob = shmem_int_atomic_compare_swap(&(lock->blob),
                                           0,
                                           t.blob,
                                           lock_owner(lock));

    if (t.blob == 0) {
        /* grabbed unset lock, now go on to set the rest of the lock */
        set_lock_execute(node, lock, me, &t);
        return 0;
    }
    else {
        /* nope, go around again */
        return 1;
    }
}

/**
 * API
 */

#ifdef ENABLE_PSHMEM
#pragma weak shmem_set_lock = pshmem_set_lock
#define shmem_set_lock pshmem_set_lock
#pragma weak shmem_test_lock = pshmem_test_lock
#define shmem_test_lock pshmem_test_lock
#pragma weak shmem_clear_lock = pshmem_clear_lock
#define shmem_clear_lock pshmem_clear_lock
#endif /* ENABLE_PSHMEM */

/*
 * split the "big" user-visible lock into the internal management
 * types
 */

#define UNPACK()                                        \
    shmem_lock_t *base = (shmem_lock_t *) lp;           \
    shmem_lock_t *node = base + 1;                      \
    shmem_lock_t *lock = base + 0

void
shmem_set_lock(long *lp)
{
    UNPACK();

    SHMEMU_CHECK_INIT();
    SHMEMU_CHECK_SYMMETRIC(lp, 1);

    set_lock(node, lock);
}

void
shmem_clear_lock(long *lp)
{
    UNPACK();

    SHMEMU_CHECK_INIT();
    SHMEMU_CHECK_SYMMETRIC(lp, 1);

    /* required to flush comms before clearing lock */
    shmem_quiet();

    clear_lock(node, lock);
}

int
shmem_test_lock(long *lp)
{
    UNPACK();

    SHMEMU_CHECK_INIT();
    SHMEMU_CHECK_SYMMETRIC(lp, 1);

    return test_lock(node, lock);
}
