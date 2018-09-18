/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

/*
 * read out a comma-separated number list.
 *
 * return 0 if ok, non-0 if error
 */
int
shmemu_parse_csv(char *str, int **out, size_t *nout)
{
    size_t i = 0;
    char *next;
    const char *sep = ",";
    size_t nnums = 8;

    if (str == NULL) {
        return -1;
    }

    *out = (int *) malloc(nnums * sizeof(**out)); /* freed by caller */
    if (*out == NULL) {
        return -1;
        /* NOT REACHED */
    }

    /* parse the numbers out of the string */

    next = strtok(str, sep);
    while (next != NULL) {
        (*out)[i] = (int) strtol(next, NULL, 10);
        i += 1;
        if (i >= nnums) {
            nnums += 8;
            *out = (int *) realloc(*out,
                                   nnums * sizeof(**out));
        }
        next = strtok(NULL, sep);
    }

    *nout = i;

    return 0;
}
