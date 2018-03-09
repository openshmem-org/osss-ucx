/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "state.h"
#include "shmemc.h"
#include "shmemu.h"
#include "boolean.h"

#include "shmem/defs.h"

#include <stdlib.h>

#include <ucp/api/ucp.h>

/*
 * how many more to allocate when we run out (magic number)
 */

static const size_t SPILL_BLOCK = 16;

static size_t spill_ctxt = 0;

/*
 * poor man's free-list
 */

static bool can_reclaim = false;
static size_t reclaim;

/*
 * insert context into PE state
 *
 * Return 0 on success, 1 on failure
 */

inline static int
register_context(shmemc_context_h ch)
{
    size_t next;

    if (can_reclaim) {
        next = reclaim;
        can_reclaim = false;
    }
    else {
        next = proc.comms.nctxts;

        /* if out of space, grab some more */
        if (next == spill_ctxt) {
            spill_ctxt += SPILL_BLOCK;

            proc.comms.ctxts = (shmemc_context_h *)
                realloc(proc.comms.ctxts,
                        spill_ctxt * sizeof(*(proc.comms.ctxts)));

            if (proc.comms.ctxts == NULL) {
                return 1;
                /* NOT REACHED */
            }
        }

        /* and for next one */
        proc.comms.nctxts += 1;
    }

    /* record this new context */
    ch->id = next;
    proc.comms.ctxts[next] = ch;

    return 0;
}

/*
 * remove context from PE state
 */

inline static void
deregister_context(shmemc_context_h ch)
{
    proc.comms.ctxts[ch->id] = NULL;
    /* this one is re-usable */
    can_reclaim = true;
    reclaim = ch->id;
}

/*
 * create new context
 *
 * Return 0 on success, 1 on failure
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

    newone->attr.serialized = options & SHMEM_CTX_SERIALIZED;
    newone->attr.private    = options & SHMEM_CTX_PRIVATE;
    newone->attr.nostore    = options & SHMEM_CTX_NOSTORE;

    wkpm.field_mask  = UCP_WORKER_PARAM_FIELD_THREAD_MODE;

    if (newone->attr.serialized) {
        wkpm.thread_mode = UCS_THREAD_MODE_SERIALIZED;
    }
    else if (newone->attr.private) {
        wkpm.thread_mode = UCS_THREAD_MODE_SINGLE;
    }
    else {
        wkpm.thread_mode = UCS_THREAD_MODE_MULTI;
    }

    s = ucp_worker_create(proc.comms.ucx_ctxt, &wkpm, &(newone->w));
    if (s != UCS_OK) {
        goto bail;
        /* NOT REACHED */
    }

    if (register_context(newone) != 0) {
        goto bail;
        /* NOT REACHED */
    }

    *ctxp = newone;             /* handle back to caller */

    return 0;
    /* NOT REACHED */

 bail:
    free(newone);
    return 1;
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
    const long default_options = 0L;

    n = shmemc_context_create(default_options, &ctx);
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
