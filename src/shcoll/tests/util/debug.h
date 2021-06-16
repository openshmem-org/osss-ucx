//
// Created by Srdan Milakovic on 5/17/18.
//

#ifndef OPENSHMEM_COLLECTIVE_ROUTINES_DEBUG_H
#define OPENSHMEM_COLLECTIVE_ROUTINES_DEBUG_H

#include "stdio.h"
#include <assert.h>

#define OUTPUT_STREAM stderr

#define gprintf(...) do { fprintf(OUTPUT_STREAM, __VA_ARGS__); fflush(OUTPUT_STREAM); } while(0);
#define PL gprintf("[%d]:%d\n", shmem_my_pe(), __LINE__);
#define PLI(A) gprintf("[%d]:%d %d\n", shmem_my_pe(), __LINE__, (A));
#define PLP(P) gprintf("[%d]:%d %p\n", shmem_my_pe(), __LINE__, (P));
#define PLII(A, B) gprintf("[%d]:%d %d %d\n", shmem_my_pe(), __LINE__, (A), (B));

#endif //OPENSHMEM_COLLECTIVE_ROUTINES_DEBUG_H
