/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <sys/types.h>

const char *
shmemu_plural(size_t n)
{
    if (n == 1) {
        return "";
    }
    else {
        return "s";
    }
}
