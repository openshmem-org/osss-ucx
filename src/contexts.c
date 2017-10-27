/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"
#include "shmem/defs.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_short_wait_until = pshmem_short_wait_until
#define shmem_short_wait_until pshmem_short_wait_until
#pragma weak shmem_int_wait_until = pshmem_int_wait_until
#define shmem_int_wait_until pshmem_int_wait_until
#pragma weak shmem_long_wait_until = pshmem_long_wait_until
#define shmem_long_wait_until pshmem_long_wait_until
#pragma weak shmem_longlong_wait_until = pshmem_longlong_wait_until
#define shmem_longlong_wait_until pshmem_longlong_wait_until
#endif /* ENABLE_PSHMEM */


int
shmem_ctx_create(long options, shmem_ctx_t *ctxp)
{
    return 0;
}

void
shmem_ctx_destroy(shmem_ctx_t ctx)
{
}
