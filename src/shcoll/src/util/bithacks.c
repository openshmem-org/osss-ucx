//
// Created by Srdan Milakovic on 6/1/18.
//

#include <limits.h>
#include "bithacks.h"


/*
 * https://graphics.stanford.edu/~seander/bithacks.html#ReverseParallel
 */
int reverse_bits(int number, int nbits) {
    unsigned int a = number;
    unsigned int s = sizeof(a) * CHAR_BIT; // bit size; must be power of 2
    unsigned int mask = (unsigned int) ~0;
    while ((s >>= 1) > 0)
    {
        mask ^= (mask << s);
        a = ((a >> s) & mask) | ((a << s) & ~mask);
    }
    return a >> (sizeof(a) * CHAR_BIT - nbits);
}