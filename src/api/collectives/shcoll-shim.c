/* For license: see LICENSE file at top-level */

#include "thispe.h"
#include "shmemu.h"
#include "collectives/table.h"

#define TRY(_cname)                                             \
    {                                                           \
        const int s = register_##_cname(proc.env.coll._cname);  \
                                                                \
        if (s != 0) {                                           \
            shmemu_fatal("couldn't register collective "        \
                         "\"%s\" (s = %d)",                     \
                         #_cname, s);                           \
        }                                                       \
    }

void
collectives_init(void)
{
    TRY(alltoall);
    TRY(alltoalls);
    TRY(collect);
    TRY(fcollect);
    TRY(barrier);
    TRY(barrier_all);
    TRY(sync);
    TRY(sync_all);
    TRY(broadcast);

    /* TODO reductions */
}

void
collectives_finalize(void)
{
    return;
}

/*
 * hand off the SHMEM API to the dispatchers
 */

#ifdef ENABLE_PSHMEM
#pragma weak shmem_alltoall32 = pshmem_alltoall32
#define shmem_alltoall32 pshmem_alltoall32
#pragma weak shmem_alltoall64 = pshmem_alltoall64
#define shmem_alltoall64 pshmem_alltoall64
#endif /* ENABLE_PSHMEM */

void
shmem_alltoall32(void *target, const void *source, size_t nelems,
                 int PE_start, int logPE_stride, int PE_size, long *pSync)
{
    colls.alltoall.f32(target, source, nelems,
                       PE_start, logPE_stride, PE_size, pSync);
}

void
shmem_alltoall64(void *target, const void *source, size_t nelems,
                 int PE_start, int logPE_stride, int PE_size, long *pSync)
{
    colls.alltoall.f64(target, source, nelems,
                       PE_start, logPE_stride, PE_size, pSync);
}

#ifdef ENABLE_PSHMEM
#pragma weak shmem_alltoalls32 = pshmem_alltoalls32
#define shmem_alltoalls32 pshmem_alltoalls32
#pragma weak shmem_alltoalls64 = pshmem_alltoalls64
#define shmem_alltoalls64 pshmem_alltoalls64
#endif /* ENABLE_PSHMEM */

void
shmem_alltoalls32(void *target, const void *source,
                  ptrdiff_t dst, ptrdiff_t sst, size_t nelems,
                  int PE_start, int logPE_stride, int PE_size,
                  long *pSync)
{
    colls.alltoalls.f32(target, source,
                        dst, sst, nelems,
                        PE_start, logPE_stride, PE_size,
                        pSync);
}

void
shmem_alltoalls64(void *target, const void *source,
                  ptrdiff_t dst, ptrdiff_t sst, size_t nelems,
                  int PE_start, int logPE_stride, int PE_size,
                  long *pSync)
{
    colls.alltoalls.f64(target, source,
                        dst, sst, nelems,
                        PE_start, logPE_stride, PE_size,
                        pSync);
}

#ifdef ENABLE_PSHMEM
#pragma weak shmem_collect32 = pshmem_collect32
#define shmem_collect32 pshmem_collect32
#pragma weak shmem_collect64 = pshmem_collect64
#define shmem_collect64 pshmem_collect64
#endif /* ENABLE_PSHMEM */

void
shmem_collect32(void *target, const void *source, size_t nelems,
                int PE_start, int logPE_stride, int PE_size, long *pSync)
{
    colls.collect.f32(target, source, nelems,
                      PE_start, logPE_stride, PE_size, pSync);
}

void
shmem_collect64(void *target, const void *source, size_t nelems,
                int PE_start, int logPE_stride, int PE_size, long *pSync)
{
    colls.collect.f64(target, source, nelems,
                      PE_start, logPE_stride, PE_size, pSync);
}

#ifdef ENABLE_PSHMEM
#pragma weak shmem_fcollect32 = pshmem_fcollect32
#define shmem_fcollect32 pshmem_fcollect32
#pragma weak shmem_fcollect64 = pshmem_fcollect64
#define shmem_fcollect64 pshmem_fcollect64
#endif /* ENABLE_PSHMEM */

void
shmem_fcollect32(void *target, const void *source, size_t nelems,
                 int PE_start, int logPE_stride, int PE_size, long *pSync)
{
    colls.fcollect.f32(target, source, nelems,
                       PE_start, logPE_stride, PE_size, pSync);
}

void
shmem_fcollect64(void *target, const void *source, size_t nelems,
                 int PE_start, int logPE_stride, int PE_size, long *pSync)
{
    colls.fcollect.f64(target, source, nelems,
                       PE_start, logPE_stride, PE_size, pSync);
}

#ifdef ENABLE_PSHMEM
#pragma weak shmem_barrier = pshmem_barrier
#define shmem_barrier pshmem_barrier
#endif /* ENABLE_PSHMEM */

void
shmem_barrier(int PE_start, int logPE_stride, int PE_size, long *pSync)
{
    colls.barrier.f(PE_start, logPE_stride, PE_size, pSync);
}

/*
 * sync variables supplied by me
 */
extern long *shmemc_barrier_all_psync;
extern long *shmemc_sync_all_psync;

#ifdef ENABLE_PSHMEM
#pragma weak shmem_barrier_all = pshmem_barrier_all
#define shmem_barrier_all pshmem_barrier_all
#endif /* ENABLE_PSHMEM */

void
shmem_barrier_all(void)
{
    colls.barrier_all.f(shmemc_barrier_all_psync);
}

#ifdef ENABLE_PSHMEM
#pragma weak shmem_sync = pshmem_sync
#define shmem_sync pshmem_sync
#endif /* ENABLE_PSHMEM */

void
shmem_sync(int PE_start, int logPE_stride, int PE_size, long *pSync)
{
    colls.sync.f(PE_start, logPE_stride, PE_size, pSync);
}

#ifdef ENABLE_PSHMEM
#pragma weak shmem_sync_all = pshmem_sync_all
#define shmem_sync_all pshmem_sync_all
#endif /* ENABLE_PSHMEM */

void
shmem_sync_all(void)
{
    colls.sync_all.f(shmemc_sync_all_psync);
}

#ifdef ENABLE_PSHMEM
#pragma weak shmem_broadcast32 = pshmem_broadcast32
#define shmem_broadcast32 pshmem_broadcast32
#pragma weak shmem_broadcast64 = pshmem_broadcast64
#define shmem_broadcast64 pshmem_broadcast64
#endif /* ENABLE_PSHMEM */

void
shmem_broadcast32(void *target, const void *source,
                  size_t nelems, int PE_root, int PE_start,
                  int logPE_stride, int PE_size, long *pSync)
{
    colls.broadcast.f32(target, source,
                        nelems, PE_root, PE_start,
                        logPE_stride, PE_size, pSync);
}

void
shmem_broadcast64(void *target, const void *source,
                  size_t nelems, int PE_root, int PE_start,
                  int logPE_stride, int PE_size, long *pSync)
{
    colls.broadcast.f64(target, source,
                        nelems, PE_root, PE_start,
                        logPE_stride, PE_size, pSync);
}

/*
 * -- WIP ----------------------------------------------------------
 */

#include "collectives/reductions.h"

/*
 * reductions:
 *
 * linear
 * binomial
 * rec_dbl
 * rabenseifner
 * rabenseifner2
 *
 */
SHIM_REDUCE_ALL(rec_dbl)
