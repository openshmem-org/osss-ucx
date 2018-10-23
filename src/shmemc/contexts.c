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

static const size_t SPILL_BLOCK = 64;

static size_t spill_ctxt = 0;

/*
 * manage free list of re-usable contexts
 */

#define __size_t_free(x)

KLIST_INIT(freelist, size_t, __size_t_free)

static klist_t(freelist) *fl = NULL;

/*
 * Register and de-register contexts
 *
 * first call performs initialization, then reroutes to real work
 */

static size_t get_usable_context_boot(bool *reused);
static size_t get_usable_context_run(bool *reused);

static size_t (*get_usable_context)(bool *reused)
    = get_usable_context_boot;

static size_t
get_usable_context_boot(bool *reused)
{
    fl = kl_init(freelist);

    get_usable_context = get_usable_context_run;

    return get_usable_context(reused);
}

static size_t
get_usable_context_run(bool *reused)
{
    size_t idx;
    kliter_t(freelist) *head = kl_begin(fl);

    if (head == kl_end(fl)) {   /* nothing in free list */
        idx = proc.comms.nctxts;

        /* if out of space, grab some more slots */
        if (idx == spill_ctxt) {
            spill_ctxt += SPILL_BLOCK;

            proc.comms.ctxts = (shmemc_context_h *)
                realloc(proc.comms.ctxts,
                        spill_ctxt * sizeof(*(proc.comms.ctxts)));

            if (shmemu_unlikely(proc.comms.ctxts == NULL)) {
                logger(LOG_FATAL,
                       "can't allocate more memory for context freelist");
                /* NOT REACHED */
            }
        }

        /* allocate context in current slot */
        proc.comms.ctxts[idx] =
            (shmemc_context_h) malloc(sizeof(shmemc_context_t));
        if (shmemu_unlikely(proc.comms.ctxts[idx] == NULL)) {
            logger(LOG_FATAL,
                   "unable to allocate memory for new context");
            /* NOT REACHED */
        }

        proc.comms.nctxts += 1; /* for next one */
        *reused = false;
    }
    else {                /* grab & remove the head of the freelist */
        idx = kl_val(head);
        kl_shift(freelist, fl, NULL);
        logger(LOG_CONTEXTS,
               "reclaiming context #%lu from free list",
               idx);
        *reused = true;
    }
    return idx;
}

/*
 * add/remove context in PE state
 */

inline static void
context_register(shmemc_context_h ch)
{
    logger(LOG_CONTEXTS, "using context #%lu", ch->id);
}

inline static void
context_deregister(shmemc_context_h ch)
{
    /* this one is re-usable */
    *kl_pushp(freelist, fl) = ch->id;

    logger(LOG_CONTEXTS, "context #%lu can be reused", ch->id);
}

/*
 * fill in context
 *
 * Return 0 on success, non-0 on failure
 */

static void
shmemc_context_set_options(long options, shmemc_context_h ch)
{
    ch->attr.serialized = options & SHMEM_CTX_SERIALIZED;
    ch->attr.private    = options & SHMEM_CTX_PRIVATE;
    ch->attr.nostore    = options & SHMEM_CTX_NOSTORE;
}

/*
 * create new context
 *
 * Return 0 on success, non-zero on failure
 */

int
shmemc_context_create(long options, shmem_ctx_t *ctxp)
{
    bool reuse;
    const size_t idx = get_usable_context(&reuse);
    shmemc_context_h ch = proc.comms.ctxts[idx];

    /* set SHMEM context behavior */
    shmemc_context_set_options(options, ch);

    /* is this reclaimed from free list or do we have to set up? */
    if (! reuse) {
        const int ret = shmemc_context_progress(ch);

        if (shmemu_unlikely(ret != 0)) {
            free(ch);
            return ret;
            /* NOT REACHED */
        }
    }

    ch->creator_thread = threadwrap_thread_id();
    ch->id = idx;

    context_register(ch);

    *ctxp = (shmem_ctx_t) ch;

    return 0;
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
 * Return 0 if successful, non-0 otherwise
 */
shmemc_context_t shmemc_default_context;

int
shmemc_init_default_context(void)
{
    shmemc_context_h ch = &shmemc_default_context;

    shmemc_context_set_options(0L, ch);
    shmemc_context_progress(ch);

    return shmemc_context_default_set_info(ch);
}
