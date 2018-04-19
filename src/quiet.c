/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
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
    logger(LOG_FENCE,
           "%s(ctx=%lu)",
           __func__,
           shmemc_context_id(ctx)
           );

    SHMEMT_MUTEX_PROTECT(shmemc_ctx_quiet(ctx));
}

void
shmem_ctx_fence(shmem_ctx_t ctx)
{
    logger(LOG_FENCE,
           "%s(ctx=%lu)",
           __func__,
           shmemc_context_id(ctx)
           );

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
    logger(LOG_FENCE,
           "%s()",
           __func__
           );

    SHMEMT_MUTEX_PROTECT(shmemc_ctx_quiet(SHMEM_CTX_DEFAULT));
}

void
shmem_fence(void)
{
    logger(LOG_FENCE,
           "%s()",
           __func__
           );

    SHMEMT_MUTEX_PROTECT(shmemc_ctx_fence(SHMEM_CTX_DEFAULT));
}

#ifdef ENABLE_EXPERIMENTAL

#ifdef ENABLE_PSHMEM
#pragma weak shmemx_quiet_test = pshmemx_quiet_test
#define shmemx_quiet_test pshmemx_quiet_test

#pragma weak shmemx_fence_test = pshmemx_fence_test
#define shmemx_fence_test pshmemx_fence_test
#endif /* ENABLE_PSHMEM */

int
shmemx_ctx_quiet_test(shmem_ctx_t ctx)
{
    int s;

    SHMEMT_MUTEX_PROTECT(s = shmemc_ctx_quiet_test(ctx));

    logger(LOG_FENCE,
           "%s(ctx=%lu) -> %d",
           __func__,
           shmemc_context_id(ctx), s
           );

    return s;
}

int
shmemx_ctx_fence_test(shmem_ctx_t ctx)
{
    int s;

    SHMEMT_MUTEX_PROTECT(s = shmemc_ctx_fence_test(ctx));

    logger(LOG_FENCE,
           "%s(ctx=%lu) -> %d",
           __func__,
           shmemc_context_id(ctx), s
           );

    return s;
}

int
shmemx_quiet_test(void)
{
    int s;

    SHMEMT_MUTEX_PROTECT(s = shmemc_ctx_quiet_test(SHMEM_CTX_DEFAULT));

    return s;
}

int
shmemx_fence_test(void)
{
    int s;

    SHMEMT_MUTEX_PROTECT(s = shmemc_ctx_fence_test(SHMEM_CTX_DEFAULT));

    return s;
}

#endif  /* ENABLE_EXPERIMENTAL */
