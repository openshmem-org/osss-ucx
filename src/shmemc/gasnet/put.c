#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"

#define SHMEMC_EMIT_PUT(_name, _type)                                   \
    void                                                                \
    shmemc_##_name##_put(_type *target, const _type *source,            \
                         size_t nelems, int pe)                         \
    {                                                                   \
        const size_t sized_nelems = nelems * sizeof(_type);             \
                                                                        \
        shmemc_put((void *) target, (void *) source, sized_nelems, pe); \
    }

SHMEMC_EMIT_PUT(char, char)
SHMEMC_EMIT_PUT(short, short)
SHMEMC_EMIT_PUT(int, int)
SHMEMC_EMIT_PUT(long, long)
SHMEMC_EMIT_PUT(float, float)
SHMEMC_EMIT_PUT(double, double)
SHMEMC_EMIT_PUT(longlong, long long)
SHMEMC_EMIT_PUT(longdouble, long double)
