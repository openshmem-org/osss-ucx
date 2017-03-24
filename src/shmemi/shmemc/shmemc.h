#ifndef _SHMEMC_COMMS_H
#define _SHMEMC_COMMS_H 1

void shmemc_quiet(void);
void shmemc_fence(void);

/*
 * just so things will build for now
 */

#include <stdlib.h>

static
inline
char *
shmemc_getenv(const char *name)
{
    return getenv(name);
}

#endif /* ! _SHMEMC_COMMS_H */
