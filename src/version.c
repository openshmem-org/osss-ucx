/*
 * TODO get the info passed through autoconf into here and shmem.h
 *
 * for now just hardcode
 *
 */

#include <stdio.h>

#define SHMEM_MAJOR_VERSION 1
#define SHMEM_MINOR_VERSION 3
#define SHMEM_MAX_NAME_LEN 64
#define SHMEM_VENDOR_STRING "OSSS/SBU Reference Implementation"

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
