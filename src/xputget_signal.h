/* For license: see LICENSE file at top-level */

#ifndef _SHMEM_XPUTGET_SIGNAL_H
#define _SHMEM_XPUTGET_SIGNAL_H 1

#include "shmemu.h"
#include "shmemc.h"
#include "shmem_mutex.h"

#define SHMEMX_CTX_TYPED_PUT_SIGNAL(_name, _type)                       \
    void                                                                \
    shmemx_ctx_##_name##_put_signal(shmem_ctx_t ctx,                    \
                                    _type *dest, const _type *src,      \
                                    size_t nelems,                      \
                                    uint64_t *sig, uint64_t sig_val,    \
                                    int pe)                             \
    {                                                                   \
        const size_t nb = sizeof(_type) * nelems;                       \
                                                                        \
        SHMEMU_CHECK_INIT();                                            \
        SHMEMU_CHECK_PE_ARG_RANGE(pe, 7);                               \
        SHMEMU_CHECK_SYMMETRIC(dest, 2);                                \
        SHMEMU_CHECK_SYMMETRIC(sig, 5);                                 \
                                                                        \
        logger(LOG_RMA,                                                 \
               "%s(ctx=%lu, dest=%p, src=%p, nelems=%lu, "              \
               "sig=%p, sig_val=%lu, pe=%d)",                           \
               __func__,                                                \
               shmemc_context_id(ctx), dest, src, nelems,               \
               sig, sig_val,                                            \
               pe                                                       \
               );                                                       \
                                                                        \
        SHMEMT_MUTEX_PROTECT(shmemc_ctx_put_signal(ctx,                 \
                                                   dest, src,           \
                                                   nb,                  \
                                                   sig, sig_val,        \
                                                   pe));                \
    }

#define SHMEMX_CTX_TYPED_GET_SIGNAL(_name, _type)                       \
    void                                                                \
    shmemx_ctx_##_name##_get_signal(shmem_ctx_t ctx,                    \
                                    _type *dest, const _type *src,      \
                                    size_t nelems,                      \
                                    uint64_t *sig, uint64_t sig_val,    \
                                    int pe)                             \
    {                                                                   \
        const size_t nb = sizeof(_type) * nelems;                       \
                                                                        \
        SHMEMU_CHECK_INIT();                                            \
        SHMEMU_CHECK_PE_ARG_RANGE(pe, 7);                               \
        SHMEMU_CHECK_SYMMETRIC(src, 3);                                 \
        SHMEMU_CHECK_SYMMETRIC(sig, 5);                                 \
                                                                        \
        logger(LOG_RMA,                                                 \
               "%s(ctx=%lu, dest=%p, src=%p, nelems=%lu, "              \
               "sig=%p, sig_val=%lu, pe=%d)",                           \
               __func__,                                                \
               shmemc_context_id(ctx), dest, src, nelems,               \
               sig, sig_val,                                            \
               pe                                                       \
               );                                                       \
                                                                        \
        SHMEMT_MUTEX_PROTECT(shmemc_ctx_get_signal(ctx,                 \
                                                   dest, src,           \
                                                   nb,                  \
                                                   sig, sig_val,        \
                                                   pe));                \
    }

#define APIX_DECL_TYPED_PUT_SIGNAL(_name, _type)                    \
    void                                                            \
    shmemx_##_name##_put_signal(_type *dest, const _type *src,      \
                                size_t nelems,                      \
                                uint64_t *sig, uint64_t sig_val,    \
                                int pe)                             \
    {                                                               \
        SHMEMU_CHECK_INIT();                                        \
        SHMEMU_CHECK_PE_ARG_RANGE(pe, 6);                           \
        SHMEMU_CHECK_SYMMETRIC(dest, 1);                            \
        SHMEMU_CHECK_SYMMETRIC(sig, 4);                             \
                                                                    \
        shmemx_ctx_##_name##_put_signal(SHMEM_CTX_DEFAULT,          \
                                        dest, src,                  \
                                        nelems,                     \
                                        sig, sig_val,               \
                                        pe);                        \
        }

#define APIX_DECL_TYPED_GET_SIGNAL(_name, _type)                    \
    void                                                            \
    shmemx_##_name##_get_signal(_type *dest, const _type *src,      \
                                size_t nelems,                      \
                                uint64_t *sig, uint64_t sig_val,    \
                                int pe)                             \
    {                                                               \
        SHMEMU_CHECK_INIT();                                        \
        SHMEMU_CHECK_PE_ARG_RANGE(pe, 6);                           \
        SHMEMU_CHECK_SYMMETRIC(src, 2);                             \
        SHMEMU_CHECK_SYMMETRIC(sig, 4);                             \
                                                                    \
        shmemx_ctx_##_name##_get_signal(SHMEM_CTX_DEFAULT,          \
                                        dest, src,                  \
                                        nelems,                     \
                                        sig, sig_val,               \
                                        pe);                        \
        }

#endif  /* ! _SHMEM_XPUTGET_SIGNAL_H */
