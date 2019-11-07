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

/*
 * set up world/shared per PE
 */

void
shmemc_teams_init(void)
{
    int pe;

    shmemc_team_world.nmembers = proc.nranks;
    shmemc_team_world.members =
        (int *) malloc(shmemc_team_world.nmembers * sizeof(int));
    shmemu_assert(shmemc_team_world.members != NULL,
                  "cannot allocate memory for world team");

    for (pe = 0; pe < proc.nranks; ++pe) {
        shmemc_team_world.members[pe] = pe;
    }

    shmemc_team_world.nctxts = 0;
    shmemc_team_world.ctxts = NULL;
}

/*
 * clean up at end
 */

void
shmemc_teams_finalize(void)
{
    shmemc_team_contexts_destroy(& shmemc_team_world);

    shmemc_ucx_team_world_destroy();

    free(shmemc_team_world.members);
}

/*
 *
 */

int
shmemc_team_my_pe(shmemc_team_h th)
{
    return th->members[proc.rank];
}

int
shmemc_team_n_pes(shmemc_team_h th)
{
    return (int) th->nmembers;
}

int
shmemc_team_get_config(shmemc_team_h th,
                       shmem_team_config_t *config)
{
    if (th != SHMEM_TEAM_INVALID) {
        *config = th->cfg;
        NO_WARN_UNUSED(config);     /* not touched further here */

        return 0;
    }
    else {
        return -1;
    }
}

int
shmemc_team_translate_pe(shmemc_team_h sh, int src_pe,
                         shmemc_team_h dh)
{
    const int phys = sh->members[src_pe];
    size_t i;

    for (i = 0; i < dh->nmembers; ++i) {
        if (dh->members[i] == phys) {
            return (int) i;
            /* NOT REACHED */
        }
    }

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

void
shmemc_team_destroy(shmemc_team_h th)
{
    if (th == SHMEM_TEAM_INVALID) {
        return;
    }

    shmemu_assert(th != &shmemc_team_world,
                  "may not destroy world team");
    free(th->members);

    /* destroy shareable contexts in this team */

    free(th);
}
