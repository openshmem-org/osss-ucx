#ifndef _SHMEMC_STATE_H
#define _SHMEMC_STATE_H 1

#include "thispe.h"
#include <ucp/api/ucp.h>

extern thispe_info_t proc;

extern ucp_mem_h global_segment;
extern ucp_mem_h symm_heap;

#endif /* ! _SHMEMC_STATE_H */
