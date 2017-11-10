/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_quiet = pshmem_ctx_quiet
#define shmem_ctx_quiet pshmem_ctx_quiet

#pragma weak shmem_ctx_fence = pshmem_ctx_fence
#define shmem_ctx_fence pshmem_ctx_fence
#endif /* ENABLE_PSHMEM */

void
shmem_ctx_quiet(shmem_ctx_t ctx)
{
    shmemc_ctx_quiet(ctx);
}

void
shmem_ctx_fence(shmem_ctx_t ctx)
{
    shmemc_ctx_fence(ctx);
}
