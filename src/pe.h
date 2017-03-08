#ifndef _PE_H
#define _PE_H 1

#include <stdbool.h>

typedef struct pe {
    int me;
    int npes;
    bool running;
} pe_t;

extern pe_t p;

#endif /* ! _PE_H */
