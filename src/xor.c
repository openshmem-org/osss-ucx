/*
 * TODO these are all just stubs, of course, for now
 */

/*
 * exclusive-xor
 */

#define SHMEMX_TYPE_XOR(_name, _type)                                   \
    void                                                                \
    shmemx_##_name##_xor(_type *target, _type value, int pe)            \
    {                                                                   \
    }

SHMEMX_TYPE_XOR(int, int);
SHMEMX_TYPE_XOR(long, long);
SHMEMX_TYPE_XOR(longlong, long long);
