/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "state.h"
#include "shmemc.h"
#include "shmemu.h"
#include "boolean.h"
#include "threading.h"

#include "shmem/defs.h"

#include "klib/klist.h"

#include <stdlib.h>

#include <ucp/api/ucp.h>

/*
 * how many more to allocate when we run out (magic number)
 */

static const size_t SPILL_BLOCK = 16;

static size_t spill_ctxt = 0;

/*
 * manage free list of re-usable contexts
 */

#define __int_free(x)

KLIST_INIT(freelist, int, __int_free)

static klist_t(freelist) *fl = NULL;

/*
 * first call performs initialization, then reroutes to real work
 */

static int register_context_boot(shmemc_context_h ch);
static int register_context_run(shmemc_context_h ch);

static int (*register_fn)(shmemc_context_h ch) = register_context_boot;

static int
register_context_boot(shmemc_context_h ch)
{
    fl = kl_init(freelist);

    register_fn = register_context_run;

    return register_fn(ch);
}

static int
register_context_run(shmemc_context_h ch)
{
    size_t next;
    kliter_t(freelist) *head = kl_begin(fl);

    if (head == kl_end(fl)) {   /* nothing in free list */
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
    else {                /* grab & remove the head of the freelist */
        next = kl_val(head);
        kl_shift(freelist, fl, NULL);
        logger(LOG_CONTEXTS,
               "reclaiming context #%lu from free list",
               next);
    }

    /* record this new context */
    ch->id = next;
    proc.comms.ctxts[next] = ch;

    return 0;
}

/*
 * insert context into PE state
 *
 * Return 0 on success, 1 on failure
 */

inline static int
register_context(shmemc_context_h ch)
{
    return register_fn(ch);
}

/*
 * remove context from PE state
 */

inline static void
deregister_context(shmemc_context_h ch)
{
    proc.comms.ctxts[ch->id] = NULL;
    /* this one is re-usable */
    *kl_pushp(freelist, fl) = ch->id;
    logger(LOG_CONTEXTS,
           "context #%lu can be reused",
           ch->id);
}

/*
 * create new context
 *
 * Return 0 on success, 1 on failure
 */

int
shmemc_context_create(long options, shmem_ctx_t *ctxp)
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
        goto cleanup;
        /* NOT REACHED */
    }

    if (register_context(newone) != 0) {
        goto cleanup;
        /* NOT REACHED */
    }

    newone->creator_thread = shmemc_thread_id();

    /* handle back to caller */
    *ctxp = (shmem_ctx_t *) newone;

    return 0;
    /* NOT REACHED */

 cleanup:
    free(newone);
    return 1;
}

/*
 * zap existing context.  Illegal to zap default context, so I will
 * interpret that as "to be avoided" and continue
 */

void
shmemc_context_destroy(shmem_ctx_t ctx)
{
    if (ctx == NULL) {
        return;
        /* NOT REACHED */
    }
    else if (ctx == SHMEM_CTX_DEFAULT) {
        logger(LOG_CONTEXTS,
               "it is illegal to destroy the default context, ignoring...");
        return;
        /* NOT REACHED */
    }
    else {
        shmemc_context_h ch = (shmemc_context_h) ctx;

        /*
         * spec 1.4 requires implicit quiet on destroy for storable
         * contexts
         */
        shmemc_ctx_quiet(ch);

        ucp_worker_destroy(ch->w);

        deregister_context(ch);
    }
}

/*
 * return the id of a context (used for logging)
 */

unsigned long
shmemc_context_id(shmem_ctx_t ctx)
{
    shmemc_context_h ch = (shmemc_context_h) ctx;

    return ch->id;
}

/*
 * the first, default, context gets a special SHMEM handle, also needs
 * address exchange through PMI, so we give it its own routine
 */

int
shmemc_create_default_context(shmem_ctx_t *ctxp)
{
    shmem_ctx_t ctx_def;
    shmemc_context_h ch;
    int n;
    ucs_status_t s;
    ucp_address_t *addr;
    size_t len;
    const long default_options = 0L;

    n = shmemc_context_create(default_options, &ctx_def);
    if (n != 0) {
        return 1;
        /* NOT REACHED */
    }

    ch = (shmemc_context_h) ctx_def;

    /* get address for remote access to worker */
    s = ucp_worker_get_address(ch->w, &addr, &len);
    if (s != UCS_OK) {
        return 1;
        /* NOT REACHED */
    }

    proc.comms.xchg_wrkr_info[proc.rank].addr = addr;
    proc.comms.xchg_wrkr_info[proc.rank].len = len;

    /* handle back to caller */
    *ctxp = ctx_def;

    return 0;
}
