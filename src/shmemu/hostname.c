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
    int s;

#if defined(HAVE_GETHOSTNAME)

    s = gethostname(hostname, MAXHOSTNAMELEN);
    return (s == 0) ? (char *) hostname : NULL;

#elif defined(HAVE_UNAME)

    {
        struct utsname u;

        s = uname(&u);

        if (s == 0) {
            strncpy(hostname, MAXHOSTNAMELEN, u.nodename);
            return (char *) hostname;
        }

        return NULL;
    }

#else

    return NULL;

#endif /* hostname check */
}
