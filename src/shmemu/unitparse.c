/* For license: see LICENSE file at top-level */

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
 * Take a scaling unit and work out its numeric value, in *sp
 *
 * Return 0 if good, -1 if not
 *
 */
static int
parse_unit(char u, size_t *sp)
{
    int foundit = 0;
    char *usp = units_string;
    size_t bytes = multiplier;

    u = tolower(u);
    while (*usp != '\0') {
        if (*usp == u) {
            foundit = 1;
            break;
            /* NOT REACHED */
        }
        bytes *= multiplier;
        usp += 1;
    }

    *sp = foundit ? bytes : 0;

    return foundit ? 0 : -1;
}

/**
 * segment size can be expressed with scaling units.  Parse those.
 *
 * *bytes_p is scaled segment size
 *
 * Return 0 if parsed, -1 if not
 */
int
shmemu_parse_size(char *size_str, size_t *bytes_p)
{
    char *units;                /* scaling factor if given */
    double ret;                 /* return value */

    ret = strtod(size_str, &units);
    if (ret < 0.0) {
        return -1;
        /* NOT REACHED */
    }

    *bytes_p = (size_t) ret;

    if (*units != '\0') {
        /* something that looks like a unit left over */
        size_t b;

        /* but we don't know what that unit is */
        if (parse_unit(*units, &b) != 0) {
            return -1;
            /* NOT REACHED */
        }

        /* scale for return */
        *bytes_p *= b;
    }

    return 0;
}
