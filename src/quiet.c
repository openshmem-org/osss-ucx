/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"

#include "shmem_mutex.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_quiet = pshmem_ctx_quiet
#define shmem_ctx_quiet pshmem_ctx_quiet

#pragma weak shmem_ctx_fence = pshmem_ctx_fence
#define shmem_ctx_fence pshmem_ctx_fence
#endif /* ENABLE_PSHMEM */

void
shmem_ctx_quiet(shmem_ctx_t ctx)
{
    SHMEMT_MUTEX_PROTECT(shmemc_ctx_quiet(ctx));
}

void
shmem_ctx_fence(shmem_ctx_t ctx)
{
    SHMEMT_MUTEX_PROTECT(shmemc_ctx_fence(ctx));
}

#ifdef ENABLE_PSHMEM
#pragma weak shmem_quiet = pshmem_quiet
#define shmem_quiet pshmem_quiet

#pragma weak shmem_fence = pshmem_fence
#define shmem_fence pshmem_fence
#endif /* ENABLE_PSHMEM */

void
shmem_quiet(void)
{
    SHMEMT_MUTEX_PROTECT(shmemc_ctx_quiet(SHMEM_CTX_DEFAULT));
}

void
shmem_fence(void)
{
    SHMEMT_MUTEX_PROTECT(shmemc_ctx_fence(SHMEM_CTX_DEFAULT));
}
