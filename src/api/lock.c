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
#include "shmem_mutex.h"

#include <stdint.h>
#include <sys/types.h>

/*
 * this overlays an opaque blob we can move around with AMOs, and the
 * signalling/PE parts.
 *
 * TODO "short" PE in this setup, should be "int"
 */

enum {
    SHMEM_LOCK_FREE = -1,
    SHMEM_LOCK_RESET = 0,       /* value matches lock initializer in spec */
    SHMEM_LOCK_ACQUIRED
};

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
get_owner_spread(uint64_t addr)
{
    return (addr >> 3) % shmemc_n_pes();
}

inline static int
lock_owner(void *addr)
{
    const uint64_t la = (const uint64_t) addr;
    int owner;

    /*
     * can only agree on distributed owners if we all agree on aligned
     * addresses
     */
#ifdef ENABLE_ALIGNED_ADDRESSES
    owner = get_owner_spread(la);
#else
    if (shmemc_global_address(la)) {
        owner = get_owner_spread(la);
    }
    else {
        /* don't choose PE 0, as it is often used for work allocation */
        owner = shmemc_n_pes() - 1;
    }
#endif /* ENABLE_ALIGNED_ADDRESSES */

    return owner;
}

/*
 * split the lock claim into 2-phase request + execute.
 *
 * "cmp" contains the claim and connects the 2 phases
 */

/*
 *
 * lock requests
 *
 */

/*
 * common lock action
 */

inline static void
try_lock_action(shmem_lock_t *lock,
                int cond, int value,
                shmem_lock_t *cmp)
{
    cmp->blob = shmem_int_atomic_compare_swap(&(lock->blob),
                                              cond, value,
                                              lock_owner(lock)
                                              );
}

/*
 * attempt to get set/clear a lock
 */

inline static void
try_request_lock(shmem_lock_t *lock, int me, shmem_lock_t *cmp)
{
    const shmem_lock_t tmp = {
        .d.locked  = SHMEM_LOCK_ACQUIRED,
        .d.next = me
    };

    try_lock_action(lock, SHMEM_LOCK_RESET, tmp.blob, cmp);
}

inline static void
try_clear_lock(shmem_lock_t *lock, int me, shmem_lock_t *cmp)
{
    const shmem_lock_t tmp = {
        .d.locked  = SHMEM_LOCK_ACQUIRED,
        .d.next = me
    };

    try_lock_action(lock, tmp.blob, SHMEM_LOCK_RESET, cmp);
}

/*
 * request phase for each routine
 */

inline static void
set_lock_request(shmem_lock_t *lock, int me, shmem_lock_t *cmp)
{
    /* push my claim into the owner */
    do {
        try_request_lock(lock, me, cmp);
    } while (cmp->blob != SHMEM_LOCK_RESET);
}

inline static void
test_lock_request(shmem_lock_t *lock, int me, shmem_lock_t *cmp)
{
    /* if owner is unset, grab the lock */
    try_request_lock(lock, me, cmp);
}

inline static void
clear_lock_request(shmem_lock_t *node, shmem_lock_t *lock,
                   int me,
                   shmem_lock_t *cmp)
{
    if (node->d.next == SHMEM_LOCK_FREE) {
        try_clear_lock(lock, me, cmp);
    }
}

/*
 * lock execution
 */

inline static void
set_lock_execute(shmem_lock_t *node, int me, shmem_lock_t *cmp)
{
    /* tail */
    node->d.next = SHMEM_LOCK_FREE;

    if (cmp->d.locked == SHMEM_LOCK_ACQUIRED) {
        node->d.locked = SHMEM_LOCK_ACQUIRED;

        /* chain me on */
        shmem_short_p(&(node->d.next), me, cmp->d.next);

        /* sit here until unlocked */
        do {
            shmemc_progress();
        } while (node->d.locked == SHMEM_LOCK_ACQUIRED);
    }
}

inline static int
test_lock_execute(shmem_lock_t *node, int me, shmem_lock_t *cmp)
{
    if (cmp->blob == SHMEM_LOCK_RESET) {
        /* grabbed unset lock, now go on to set the rest of the lock */
        set_lock_execute(node, me, cmp);
        return 0;
    }
    else {
        /* nope, go around again */
        return 1;
    }
}

inline static void
clear_lock_execute(shmem_lock_t *node, int me, shmem_lock_t *cmp)
{
    /* any more chainers? */
    if (cmp->d.next == me) {
        return;
        /* NOT REACHED */
    }

    /* wait for a chainer PE to appear */
    do {
        shmemc_progress();
    } while (node->d.next == SHMEM_LOCK_FREE);

    /* tell next pe about release */
    shmem_short_p(&(node->d.locked), SHMEM_LOCK_RESET, node->d.next);
}

/*
 * internal blocking calls
 */

inline static void
set_lock(shmem_lock_t *node, shmem_lock_t *lock, int me)
{
    shmem_lock_t t;

    set_lock_request(lock, me, &t);
    set_lock_execute(node, me, &t);
}

inline static void
clear_lock(shmem_lock_t *node, shmem_lock_t *lock, int me)
{
    shmem_lock_t t;

    /* required to flush comms before clearing lock */
    shmemc_quiet();

    clear_lock_request(node, lock, me, &t);
    clear_lock_execute(node, me, &t);
}

inline static int
test_lock(shmem_lock_t *node, shmem_lock_t *lock, int me)
{
    shmem_lock_t t;

    test_lock_request(lock, me, &t);
    return test_lock_execute(node, me, &t);
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
    SHMEMU_CHECK_NOT_NULL(lp, 1);
    SHMEMU_CHECK_SYMMETRIC(lp, 1);

    logger(LOG_LOCKS, "%s(lock=%p)", __func__, lock);

    SHMEMT_MUTEX_NOPROTECT(set_lock(node, lock, shmemc_my_pe()));
}

void
shmem_clear_lock(long *lp)
{
    UNPACK();

    SHMEMU_CHECK_INIT();
    SHMEMU_CHECK_NOT_NULL(lp, 1);
    SHMEMU_CHECK_SYMMETRIC(lp, 1);

    logger(LOG_LOCKS, "%s(lock=%p)", __func__, lock);

    SHMEMT_MUTEX_NOPROTECT(clear_lock(node, lock, shmemc_my_pe()));
}

int
shmem_test_lock(long *lp)
{
    int ret;
    UNPACK();

    SHMEMU_CHECK_INIT();
    SHMEMU_CHECK_NOT_NULL(lp, 1);
    SHMEMU_CHECK_SYMMETRIC(lp, 1);

    logger(LOG_LOCKS, "%s(lock=%p)", __func__, lock);

    SHMEMT_MUTEX_NOPROTECT(ret = test_lock(node, lock, shmemc_my_pe()));

    return ret;
}
