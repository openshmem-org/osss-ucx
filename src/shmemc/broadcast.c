/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemc.h"
#include "state.h"

#include <sys/types.h>

/*
 * stupid linear broadcast during development
 */

#define SHMEMC_BROADCAST_TYPE(_name, _size)                             \
    void                                                                \
    shmemc_broadcast##_name(void *target, const void *source,           \
                            size_t nelems,                              \
                            int PE_root, int PE_start,                  \
                            int logPE_stride, int PE_size,              \
                            long *pSync)                                \
    {                                                                   \
        const int typed_nelems = nelems * _size;                        \
        const int step = 1 << logPE_stride;                             \
        const int root = (PE_root * step) + PE_start;                   \
        const int me = proc.rank;                                       \
                                                                        \
        shmemc_barrier(PE_start, logPE_stride, PE_size, pSync);         \
        if (me != root) {                                               \
            shmemc_get(target, source, typed_nelems, root);             \
        }                                                               \
    }

SHMEMC_BROADCAST_TYPE(32, 4)

SHMEMC_BROADCAST_TYPE(64, 8)
