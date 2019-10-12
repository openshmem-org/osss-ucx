/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"

#include "shmemu.h"
#include "state.h"
// #include "memfence.h"
#include "shmem/api.h"

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

    // LOAD_STORE_FENCE();

    /* request for ownership */
    t.d.locked = 1;
    t.d.next = proc.rank;

    t.blob = shmem_int_atomic_swap(&(lock->blob),
                                   t.blob,
                                   lock_owner(lock));

    if (t.blob == SHMEM_LOCK_FREE) {
        t.blob = SHMEM_LOCK_RESET;
    }

    prev = t.d.next;
    locked = t.d.locked;

    if (locked) {
        node->d.locked = 1;

        // LOAD_STORE_FENCE();

        shmem_short_p(&(node->d.next), proc.rank, prev);

        /* sit here until unlocked */
        shmem_short_wait_until(&(node->d.locked), SHMEM_CMP_EQ, 0);
    }
}

static void
clear_lock(shmem_lock_t *node, shmem_lock_t *lock)
{
    if (node->d.next == SHMEM_LOCK_FREE) {
        shmem_lock_t t;

        t.d.locked = 1;
        t.d.next = proc.rank;

        t.blob = shmem_int_atomic_compare_swap(&(lock->blob),
                                               t.blob,
                                               SHMEM_LOCK_RESET,
                                               lock_owner(lock));

        /* only self here */
        if (t.d.next == proc.rank) {
            return;
            /* NOT REACHED */
        }

        /* wait for a chainer PE to appear */
        shmem_short_wait_until(&(node->d.next), SHMEM_CMP_GE, 0);
    }

    /* unlock */
    shmem_short_p(&(node->d.locked), 0, node->d.next);
}

inline static int
test_lock(shmem_lock_t *node, shmem_lock_t *lock)
{
    shmem_lock_t t;
    int ret;

    t.blob = shmem_int_g(&(lock->blob), lock_owner(lock));

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
    shmem_lock_t *lp = (shmem_lock_t *) lock;

    set_lock(&(lp[1]), &(lp[0]));
}

void
shmem_clear_lock(long *lock)
{
    shmem_lock_t *lp = (shmem_lock_t *) lock;

    /* flush before release */
    shmemc_quiet();

    clear_lock(&(lp[1]), &(lp[0]));
}

int
shmem_test_lock(long *lock)
{
    shmem_lock_t *lp = (shmem_lock_t *) lock;

    return test_lock(&(lp[1]), &(lp[0]));
}
