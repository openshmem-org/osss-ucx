/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"
#include "shmem_mutex.h"

/*
 * Fence
 */

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_fence = pshmem_ctx_fence
#define shmem_ctx_fence pshmem_ctx_fence
#endif /* ENABLE_PSHMEM */

void
shmem_ctx_fence(shmem_ctx_t ctx)
{
    logger(LOG_FENCE, "%s(ctx=%lu)", __func__, shmemc_context_id(ctx));

    SHMEMT_MUTEX_NOPROTECT(shmemc_ctx_fence(ctx));
}

#ifdef ENABLE_PSHMEM
#pragma weak shmem_fence = pshmem_fence
#define shmem_fence pshmem_fence
#endif /* ENABLE_PSHMEM */

void
shmem_fence(void)
{
    logger(LOG_FENCE, "%s()", __func__);

    SHMEMT_MUTEX_NOPROTECT(shmemc_ctx_fence(SHMEM_CTX_DEFAULT));
}

/*
 * Quiet
 */

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_quiet = pshmem_ctx_quiet
#define shmem_ctx_quiet pshmem_ctx_quiet
#endif /* ENABLE_PSHMEM */

void
shmem_ctx_quiet(shmem_ctx_t ctx)
{
    logger(LOG_QUIET, "%s(ctx=%lu)", __func__, shmemc_context_id(ctx));

    SHMEMT_MUTEX_NOPROTECT(shmemc_ctx_quiet(ctx));
}

#ifdef ENABLE_PSHMEM
#pragma weak shmem_quiet = pshmem_quiet
#define shmem_quiet pshmem_quiet
#endif /* ENABLE_PSHMEM */

void
shmem_quiet(void)
{
    logger(LOG_QUIET, "%s()", __func__);

    SHMEMT_MUTEX_NOPROTECT(shmemc_ctx_quiet(SHMEM_CTX_DEFAULT));
}
