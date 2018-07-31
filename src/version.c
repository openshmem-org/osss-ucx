/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_info_get_version = pshmem_info_get_version
#define shmem_info_get_version pshmem_info_get_version
#pragma weak shmem_info_get_name = pshmem_info_get_name
#define shmem_info_get_name pshmem_info_get_name
#endif /* ENABLE_PSHMEM */

void
shmem_info_get_version(int *major, int *minor)
{
    *major = SHMEM_MAJOR_VERSION;
    *minor = SHMEM_MINOR_VERSION;
}

void
shmem_info_get_name(char *name)
{
    STRNCPY_SAFE(name, SHMEM_VENDOR_STRING, SHMEM_MAX_NAME_LEN);
}
