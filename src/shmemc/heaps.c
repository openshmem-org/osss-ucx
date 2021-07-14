/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "state.h"
#include "shmemu.h"
#include "module.h"

#include <stdio.h>
#include <stdlib.h>

void
shmemc_heaps_init(void)
{
    size_t hs;
    int r;

    /* for now: could change with multiple heaps */
    proc.heaps.nheaps = 1;

    hs = proc.heaps.nheaps * sizeof(*proc.heaps.heapsize);

    proc.heaps.heapsize = (size_t *) malloc(hs);

    shmemu_assert(proc.heaps.heapsize != NULL,
                  MODULE ": can't allocate memory for %lu heap%s",
                  (unsigned long) proc.heaps.nheaps,
                  shmemu_plural(proc.heaps.nheaps));

    r = shmemu_parse_size(proc.env.heap_spec, &proc.heaps.heapsize[0]);
    shmemu_assert(r == 0,
                  MODULE ": couldn't work out requested heap size \"%s\"",
                  proc.env.heap_spec);
}

void
shmemc_heaps_finalize(void)
{
    free(proc.heaps.heapsize);
}
