/* For license: see LICENSE file at top-level */

#ifndef _SHMEM_PUTGET_H
#define _SHMEM_PUTGET_H 1

#include "shmem_mutex.h"

#define SHMEM_CTX_TYPED_PUTGET(_op, _name, _type)                       \
    void                                                                \
    shmem_ctx_##_name##_##_op(shmem_ctx_t ctx,                          \
                              _type *dest, const _type *src,            \
                              size_t nelems, int pe)                    \
    {                                                                   \
        SHMEML_MUTEX_PROTECT(shmemc_ctx_##_op(ctx,                      \
                                              dest, src,                \
                                              sizeof(_type) * nelems,   \
                                              pe));                     \
    }

#define SHMEM_CTX_SIZED_PUTGET(_op, _size)                              \
    void                                                                \
    shmem_ctx_##_op####_size(shmem_ctx_t ctx,                           \
                             void *dest, const void *src,               \
                             size_t nelems, int pe)                     \
    {                                                                   \
        SHMEML_MUTEX_PROTECT(shmemc_ctx_##_op(ctx,                      \
                                              dest, src,                \
                                              _size * nelems,           \
                                              pe));                     \
    }

#define SHMEM_CTX_PUTGET_MEM(_op)                           \
    void                                                    \
    shmem_ctx_##_op##mem(shmem_ctx_t ctx,                   \
                         void *dest,                        \
                         const void *src,                   \
                         size_t nelems,                     \
                         int pe)                            \
    {                                                       \
        SHMEML_MUTEX_PROTECT(shmemc_ctx_##_op(ctx,          \
                                              dest,         \
                                              src,          \
                                              nelems,       \
                                              pe));         \
    }

#define SHMEM_CTX_TYPED_IPUTGET(_op, _name, _type)                      \
    void                                                                \
    shmem_ctx_##_name##_i##_op(shmem_ctx_t ctx,                         \
                               _type *target, const _type *source,      \
                               ptrdiff_t tst, ptrdiff_t sst,            \
                               size_t nelems, int pe)                   \
    {                                                                   \
        size_t ti = 0, si = 0;                                          \
        size_t i;                                                       \
                                                                        \
        for (i = 0; i < nelems; i += 1) {                               \
            shmem_ctx_##_name##_##_op(ctx,                              \
                                      &(target[ti]),                    \
                                      &(source[si]),                    \
                                      1, pe);                           \
            ti += tst;                                                  \
            si += sst;                                                  \
        }                                                               \
    }

#define SHMEM_CTX_SIZED_IPUTGET(_op, _size)                             \
    void                                                                \
    shmem_ctx_i##_op##_size(shmem_ctx_t ctx,                            \
                            void *target, const void *source,           \
                            ptrdiff_t tst, ptrdiff_t sst,               \
                            size_t nelems, int pe)                      \
    {                                                                   \
        size_t ti = 0, si = 0;                                          \
        size_t i;                                                       \
                                                                        \
        for (i = 0; i < nelems; i += 1) {                               \
            shmem_ctx_##_op##_size(ctx,                                 \
                                   (void *) &((char *) target)[ti],     \
                                   (void *) &((char *) source)[si],     \
                                   1, pe);                              \
            ti += tst;                                                  \
            si += sst;                                                  \
        }                                                               \
    }

#define SHMEM_CTX_TYPED_PUTGET_NBI(_op, _name, _type)                   \
    void                                                                \
    shmem_ctx_##_name##_##_op##_nbi(shmem_ctx_t ctx,                    \
                                    _type *dest, const _type *src,      \
                                    size_t nelems, int pe)              \
    {                                                                   \
        const size_t nb = sizeof(_type) * nelems;                       \
                                                                        \
        SHMEML_MUTEX_PROTECT(shmemc_ctx_##_op##_nbi(ctx,                \
                                                    dest, src,          \
                                                    nb,                 \
                                                    pe));               \
    }

#define SHMEM_CTX_SIZED_PUTGET_NBI(_op, _size)                          \
    void                                                                \
    shmem_ctx_##_op####_size##_nbi(shmem_ctx_t ctx,                     \
                                   void *dest, const void *src,         \
                                   size_t nelems, int pe)               \
    {                                                                   \
        const size_t nb = (_size) * nelems;                             \
                                                                        \
        SHMEML_MUTEX_PROTECT(shmemc_ctx_##_op##_nbi(ctx,                \
                                                    dest, src,          \
                                                    nb,                 \
                                                    pe));               \
    }

#define SHMEM_CTX_PUTGET_MEM_NBI(_op)                       \
    void                                                    \
    shmem_ctx_##_op##mem_nbi(shmem_ctx_t ctx,               \
                             void *dest,                    \
                             const void *src,               \
                             size_t nelems,                 \
                             int pe)                        \
    {                                                       \
        SHMEML_MUTEX_PROTECT(shmemc_ctx_##_op##_nbi(ctx,    \
                                                    dest,   \
                                                    src,    \
                                                    nelems, \
                                                    pe));   \
}

