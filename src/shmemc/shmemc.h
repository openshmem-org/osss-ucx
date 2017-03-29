#ifndef _SHMEMC_COMMS_H
#define _SHMEMC_COMMS_H 1

#include <sys/types.h>

void shmemc_quiet(void);
void shmemc_fence(void);

/*
 * just so things will build for now
 */

#include <stdlib.h>

static inline char *
shmemc_getenv(const char *name)
{
    return getenv(name);
}

/*
 * aligned (or not) puts
 */

static inline void
shmemc_put(void *dest, const void *src, size_t nelems, int pe)
{
    return;
}

#endif /* ! _SHMEMC_COMMS_H */
