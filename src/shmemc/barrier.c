/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"
#include "shmemu.h"
#include "state.h"
#include "shmem/defs.h"

/*
 * internal debugging tracker for barrier counts
 */
static unsigned long bar_count = 0;

/*
 * just play with a simple linear barrier for now
 */

inline static void
barrier_helper(int start, int log2stride, int size, long *pSync)
{
    const int me = proc.rank;
    const int stride = 1 << log2stride;

    shmemc_quiet();

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

#if 0
    logger(LOG_BARRIER,
           "barrier #%lu return: start = %d, stride = %d, size = %d",
           bar_count, start, stride, size);
#endif
    bar_count += 1;
}

void
shmemc_barrier(int start, int log2stride, int size, long *pSync)
{
    barrier_helper(start, log2stride, size, pSync);
}

long shmemc_all_sync = SHMEM_SYNC_VALUE;

void
shmemc_barrier_all(void)
{
    barrier_helper(0,
                   0,
                   proc.nranks,
                   &shmemc_all_sync
                   );
}
