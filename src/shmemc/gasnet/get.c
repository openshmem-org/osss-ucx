#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"

#define SHMEMC_EMIT_GET(_name, _type)                                   \
    void                                                                \
    shmemc_##_name##_get(_type *target, const _type *source,            \
                         size_t nelems, int pe)                         \
    {                                                                   \
        const size_t sized_nelems = nelems * sizeof(_type);             \
                                                                        \
        shmemc_get((void *) target, (void *) source, sized_nelems, pe); \
    }

SHMEMC_EMIT_GET(char, char)
SHMEMC_EMIT_GET(short, short)
SHMEMC_EMIT_GET(int, int)
SHMEMC_EMIT_GET(long, long)
SHMEMC_EMIT_GET(float, float)
SHMEMC_EMIT_GET(double, double)
SHMEMC_EMIT_GET(longlong, long long)
SHMEMC_EMIT_GET(longdouble, long double)
