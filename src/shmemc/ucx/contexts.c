/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "state.h"
#include "shmemc.h"
#include "shmemu.h"
#include "ucx/api.h"

#include <stdlib.h>

#include <ucp/api/ucp.h>

/*
 * fill in context's worker
 *
 * Return 0 on success, non-0 on failure
 */

int
shmemc_ucx_context_progress(shmemc_context_h ch)
{
    ucp_worker_params_t wkpm;
    ucs_status_t s;

    wkpm.field_mask = UCP_WORKER_PARAM_FIELD_THREAD_MODE;

    if (ch->attr.serialized) {
        wkpm.thread_mode = UCS_THREAD_MODE_SERIALIZED;
    }
    else if (ch->attr.private) {
        wkpm.thread_mode = UCS_THREAD_MODE_SINGLE;
    }
    else {
        wkpm.thread_mode = UCS_THREAD_MODE_MULTI;
    }

    s = ucp_worker_create(proc.comms.ucx_ctxt, &wkpm, &(ch->w));
    if (shmemu_unlikely(s != UCS_OK)) {
        return 1;
        /* NOT REACHED */
    }

    return 0;
}

/*
 * Fill out info for default context
 *
 * Return 0 on success, non-0 otherwise
 */

int
shmemc_ucx_context_default_set_info(void)
{
    ucs_status_t s;
    ucp_address_t *addr;
    size_t len;

    /* get address for remote access to worker */
    s = ucp_worker_get_address(defcp->w, &addr, &len);
    if (shmemu_unlikely(s != UCS_OK)) {
        return 1;
        /* NOT REACHED */
    }

    proc.comms.xchg_wrkr_info[proc.li.rank].addr = addr;
    proc.comms.xchg_wrkr_info[proc.li.rank].len = len;

    return 0;
}

void
shmemc_ucx_context_default_destroy(void)
{
    ucp_worker_release_address(defcp->w,
                               proc.comms.xchg_wrkr_info[proc.li.rank].addr);
    shmemc_ucx_teardown_context(defcp);
}
