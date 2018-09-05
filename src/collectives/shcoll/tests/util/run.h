//
// Created by Srdan Milakovic on 5/21/18.
//

#ifndef OPENSHMEM_COLLECTIVE_ROUTINES_RUN_H
#define OPENSHMEM_COLLECTIVE_ROUTINES_RUN_H

#include <shmem.h>
#include "debug.h"

#ifdef CSV

    #define RUN(_func, _name, _iterations, _count, ...)                                             \
        do {                                                                                        \
            double t = test_##_func(shcoll_##_func##_##_name, _iterations, _count, __VA_ARGS__);    \
                                                                                                    \
            if (shmem_my_pe() == 0) {                                                               \
                gprintf("%s,%d,%d,%zu,%lf,%.10lf\n",                                                \
                    #_name, shmem_n_pes(), _iterations, _count, t, t * 1e3 / _iterations);          \
            }                                                                                       \
        } while (0)                                                                                 \


    #define RUNC(_cond, _func, _name, _iterations, _count, ...)                                     \
        do {                                                                                        \
            if (_cond) {                                                                            \
                RUN(_func, _name, _iterations, _count, __VA_ARGS__);                                \
            } else if (shmem_my_pe() == 0) {                                                        \
                gprintf("%s,%d,%d,%d,skipped,skipped\n", #_name, shmem_n_pes(), -1, -1);            \
            }                                                                                       \
        } while (0)                                                                                 \

#else

    #define RUN(_func, _name, ...)                                                                  \
        do {                                                                                        \
            if (shmem_my_pe() == 0) {                                                               \
                gprintf(#_name ": ");                                                               \
            }                                                                                       \
                                                                                                    \
            double t = test_##_func(shcoll_##_func##_##_name, __VA_ARGS__);                         \
                                                                                                    \
            if (shmem_my_pe() == 0) {                                                               \
                gprintf("%lf\n", t);                                                                \
            }                                                                                       \
        } while (0)                                                                                 \

    #define RUNC(_cond, _func, _name, ...)                                                          \
        do {                                                                                        \
            if (_cond) {                                                                            \
                RUN(_func, _name, __VA_ARGS__);                                                     \
            } else if (shmem_my_pe() == 0) {                                                        \
                gprintf(#_name ": skipped\n");                                                      \
            }                                                                                       \
        } while (0)                                                                                 \

#endif


#endif //OPENSHMEM_COLLECTIVE_ROUTINES_RUN_H
