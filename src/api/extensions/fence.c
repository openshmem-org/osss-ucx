/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"
#include "shmem_mutex.h"
#include "shmemx.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmemx_fence_test = pshmemx_fence_test
#define shmemx_fence_test pshmemx_fence_test
#endif /* ENABLE_PSHMEM */

int
shmemx_ctx_fence_test(shmem_ctx_t ctx)
{
    int s;

    SHMEMT_MUTEX_NOPROTECT(s = shmemc_ctx_fence_test(ctx));

    logger(LOG_FENCE,
           "%s(ctx=%lu) -> %d",
           __func__,
           shmemc_context_id(ctx), s
           );

    return s;
}

int
shmemx_fence_test(void)
{
    int s;

    SHMEMT_MUTEX_NOPROTECT(s = shmemc_ctx_fence_test(SHMEM_CTX_DEFAULT));

    return s;
}

#ifdef ENABLE_PSHMEM
#pragma weak shmemx_pe_fence = pshmemx_pe_fence
#define shmemx_pe_fence pshmemx_pe_fence
#endif /* ENABLE_PSHMEM */

int
shmemx_pe_fence(shmem_ctx_t ctx, int pe)
{
    int s;

    NO_WARN_UNUSED(pe);

    SHMEMT_MUTEX_NOPROTECT(s = shmemc_ctx_fence_test(ctx));

    logger(LOG_FENCE,
           "%s(ctx=%lu) -> %d",
           __func__,
           shmemc_context_id(ctx), s
           );

    return s;
}
