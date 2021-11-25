/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"

#include "shmemu.h"
#include "shmemc.h"
#include "shmemx.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_create = pshmem_ctx_create
#define shmem_ctx_create pshmem_ctx_create
#pragma weak shmem_ctx_destroy = pshmem_ctx_destroy
#define shmem_ctx_destroy pshmem_ctx_destroy
#endif /* ENABLE_PSHMEM */

/*
 * the default context is a link-time constant and has to be
 * instantiated at all times
 */

shmem_ctx_t SHMEM_CTX_DEFAULT = (shmem_ctx_t) &shmemc_default_context;

/*
 * create new context with supplied options
 *
 * Return 1 on success, 0 on failure
 */

int
shmem_ctx_create(long options, shmem_ctx_t *ctxp)
{
    int s;

    SHMEMU_CHECK_INIT();

    /* defaults to world team */
    SHMEMT_MUTEX_PROTECT(s = shmemc_context_create(SHMEM_TEAM_WORLD,
                                                   options,
                                                   (shmemc_context_h *) ctxp));

    logger(LOG_CONTEXTS,
           "%s(options=%#lx, ctxp->%p)",
           __func__,
           options, *ctxp
           );

    return s;
}

/*
 * zap context
 */

void
shmem_ctx_destroy(shmem_ctx_t ctx)
{
    SHMEMU_CHECK_INIT();
    SHMEMU_CHECK_SAME_THREAD(ctx);

    SHMEMT_MUTEX_PROTECT(shmemc_context_destroy(ctx));

    logger(LOG_CONTEXTS,
           "%s(ctx=%p)",
           __func__,
           ctx
           );
}

#ifdef ENABLE_EXPERIMENTAL

/*
 * tell OpenSHMEM there's region of communication coming up
 */

void
shmemx_ctx_start_session(shmem_ctx_t ctx)
{
    NO_WARN_UNUSED(ctx);

    SHMEMU_CHECK_INIT();
}

void
shmemx_ctx_end_session(shmem_ctx_t ctx)
{
    NO_WARN_UNUSED(ctx);

    SHMEMU_CHECK_INIT();
}

#endif  /* ENABLE_EXPERIMENTAL */
