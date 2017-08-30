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
memory_region_t global_segment;

/*
 * some memory to play with registering
 *
 * TODO: multiple heap support
 */
memory_region_t symm_segment;
