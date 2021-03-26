/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/param.h>

#ifdef HAVE_UNAME
# include <sys/utsname.h>
#endif /* HAVE_UNAME */

void
shmemc_nodename_init(void)
{
#if defined(HAVE_GETHOSTNAME)

    char nodename[MAXHOSTNAMELEN];
    const int s = gethostname(nodename, MAXHOSTNAMELEN);

    if (s == 0) {
        proc.nodename = strdup(nodename); /* free@end */
        return;
        /* NOT REACHED */
    }

#elif defined(HAVE_UNAME)

    struct utsname u;
    const int s = uname(&u);

    if (s == 0) {
        proc.nodename = strdup(u.nodename); /* free@end */
        return;
        /* NOT REACHED */
    }

#endif /* hostname check */

    proc.nodename = strdup("unknown"); /* free@end */
}

void
shmemc_nodename_finalize(void)
{
    free(proc.nodename);
}
