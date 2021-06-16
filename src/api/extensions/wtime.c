/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemx.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmemx_wtime = pshmemx_wtime
#define shmemx_wtime pshmemx_wtime
#endif /* ENABLE_PSHMEM */

double
shmemx_wtime(void)
{
    return shmemu_timer();
}
