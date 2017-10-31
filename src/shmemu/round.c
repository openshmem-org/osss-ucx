/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <sys/types.h>
#include <unistd.h>

void *
shmemu_round_down_address_to_pagesize(void *addr)
{
    const long psz = sysconf(_SC_PAGESIZE);
    /* make sure aligned to page size multiples */
    const size_t mod = (size_t) addr % psz;

    if (mod != 0) {
        const size_t div = (size_t) addr / psz;

        addr = (void *) ((div - 1) * psz);
    }

    return addr;
}
