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

/*
 *    Copyright (c) 1996-2002 by Quadrics Supercomputers World Ltd.
 *    Copyright (c) 2003-2005 by Quadrics Ltd.
 *
 *    For licensing information please see the supplied COPYING file
 *
 */

/**
 * Implement the CRAY SHMEM locking API using MCS locks
 *
 * Mellor-Crummey & Scott, Algorithms for scalable synchronisation on
 * shared-memory multiprocessors ACM Trans. Computer Systems, 1991
 *
 * With CRAY SHMEM locks we are given an 8-byte global symmetric
 * object. This memory is pre-initialised to zero in all processes.
 *
 * We split this lock memory into two 32-bit halves where each half
 * then represents a SHMEM_LOCK.  The SHMEM_LOCK struct consists of a
 * 16-bit boolean flag (locked) and a 16-bit vp (next)
 *
 * One vp is chosen to the global lock owner process and here the 1st
 * SHMEM_LOCK acts as the 'tail' of a globally distributed linked
 * list.  In all processes the 2nd SHMEM_LOCK is used to hold and
 * manage the distributed linked list state.
 */

typedef struct
{
    union
    {
        struct
        {
            volatile uint16_t locked;   /* boolean to indicate current state
                                           of lock */
            volatile int16_t next;  /* vp of next requestor */
        } s;
        volatile uint32_t word;
    } u;
#define l_locked        u.s.locked
#define l_next          u.s.next
#define l_word          u.word
} SHMEM_LOCK;

enum shmem_lock {
    SHMEM_LOCK_FREE = 0,
    SHMEM_LOCK_RESET,
    SHMEM_LOCK_SET
};

/* Macro to map lock virtual address to owning process vp */
#define LOCK_OWNER(LOCK) ( ((uintptr_t)(LOCK) >> 3) % proc.nranks )

static void
lock_acquire(SHMEM_LOCK * node, SHMEM_LOCK * lock, int this_pe)
{
    SHMEM_LOCK tmp;
    long locked;
    int prev_pe;

    node->l_next = SHMEM_LOCK_FREE;

    /* Form our lock request (integer) */
    tmp.l_locked = 1;
    tmp.l_next = this_pe;

    /*
     * Swap this_pe into the global lock owner, returning previous
     * value, atomically
     */
    tmp.l_word =
        shmemc_swap32(&lock->l_word, tmp.l_word, LOCK_OWNER(lock));

    /* Translate old (broken) default lock state */
    if (tmp.l_word == SHMEM_LOCK_FREE) {
        tmp.l_word = SHMEM_LOCK_RESET;
    }

    /* Extract the global lock (tail) state */
    prev_pe = tmp.l_next;
    locked = tmp.l_locked;

    /* Is the lock held by someone else ? */
    if (locked) {
        /*
         * This flag gets cleared (remotely) once the lock is dropped
         */
        node->l_locked = 1;

        /*
         * I'm now next in global linked list, update l_next in the
         * prev_pe process with our vp
         */
        shmemc_put(&node->l_next, &this_pe, sizeof(node->l_next), prev_pe);

        /* Wait for flag to be released */
        shmemc_wait_ne_until16(&node->l_locked, 0);
#if 0
        GASNET_BLOCKUNTIL( !(node->l_locked) );
#endif
    }
}

static void
lock_release(SHMEM_LOCK * node, SHMEM_LOCK * lock, int this_pe)
{
    int zero = 0;

    /* Is there someone on the linked list ? */
    if (node->l_next == SHMEM_LOCK_FREE) {
        SHMEM_LOCK tmp;

        /* Form the remote atomic compare value (int) */
        tmp.l_locked = 1;
        tmp.l_next = this_pe;

        /*
         * If global lock owner value still equals this_pe, load RESET
         * into it & return prev value
         */
        tmp.l_word = shmemc_cswap32(&lock->l_word,
                                    tmp.l_word,
                                    SHMEM_LOCK_RESET, LOCK_OWNER(lock));

        if (tmp.l_next == this_pe) {
            /* We were still the only requestor, all done */
            return;
        }

        /*
         * Somebody is about to chain themself off us, wait for them to do it.
         *
         * Quadrics: we have seen l_next being written as two individual
         * bytes here when when the usercopy device is active, poll for
         * it being valid as well as it being set to ensure both bytes
         * are written before we try to use its value below.
         *
         */
        do {
            shmemc_test_eq16(&node->l_next, SHMEM_LOCK_FREE);
        } while ( ! ( (node->l_next == SHMEM_LOCK_FREE) ||
                      (node->l_next < 0)));
#if 0
        shmemc_wait_ne_until16(&node->l_next, SHMEM_LOCK_FREE);
        GASNET_BLOCKUNTIL(!
                          ((node->l_next == SHMEM_LOCK_FREE) ||
                           (node->l_next < 0))
                          );
#endif

    }

    /* Be more strict about the test above, this memory consistency problem is
       a tricky one */
    shmemc_wait_ge_until16(&node->l_next, 0);
#if 0
    GASNET_BLOCKUNTIL( !(node->l_next < 0) );
#endif

    /*
     * Release any waiters on the linked list
     */

    shmemc_put(&node->l_locked, &zero, sizeof(node->l_locked), node->l_next);
}


/*
 * I am not sure this is strictly correct. The Cray man pages suggest
 * that this routine should not block. With this implementation we could
 * race with another PE doing the same and then block in the acquire
 * Perhaps a conditional swap at the beginning would fix it ??
 *
 * (addy 12.10.05)
 */
static int
lock_test(SHMEM_LOCK * node, SHMEM_LOCK * lock, int this_pe)
{
    SHMEM_LOCK tmp;
    int retval;

    /* Read the remote global lock value */
    shmemc_get(&tmp.l_word,
               &lock->l_word,
               sizeof(tmp.l_word),
               LOCK_OWNER(lock));

    /* Translate old (broken) default lock state */
    if (tmp.l_word == SHMEM_LOCK_FREE)
        tmp.l_word = SHMEM_LOCK_RESET;

    /* If lock already set then return 1, otherwise grab the lock & return 0 */
    if (tmp.l_word == SHMEM_LOCK_RESET) {
        lock_acquire(node, lock, this_pe);
        retval = 0;
    }
    else {
        retval = 1;
    }

    return retval;
}

void
shmemc_set_lock(long *lock)
{
    lock_acquire(&((SHMEM_LOCK *) lock)[1],
                 &((SHMEM_LOCK *) lock)[0],
                 proc.rank);
}

void
shmemc_clear_lock(long *lock)
{
    /* The Cray man pages suggest we also need to do this (addy
       12.10.05) */
    shmemc_quiet();

    lock_release(&((SHMEM_LOCK *) lock)[1],
                 &((SHMEM_LOCK *) lock)[0],
                 proc.rank);
}

int
shmemc_test_lock(long *lock)
{
    return lock_test(&((SHMEM_LOCK *) lock)[1],
                     &((SHMEM_LOCK *) lock)[0],
                     proc.rank);
}
