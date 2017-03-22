#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/param.h>

#ifdef HAVE_UNAME
# include <sys/utsname.h>
#endif /* HAVE_UNAME */

int
shmemu_gethostname(char *name, int maxlen)
{
#if defined(HAVE_GETHOSTNAME)

    return gethostname(name, maxlen);

#elif defined(HAVE_UNAME)

    {
        struct utsname u;
        int s = uname(&u);

        if (s == 0) {
            strncpy(name, maxlen, u.nodename);
        }

        return s;
    }

#else

    return -1;

#endif /* hostname check */
}
