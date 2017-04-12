#include <stdlib.h>

char *
shmemc_getenv(const char *name)
{
    return getenv(name);
}
