#include "state.h"

#include <stdio.h>

/*
 * the PE's state
 */
thispe_info_t proc = {
    .status = SHMEM_PE_UNKNOWN,
    .refcount = 0,
};
