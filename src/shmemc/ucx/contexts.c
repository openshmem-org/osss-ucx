/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "state.h"
#include "shmemc.h"
#include "shmemu.h"

#include <ucp/api/ucp.h>

/*
 * fill in context
 *
 * Return 0 on success, 1 on failure
 */

int
shmemc_context_fill(long options, shmemc_context_h ch)
{
    ucs_status_t s;
    ucp_worker_params_t wkpm;

    ch->attr.serialized = options & SHMEM_CTX_SERIALIZED;
    ch->attr.private    = options & SHMEM_CTX_PRIVATE;
    ch->attr.nostore    = options & SHMEM_CTX_NOSTORE;

    wkpm.field_mask  = UCP_WORKER_PARAM_FIELD_THREAD_MODE;

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

    ch->creator_thread = threadwrap_thread_id();

    return 0;
}

/*
 * zap an existing context
 */

void
shmemc_context_cleanup(shmemc_context_h ch)
{
    ucp_worker_destroy(ch->w);
}

int
shmemc_context_default_set_info(shmemc_context_h ch)
{
    ucs_status_t s;
    ucp_address_t *addr;
    size_t len;

    /* get address for remote access to worker */
    s = ucp_worker_get_address(ch->w, &addr, &len);
    if (shmemu_unlikely(s != UCS_OK)) {
        return 1;
        /* NOT REACHED */
    }

    proc.comms.xchg_wrkr_info[proc.rank].addr = addr;
    proc.comms.xchg_wrkr_info[proc.rank].len = len;

    return 0;
}
