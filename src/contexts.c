/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"
#include "shmem/defs.h"

#include <stdlib.h>

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_create = pshmem_ctx_create
#define shmem_ctx_create pshmem_ctx_create
#pragma weak shmem_ctx_destroy = pshmem_ctx_destroy
#define shmem_ctx_destroy pshmem_ctx_destroy
#endif /* ENABLE_PSHMEM */


int
shmem_ctx_create(long options, shmem_ctx_t *ctxp)
{
    *ctxp = 1;

    return 1;
}

void
shmem_ctx_destroy(shmem_ctx_t ctx)
{
}
