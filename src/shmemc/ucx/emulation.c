/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"

#include <ucp/api/ucp.h>

int
shmemc_ucx_emulation_query(void)
{
#ifdef UCP_WORKER_ATTR_FIELD_EMULATION_MODE
    ucs_status_t s;
    shmemc_context_h ch = (shmemc_context_h) SHMEM_CTX_DEFAULT;
    ucp_worker_attr_t a;

    a.field_mask = UCP_WORKER_ATTR_FIELD_EMULATION_MODE;
    s = ucp_worker_query(ch->w, &a);
    shmemu_assert(s == UCS_OK,
                  "can't query worker (%s)",
                  ucs_status_string(s));
    return a.emulation_mode;
#else
    return 0;
#endif /* UCP_WORKER_ATTR_FIELD_EMULATION_MODE */
}
