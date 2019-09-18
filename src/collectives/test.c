#include "table.h"

#include <stdio.h>

int
main(int argc, char *argv[])
{
    int i, r;

    for (i = 1; i < argc; ++i) {
        printf("barrier_all: %s -> ", argv[i]);

        r = register_alltoalls(argv[i]);
        if (r == 0) {
            printf("registered OK");
        }
        else {
            printf("unknown (returned %d)", r);
        }
        printf("\n");

    }

    return 0;
}
