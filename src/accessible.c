#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_pe_accessible = pshmem_pe_accessible
#define shmem_pe_accessible pshmem_pe_accessible
#pragma weak shmem_addr_accessible = pshmem_addr_accessible
#define shmem_addr_accessible pshmem_addr_accessible
#endif /* ENABLE_PSHMEM */

int
shmem_pe_accessible(int pe)
{
    return shmemc_pe_accessible(pe);
}

int
shmem_addr_accessible(const void *addr, int pe)
{
    return shmemc_addr_accessible(addr, pe);
}
