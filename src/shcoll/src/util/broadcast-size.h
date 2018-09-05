//
// Created by Srdan Milakovic on 6/11/18.
//

#ifndef OPENSHMEM_COLLECTIVE_ROUTINES_BROADCAST_SIZE_H
#define OPENSHMEM_COLLECTIVE_ROUTINES_BROADCAST_SIZE_H

#include <stddef.h>

void broadcast_size(size_t *value, int PE_root, int PE_start, int logPE_stride, int PE_size, long *pSync);

#endif //OPENSHMEM_COLLECTIVE_ROUTINES_BROADCAST_SIZE_H
