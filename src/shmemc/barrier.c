/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"
#include "state.h"
#include "memfence.h"

#include "shmem/defs.h"

/*
 * just play with a simple linear barrier for now
 */

inline static void
barrier_sync_helper_linear(int start, int log2stride, int size, long *pSync)
{
    const int me = proc.rank;
    const int stride = 1 << log2stride;

    if (start == me) {
        const int npokes = size - 1;
        long poke = *pSync + 1;
        int pe;
        int i;

        /* wait for the rest of the AS to poke me */
        shmemc_wait_eq_until64(pSync, npokes);
        *pSync = SHMEM_SYNC_VALUE;

        /* send acks out */
        pe = start + stride;
        for (i = 1; i < size; i += 1) {
            shmemc_put(pSync, &poke, sizeof(poke), pe);
            pe += stride;
        }
    }
    else {
        /* poke root */
        shmemc_inc64(pSync, start);

        /* get ack */
        shmemc_wait_ne_until64(pSync, SHMEM_SYNC_VALUE);
        *pSync = SHMEM_SYNC_VALUE;
    }
}

/*
 * chosen implementation.  Later can be selected through e.g. env var
 */
static void (*barrier_sync_helper)(int start, int log2stride,
                                   int size, long *pSync) =
    barrier_sync_helper_linear;

/*
 * internal psyncs
 */
long shmemc_all_barrier = SHMEM_SYNC_VALUE;
long shmemc_all_sync = SHMEM_SYNC_VALUE;

/*
 * API
 */

void
shmemc_barrier(int start, int log2stride, int size, long *pSync)
{
    shmemc_quiet();

    barrier_sync_helper(start, log2stride, size, pSync);
}

void
shmemc_barrier_all(void)
{
    shmemc_barrier(0, 0, proc.nranks, &shmemc_all_barrier);
}

void
shmemc_sync(int start, int log2stride, int size, long *pSync)
{
    LOAD_STORE_FENCE();

    barrier_sync_helper(start, log2stride, size, pSync);
}

void
shmemc_sync_all(void)
{
    shmemc_sync(0, 0, proc.nranks, &shmemc_all_sync);
}
