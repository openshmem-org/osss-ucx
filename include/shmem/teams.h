/* For license: see LICENSE file at top-level */

#ifndef _SHMEM_API_TEAMS_H
#define _SHMEM_API_TEAMS_H 1

/*
 * an opaque OpenSHMEM team
 */

typedef void *shmem_team_t;

/*
 * pre-defined teams for whole program at start
 */

extern shmem_team_t SHMEM_TEAM_WORLD;
extern shmem_team_t SHMEM_TEAM_SHARED;
extern shmem_team_t SHMEM_TEAM_INVALID;

/*
 * team configuration structure
 */

typedef struct shmem_team_config {
    int num_contexts;           /*<< expected # of contexts used by team */
} shmem_team_config_t;

int shmem_team_my_pe(shmem_team_t team);
int shmem_team_n_pes(shmem_team_t team);
int shmem_team_get_config(shmem_team_t team, shmem_team_config_t *config);
int shmem_team_translate_pe(shmem_team_t src_team, int src_pe,
                            shmem_team_t dest_team);
int shmem_team_split_strided(shmem_team_t parent_team,
                             int start, int stride, int size,
                             const shmem_team_config_t *config,
                             long config_mask, shmem_team_t *new_team);
int shmem_team_split_2d(shmem_team_t parent_team, int xrange,
                        const shmem_team_config_t *xaxis_config,
                        long xaxis_mask,
                        shmem_team_t *xaxis_team,
                        const shmem_team_config_t *yaxis_config,
                        long yaxis_mask,
                        shmem_team_t *yaxis_team);
void shmem_team_destroy(shmem_team_t team);

#endif  /* ! _SHMEM_API_TEAMS_H */
