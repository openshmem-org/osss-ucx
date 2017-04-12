#include <sys/types.h>
#include <stddef.h>
#include <string.h>

/* TODO dummy implementation */

void
shmemc_get(void *dest, const void *src,
           size_t nbytes, int pe)
{
    memcpy(dest, src, nbytes);
}

void
shmemc_get_nbi(void *dest, const void *src,
               size_t nbytes, int pe)
{
    memcpy(dest, src, nbytes);
}

void
shmemc_iget(void *dest, const void *src, size_t nbytes,
            ptrdiff_t tst, ptrdiff_t sst, int pe)
{
    memcpy(dest, src, nbytes);
}
