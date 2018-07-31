/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <unistd.h>
#include <sys/param.h>

#ifdef HAVE_UNAME
# include <sys/utsname.h>
#endif /* HAVE_UNAME */

static char hostname[MAXHOSTNAMELEN];

char *
shmemu_gethostname(void)
{
#if defined(HAVE_GETHOSTNAME)

    const int s = gethostname(hostname, MAXHOSTNAMELEN);
    return (s == 0) ? (char *) hostname : NULL;

#elif defined(HAVE_UNAME)

    {
        struct utsname u;
        const int s = uname(&u);

        if (s == 0) {
            STRNCPY_SAFE(hostname, MAXHOSTNAMELEN, u.nodename);
            return (char *) hostname;
            /* NOT REACHED */
        }

        return NULL;
    }

#else

    return NULL;

#endif /* hostname check */
}
