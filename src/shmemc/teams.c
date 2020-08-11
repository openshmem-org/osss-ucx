/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"
#include "shmem/teams.h"
#include "ucx/api.h"

#include <stdlib.h>

shmemc_team_t shmemc_team_world;
shmemc_team_t shmemc_team_shared;

static shmemc_team_h world = & shmemc_team_world;
static shmemc_team_h shared = & shmemc_team_shared;
static shmemc_team_h invalid = NULL;

/*
 * clear up the allocated SHMEM contexts in a team
 */

inline static void
shmemc_team_contexts_destroy(shmemc_team_h th)
{
    size_t c;

    for (c = 0; c < th->nctxts; ++c) {
        shmemc_ucx_teardown_context(th->ctxts[c]);
    }
    free(th->ctxts);
}

#if 1
inline static void
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

/*
 * common setup
 */
inline static void
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
}

/*
 * set up world/shared per PE
 *
 */

inline static void
initialize_team_world(void)
{
    size_t i;
    khiter_t k;
    int absent;

    initialize_common_team(world, "world", proc.env.prealloc_contexts);

    world->rank = proc.li.rank;
    world->nranks = proc.li.nranks;

    for (i = 0; i < proc.li.nranks; ++i) {
        k = kh_put(map, world->fwd, i, &absent);
        kh_val(world->fwd, k) = i;
        k = kh_put(map, world->rev, i, &absent);
        kh_val(world->rev, k) = i;
    }
}

inline static void
initialize_team_shared(void)
{
    size_t i;
    khiter_t k;
    int absent;

    initialize_common_team(shared, "shared",
                           proc.env.prealloc_contexts / proc.li.nnodes);

    shared->rank = -1;
    shared->nranks = proc.li.npeers;

    for (i = 0; i < proc.li.npeers; ++i) {
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
 * clean up world/shared allocated resources at end
 */

inline static void
finalize_team(shmemc_team_h th)
{
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
    return -1;
}

int
shmemc_team_split_strided(shmemc_team_h parh,
                          int start, int stride, int size,
                          const shmem_team_config_t *config,
                          long config_mask,
                          shmemc_team_h *newh)
{
    /* TODO */
    NO_WARN_UNUSED(parh);
    NO_WARN_UNUSED(start);
    NO_WARN_UNUSED(stride);
    NO_WARN_UNUSED(size);
    NO_WARN_UNUSED(config);
    NO_WARN_UNUSED(config_mask);
    NO_WARN_UNUSED(newh);

    return -1;
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