#define SHMEM_CTX_TYPED_P(_name, _type)                                 \
    void                                                                \
    shmem_ctx_##_name##_p(shmem_ctx_t ctx,                              \
                          _type *addr, _type val, int pe)               \
    {                                                                   \
        SHMEML_MUTEX_PROTECT(shmemc_ctx_put(ctx,                        \
                                            addr, &val,                 \
                                            sizeof(val),                \
                                            pe));                       \
    }

#define SHMEM_CTX_TYPED_G(_name, _type)                                 \
    _type                                                               \
    shmem_ctx_##_name##_g(shmem_ctx_t ctx,                              \
                          const _type *addr, int pe)                    \
    {                                                                   \
        _type val;                                                      \
                                                                        \
        SHMEML_MUTEX_PROTECT(shmemc_ctx_get(ctx,                        \
                                            &val, addr,                 \
                                            sizeof(val),                \
                                            pe));                       \
                                                                        \
        return val;                                                     \
    }

/*
 * operating on implicit default context
 */

#define API_DECL_TYPED_PUTGET(_opname, _name, _type)                    \
    void                                                                \
    shmem_##_name##_##_opname(_type *dest, const _type *src,            \
                              size_t nelems, int pe)                    \
    {                                                                   \
        shmem_ctx_##_name##_##_opname(SHMEM_CTX_DEFAULT,                \
                                      dest, src,                        \
                                      nelems, pe);                      \
    }                                                                   \
    void                                                                \
    shmem_##_name##_##_opname##_nbi(_type *dest,                        \
                                    const _type *src,                   \
                                    size_t nelems, int pe)              \
    {                                                                   \
        shmem_ctx_##_name##_##_opname##_nbi(SHMEM_CTX_DEFAULT,          \
                                         dest, src,                     \
                                         nelems, pe);                   \
    }                                                                   \
    void                                                                \
    shmem_##_name##_i##_opname(_type *dest,                             \
                               const _type *src,                        \
                               ptrdiff_t tst, ptrdiff_t sst,            \
                               size_t nelems, int pe)                   \
    {                                                                   \
        shmem_ctx_##_name##_i##_opname(SHMEM_CTX_DEFAULT,               \
                                       dest, src,                       \
                                       tst, sst,                        \
                                       sizeof(_type) * nelems, pe);     \
    }

#define API_DECL_SIZED_PUTGET(_opname, _size)                           \
    void                                                                \
    shmem_##_opname##_size(void *dest, const void *src,                 \
                           size_t nelems, int pe)                       \
    {                                                                   \
        shmem_ctx_##_opname##_size(SHMEM_CTX_DEFAULT,                   \
                                   dest, src, nelems, pe);              \
    }                                                                   \
    void                                                                \
    shmem_##_opname##_size##_nbi(void *dest, const void *src,           \
                                 size_t nelems, int pe)                 \
    {                                                                   \
        shmem_ctx_##_opname##_size##_nbi(SHMEM_CTX_DEFAULT,             \
                                         dest, src, nelems, pe);        \
    }                                                                   \
    void                                                                \
    shmem_i##_opname##_size(void *dest, const void *src,                \
                            ptrdiff_t tst, ptrdiff_t sst,               \
                            size_t nelems, int pe)                      \
    {                                                                   \
        shmem_ctx_i##_opname##_size(SHMEM_CTX_DEFAULT,                  \
                                    dest, src, tst, sst,                \
                                    nelems, pe);                        \
    }

#define API_DECL_PUTGET_MEM(_opname)                                    \
    void                                                                \
    shmem_##_opname##mem(void *dest, const void *src,                   \
                         size_t nelems, int pe)                         \
    {                                                                   \
        shmem_ctx_##_opname##mem(SHMEM_CTX_DEFAULT,                     \
                                 dest, src, nelems, pe);                \
    }                                                                   \
    void                                                                \
    shmem_##_opname##mem_nbi(void *dest, const void *src,               \
                             size_t nelems, int pe)                     \
    {                                                                   \
        shmem_ctx_##_opname##mem_nbi(SHMEM_CTX_DEFAULT,                 \
                                     dest, src, nelems, pe);            \
    }

#define API_DECL_TYPED_P(_name, _type)                  \
    void                                                \
    shmem_##_name##_p(_type *dest, _type src, int pe)   \
    {                                                   \
        shmem_ctx_##_name##_put(SHMEM_CTX_DEFAULT,      \
                                dest, &src, 1, pe);     \
    }

#define API_DECL_TYPED_G(_name, _type)              \
    _type                                           \
    shmem_##_name##_g(const _type *src, int pe)     \
    {                                               \
        _type val;                                  \
                                                    \
        shmem_ctx_##_name##_get(SHMEM_CTX_DEFAULT,  \
                                &val, src, 1, pe);  \
        return val;                                 \
    }

#endif  /* ! _SHMEM_PUTGET_H */
