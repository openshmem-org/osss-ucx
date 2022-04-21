/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"
#include "shmem/teams.h"
#include "ucx/api.h"
#include "allocator/memalloc.h"
#include "module.h"

#include <stdlib.h>

shmemc_team_t shmemc_team_world;
shmemc_team_t shmemc_team_shared;

static shmemc_team_h world = & shmemc_team_world;
static shmemc_team_h shared = & shmemc_team_shared;
static shmemc_team_h invalid = NULL;

/*
 * clear up the allocated SHMEM contexts in a team
 */

static void
shmemc_team_contexts_destroy(shmemc_team_h th)
{
    size_t c;

    for (c = 0; c < th->nctxts; ++c) {
        shmemc_ucx_teardown_context(th->ctxts[c]);
    }
    free(th->ctxts);
}

#if 0
static void
dump_team(shmemc_team_h th)
{
   int key, val;

   printf("==========================================\n");

   kh_foreach(th->fwd, key, val,
              {
                  printf("fwd: %d -> %d\n", key, val);
              }
              );
   kh_foreach(th->rev, key, val,
              {
                  printf("rev: %d -> %d\n", key, val);
              }
              );
   printf("\n");

   printf("Team = %p (%s)\n", (void *) th, th->name);

   printf("  global rank = %d, mype = %4d, npes = %4d\n",
          proc.li.rank,
          th->rank,
          th->nranks);
   printf("------------------------------------------\n");
}
#endif

static void
initialize_psync_buffers(shmemc_team_h th)
{
    unsigned nsync;

    for (nsync = 0; nsync < SHMEMC_NUM_PSYNCS; ++nsync) {
        unsigned i;

        const size_t nbytes = SHMEM_BARRIER_SYNC_SIZE * sizeof(*(th->pSyncs));
        th->pSyncs[nsync] = (long *) shmema_malloc(nbytes);

        shmemu_assert(th->pSyncs[nsync] != NULL,
                      MODULE ": can't allocate sync memory "
                      "#%u in %s team (%p)",
                      nsync,
                      th->parent == NULL ? th->name : "created",
                      th);

        for (i = 0; i < SHMEM_BARRIER_SYNC_SIZE; ++i) {
            th->pSyncs[nsync][i] = SHMEM_SYNC_VALUE;
        }
    }
}

static void
finalize_psync_buffers(shmemc_team_h th)
{
    unsigned nsync;

    for (nsync = 0; nsync < SHMEMC_NUM_PSYNCS; ++nsync) {
        shmema_free(th->pSyncs[nsync]);
    }
}

/*
 * common setup
 */
static void
initialize_common_team(shmemc_team_h th, const char *name, int cfg_nctxts)
{
    th->parent = NULL;
    th->name   = name;

    /* nothing allocated yet */
    th->nctxts = 0;
    th->ctxts  = NULL;

    th->cfg.num_contexts = cfg_nctxts;

    th->fwd = kh_init(map);
    th->rev = kh_init(map);

    initialize_psync_buffers(th);
}

/*
 * set up world/shared per PE
 *
 */

static void
initialize_team_world(void)
{
    int i;
    int absent;

    initialize_common_team(world, "world", proc.env.prealloc_contexts);

    /* populate from launch info */
    world->rank = proc.li.rank;
    world->nranks = proc.li.nranks;

    for (i = 0; i < proc.li.nranks; ++i) {
        khiter_t k;

        k = kh_put(map, world->fwd, i, &absent);
        kh_val(world->fwd, k) = i;
        k = kh_put(map, world->rev, i, &absent);
        kh_val(world->rev, k) = i;
    }
}

static void
initialize_team_shared(void)
{
    int i;
    int absent;

    initialize_common_team(shared, "shared",
                           proc.env.prealloc_contexts / proc.li.nnodes);

    shared->rank = -1;
    shared->nranks = proc.li.npeers;

    for (i = 0; i < proc.li.npeers; ++i) {
        khiter_t k;

        if (proc.li.rank == proc.li.peers[i]) {
            shared->rank = i;
        }

        k = kh_put(map, shared->fwd, i, &absent);
        kh_val(shared->fwd, k) = proc.li.peers[i];
        k = kh_put(map, shared->rev, proc.li.peers[i], &absent);
        kh_val(shared->rev, k) = i;
    }
}

/*
 * clean up team resources at end
 */

