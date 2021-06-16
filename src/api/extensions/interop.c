/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemx.h"

int
shmemx_query_interoperability(int property)
{
    int ret;

    switch (property) {
    case UPC_THREADS_ARE_PES:
        ret = 0;
        break;
    case MPI_PROCESSES_ARE_PES:
        ret = 0;
        break;
    case SHMEM_INITIALIZES_MPI:
        ret = 0;
        break;
    case MPI_INITIALIZES_SHMEM:
        ret = 0;
        break;
    default:                    /* should be error */
        ret = 0;
        break;
    }
    return ret;
}
