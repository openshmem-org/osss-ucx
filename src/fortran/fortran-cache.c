/* For license: see LICENSE file at top-level */

/*
 * stop deprecation warnings
 */
#define _SHMEM_IN_SOURCE_TREE_ 123321
#include "shmem/api.h"
#undef _SHMEM_IN_SOURCE_TREE_

#include "fortran-common.h"

/**
 *
 * DEPRECATED API
 *
 * cache flushing
 */

FORTRANIFY_VOID_VOID(shmem_clear_cache_inv)
FORTRANIFY_VOID_VOID(shmem_set_cache_inv)
FORTRANIFY_VOID_VOID(shmem_udcflush)

FORTRANIFY_CACHE(shmem_set_cache_line_inv)
FORTRANIFY_CACHE(shmem_clear_cache_line_inv)
FORTRANIFY_CACHE(shmem_udcflush_line)
