/* For license: see LICENSE file at top-level */

/*
 * ------------------------------------------------------------------
 *
 * Low-level lock routines
 *
 */

#include "shmemc.h"
#include "shmemu.h"
#include "state.h"
#include "memfence.h"

#include <sys/types.h>
#include <string.h>

typedef union shmem_lock {
    struct data_split {
        int16_t locked;
        int16_t next;
    } d;
    int32_t blob;
} shmem_lock_t;

#define SHMEM_LOCK_FREE -1
#define SHMEM_LOCK_RESET 0

inline static int
lock_owner(void *addr)
{
    uintptr_t la = (uintptr_t) addr;

    return (la >> 3) % proc.nranks;
}

inline static void
set_lock(shmem_lock_t *node, shmem_lock_t *lock)
{
    shmem_lock_t t;

    node->d.next = SHMEM_LOCK_FREE;
    LOAD_STORE_FENCE();

    t.d.locked = 1;
    t.d.next = proc.rank;

    t.blob = shmemc_swap32(&(lock->blob), t.blob, lock_owner(lock));

    if (t.blob == SHMEM_LOCK_FREE) {
        t.blob = SHMEM_LOCK_RESET;
    }

    if (t.d.locked) {
        int16_t me = proc.rank;

        node->d.locked =1;
        LOAD_STORE_FENCE();

        shmemc_put(&(node->d.next), &(me), sizeof(me), t.d.next);

        shmemc_wait_eq_until16(&(node->d.locked), 0);
    }
}

inline static void
clear_lock(shmem_lock_t *node, shmem_lock_t *lock)
{
    int16_t zero = 0;

    if (node->d.next == SHMEM_LOCK_FREE) {
        shmem_lock_t t;

        t.d.locked = 1;
        t.d.next = proc.rank;

        t.blob = shmemc_cswap32(&(lock->blob),
                                t.blob,
                                SHMEM_LOCK_RESET,
                                lock_owner(lock));
        if (t.d.next == proc.rank) {
            return;
        }

        shmemc_wait_ge_until16(&(node->d.next), 0);
    }

    shmemc_put(&(node->d.locked), &zero, sizeof(zero), node->d.next);
}

inline static int
test_lock(shmem_lock_t *node, shmem_lock_t *lock)
{
    shmem_lock_t t;

    shmemc_get(&(t.blob), &(lock->blob), sizeof(t.blob), lock_owner(lock));

    if (t.blob == SHMEM_LOCK_FREE) {
        t.blob = SHMEM_LOCK_RESET;
    }

    if (t.blob == SHMEM_LOCK_RESET) {
        set_lock(node, lock);
        return 0;
    }

    return 1;
}



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

    shmemc_quiet();

    clear_lock(&(lp[1]), &(lp[0]));
}

int
shmemc_test_lock(long *lock)
{
    shmem_lock_t *lp = (shmem_lock_t *) lock;

    return test_lock(&(lp[1]), &(lp[0]));
}
