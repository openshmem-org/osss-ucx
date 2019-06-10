
/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "state.h"
#include "shmemc.h"
#include "shmemu.h"
#include "ucx/api.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>

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
                  "can't allocate memory "
                  "for remotely accessible endpoints: %s",
                  strerror(errno));
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
ep_wait(shmemc_context_h ch, ucs_status_ptr_t req)
{
    ucs_status_t s;

    if (req == UCS_OK || UCS_PTR_IS_ERR(req)) {
        return;
        /* NOT REACHED */
    }

    do {
        shmemc_ctx_progress(ch);

#ifdef HAVE_UCP_REQUEST_CHECK_STATUS
        s = ucp_request_check_status(req);
#else
        s = ucp_request_test(req, NULL);
#endif  /* HAVE_UCP_REQUEST_CHECK_STATUS */
    } while (s == UCS_INPROGRESS);

    ucp_request_free(req);
}

ucs_status_t
shmemc_ucx_rkey_pack(ucp_mem_h mh, void **packed_rkey_p, size_t *rkey_len_p)
{
    return ucp_rkey_pack(proc.comms.ucx_ctxt,
                         mh,
                         packed_rkey_p, rkey_len_p
                         );
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
                  "failed to allocate memory "
                  "for UCP endpoint disconnect: %s",
                  strerror(errno));

    for (i = 0; i < proc.nranks; ++i) {
        req[i] = ep_disconnect_nb(ch->eps[i]);
    }

    for (i = 0; i < proc.nranks; ++i) {
        ep_wait(ch, req[i]);
    }

    free(req);
}

void
shmemc_ucx_make_eps(shmemc_context_h ch)
{
    ucp_ep_params_t epm;
    ucs_status_t s;
    size_t r;
    int pe;

    /* allocate remote access fields */

    ch->racc = (mem_region_access_t *) calloc(proc.comms.nregions,
                                              sizeof(mem_region_access_t));
    shmemu_assert(ch->racc != NULL,
                  "can't allocate memory for remote access rkeys");

    for (r = 0; r < proc.comms.nregions; ++r) {
        ch->racc[r].rinfo = (mem_access_t *) calloc(proc.nranks,
                                                    sizeof(mem_access_t));
        shmemu_assert(ch->racc[r].rinfo != NULL,
                      "can't allocate remote access info "
                      "for memory region %lu: %s",
                      (unsigned long) r,
                      strerror(errno));
    }

    ch->eps = (ucp_ep_h *) calloc(proc.nranks, sizeof(ucp_ep_h));
    shmemu_assert(ch->eps != NULL,
                  "can't allocate memory for endpoints "
                  "for context %lu: %s",
                  ch->id,
                  strerror(errno));

    /* create endpoints and unpack rkeys onto them */

    epm.field_mask = UCP_EP_PARAM_FIELD_REMOTE_ADDRESS;

    for (pe = 0; pe < proc.nranks; ++pe) {

        epm.address = (ucp_address_t *) proc.comms.xchg_wrkr_info[pe].buf;

        s = ucp_ep_create(ch->w, &epm, & ch->eps[pe]);

        shmemu_assert(s == UCS_OK,
                      "Unable to create remote endpoints for PE %d: %s",
                      pe, ucs_status_string(s)
                      );

        for (r = 0; r < proc.comms.nregions; ++r) {
            s = ucp_ep_rkey_unpack(ch->eps[pe],
                                   proc.comms.orks[r].rkeys[pe].data,
                                   & ch->racc[r].rinfo[pe].rkey
                                   );
            shmemu_assert(s == UCS_OK,
                          "can't unpack remote rkey "
                          "for memory region %lu, PE %d: %s",
                          (unsigned long) r, pe,
                          ucs_status_string(s));
        }
    }

    /* TODO free racc memory on teardown */
}

inline ucs_status_t
shmemc_ucx_worker_wireup(shmemc_context_h ch)
{
    ucs_status_ptr_t req;

    req = ucp_worker_flush_nb(ch->w, 0, noop_callback);

    if (req == UCS_OK) {
        return UCS_OK;
    }
    else if (UCS_PTR_IS_ERR(req)) {
        return UCS_PTR_STATUS(req);
    }
    else {
        ucs_status_t s;

        do {
            shmemc_ctx_progress(ch);
            shmemc_ctx_progress(defcp);

#ifdef HAVE_UCP_REQUEST_CHECK_STATUS
            s = ucp_request_check_status(req);
#else
            s = ucp_request_test(req, NULL);
#endif  /* HAVE_UCP_REQUEST_CHECK_STATUS */
        } while (s == UCS_INPROGRESS);

        ucp_request_free(req);

        return s;
    }
}
