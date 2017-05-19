#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>

/**
 * define accepted size units in ascending order, fits in size_t
 *
 * See section 3.1 in http://physics.nist.gov/Pubs/SP330/sp330.pdf
 *
 */

static char *units_string = "kmgtpe";
static const size_t multiplier = 1024;

/**
 * Take a scaling unit and work out its numeric value.
 *
 * Return scaled value if known, otherwise 0
 *
 */

static void
parse_unit(char u, size_t * sp, int *ok)
{
    int foundit = 0;
    char *usp = units_string;
    size_t bytes = 1;

    u = tolower (u);
    while (*usp != '\0') {
        bytes *= multiplier;
        if (*usp == u) {
            foundit = 1;
            break;
            /* NOT REACHED */
        }
        usp += 1;
    }

    if (foundit) {
        *sp = bytes;
        *ok = 1;
    }
    else {
        *sp = 0;
        *ok = 0;
    }
}

/**
 * segment size can be expressed with scaling units.  Parse those.
 *
 * Return segment size, scaled where necessary by unit
 */
void
shmemu_parse_size(char *size_str, size_t *bytes_p, int *ok_p)
{
    char unit = '\0';
    size_t ret = 0;
    char *p;

    p = size_str;
    while (*p != '\0') {
        if (!isdigit(*p)) {
            unit = *p;
            break;
            /* NOT REACHED */
        }

        ret = ret * 10 + (*p - '0');    /* works for ASCII/EBCDIC */
        p += 1;
    }

    /* if no unit, we already have value.  Otherwise, do scaling */
    if (unit == '\0') {
        *bytes_p = ret;
        *ok_p = 1;
    }
    else {
        size_t b;
        int ok;

        parse_unit(unit, &b, &ok);
        if (ok) {
            *bytes_p = b * ret;
            *ok_p = 1;
        }
    }
}
