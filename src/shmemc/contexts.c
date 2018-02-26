/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "state.h"
#include "shmemc.h"
#include "shmemu.h"

#include "shmem/defs.h"

#include <stdlib.h>
#include <assert.h>

#include <ucp/api/ucp.h>

/*
 * insert and remove contexts in PE state
 */

static const size_t context_block = 8;

static size_t top_ctxt = 0;

inline static void
register_context(shmemc_context_h ch)
{
    if (proc.comms.nctxts == top_ctxt) {
        top_ctxt += context_block;
        proc.comms.ctxts =
            (shmemc_context_h *) realloc(proc.comms.ctxts, top_ctxt);
        assert(proc.comms.ctxts != NULL);
    }

    ch->id = proc.comms.nctxts;
    proc.comms.ctxts[proc.comms.nctxts] = ch;
    proc.comms.nctxts += 1;
}

inline static void
deregister_context(shmemc_context_h ch)
{
    /* TODO: freed at teardown, should better maintain a pool */
    proc.comms.ctxts[ch->id] = NULL;
}

/*
 * create new context
 */

int
shmemc_context_create(long options, shmemc_context_h *ctxp)
{
    ucs_status_t s;
    shmemc_context_h newone;
    ucp_worker_params_t wkpm;

    newone = (shmemc_context_h) malloc(sizeof(*newone));
    if (newone == NULL) {
        return 1;               /* fail if no memory free for new context */
        /* NOT REACHED */
    }

    newone->serialized = options & SHMEM_CTX_SERIALIZED;
    newone->private    = options & SHMEM_CTX_PRIVATE;
    newone->nostore    = options & SHMEM_CTX_NOSTORE;

    wkpm.field_mask  = UCP_WORKER_PARAM_FIELD_THREAD_MODE;
    if (newone->serialized) {
        wkpm.thread_mode = UCS_THREAD_MODE_SERIALIZED;
    }
    else if (newone->private) {
        wkpm.thread_mode = UCS_THREAD_MODE_SINGLE;
    }
    else {
        wkpm.thread_mode = UCS_THREAD_MODE_MULTI;
    }
    s = ucp_worker_create(proc.comms.ucx_ctxt, &wkpm, &(newone->w));
    assert(s == UCS_OK);

    register_context(newone);

    *ctxp = newone;             /* handle back to caller */

    return 0;
}

/*
 * zap existing context.  Illegal to zap default context, so I will
 * interpret that as "to be avoided" and continue
 */

void
shmemc_context_destroy(shmemc_context_h ctx)
{
    if (ctx == NULL) {
        return;
        /* NOT REACHED */
    }
    else if (ctx == SHMEM_CTX_DEFAULT) {
        logger(LOG_CONTEXT,
               "it is illegal to destroy the default context, ignoring...");
        return;
        /* NOT REACHED */
    }
    else {
        /*
         * spec 1.4 requires implicit quiet on destroy for storable
         * contexts
         */
        shmemc_ctx_quiet(ctx);

        ucp_worker_destroy(ctx->w);

        deregister_context(ctx);
    }
}

/*
 * the first, default, context gets a special SHMEM handle, also needs
 * address exchange through PMI, so we give it its own routine
 */

int
shmemc_create_default_context(shmem_ctx_t *ctx_p)
{
    shmemc_context_h ctx;
    int n;
    ucs_status_t s;
    ucp_address_t *addr;
    size_t len;

    n = shmemc_context_create(0, &ctx);
    if (n != 0) {
        return 1;
        /* NOT REACHED */
    }

    /* get address for remote access to worker */
    s = ucp_worker_get_address(ctx->w, &addr, &len);
    if (s != UCS_OK) {
        return 1;
        /* NOT REACHED */
    }

    /* handle back to caller */
    *ctx_p = (shmem_ctx_t) ctx;

    proc.comms.xchg_wrkr_info[proc.rank].addr = addr;
    proc.comms.xchg_wrkr_info[proc.rank].len = len;

    return 0;
}
