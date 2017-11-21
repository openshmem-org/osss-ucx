/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "thispe.h"
#include "state.h"
#include "shmemc.h"

#include "shmem/defs.h"

#include <stdlib.h>
#include <assert.h>

#include <ucp/api/ucp.h>

/*
 * give each new context an ID
 */
static unsigned long id_track = 0;

/*
 * create new context
 */

int
shmemc_context_create(long options, shmemc_context_h *ctxp)
{
#if 0
    ucp_worker_params_t wkpm;
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

    /* need to adjust mode depending on above */
    wkpm.field_mask  = UCP_WORKER_PARAM_FIELD_THREAD_MODE;
    wkpm.thread_mode = UCS_THREAD_MODE_SINGLE;

    s = ucp_worker_create(proc.comms.ctxt,
                          &wkpm,
                          &newone->wrkr);
    assert(s == UCS_OK);

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
            shmemc_ctx_quiet(ctx->wrkr);

            ucp_worker_destroy(ctx->wrkr);
        }
        free(ctx);
    }
#endif
}
