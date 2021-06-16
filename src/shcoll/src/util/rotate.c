//
// Created by Srdan Milakovic on 5/18/18.
//

#include <stdlib.h>
#include <memory.h>
#include "rotate.h"

static inline size_t
gcd(size_t a, size_t b)
{
    while (1) {
        const size_t r = a % b;

        if (r == 0) {
            return b;
        }

        a = b;
        b = r;
    }
}

void
rotate_inplace(char *arr, size_t size, size_t dist)
{
    size_t i;

    if (dist == 0) {
        return;
    }

    size_t groups = gcd(dist, size);

    for (i = 0; i < groups; i++) {
        char temp;
        size_t j = i;

        temp = arr[i];
        while (1) {
            size_t k;

            if (j < dist) {
                k = size - dist + j;
            } else {
                k = j - dist;
            }

            if (k == i) {
                break;
            }

            arr[j] = arr[k];
            j = k;
        }
        arr[j] = temp;
    }
}

void rotate(char *arr, size_t size, size_t dist) {
    char *tmp = malloc(dist);

    memcpy(tmp, arr + (size - dist), dist);
    memmove(arr + dist, arr, (size - dist));
    memcpy(arr, tmp, dist);

    free(tmp);
}
