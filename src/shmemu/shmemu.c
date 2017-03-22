#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/time.h>
#include <assert.h>

#include "shmemu.h"

/*
 * set up hostname
 */

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

/*
 * timers
 */

static double epoch;

static
inline
double
read_time(void)
{
    struct timeval t;

    gettimeofday(&t, NULL);

    return (double) (t.tv_sec + (t.tv_usec / 1.0e6));
}

void
shmemu_timer_init(void)
{
    epoch = read_time();
}

void
shmemu_timer_finalize(void)
{
    return;
}

double
shmemu_timer(void)
{
    return read_time() - epoch;
}

/*
 * setup and teardown
 */

void
shmemu_init(void)
{
    shmemu_timer_init();
}

void
shmemu_finalize(void)
{
    shmemu_timer_finalize();
}
