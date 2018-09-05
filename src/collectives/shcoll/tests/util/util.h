//
// Created by Srdan Milakovic on 5/17/18.
//

#ifndef OPENSHMEM_COLLECTIVE_ROUTINES_UTIL_H
#define OPENSHMEM_COLLECTIVE_ROUTINES_UTIL_H

#include <time.h>

typedef unsigned long long time_ns_t;

static unsigned long long current_time_ns() {
    struct timespec t = {0, 0};
    clock_gettime(CLOCK_MONOTONIC, &t);
    unsigned long long s = 1000000000ULL * (unsigned long long) t.tv_sec;
    return (((unsigned long long) t.tv_nsec)) + s;
}


#endif //OPENSHMEM_COLLECTIVE_ROUTINES_UTIL_H
