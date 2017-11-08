/* For license: see LICENSE file at top-level */

#ifndef FORTRAN_COMMON_H
#define FORTRAN_COMMON_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem/defs.h"

#define FORTRANIFY(_sym) FC_FUNC_(_sym, _sym)

#define FORTRANIFY_VOID_VOID(_f)                \
    void FORTRANIFY(_f) (void) { _f(); }

#define FORTRANIFY_CACHE(_name)                 \
    void                                        \
    FORTRANIFY(_name)(void *target)             \
    {                                           \
        _name(target);                          \
    }

/* these are part of Fortran but not in C API */

#define FORTRAN_EXTERN_COMPLEX_OPS(_opname, _optype, _type)             \
    extern void                                                         \
    shmem_ctx_complex##_optype##_##_opname(shmem_ctx_t ctx,             \
                                           COMPLEXIFY(_type) * dest,    \
                                           const COMPLEXIFY(_type) * src, \
                                           size_t nelems, int pe);

FORTRAN_EXTERN_COMPLEX_OPS(put, f, float)
FORTRAN_EXTERN_COMPLEX_OPS(put, d, double)

FORTRAN_EXTERN_COMPLEX_OPS(get, f, float)
FORTRAN_EXTERN_COMPLEX_OPS(get, d, double)

FORTRAN_EXTERN_COMPLEX_OPS(put_nbi, f, float)
FORTRAN_EXTERN_COMPLEX_OPS(put_nbi, d, double)

FORTRAN_EXTERN_COMPLEX_OPS(get_nbi, f, float)
FORTRAN_EXTERN_COMPLEX_OPS(get_nbi, d, double)

#undef FORTRAN_EXTERN_COMPLEX_OPS

#define FORTRAN_COMPLEX_OPS(_opname, _optype, _type)                    \
    inline static void                                                  \
    shmem_complex##_optype##_##_opname(COMPLEXIFY(_type) * dest,        \
                                       const COMPLEXIFY(_type) * src,   \
                                       size_t nelems, int pe)           \
    {                                                                   \
        shmem_ctx_complex##_optype##_##_opname(SHMEM_CTX_DEFAULT,       \
                                               dest, src, nelems, pe);  \
    }

FORTRAN_COMPLEX_OPS(put, f, float)
FORTRAN_COMPLEX_OPS(put, d, double)

FORTRAN_COMPLEX_OPS(get, f, float)
FORTRAN_COMPLEX_OPS(get, d, double)

FORTRAN_COMPLEX_OPS(put_nbi, f, float)
FORTRAN_COMPLEX_OPS(put_nbi, d, double)

FORTRAN_COMPLEX_OPS(get_nbi, f, float)
FORTRAN_COMPLEX_OPS(get_nbi, d, double)

#undef FORTRAN_COMPLEX_OPS

#define FORTRAN_EXTERN_STRIDED_COMPLEX_OPS(_opname, _optype, _type)     \
    extern void                                                         \
    shmem_ctx_complex##_optype##_i##_opname(shmem_ctx_t ctx,            \
                                            COMPLEXIFY(_type) * dest,   \
                                            const COMPLEXIFY(_type) * src, \
                                            size_t tst, size_t sst,     \
                                            size_t nelems, int pe);

FORTRAN_EXTERN_STRIDED_COMPLEX_OPS(put, f, float)
FORTRAN_EXTERN_STRIDED_COMPLEX_OPS(put, d, double)

FORTRAN_EXTERN_STRIDED_COMPLEX_OPS(get, f, float)
FORTRAN_EXTERN_STRIDED_COMPLEX_OPS(get, d, double)

#undef FORTRAN_EXTERN_STRIDED_COMPLEX_OPS

#define FORTRAN_STRIDED_COMPLEX_OPS(_opname, _optype, _type)            \
    inline static void                                                  \
    shmem_complex##_optype##_i##_opname(COMPLEXIFY(_type) * dest,       \
                                        const COMPLEXIFY(_type) * src,  \
                                        size_t tst, size_t sst,         \
                                        size_t nelems, int pe)          \
    {                                                                   \
        shmem_ctx_complex##_optype##_i##_opname(SHMEM_CTX_DEFAULT,      \
                                                dest, src,              \
                                                tst, sst,               \
                                                nelems, pe);            \
    }

FORTRAN_STRIDED_COMPLEX_OPS(put, f, float)
FORTRAN_STRIDED_COMPLEX_OPS(put, d, double)

FORTRAN_STRIDED_COMPLEX_OPS(get, f, float)
FORTRAN_STRIDED_COMPLEX_OPS(get, d, double)

#undef FORTRAN_STRIDED_COMPLEX_OPS

#endif /* ! FORTRAN_COMMON_H */
