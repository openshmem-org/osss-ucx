/*
 * These are compatibility routines for older SGI architectures.  They
 * are now defined in OpenSHMEM to do nothing.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

/*
 * Compatibility no-op cache routines
 */

void
shmem_set_cache_inv(void)
{
}

void
shmem_clear_cache_inv(void)
{
}

void
shmem_set_cache_line_inv(void *target)
{
}

void
shmem_clear_cache_line_inv(void *target)
{
}

void
shmem_udcflush(void)
{
}

void
shmem_udcflush_line(void *target)
{
}
