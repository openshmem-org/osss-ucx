/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"
#include "state.h"

#include <ucp/api/ucp.h>

/*
 * callback for non-blocking(-implicit) ops, need to release request
 */

void
nb_callback(void *req, ucs_status_t status)
{
    ucp_request_release(req);
    NO_WARN_UNUSED(status);
}

void
nb_callbackx(void *req, ucs_status_t status, void *user_data)
{
    ucp_request_release(req);
    NO_WARN_UNUSED(status);
    NO_WARN_UNUSED(user_data);
}

/*
 * dummy callback
 */

void
noop_callback(void *req, ucs_status_t status)
{
    NO_WARN_UNUSED(req);
    NO_WARN_UNUSED(status);
}

/*
 * dummy callback for _nbx
 */

void
noop_callbackx(void *req, ucs_status_t status, void *user_data)
{
    NO_WARN_UNUSED(req);
    NO_WARN_UNUSED(status);
    NO_WARN_UNUSED(user_data);
}
