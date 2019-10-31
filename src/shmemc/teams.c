/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"

#include "shmem/teams.h"

int shmem_team_my_pe(shmem_team_t team)
{
    shmemc_team_h th = (shmemc_team_h) team;

    return th->members[proc.rank];
}

int shmem_team_n_pes(shmem_team_t team)
{
    shmemc_team_h th = (shmemc_team_h) team;

    return (int) th->nmembers;
}

int shmem_team_get_config(shmem_team_t team, shmem_team_config_t *config)
{
    NO_WARN_UNUSED(team);
    NO_WARN_UNUSED(config);

    return -1;
}

int
shmemc_team_translate_pe(shmem_team_t src_team, int src_pe,
                         shmem_team_t dest_team)
{
    shmemc_team_h sh = (shmemc_team_h) src_team;
    shmemc_team_h dh = (shmemc_team_h) dest_team;
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

void shmem_team_destroy(shmem_team_t team)
{
    NO_WARN_UNUSED(team);
}
