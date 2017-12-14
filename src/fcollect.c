/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "state.h"
#include "shmemc.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_fcollect32 = pshmem_fcollect32
#define shmem_fcollect32 pshmem_fcollect32

#pragma weak shmem_fcollect64 = pshmem_fcollect64
#define shmem_fcollect64 pshmem_fcollect64
#endif /* ENABLE_PSHMEM */


/**
 * Collective concatenation of 32/64-bit data from participating PEs
 * into a target array on all those PEs
 */

#define SHMEM_FCOLLECT(_bits, _bytes)                                   \
    void                                                                \
    shmem_fcollect##_bits(void *target, const void *source,             \
                          size_t nelems,                                \
                          int PE_start, int logPE_stride,               \
                          int PE_size,                                  \
                          long *pSync)                                  \
    {                                                                   \
        shmemc_fcollect##_bits(target, source,                          \
                               nelems,                                  \
                               PE_start, logPE_stride, PE_size,         \
                               pSync);                                  \
    }

SHMEM_FCOLLECT(32, 4)
SHMEM_FCOLLECT(64, 8)
