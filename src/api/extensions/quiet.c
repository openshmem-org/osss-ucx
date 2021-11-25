/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"
#include "shmem_mutex.h"
#include "shmemx.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmemx_quiet_test = pshmemx_quiet_test
#define shmemx_quiet_test pshmemx_quiet_test
#endif /* ENABLE_PSHMEM */

int
shmemx_ctx_quiet_test(shmem_ctx_t ctx)
{
    int s;

    SHMEMT_MUTEX_NOPROTECT(s = shmemc_ctx_quiet_test(ctx));

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

    SHMEMT_MUTEX_NOPROTECT(s = shmemc_ctx_quiet_test(SHMEM_CTX_DEFAULT));

    return s;
}

#ifdef ENABLE_PSHMEM
#pragma weak shmemx_pe_quiet = pshmemx_pe_quiet
#define shmemx_pe_quiet pshmemx_pe_quiet
#endif /* ENABLE_PSHMEM */

int
shmemx_pe_quiet(shmem_ctx_t ctx, int pe)
{
    int s;

    NO_WARN_UNUSED(pe);

    SHMEMT_MUTEX_NOPROTECT(s = shmemc_ctx_quiet_test(ctx));

    logger(LOG_QUIET,
           "%s(ctx=%lu) -> %d",
           __func__,
           shmemc_context_id(ctx), s
           );

    return s;
}
