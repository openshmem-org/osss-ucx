/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"

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

static const char units_string[] = "KMGTPE";
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
    char *usp = (char *) units_string;
    size_t bytes = multiplier;

    u = toupper(u);
    while (*usp != '\0') {
        if (*usp == u) {
            foundit = 1;
            break;
            /* NOT REACHED */
        }
        bytes *= multiplier;
        ++usp;
    }

    if (foundit) {
        *sp = bytes;
        return 0;
    }
    else {
        *sp = 0;
        return -1;
    }
}

/**
 * segment size can be expressed with scaling units.  Parse those.
 *
 * *bytes_p is scaled segment size
 *
 * Return 0 if parsed, -1 if not
 */

int
shmemu_parse_size(const char *size_str, size_t *bytes_p)
{
    char *units;                /* scaling factor if given */
    double bytes;

    bytes = strtod(size_str, &units);
    if (bytes < 0.0) {
        return -1;
        /* NOT REACHED */
    }

    if (*units != '\0') {
        /* something that could be a unit */
        size_t b;

        /* but we don't know what that unit is */
        if (parse_unit(*units, &b) != 0) {
            return -1;
            /* NOT REACHED */
        }

        /* scale for return */
        bytes *= b;
    }

    *bytes_p = (size_t) bytes;

    return 0;
}

/*
 * do the reverse of parse_size: put a human-readble form of "bytes"
 * in "buf"
 *
 * Return 0 if successful, -1 if not
 */

int
shmemu_human_number(double bytes, char *buf, size_t buflen)
{
    char *walk = (char *) units_string;
    unsigned wc = 0;            /* walk count */
    size_t divvy = multiplier;
    double b = bytes;
    char unit = '\0';

    while (*walk) {
        const size_t d = (size_t) bytes / divvy;

        /* find when we've gone too far */
        if (d == 0) {
            if (wc > 0) {
                walk -= 1;
                unit = *walk;
            }
            break;
            /* NOT REACHED */
        }
        ++wc, ++walk;
        divvy *= multiplier;
        b /= multiplier;
    }

    snprintf(buf, buflen, "%.1f%c", b, unit);

    return 0;
}

/*
 * human-readable option setting
 */

const char *
shmemu_human_option(int v)
{
    return (v == 0) ? "no" : "yes";
}
