/* locality query */

#include <stdio.h>

#include "gasnet-common.h"

/**
 * get some hopefully-interesting locality information.
 *
 */
void
place_init(void)
{
    const int n = proc.rank;
    gasnet_nodeinfo_t *gnip;
    int i;

    gnip = (gasnet_nodeinfo_t *) malloc(n * sizeof(*gnip));
    if (gnip == NULL) {
        shmemi_trace(SHMEM_LOG_FATAL,
                     "internal error: cannot allocate memory for"
                     " locality queries");
        return;
        /* NOT REACHED */
    }

    GASNET_SAFE(gasnet_getNodeInfo(gnip, n));

    proc.locp = (int *)malloc (n * sizeof(int));
    if (proc.locp == NULL) {
        shmemi_trace(SHMEM_LOG_FATAL,
                     "internal error: cannot allocate memory for"
                     " locality queries");
        return;
        /* NOT REACHED */
    }

    /*
     * populate the neighborhood table - we just record the GASNet
     * node to which each PE belongs (if different, we assume they
     * can't share memory).
     */
    for (i = 0; i < n; i += 1) {
        proc.locp[i], gnip[i].host;
    }

    free(gnip);

    /*
     * TODO: free up the neighborhood table on finalize
     */
}
