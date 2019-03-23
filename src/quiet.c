/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"
#include "shmem_mutex.h"
#include "shmem/api.h"
#include "shmemx.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_quiet = pshmem_ctx_quiet
#define shmem_ctx_quiet pshmem_ctx_quiet
#endif /* ENABLE_PSHMEM */

void
shmem_ctx_quiet(shmem_ctx_t ctx)
{
    logger(LOG_QUIET, "%s(ctx=%lu)", __func__, shmemc_context_id(ctx));

    SHMEMT_MUTEX_PROTECT(shmemc_ctx_quiet(ctx));
}

#ifdef ENABLE_PSHMEM
#pragma weak shmem_quiet = pshmem_quiet
#define shmem_quiet pshmem_quiet
#endif /* ENABLE_PSHMEM */

void
shmem_quiet(void)
{
    logger(LOG_QUIET, "%s()", __func__);

    SHMEMT_MUTEX_PROTECT(shmemc_ctx_quiet(SHMEM_CTX_DEFAULT));
}

#ifdef ENABLE_EXPERIMENTAL

#ifdef ENABLE_PSHMEM
#pragma weak shmemx_quiet_test = pshmemx_quiet_test
#define shmemx_quiet_test pshmemx_quiet_test
#endif /* ENABLE_PSHMEM */

int
shmemx_ctx_quiet_test(shmem_ctx_t ctx)
{
    int s;

    SHMEMT_MUTEX_PROTECT(s = shmemc_ctx_quiet_test(ctx));

    logger(LOG_QUIET,
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

#endif  /* ENABLE_EXPERIMENTAL */
