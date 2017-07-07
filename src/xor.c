#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

/*
 * TODO these are all just stubs, of course, for now
 */

/*
 * exclusive-xor
 */

#ifdef ENABLE_PSHMEM
#pragma weak shmemx_int_xor = pshmemx_int_xor
#define shmemx_int_xor pshmemx_int_xor
#pragma weak shmemx_long_xor = pshmemx_long_xor
#define shmemx_long_xor pshmemx_long_xor
#pragma weak shmemx_longlong_xor = pshmemx_longlong_xor
#define shmemx_longlong_xor pshmemx_longlong_xor
#endif /* ENABLE_PSHMEM */

#define SHMEMX_TYPE_XOR(_name, _type)                                   \
    void                                                                \
    shmemx_##_name##_xor(_type *target, _type value, int pe)            \
    {                                                                   \
    }

SHMEMX_TYPE_XOR(int, int)
SHMEMX_TYPE_XOR(long, long)
SHMEMX_TYPE_XOR(longlong, long long)
