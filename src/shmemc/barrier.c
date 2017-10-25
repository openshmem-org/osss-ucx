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

void
shmemc_barrier(int start, int log2stride, int size, long *pSync)
{
    const int me = proc.rank;
    const int stride = 1 << log2stride;
    long poke = 1;

    shmemc_quiet();

    *pSync = 0;

    if (start == me) {
        int pe;
        int i;

        /* wait for the rest of the AS to poke me */
        shmemc_long_wait_eq_until(pSync, size - 1);

        /* send acks out */
        pe = start + stride;
        for (i = 1; i < size; i += 1) {
            shmemc_put(pSync, &poke, sizeof(poke), pe);
            pe += stride;
        }
    }
    else {
        /* poke root */
        shmemc_long_add(pSync, 1, start);

        /* get ack */
        shmemc_long_wait_ne_until(pSync, 0);
    }

    /* restore */
    *pSync = SHMEM_SYNC_VALUE;

#if 0
    logger(LOG_INFO,
           "barrier #%lu return: start = %d, stride = %d, size = %d",
           bar_count, start, stride, size);
#endif
    bar_count += 1;
}

#if 0
static long shmemc_all_sync[SHMEM_BARRIER_SYNC_SIZE] = { SHMEM_SYNC_VALUE };
#endif
long shmemc_all_sync = SHMEM_SYNC_VALUE;

void
shmemc_barrier_all(void)
{
    shmemc_barrier(0, 0, proc.nranks, &shmemc_all_sync);
}
