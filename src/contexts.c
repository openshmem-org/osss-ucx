/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"

#include "thispe.h"
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

    SHMEMT_MUTEX_PROTECT(s = shmemc_context_create(options,
                                                   (shmemc_context_h *) ctxp));

    return s;
}

/*
 * zap context
 */

void
shmem_ctx_destroy(shmem_ctx_t ctx)
{
    SHMEMT_MUTEX_PROTECT(shmemc_context_destroy((shmemc_context_h) ctx));
}

#ifdef ENABLE_EXPERIMENTAL

/*
 * tell OpenSHMEM there's region of communication coming up
 */

void
shmemx_ctx_start_session(shmem_ctx_t ctx)
{
}

void
shmemx_ctx_end_session(shmem_ctx_t ctx)
{
}

#endif  /* ENABLE_EXPERIMENTAL */
