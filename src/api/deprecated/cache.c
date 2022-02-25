/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"

/*
 * These are compatibility routines for older SGI architectures.  They
 * are now defined in OpenSHMEM to do nothing.
 */

#ifdef ENABLE_PSHMEM
#pragma weak shmem_clear_cache_inv = pshmem_clear_cache_inv
#define shmem_clear_cache_inv pshmem_clear_cache_inv
#pragma weak shmem_set_cache_inv = pshmem_set_cache_inv
#define shmem_set_cache_inv pshmem_set_cache_inv
#pragma weak shmem_set_cache_line_inv = pshmem_set_cache_line_inv
#define shmem_set_cache_line_inv pshmem_set_cache_line_inv
#pragma weak shmem_clear_cache_line_inv = pshmem_clear_cache_line_inv
#define shmem_clear_cache_line_inv pshmem_clear_cache_line_inv
#pragma weak shmem_udcflush = pshmem_udcflush
#define shmem_udcflush pshmem_udcflush
#pragma weak shmem_udcflush_line = pshmem_udcflush_line
#define shmem_udcflush_line pshmem_udcflush_line
#endif /* ENABLE_PSHMEM */

/*
 * Compatibility no-op cache routines
 */

static const shmemu_version_t v = { .major = 1, .minor = 3 };

void
shmem_set_cache_inv(void)
{
    deprecate(__func__, &v);
}

void
shmem_clear_cache_inv(void)
{
    deprecate(__func__, &v);
}

void
shmem_set_cache_line_inv(void *target)
{
    NO_WARN_UNUSED(target);

    deprecate(__func__, &v);
}

void
shmem_clear_cache_line_inv(void *target)
{
    NO_WARN_UNUSED(target);

    deprecate(__func__, &v);
}

void
shmem_udcflush(void)
{
    deprecate(__func__, &v);
}

void
shmem_udcflush_line(void *target)
{
    NO_WARN_UNUSED(target);

    deprecate(__func__, &v);
}
