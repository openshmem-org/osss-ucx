#ifndef _SHMEMC_STATE_H
#define _SHMEMC_STATE_H 1

#include "thispe.h"

#include <ucp/api/ucp.h>

extern thispe_info_t proc;

extern memory_region_t global_segment;
extern memory_region_t symm_segment;

#endif /* ! _SHMEMC_STATE_H */
