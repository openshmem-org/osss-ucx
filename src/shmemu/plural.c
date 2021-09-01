/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <sys/types.h>
#include <string.h>

const char *
shmemu_plural(const char *noun, size_t n)
{
    int eos = strlen(noun);
    char last = noun[eos - 1];
    const char *ess = "h";

    if (n == 1) {
        return "";
        /* NOT REACHED */
    }

    if (strchr(ess, last) != NULL) {
        return "es";
    }
    else {
        return "s";
    }
}
