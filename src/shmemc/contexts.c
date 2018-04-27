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
 * Register and de-register contexts
 *
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
 * fill in context
 *
 * Return 0 on success, 1 on failure
 */

inline static int
shmemc_context_fill(long options, shmemc_context_h ch)
{
    ucs_status_t s;
    ucp_worker_params_t wkpm;

    ch->attr.serialized = options & SHMEM_CTX_SERIALIZED;
    ch->attr.private    = options & SHMEM_CTX_PRIVATE;
    ch->attr.nostore    = options & SHMEM_CTX_NOSTORE;

    wkpm.field_mask  = UCP_WORKER_PARAM_FIELD_THREAD_MODE;

    if (ch->attr.serialized) {
        wkpm.thread_mode = UCS_THREAD_MODE_SERIALIZED;
    }
    else if (ch->attr.private) {
        wkpm.thread_mode = UCS_THREAD_MODE_SINGLE;
    }
    else {
        wkpm.thread_mode = UCS_THREAD_MODE_MULTI;
    }

    s = ucp_worker_create(proc.comms.ucx_ctxt, &wkpm, &(ch->w));
    if (s != UCS_OK) {
        return 1;
        /* NOT REACHED */
    }

    if (register_context(ch) != 0) {
        return 1;
        /* NOT REACHED */
    }

    ch->creator_thread = shmemc_thread_id();

    return 0;
}

/*
 * create new context
 *
 * Return 0 on success, 1 on failure
 */

int
shmemc_context_create(long options, shmem_ctx_t *ctxp)
{
    int n;
    shmemc_context_h ch;

    ch = (shmemc_context_h) malloc(sizeof(*ch));
    if (ch == NULL) {
        return 1;     /* fail if no memory free for new context */
        /* NOT REACHED */
    }

    n = shmemc_context_fill(options, ch);
    if (n == 0) {
        *ctxp = (shmem_ctx_t) ch;
    }
    else {
        free(ch);
    }

    return n;
}

/*
 * zap existing context.  Illegal to zap default context, so I will
 * interpret that as "to be avoided" and continue
 */

void
shmemc_context_destroy(shmem_ctx_t ctx)
{
    if (ctx != NULL) {
        if (ctx != SHMEM_CTX_DEFAULT) {
            shmemc_context_h ch = (shmemc_context_h) ctx;

            /* spec 1.4 ++ has implicit quiet for storable contexts */
            shmemc_ctx_quiet(ch);

            ucp_worker_destroy(ch->w);

            deregister_context(ch);
        }
        else {
            logger(LOG_FATAL,
                   "cannot destroy the default context"
                   );
        }
    }
    else {
        logger(LOG_FATAL,
               "attempt to destroy a null context"
               );
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
 *
 * Return 0 if successful, 1 otherwise
 */

shmemc_context_t shmemc_default_context;

int
shmemc_create_default_context(void)
{
    shmemc_context_h ch = &shmemc_default_context;
    int n;
    ucs_status_t s;
    ucp_address_t *addr;
    size_t len;
    const long default_options = 0L;

    n = shmemc_context_fill(default_options, ch);
    if (n != 0) {
        return 1;
        /* NOT REACHED */
    }

    /* get address for remote access to worker */
    s = ucp_worker_get_address(ch->w, &addr, &len);
    if (s != UCS_OK) {
        return 1;
        /* NOT REACHED */
    }

    proc.comms.xchg_wrkr_info[proc.rank].addr = addr;
    proc.comms.xchg_wrkr_info[proc.rank].len = len;

    return 0;
}
