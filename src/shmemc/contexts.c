/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "state.h"
#include "shmemc.h"
#include "shmemu.h"
#include "boolean.h"
#include "shmem/defs.h"
#include "../klib/klist.h"
#include "ucx/api.h"

#include <stdlib.h>

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

static size_t get_usable_context_boot(shmemc_team_h th, bool *reused);
static size_t get_usable_context_run(shmemc_team_h th, bool *reused);

static size_t (*get_usable_context)(shmemc_team_h th, bool *reused)
    = get_usable_context_boot;

/*
 * how many more to allocate when we run out
 */

static size_t spill_block;

static size_t spill_ctxt = 0;

inline static shmemc_context_h *
resize_spill_block(shmemc_team_h th, size_t n)
{
    shmemc_context_h *chp =
        (shmemc_context_h *) realloc(th->ctxts,
                                     n * sizeof(*(th->ctxts))
                                     );

    if (chp == NULL) {
        shmemu_fatal("can't allocate %lu bytes for context freelist",
                     (unsigned long) n);
        /* NOT REACHED */
    }

    return chp;
}

/*
 * allocate new context
 */

inline static shmemc_context_h
alloc_freelist_slot(void)
{
    shmemc_context_h ch =
        (shmemc_context_h) malloc(sizeof(shmemc_context_t));

    if (ch == NULL) {
        shmemu_fatal("unable to allocate memory for new context");
        /* NOT REACHED */
    }

    return ch;
}

static size_t
get_usable_context_boot(shmemc_team_h th, bool *reused)
{
    fl = kl_init(freelist);

    /* pre-alloc */
    spill_block = proc.env.prealloc_contexts;
    th->ctxts = resize_spill_block(th, spill_block);

    get_usable_context = get_usable_context_run;

    return get_usable_context(th, reused);
}

static size_t
get_usable_context_run(shmemc_team_h th, bool *reused)
{
    size_t idx;
    kliter_t(freelist) *head = kl_begin(fl);

    if (head == kl_end(fl)) {   /* nothing in free list */
        idx = th->nctxts;

        /* if out of space, grab some more slots */
        if (idx == spill_ctxt) {
            spill_ctxt += spill_block;

            th->ctxts = resize_spill_block(th, spill_ctxt);

            if (th->ctxts == NULL) {
                shmemu_fatal("can't allocate more memory "
                             "for context freelist");
                /* NOT REACHED */
            }
        }

        /* allocate context in current slot */
        th->ctxts[idx] = alloc_freelist_slot();

        ++ th->nctxts;          /* for next one */
        *reused = false;
    }
    else {                /* grab & remove the head of the freelist */
        idx = kl_val(head);
        kl_shift(freelist, fl, NULL);
        logger(LOG_CONTEXTS,
               "reclaiming context #%lu from free list",
               (unsigned long) idx);
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
    NO_WARN_UNUSED(ch);

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
 * fill in context from provided options
 */

inline static void
context_set_options(long options, shmemc_context_h ch)
{
    ch->attr.serialized = options & SHMEM_CTX_SERIALIZED;
    ch->attr.private    = options & SHMEM_CTX_PRIVATE;
    ch->attr.nostore    = options & SHMEM_CTX_NOSTORE;
}

/*
 * allocate space for contexts in team
 */
shmemc_context_h *
shmemc_alloc_contexts(shmemc_team_h th)
{
    if (th->nctxts > 0) {
        return resize_spill_block(th, th->nctxts);
    }
    else {
        return NULL;
    }
}

/*
 * create new context in a team
 *
 * Return 0 on success, non-zero on failure
 */

int
shmemc_context_create(shmemc_team_h th, long options, shmemc_context_h *ctxp)
{
    bool reuse;
    size_t idx;
    shmemc_context_h ch;

    /* identify context to use */
    idx = get_usable_context(th, &reuse);
    ch = th->ctxts[idx];

    /* set SHMEM context behavior */
    context_set_options(options, ch);

    /* is this reclaimed from free list or do we have to set up? */
    if (! reuse) {
        ucs_status_t s;

        const int ret = shmemc_ucx_context_progress(ch);

        if (ret != 0) {
            free(ch);
            return ret;
            /* NOT REACHED */
        }
        shmemc_ucx_make_eps(ch);

        s = shmemc_ucx_worker_wireup(ch);

        if (s != UCS_OK) {
            shmemu_fatal("cannot complete new context worker wireup");
            /* NOT REACHED */
        }
    }

    ch->creator_thread = threadwrap_thread_id();
    ch->id = idx;
    ch->team = th;              /* connect context to its owning team */

    context_register(ch);

    *ctxp = ch;

    return 0;
}

/*
 * zap existing context.  Illegal to zap default context, so I will
 * interpret that as "to be avoided" and continue
 */

void
shmemc_context_destroy(shmem_ctx_t ctx)
{
    if (ctx == SHMEM_CTX_INVALID) {
        shmemu_warn("ignoring attempt to destroy invalid context");
    }
    else if (ctx == SHMEM_CTX_DEFAULT) {
        shmemu_fatal("cannot destroy the default context");
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
shmemc_context_h defcp = & shmemc_default_context;

int
shmemc_context_init_default(void)
{
    context_set_options(0L, defcp);

    shmemc_ucx_context_progress(defcp);

    return shmemc_ucx_context_default_set_info();
}
