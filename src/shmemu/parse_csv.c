/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/types.h>
#include <regex.h>

/*
 * range has beginning and end (+1 for regex engine)
 */
#define NMATCH 3

#define ASSIGN(_n)                                                  \
    do {                                                            \
        (*out)[i] = _n;                                             \
        ++i;                                                        \
        if (shmemu_unlikely(i >= nnums)) {                          \
            nnums += 8;                                             \
            *out = (int *) realloc(*out, nnums * sizeof(**out));    \
            if (shmemu_unlikely(*out == NULL)) {                    \
                return -1;                                          \
                /* NOT REACHED */                                   \
            }                                                       \
        }                                                           \
    } while (0)

inline static int
intify(const char *s)
{
    return (int) strtol(s, NULL, 10);
}

/*
 * read out a comma-separated number list.
 *
 * return 1 if ok, 0 if error
 */
int
shmemu_parse_csv(char *str, int **out, size_t *nout)
{
    size_t i = 0;
    char *next;
    const char *sep = ",";
    size_t nnums = 8;
    int s;
    regex_t range;
    regmatch_t matches[NMATCH];

    if (shmemu_unlikely(str == NULL)) {
        return 0;
        /* NOT REACHED */

    }

    s = regcomp(&range,
                "([0-9]*)-([0-9]*)",
                REG_EXTENDED);
    if (shmemu_unlikely(s != 0)) {
        return 0;
        /* NOT REACHED */
    }

    *out = (int *) malloc(nnums * sizeof(**out)); /* freed by caller */
    if (shmemu_unlikely(*out == NULL)) {
        return 0;
        /* NOT REACHED */
    }

    next = strtok(str, sep);
    while (next != NULL) {
        s = regexec(&range, next, NMATCH, matches, 0);

        if (s == REG_NOMATCH) {
            const int v = intify(next);

            ASSIGN(v);
        }
        else {
            const int s1 = matches[1].rm_so;
            const int s2 = matches[2].rm_so;
            const int v1 = intify(&next[s1]);
            const int v2 = intify(&next[s2]);
            int j;

            for (j = v1; j <= v2; ++j) {
                ASSIGN(j);
            }
        }
        next = strtok(NULL, sep);
    }

    *nout = i;

    regfree(&range);

    return 1;
}