static void
finalize_team(shmemc_team_h th)
{
    finalize_psync_buffers(th);

    shmemc_team_contexts_destroy(th);
}

void
shmemc_teams_init(void)
{
    initialize_team_world();
    initialize_team_shared();
}

void
shmemc_teams_finalize(void)
{
    finalize_team(shared);
    finalize_team(world);
}

/*
 * ----------------------------------------------------------------
 */

/*
 * per-team rank queries
 */

int
shmemc_team_my_pe(shmemc_team_h th)
{
    return th->rank;
}

int
shmemc_team_n_pes(shmemc_team_h th)
{
    return th->nranks;
}

/*
 * retrieve the team's configuration
 */

int
shmemc_team_get_config(shmemc_team_h th,
                       shmem_team_config_t *config)
{
    *config = th->cfg;
    NO_WARN_UNUSED(config);     /* not touched further here */

    return 0;
}

/*
 * what's the SOURCEHANDLE team SRC_PE in the DESTHANDLE team?
 */

int
shmemc_team_translate_pe(shmemc_team_h sh, int src_pe,
                         shmemc_team_h dh)
{
    khiter_t k;
    int wpe;

    /* can we find the source PE? */
    k = kh_get(map, sh->fwd, src_pe);
    if (k == kh_end(sh->fwd)) {
        return -1;
        /* NOT REACHED */
    }

    /* world equiv PE */
    wpe = kh_val(sh->fwd, k);

    /* map to world equiv in destination team */
    k = kh_get(map, dh->rev, wpe);
    if (k == kh_end(dh->rev)) {
        return -1;
        /* NOT REACHED */
    }

    /* world equiv is this in destination team */
    return kh_val(dh->rev, k);
}

static bool
is_member(int parent_pe, int start, int stride)
{
    return ((parent_pe - start) % stride) == 0;
}

int
shmemc_team_split_strided(shmemc_team_h parh,
                          int start, int stride, int size,
                          const shmem_team_config_t *config,
                          long config_mask,
                          shmemc_team_h *newh)
{
    int i;                      /* new team PE # */
    int walk;                   /* iterate over parent PEs */
    shmemc_team_h newt;
    int absent;
    int nc;

    newt = (shmemc_team_h) malloc(sizeof(*newt));
    if (newt == NULL) {
        *newh = SHMEM_TEAM_INVALID;
        return -1;
    }

    nc = (config_mask & SHMEM_TEAM_NUM_CONTEXTS)
        ? config->num_contexts
        : 0;

    initialize_common_team(newt, NULL, nc);

    newt->parent = parh;

    newt->nranks = size;

    walk = start;
    for (i = 0; i < size; ++i) {
        khint_t k;

        k = kh_get(map, parh->fwd, walk);
        const int up = kh_val(parh->fwd, k);

        if (is_member(up, start, size)) {
            k = kh_put(map, newt->fwd, i, &absent);
            kh_val(newt->fwd, k) = up;

            k = kh_put(map, newt->rev, up, &absent);
            kh_val(newt->rev, k) = i;
        }

        walk += stride;
    }

    *newh = newt;

    return 0;
}

int
shmemc_team_split_2d(shmemc_team_h parh,
                     int xrange,
                     const shmem_team_config_t *xaxis_config,
                     long xaxis_mask,
                     shmemc_team_h *xaxish,
                     const shmem_team_config_t *yaxis_config,
                     long yaxis_mask,
                     shmemc_team_h *yaxish)
{
    /* TODO */
    NO_WARN_UNUSED(parh);
    NO_WARN_UNUSED(xrange);
    NO_WARN_UNUSED(xaxis_config);
    NO_WARN_UNUSED(xaxis_mask);
    NO_WARN_UNUSED(xaxish);
    NO_WARN_UNUSED(yaxis_config);
    NO_WARN_UNUSED(yaxis_mask);
    NO_WARN_UNUSED(yaxish);

    return -1;
}

/*
 * teams that the library defined in advance cannot be destroyed.
 */

void
shmemc_team_destroy(shmemc_team_h th)
{
    if (th->parent != NULL) {
        size_t c;

        for (c = 0; c < th->nctxts; ++c) {
            if (! th->ctxts[c]->attr.private) {
                shmemc_context_destroy(th->ctxts[c]);
            }
        }

        free(th);

        th = invalid;
    }
    else {
        shmemu_fatal("cannot destroy predefined team \"%s\"", th->name);
        /* NOT REACHED */
    }
}
