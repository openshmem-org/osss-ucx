#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"

void
shmemc_iget(void *target, const void *source,
            ptrdiff_t tst, ptrdiff_t sst,
            size_t nelems, int pe)
{
    size_t ti = 0, si = 0;
    size_t i;

    for (i = 0; i < nelems; i += 1) {
        shmemc_get(&(target[ti]), &(source[si]), 1, pe);
        ti += tst;
        si += sst;
    }
}

#if 0
SHMEMC_EMIT_IGET(char, char)
SHMEMC_EMIT_IGET(short, short)
SHMEMC_EMIT_IGET(int, int)
SHMEMC_EMIT_IGET(long, long)
SHMEMC_EMIT_IGET(float, float)
SHMEMC_EMIT_IGET(double, double)
SHMEMC_EMIT_IGET(longlong, long long)
SHMEMC_EMIT_IGET(longdouble, long double)
#endif
