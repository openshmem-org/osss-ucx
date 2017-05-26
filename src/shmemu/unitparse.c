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
    size_t bytes = 1;

    u = tolower(u);
    while (*usp != '\0') {
        bytes *= multiplier;
        if (*usp == u) {
            foundit = 1;
            break;
            /* NOT REACHED */
        }
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
    char *units;
    double ret;
    int s = -1;

    ret = strtod(size_str, &units);

    if ((ret == 0) && (units == size_str)) {
        *bytes_p = 0;
    }
    else {
        size_t b;

        if (parse_unit(*units, &b) == 0) {
            *bytes_p = (size_t) (b * ret);
            s = 0;
        }
    }

    return s;
}
