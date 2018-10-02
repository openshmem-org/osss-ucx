/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"

#include <ucp/api/ucp.h>

/*
 * return non-zero if external progress might be needed by the caller.
 * 0 means UCX is definitely handling it
 */

int
shmemc_ucx_progress_needed(void)
{
    ucs_status_t s;
    shmemc_context_h ch = (shmemc_context_h) SHMEM_CTX_DEFAULT;
    ucp_worker_attr_t a;

    a.field_mask = UCP_WORKER_ATTR_FIELD_EXT_PROGRESS;
    s = ucp_worker_query(ch->w, &a);
    shmemu_assert(s == UCS_OK,
                  "can't query worker (%s)",
                  ucs_status_string(s));
    return a.ext_progress;
}
