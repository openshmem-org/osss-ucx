/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "state.h"
#include "shmemc.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_collect32 = pshmem_collect32
#define shmem_collect32 pshmem_collect32

#pragma weak shmem_collect64 = pshmem_collect64
#define shmem_collect64 pshmem_collect64
#endif /* ENABLE_PSHMEM */

/**
 * collect puts nelems (can vary from PE to PE) from source on each
 * PE in the set to target on all PEs in the set.  source -> target is
 * done in PE order.
 *
 * We set up a wavefront that propagates the accumulated offsets and
 * then overlaps forwarding the extra offset contribution from each PE
 * with actually sending source -> target.  Control data goes left to
 * right, application data goes "top to bottom", as it were.
 *
 */

#define SHMEM_COLLECT(_bits, _bytes)                                    \
    void                                                                \
    shmem_collect##_bits(void *target, const void *source,              \
                         size_t nelems,                                 \
                         int start, int logstride, int size,            \
                         long *pSync)                                   \
    {                                                                   \
        const int step = 1 << logstride;                                \
        const int last_pe = start + step * (size - 1);                  \
                                                                        \
        /* initialize left-most or wait for left-neighbor to notify */  \
        if (proc.rank == start) {                                       \
            *pSync = 0;                                                 \
        }                                                               \
        else {                                                          \
            shmemc_wait_ne_until64(pSync, SHMEM_SYNC_VALUE);            \
        }                                                               \
                                                                        \
        /*                                                              \
         * forward my contribution to (notify) right neighbor if not    \
         * last PE in set                                               \
         */                                                             \
        if (proc.rank < last_pe) {                                      \
            const long next_off = *pSync + nelems;                      \
                                                                        \
            shmemc_put(pSync, &next_off,                                \
                       sizeof(next_off),                                \
                       proc.rank + step);                               \
        }                                                               \
                                                                        \
        /* send my array slice to target everywhere */                  \
        {                                                               \
            const long tidx = *pSync * _bytes;                          \
            int i;                                                      \
            int pe = start;                                             \
                                                                        \
            for (i = 0; i < size; i += 1) {                             \
                shmemc_put(&((char *)target)[tidx], source,             \
                           nelems * _bytes,                             \
                           pe);                                         \
                pe += step;                                             \
            }                                                           \
        }                                                               \
                                                                        \
        /* clean up, and wait for everyone to finish */                 \
        *pSync = SHMEM_SYNC_VALUE;                                      \
        shmemc_barrier(start, logstride, size, pSync);                  \
    }

SHMEM_COLLECT(32, 4)
SHMEM_COLLECT(64, 8)
