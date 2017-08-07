#include "state.h"

#include <stdio.h>

/*
 * the PE's state
 */
thispe_info_t proc = {
    .status = SHMEM_PE_UNKNOWN,
    .refcount = 0,
    .heaps = NULL
};

/*
 * where globals/statics/commons/saves live
 *
 */
ucp_mem_h global_segment;

/*
 * some memory to play with registering
 *
 * TODO: multiple heap support
 */
ucp_mem_h symm_heap;
