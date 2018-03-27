/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"

#include "thispe.h"
#include "shmemu.h"
#include "shmemc.h"
#include "shmem/defs.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_create = pshmem_ctx_create
#define shmem_ctx_create pshmem_ctx_create
#pragma weak shmem_ctx_destroy = pshmem_ctx_destroy
#define shmem_ctx_destroy pshmem_ctx_destroy
#endif /* ENABLE_PSHMEM */

/*
 * link-time constants
 */
shmem_ctx_t SHMEM_CTX_DEFAULT = NULL;

/*
 * create new context with supplied options
 */

int
shmem_ctx_create(long options, shmem_ctx_t *ctxp)
{
    int s;

    SHMEMU_CHECK_INIT();

    SHMEMT_MUTEX_PROTECT(s = shmemc_context_create(options, ctxp));

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
}

#ifdef ENABLE_EXPERIMENTAL

/*
 * tell OpenSHMEM there's region of communication coming up
 */

void
shmemx_ctx_start_session(shmem_ctx_t ctx)
{
    SHMEMU_CHECK_INIT();
}

void
shmemx_ctx_end_session(shmem_ctx_t ctx)
{
    SHMEMU_CHECK_INIT();
}

#endif  /* ENABLE_EXPERIMENTAL */
