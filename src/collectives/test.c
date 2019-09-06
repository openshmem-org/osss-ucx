#include "table.h"

#include <stdio.h>

int
main(int argc, char *argv[])
{
    int i;

    for (i = 1; i < argc; ++i) {
        void *ba;
        void *b32, *b64;

        barrier_all_lookup(argv[i], &ba);
        printf("barrier_all: %s -> %p\n", argv[i], ba);

        broadcast_lookup(argv[i], &b32, &b64);
        printf("broadcast: %s -> { 32: %p, 64: %p }\n", argv[i], b32, b64);
    }

    return 0;
}
