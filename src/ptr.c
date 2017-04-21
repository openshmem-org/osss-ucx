#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>

/*
 * boringly implemented stub
 */

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ptr = pshmem_ptr
#define shmem_ptr pshmem_ptr
#endif /* ENABLE_PSHMEM */

void *
shmem_ptr(const void *target, int pe)
{
    return (void *) NULL;
}
