#include <stdio.h>

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
    snprintf(name, SHMEM_MAX_NAME_LEN, "%s", SHMEM_VENDOR_STRING);
}
