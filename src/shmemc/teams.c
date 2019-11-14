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

shmemc_team_h shmemc_team_world_h = & shmemc_team_world;
shmemc_team_h shmemc_team_shared_h = & shmemc_team_shared;

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
 * populate team from source PEs
 */

inline static void
allocate_team_memory(shmemc_team_h th, int n)
{
    th->nmembers = n;
    th->members = (int *) malloc(th->nmembers * sizeof(int));
    shmemu_assert(th->members != NULL,
                  "cannot allocate memory for new team");
}

inline static void
create_team_members_from_range(shmemc_team_h th, int first, int last)
{
    int i;
    int n;

    allocate_team_memory(th, last - first + 1);

    for (n = 0, i = first; i <= last; ++i) {
        th->members[n] = i;
        ++n;
    }
}

inline static void
create_team_members_from_array(shmemc_team_h th, const int *pes, int npes)
{
    int i;

    allocate_team_memory(th, npes);

    for (i = 0; i < npes; ++i) {
        th->members[i] = pes[i];
    }
}

#if 0
inline static void
dump_team(shmemc_team_h th)
{
    size_t i;

    for (i = 0; i < th->nmembers; ++i) {
        fprintf(stderr, "%-8d %lu -> %d\n", proc.rank, i, th->members[i]);
    }
    fprintf(stderr, "\n");
}
#endif

/*
 * set up world/shared per PE
 */

void
shmemc_teams_init(void)
{
    /*
     * world is just all PEs in program.
     */
    create_team_members_from_range(shmemc_team_world_h, 0, proc.nranks - 1);
    shmemc_team_world_h->nctxts = 0;
    shmemc_team_world_h->ctxts = NULL;

    /*
     * assume that all peers on a node can share memory.
     *
     * could extend this with a ptr() test across peers.
     */
    create_team_members_from_array(shmemc_team_shared_h,
                                   proc.peers, proc.npeers);
    shmemc_team_shared_h->nctxts = 0;
    shmemc_team_shared_h->ctxts = NULL;
}

/*
 * clean up world/shared allocated resources at end
 */

void
shmemc_teams_finalize(void)
{
    shmemc_team_contexts_destroy(shmemc_team_shared_h);
    free(shmemc_team_shared_h->members);

    shmemc_team_contexts_destroy(shmemc_team_world_h);
    free(shmemc_team_world_h->members);
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
    size_t c;

    if (th == SHMEM_TEAM_INVALID) {
        return;
    }

    shmemu_assert(th != shmemc_team_world_h,
                  "may not destroy world team");
    free(th->members);

    for (c = 0; c < th->nctxts; ++c) {
        if (! th->ctxts[c]->attr.private) {
            shmemc_context_destroy(th->ctxts[c]);
        }
    }

    free(th);
}
