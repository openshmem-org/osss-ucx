#ifndef _SHMEMI_PE_H
#define _SHMEMI_PE_H 1

#include <stdbool.h>

typedef struct pe {
    int me;
    int npes;
    bool running;
} pe_t;

extern pe_t p;

#endif /* ! _SHMEMI_PE_H */
