/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ptr = pshmem_ptr
#define shmem_ptr pshmem_ptr
#endif /* ENABLE_PSHMEM */

void *
shmem_ptr(const void *target, int pe)
{
    void *rw = shmemc_ctx_ptr(SHMEM_CTX_DEFAULT, target, pe);

    logger(LOG_MEMORY,
           "%s(target=%p, pe=%d) -> %p",
           __func__,
           target, pe, rw
           );

    return rw;
}
