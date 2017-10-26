/* For license: see LICENSE file at top-level */

#include "state.h"

/*
 * the PE's initial state
 */
thispe_info_t proc = {
    .status = SHMEM_PE_UNKNOWN,
    .refcount = 0,
};
