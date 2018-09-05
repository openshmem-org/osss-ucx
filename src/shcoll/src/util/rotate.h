//
// Created by Srdan Milakovic on 5/18/18.
//

#ifndef OPENSHMEM_COLLECTIVE_ROUTINES_ROTATE_H
#define OPENSHMEM_COLLECTIVE_ROUTINES_ROTATE_H

#include <stddef.h>

void rotate_inplace(char *arr, size_t size, size_t dist);

void rotate(char *arr, size_t size, size_t dist);

#endif //OPENSHMEM_COLLECTIVE_ROUTINES_ROTATE_H
