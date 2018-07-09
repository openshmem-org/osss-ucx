/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"

#include "state.h"
#include "shmemu.h"
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
                         int PE_start, int logPE_stride, int PE_size,   \
                         long *pSync)                                   \
    {                                                                   \
        SHMEMU_CHECK_INIT();                                            \
        SHMEMU_CHECK_PE_ARG_RANGE(PE_start, 4);                         \
        SHMEMU_CHECK_SYMMETRIC(target, 1);                              \
        SHMEMU_CHECK_SYMMETRIC(source, 2);                              \
        SHMEMU_CHECK_SYMMETRIC(pSync, 7);                               \
                                                                        \
        SHMEMT_MUTEX_PROTECT(shmemc_collect##_bits(target, source,      \
                                                   nelems,              \
                                                   PE_start,            \
                                                   logPE_stride,        \
                                                   PE_size,             \
                                                   pSync)               \
                             );                                         \
    }

SHMEM_COLLECT(32, 4)
SHMEM_COLLECT(64, 8)
