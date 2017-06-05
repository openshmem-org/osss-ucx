#ifndef _PE_H
#define _PE_H 1

#include "comms.h"

typedef enum shmem_status {
    SHMEM_PE_SHUTDOWN = 0,
    SHMEM_PE_UP,
    SHMEM_PE_UNKNOWN
} shmem_status_t;

typedef struct pe_info {
    /* per-comms layer info */
    comms_info_t comms;

    /* rank info */
    int rank;
    int nranks;

    /* up, down, out to lunch etc */
    shmem_status_t status;

    /* how many times library has been initialized */
    int refcount;
} pe_info_t;

extern pe_info_t proc;

#endif /* ! _PE_H */
