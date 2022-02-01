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
    SHMEMU_CHECK_INIT();
    SHMEMU_CHECK_NOT_NULL(major, 1);
    SHMEMU_CHECK_NOT_NULL(minor, 2);

    *major = SHMEM_MAJOR_VERSION;
    *minor = SHMEM_MINOR_VERSION;
}

void
shmem_info_get_name(char *name)
{
    SHMEMU_CHECK_INIT();
    SHMEMU_CHECK_NOT_NULL(name, 1);

    STRNCPY_SAFE(name, SHMEM_VENDOR_STRING, SHMEM_MAX_NAME_LEN);
}

#ifdef PR463

/*
 * https://github.com/openshmem-org/specification/issues/463
 *
 */

#define SHMEM_VERSION_CVT(_maj, _min)           \
    ( (100 * (_maj)) + (_mi)n )

#define SHMEM_VENDOR_VERSION_CVT(_maj, _min, _pth)          \
    ( ( 100 * SHMEM_VERSION_CVT(_maj, _min) ) + (_pth) )

static const int saved_version =
    SHMEM_VERSION_CVT(SHMEM_MAJOR_VERSION, SHMEM_MINOR_VERSION);
static const int saved_vendor_version =
    SHMEM_VENDOR_VERSION_CVT(SHMEM_VENDOR_MAJOR_VERSION,
                             SHMEM_VENDOR_MINOR_VERSION,
                             SHMEM_VENDOR_PATCH_VERSION);

#ifdef ENABLE_PSHMEM
#pragma weak shmem_info_get_version_number = pshmem_info_get_version_number
#define shmem_info_get_version_number pshmem_info_get_version_number
#endif /* ENABLE_PSHMEM */

void
shmem_info_get_version_number(int *version)
{
    SHMEMU_CHECK_INIT();
    SHMEMU_CHECK_NOT_NULL(version, 1);

    *version = saved_version;
}

#ifdef ENABLE_PSHMEM
#pragma weak shmem_info_get_vendor_version = pshmem_info_get_vendor_version
#define shmem_info_get_vendor_version pshmem_info_get_vendor_version
#endif /* ENABLE_PSHMEM */

void
shmem_info_get_vendor_version(int *major, int *minor, int *patch)
{
    SHMEMU_CHECK_INIT();
    SHMEMU_CHECK_NOT_NULL(major, 1);
    SHMEMU_CHECK_NOT_NULL(minor, 2);
    SHMEMU_CHECK_NOT_NULL(patch, 3);

    *major = SHMEM_VENDOR_MAJOR_VERSION;
    *minor = SHMEM_VENDOR_MINOR_VERSION;
    *patch = SHMEM_VENDOR_PATCH_VERSION;
}

#ifdef ENABLE_PSHMEM
#pragma weak shmem_info_get_vendor_version_number = pshmem_info_get_vendor_version_number
#define shmem_info_get_vendor_version_number pshmem_info_get_vendor_version_number
#endif /* ENABLE_PSHMEM */

void
shmem_info_get_vendor_version_number(int *version)
{
    SHMEMU_CHECK_INIT();
    SHMEMU_CHECK_NOT_NULL(version, 1);

    *version = saved_vendor_version;
}

#endif
