/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "thispe.h"

/*
 * the PE's initial state
 */
thispe_info_t proc = {
    .status = SHMEMC_PE_UNKNOWN, /* uninitialized */
    .refcount = 0,               /* init never called */
    .li.rank = -1,               /* undefined position */
    .progress_thread = false,    /* no special progress */
};
