/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "state.h"
#include "shmemc.h"
#include "shmemu.h"
#include "boolean.h"

#include "ucx/api.h"

#include "shmem/defs.h"

#include "klib/klist.h"

#include <stdlib.h>

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

            if (shmemu_unlikely(proc.comms.ctxts == NULL)) {
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
context_register(shmemc_context_h ch)
{
    return register_fn(ch);
}

/*
 * remove context from PE state
 */

inline static void
context_deregister(shmemc_context_h ch)
{
    proc.comms.ctxts[ch->id] = NULL;

    /* this one is re-usable */
    *kl_pushp(freelist, fl) = ch->id;

    logger(LOG_CONTEXTS, "context #%lu can be reused", ch->id);
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
    if (shmemu_likely(n == 0)) {
        if (shmemu_unlikely(context_register(ch) != 0)) {
            return 1;
            /* NOT REACHED */
        }
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
    if (shmemu_unlikely(ctx == SHMEM_CTX_INVALID)) {
        logger(LOG_CONTEXTS,
               "ignoring attempt to destroy invalid context");
        return;
        /* NOT REACHED */
    }
    else if (shmemu_unlikely(ctx == SHMEM_CTX_DEFAULT)) {
        logger(LOG_FATAL,
               "cannot destroy the default context"
               );
        /* NOT REACHED */
    }
    else {
        shmemc_context_h ch = (shmemc_context_h) ctx;

        /* spec 1.4 ++ has implicit quiet for storable contexts */
        shmemc_ctx_quiet(ch);

        shmemc_context_cleanup(ch);

        context_deregister(ch);
    }
}

/*
 * return the id of a context (used for logging)
 */

size_t
shmemc_context_id(shmem_ctx_t ctx)
{
    const shmemc_context_h ch = (shmemc_context_h) ctx;

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
shmemc_init_default_context(void)
{
    shmemc_context_h ch = &shmemc_default_context;
    int n;
    const long default_options = 0L;

    n = shmemc_context_fill(default_options, ch);
    if (shmemu_unlikely(n != 0)) {
        return 1;
        /* NOT REACHED */
    }

    return shmemc_context_default_set_info(ch);
}
