/*
 * This is the Fortran interface.  Parameter-less routines are
 * the same as C, but with the standard underscore mangling we all love.
 *
 * Also provides pass-by-reference wrappers to translate C value params
 *
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdint.h>
#include <stddef.h>             /* ptrdiff_t */
#include <string.h>

#include "fortran-common.h"
#include "shmemx.h"

/*
 * atomic xor
 *
 * Proposed by IBM Zurich
 *
 */

#ifdef ENABLE_PSHMEM
#pragma weak shmemx_int4_xor_ = pshmemx_int4_xor_
#define shmemx_int4_xor_ pshmemx_int4_xor_
#pragma weak shmemx_int8_xor_ = pshmemx_int8_xor_
#define shmemx_int8_xor_ pshmemx_int8_xor_
#endif /* ENABLE_PSHMEM */

void
FORTRANIFY(shmemx_int4_xor)(int *target, int *value, int *pe)
{
    shmemx_int_xor(target, *value, *pe);
}

void
FORTRANIFY(shmemx_int8_xor)(long *target, long *value, int *pe)
{
    shmemx_long_xor(target, *value, *pe);
}

#if 0

/*
 * WORK IN PROGRESS
 *
 *
 *
 * non-blocking putss
 */

#define SHMEMX_FORTRAN_PUT_NB(_fname, _cname, _ctype)                   \
    void                                                                \
    FORTRANIFY(shmemx_##_fname##_put_nb)(_ctype *target, const _ctype *source, \
                                         int *size, int *pe,            \
                                         shmemx_request_handle_t *desc) \
    {                                                                   \
        shmemx_##_cname##_put_nb(target, source, *size, *pe, desc);     \
    }

#define SHMEMX_FORTRAN_PUT_SIZE_NB(Size, _cname, _ctype)                \
    void                                                                \
    FORTRANIFY(shmemx_put##Size##_nb)(_ctype *target, const _ctype *source, \
                                      int *size, int *pe,               \
                                      shmemx_request_handle_t *desc)    \
    {                                                                   \
        shmemx_##_cname##_put_nb(target, source, *size, *pe, desc);     \
    }

/* SHMEMX_FORTRAN_PUT_NB(character, char, char) */
SHMEMX_FORTRAN_PUT_NB(double, double, double)
SHMEMX_FORTRAN_PUT_NB(integer, int, int)
SHMEMX_FORTRAN_PUT_NB(logical, int, int)
SHMEMX_FORTRAN_PUT_NB(real, int, int)
/* SHMEMX_FORTRAN_PUT_NB(complex, complexf, COMPLEXIFY(float)) */
SHMEMX_FORTRAN_PUT_SIZE_NB(4, int, int)
SHMEMX_FORTRAN_PUT_SIZE_NB(8, long, long)
SHMEMX_FORTRAN_PUT_SIZE_NB(32, int, int)
SHMEMX_FORTRAN_PUT_SIZE_NB(64, long, long)
SHMEMX_FORTRAN_PUT_SIZE_NB(128, longlong, long long)

/*
 * non-blocking gets
 */

#define SHMEMX_FORTRAN_GET_NB(_fname, _cname, _ctype)                   \
    void                                                                \
    FORTRANIFY(shmemx_##_fname##_get_nb)(_ctype *target, const _ctype *source, \
                                         int *size, int *pe,            \
                                         shmemx_request_handle_t *desc) \
    {                                                                   \
        shmemx_##_cname##_get_nb(target, source, *size, *pe, desc);     \
    }

#define SHMEMX_FORTRAN_GET_SIZE_NB(Size, _cname, _ctype)                \
    void                                                                \
    FORTRANIFY(shmemx_get##Size##_nb)(_ctype *target, const _ctype *source, \
                                      int *size, int *pe,               \
                                      shmemx_request_handle_t *desc)    \
    {                                                                   \
        shmemx_##_cname##_get_nb(target, source, *size, *pe, desc);     \
    }

/* SHMEMX_FORTRAN_GET_NB(character, char, char) */
SHMEMX_FORTRAN_GET_NB(double, double, double)
SHMEMX_FORTRAN_GET_NB(integer, int, int)
SHMEMX_FORTRAN_GET_NB(logical, int, int)
SHMEMX_FORTRAN_GET_NB(real, int, int)
/* SHMEMX_FORTRAN_GET_NB(complex, complexf, COMPLEXIFY(float)) */
SHMEMX_FORTRAN_GET_SIZE_NB(4, int, int)
SHMEMX_FORTRAN_GET_SIZE_NB(8, long, long)
SHMEMX_FORTRAN_GET_SIZE_NB(32, int, int)
SHMEMX_FORTRAN_GET_SIZE_NB(64, long, long)
SHMEMX_FORTRAN_GET_SIZE_NB(128, longlong, long long)

#endif

double
FORTRANIFY(shmemx_wtime)(void)
{
    return shmemx_wtime();
}

#if 0

uintptr_t *
FORTRANIFY(shmemx_lookup_remote_addr)(uintptr_t *addr, int *pe)
{
    return shmemx_lookup_remote_addr(addr, *pe);
}

int
FORTRANIFY(shmemx_fence_test)(void)
{
    return shmemx_fence_test();
}

int
FORTRANIFY(shmemx_quiet_test)(void)
{
    return shmemx_quiet_test();
}

#endif
