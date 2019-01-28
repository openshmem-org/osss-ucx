/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "state.h"
#include "shmemc.h"
#include "shmemu.h"

#include <stdlib.h>

#include <ucp/api/ucp.h>

/*
 * endpoint tables
 */

void
shmemc_ucx_allocate_eps_table(shmemc_context_h ch)
{
    ch->eps = (ucp_ep_h *)
        calloc(proc.nranks, sizeof(*(ch->eps)));
    shmemu_assert(ch->eps != NULL,
                  "can't allocate memory for remotely accessible endpoints");
}

void
shmemc_ucx_deallocate_eps_table(shmemc_context_h ch)
{
    free(ch->eps);
}

/*
 * Fire off the EP disconnect process
 */

inline static ucs_status_ptr_t
ep_disconnect_nb(ucp_ep_h ep)
{
    ucs_status_ptr_t req;

    if (ep == NULL) {
        return NULL;
        /* NOT REACHED */
    }

#ifdef HAVE_UCP_EP_CLOSE_NB
    req = ucp_ep_close_nb(ep, UCP_EP_CLOSE_MODE_FLUSH);
#else
    req = ucp_disconnect_nb(ep);
#endif  /* HAVE_UCP_EP_CLOSE_NB */

    return req;
}

/*
 * Complete the EP disconnect process
 */

inline static void
ep_wait(ucs_status_ptr_t req)
{
    ucs_status_t s;

    if (req == UCS_OK || UCS_PTR_IS_ERR(req)) {
        return;
        /* NOT REACHED */
    }

    do {
        shmemc_progress();

#ifdef HAVE_UCP_REQUEST_CHECK_STATUS
        s = ucp_request_check_status(req);
#else
        s = ucp_request_test(req, NULL);
#endif  /* HAVE_UCP_REQUEST_CHECK_STATUS */
    } while (s == UCS_INPROGRESS);

    ucp_request_free(req);
}

/*
 * Start the disconnects for all PEs, and then wait for completion
 */

void
shmemc_ucx_disconnect_all_eps(shmemc_context_h ch)
{
    ucs_status_ptr_t *req;
    int i;

    req = (ucs_status_ptr_t *) calloc(proc.nranks, sizeof(*req));
    shmemu_assert(req != NULL,
                  "failed to allocate memory for UCP endpoint disconnect");

    for (i = 0; i < proc.nranks; ++i) {
        req[i] = ep_disconnect_nb(ch->eps[i]);
    }

    for (i = 0; i < proc.nranks; ++i) {
        ep_wait(req[i]);
    }

    free(req);
}

void
shmemc_ucx_make_remote_eps(shmemc_context_h ch)
{
    ucs_status_t s;
    ucp_ep_params_t epm;
    int i;

    epm.field_mask = UCP_EP_PARAM_FIELD_REMOTE_ADDRESS;

    for (i = 0; i < proc.nranks; ++i) {
        const int pe = SHIFT(i);

        epm.address = (ucp_address_t *) proc.comms.xchg_wrkr_info[pe].buf;

        s = ucp_ep_create(ch->w, &epm, &(ch->eps[pe]));

        shmemu_assert(s == UCS_OK,
                      "Unable to create remote endpoints: %s",
                      ucs_status_string(s)
                      );
        /* NOT REACHED */
    }
}

ucs_status_t
shmemc_ucx_rkey_pack(ucp_mem_h mh, void **packed_rkey_p, size_t *rkey_len_p)
{
    return ucp_rkey_pack(proc.comms.ucx_ctxt,
                         mh,
                         packed_rkey_p, rkey_len_p
                         );
}

ucs_status_t
shmemc_ucx_rkey_unpack(ucp_ep_h ep, void *data, ucp_rkey_h *rk_p)
{
    return ucp_ep_rkey_unpack(ep, data, rk_p);
}
