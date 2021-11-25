/* For license: see LICENSE file at top-level */

#ifndef _SHMEM_PUTGET_COMPLEX_H
#define _SHMEM_PUTGET_COMPLEX_H 1

#define DECL_TYPED_CTX_PUT_PROTO(_name, _type)                      \
    void shmem_ctx_##_name##_put(shmem_ctx_t ctx,                   \
                                 _type *dest, const _type *src,     \
                                 size_t nelems, int pe);            \
    void shmem_ctx_##_name##_put_nbi(shmem_ctx_t ctx,               \
                                     _type *dest,                   \
                                     const _type *src,              \
                                     size_t nelems, int pe);        \
    void shmem_ctx_##_name##_iput(shmem_ctx_t ctx,                  \
                                  _type *dest,                      \
                                  const _type *src,                 \
                                  ptrdiff_t tst, ptrdiff_t sst,     \
                                  size_t nelems, int pe);

DECL_TYPED_CTX_PUT_PROTO(complexf, COMPLEXIFY(float))
DECL_TYPED_CTX_PUT_PROTO(complexd, COMPLEXIFY(double))

#define DECL_TYPED_CTX_GET_PROTO(_name, _type)                      \
    void shmem_ctx_##_name##_get(shmem_ctx_t ctx,                   \
                                 _type *dest, const _type *src,     \
                                 size_t nelems, int pe);            \
    void shmem_ctx_##_name##_get_nbi(shmem_ctx_t ctx,               \
                                     _type *dest,                   \
                                     const _type *src,              \
                                     size_t nelems, int pe);        \
    void shmem_ctx_##_name##_iget(shmem_ctx_t ctx,                  \
                                  _type *dest,                      \
                                  const _type *src,                 \
                                  ptrdiff_t tst, ptrdiff_t sst,     \
                                  size_t nelems, int pe);

DECL_TYPED_CTX_GET_PROTO(complexf, COMPLEXIFY(float))
DECL_TYPED_CTX_GET_PROTO(complexd, COMPLEXIFY(double))

#define DECL_TYPED_CTX_P_PROTO(_name, _type)                    \
    void shmem_ctx_##_name##_p(shmem_ctx_t ctx,                 \
                               _type *addr, _type val, int pe);

DECL_TYPED_CTX_P_PROTO(complexf, COMPLEXIFY(float))
DECL_TYPED_CTX_P_PROTO(complexd, COMPLEXIFY(double))

#define DECL_TYPED_CTX_G_PROTO(_name, _type)                \
    _type shmem_ctx_##_name##_g(shmem_ctx_t ctx,            \
                                const _type *addr, int pe); \

DECL_TYPED_CTX_G_PROTO(complexf, COMPLEXIFY(float))
DECL_TYPED_CTX_G_PROTO(complexd, COMPLEXIFY(double))

#endif  /* ! _SHMEM_PUTGET_COMPLEX_H */
