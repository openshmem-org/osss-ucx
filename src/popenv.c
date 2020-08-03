/* For license: see LICENSE file at top-level */

/*
 * Populate the PE environment just before init finishes.  Taken from
 * Open-MPI behavior, see
 *
 *   https://www.open-mpi.org/faq/?category=running#mpi-environmental-variables
 *
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"

#include <stdlib.h>

static void
env_setter(const char *var, int val)
{
#ifdef HAVE_SETENV
    const int overwrite = 1;

#define MAXBUF 16

    static char buf[MAXBUF];

    snprintf(buf, MAXBUF, "%d", val);
    setenv(var, buf, overwrite);

#else

#ifdef HAVE_PUTENV

#define MAXBUF 64

    static char buf[MAXBUF];

    snprintf(buf, MAXBUF, "%s=%d", var, val);
    putenv(buf);

#endif /* HAVE_PUTENV */

    /* silently do nothing */

#endif /* HAVE_SETENV */
}

void
populate_env(void)
{
    int i;

    env_setter("SHMEM_TEAM_PE", proc.rank);

    env_setter("SHMEM_TEAM_WORLD_SIZE", proc.nranks);

    env_setter("SHMEM_TEAM_UNIVERSE_SIZE", proc.maxranks);

    /*
     * we don't have multi-job runs in OpenSHMEM
     */
    for (i = 0; i < proc.npeers; ++i) {
        if (proc.peers[i] == proc.rank) {
            env_setter("SHMEM_TEAM_WORLD_LOCAL_PE", i);
            env_setter("SHMEM_TEAM_WORLD_NODE_PE", i);
            break;
            /* NOT REACHED */
        }
    }

    env_setter("SHMEM_TEAM_WORLD_LOCAL_SIZE", proc.npeers);
}
