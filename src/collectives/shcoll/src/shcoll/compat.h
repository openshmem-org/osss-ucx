#ifndef _SHCOLL_COMPAT_H
#define _SHCOLL_COMPAT_H 1

#include <shmem.h>

#if SHMEM_MAJOR_VERSION == 1 && SHMEM_MINOR_VERSION < 4

#define shmem_sync_all shmem_barrier_all
#define shmem_long_atomic_inc shmem_long_inc
#define shmem_long_atomic_fetch_add shmem_long_fadd
#define shmem_long_atomic_add shmem_long_add
#define shmem_long_atomic_fetch shmem_long_fetch
#define shmem_size_atomic_set shmem_long_set
#define shmem_size_atomic_add shmem_long_fadd
#define shmem_size_wait_until shmem_long_wait_until
#define shmem_size_put shmem_long_put
#define shmem_size_p shmem_long_p

#include <string.h>

static void *shmem_calloc(size_t count, size_t size)
{
    void *mem = shmem_malloc(count * size);

    memset(mem, 0, count * size);
    return mem;
}

#endif  /* version check */

#ifndef CRAY_SHMEM_NUMVERSION

static void shmem_putmem_signal_nb(void *dest,
                                   const void *source,
                                   size_t nelems,
                                   uint64_t *sig_addr,
                                   uint64_t sig_value,
                                   int pe,
                                   void **transfer_handle)
{
    shmem_putmem_nbi(dest, source, nelems, pe);
    shmem_fence();
    shmem_uint64_p(sig_addr, sig_value, pe);
}

#endif  /* CRAY_SHMEM_NUMVERSION */

#define SHMEM_IPUT_NBI_DEFINITION(_size)                            \
    inline static void shmem_iput##_size##_nbi(void *dest,          \
                                               const void *source,  \
                                               ptrdiff_t dst,       \
                                               ptrdiff_t sst,       \
                                               size_t nelems,       \
                                               int pe)              \
    {                                                               \
        size_t i;                                                   \
        uint##_size##_t *dest_ptr =  (uint##_size##_t *) dest;      \
        const uint##_size##_t *source_ptr =                         \
            (const uint##_size##_t *) source;                       \
                                                                    \
        for (i = 0; i < nelems; i++) {                              \
            shmem_put##_size##_nbi(dest_ptr, source_ptr, 1, pe);    \
            dest_ptr += dst;                                        \
            source_ptr += sst;                                      \
        }                                                           \
    }                                                               \

SHMEM_IPUT_NBI_DEFINITION(32)
SHMEM_IPUT_NBI_DEFINITION(64)

#endif  /* ! _SHCOLL_COMPAT_H */
