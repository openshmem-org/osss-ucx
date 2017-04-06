/*
 * These are compatibility routines for older SGI architectures.  They
 * are now defined in OpenSHMEM to do nothing.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemi/shmemi.h"

static
void
inline
deprecate(const char *fn)
{
    logger(LOG_INFO, "\"%s\" is deprecated", fn);
}

/*
 * Compatibility no-op cache routines
 */

void
shmem_set_cache_inv(void)
{
    deprecate(__func__);
}

void
shmem_clear_cache_inv(void)
{
    deprecate(__func__);
}

void
shmem_set_cache_line_inv(void *target)
{
    deprecate(__func__);
}

void
shmem_clear_cache_line_inv(void *target)
{
    deprecate(__func__);
}

void
shmem_udcflush(void)
{
    deprecate(__func__);
}

void
shmem_udcflush_line(void *target)
{
    deprecate(__func__);
}
