#include <string.h>

#include "version.h"

void
shmem_info_get_version(int *major, int *minor)
{
    *major = SHMEM_MAJOR_VERSION;
    *minor = SHMEM_MINOR_VERSION;
}

void
shmem_info_get_name(char *name)
{
    strncpy(name, SHMEM_VENDOR_STRING, SHMEM_MAX_NAME_LEN);
}
