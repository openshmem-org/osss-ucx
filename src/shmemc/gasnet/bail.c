#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>             /* note dummy */

#define MSG_BUF_SIZE 256

/**
 * Handle error messages while initializing the comms layer.  We don't
 * have access to the trace sub-system yet, since it depends on comms
 * being up to get PE and other informational output
 */

void
shmemc_bailout(char *fmt, ...)
{
    char tmp1[MSG_BUF_SIZE];
    char tmp2[MSG_BUF_SIZE];    /* incoming args */
    va_list ap;

    strncpy(tmp1, "INTERNAL COMMS ERROR: ", MSG_BUF_SIZE);

    va_start(ap, fmt);
    vsnprintf(tmp2, MSG_BUF_SIZE, fmt, ap);
    va_end(ap);

    strncat(tmp1, tmp2, strlen (tmp2));
    strncat(tmp1, "\n", 1);

    fputs(tmp1, stderr);
    fflush(stderr);

    exit(1);
}
