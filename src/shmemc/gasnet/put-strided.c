#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"

#define SHMEMC_EMIT_IPUT(_name, _type)                                  \
    void                                                                \
    shmemc_##_name##_iput(_type *target, const _type *source,           \
                          ptrdiff_t tst, ptrdiff_t sst,                 \
                          size_t nelems, int pe)                        \
    {                                                                   \
        size_t ti = 0, si = 0;                                          \
        size_t i;                                                       \
        for (i = 0; i < nelems; i += 1) {                               \
            shmemc_##_name##_p(& (target[ti]), source[si], pe);         \
            ti += tst;                                                  \
            si += sst;                                                  \
        }                                                               \
    }

SHMEMC_EMIT_IPUT(char, char)
SHMEMC_EMIT_IPUT(short, short)
SHMEMC_EMIT_IPUT(int, int)
SHMEMC_EMIT_IPUT(long, long)
SHMEMC_EMIT_IPUT(float, float)
SHMEMC_EMIT_IPUT(double, double)
SHMEMC_EMIT_IPUT(longlong, long long)
SHMEMC_EMIT_IPUT(longdouble, long double)
