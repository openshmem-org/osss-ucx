/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"
#include "state.h"

#include <ucp/api/ucp.h>

void
nb_callback(void *req, ucs_status_t status)
{
    ucp_request_free(req);

    NO_WARN_UNUSED(status);
}

void
noop_callback(void *req, ucs_status_t status)
{
    NO_WARN_UNUSED(req);

    NO_WARN_UNUSED(status);
}
