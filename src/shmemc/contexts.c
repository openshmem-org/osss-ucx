/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "thispe.h"
#include "state.h"
#include "shmemc.h"

#include "shmem/defs.h"

#include <stdlib.h>

#include <ucp/api/ucp.h>

#if 0
static unsigned long id_track = 0; /* give each new context an ID */
#endif

/*
 * create new context
 */

int
shmemc_context_create(long options, shmemc_context_h *ctxp)
{
#if 0
    ucs_status_t s;
    shmemc_context_h newone;

    newone = (shmemc_context_h) malloc(sizeof(*newone));
    if (newone == NULL) {
        return 1;               /* fail if no memory free for new context */
    }

    newone->serialized = SHMEM_TEST_BIT(options, SHMEM_CTX_SERIALIZED);
    newone->private    = SHMEM_TEST_BIT(options, SHMEM_CTX_PRIVATE);
    newone->nostore    = SHMEM_TEST_BIT(options, SHMEM_CTX_NOSTORE);
    newone->id         = id_track ++;

    /* set up new endpoints */

    *ctxp = newone;
#endif
    return 0;
}

/*
 * zap existing context
 */

void
shmemc_context_destroy(shmemc_context_h ctx)
{
#if 0
    if (ctx != NULL) {
        if (ctx->wrkr != NULL) {
            /* spec 1.4 requires implicit quiet on destroy */
            shmemc_ctx_quiet(ctx, proc.comms.wrkr);

            /* kill endpoints */
        }
        free(ctx);
    }
#endif
}
