/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem/api.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_create = pshmem_ctx_create
#define shmem_ctx_create pshmem_ctx_create
#pragma weak shmem_ctx_destroy = pshmem_ctx_destroy
#define shmem_ctx_destroy pshmem_ctx_destroy
#endif /* ENABLE_PSHMEM */

/*
 * link-time constants
 */
shmem_ctx_t SHMEM_CTX_DEFAULT    = 0;
shmem_ctx_t SHMEM_CTX_SERIALIZED = SHMEM_BIT_SET(0);
shmem_ctx_t SHMEM_CTX_PRIVATE    = SHMEM_BIT_SET(1);
shmem_ctx_t SHMEM_CTX_NOSTORE    = SHMEM_BIT_SET(2);

/*
 * just hand back the default context for now
 */

int
shmem_ctx_create(long options, shmem_ctx_t *ctxp)
{
    *ctxp = SHMEM_CTX_DEFAULT;

    return 1;
}

/*
 * no-op for now
 */
void
shmem_ctx_destroy(shmem_ctx_t ctx)
{
}

/*
 * implementation currently missing - in progress
 *
 * decls in shmem.h
 */
