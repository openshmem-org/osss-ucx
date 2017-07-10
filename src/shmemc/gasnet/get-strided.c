#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"

#define SHMEMC_EMIT_IGET(_name, _type)                                  \
    void                                                                \
    shmemc_##_name##_iget(_type *target, const _type *source,           \
                          ptrdiff_t tst, ptrdiff_t sst,                 \
                          size_t nelems, int pe)                        \
    {                                                                   \
        size_t ti = 0, si = 0;                                          \
        size_t i;                                                       \
        for (i = 0; i < nelems; i += 1) {                               \
            shmemc_##_name##_g(& (target[ti]), source[si], pe);         \
            ti += tst;                                                  \
            si += sst;                                                  \
        }                                                               \
    }

SHMEMC_EMIT_IGET(char, char)
SHMEMC_EMIT_IGET(short, short)
SHMEMC_EMIT_IGET(int, int)
SHMEMC_EMIT_IGET(long, long)
SHMEMC_EMIT_IGET(float, float)
SHMEMC_EMIT_IGET(double, double)
SHMEMC_EMIT_IGET(longlong, long long)
SHMEMC_EMIT_IGET(longdouble, long double)
