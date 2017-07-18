#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"

void
shmemc_iput(void *target, const void *source,
            ptrdiff_t tst, ptrdiff_t sst,
            size_t nelems, int pe)
{
    size_t ti = 0, si = 0;
    size_t i;

    for (i = 0; i < nelems; i += 1) {
        shmemc_put(&(target[ti]), &(source[si]), 1, pe);
        ti += tst;
        si += sst;
    }
}

#if 0
SHMEMC_EMIT_IPUT(char, char)
SHMEMC_EMIT_IPUT(short, short)
SHMEMC_EMIT_IPUT(int, int)
SHMEMC_EMIT_IPUT(long, long)
SHMEMC_EMIT_IPUT(longlong, long long)
SHMEMC_EMIT_IPUT(longdouble, long double)
SHMEMC_EMIT_IPUT(float, float)
SHMEMC_EMIT_IPUT(double, double)
#endif
