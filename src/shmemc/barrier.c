#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"
#include "shmemu.h"
#include "state.h"

/*
 * internal debugging tracker for barrier counts
 */
static long bar_count = 0;

/*
 * just play with a simple linear barrier for now
 */

void
shmemc_barrier(int start, int log2stride, int size, long *pSync)
{
    const int me = proc.rank;
    const int stride = 1 << log2stride;
    long one = 1;

    shmemc_quiet();

    if (start == me) {
        int pe;
        int i;

        /* wait for the rest of the AS to poke me */
        shmemc_long_wait_eq_until(pSync, size - 1);

        /* send acks out */
        pe = start + stride;
        for (i = 1; i < size; i += 1) {
            shmemc_put(pSync, &one, sizeof(one), pe); /* TODO: shmem_p */
            pe += stride;
        }
    }
    else {
        /* poke root */
        shmemc_long_add(pSync, 1, start);

        /* get ack */
        shmemc_long_wait_ne_until(pSync, 0);
    }

    /* reset */
    *pSync = 0;

    logger(LOG_INFO,
           "barrier #%ld return: start = %d, stride = %d, size = %d",
           bar_count, start, stride, size);

    bar_count += 1;
}

void
shmemc_barrier_all(void)
{
    shmemc_barrier(0, 0, proc.nranks, shmemc_all_sync);
}
