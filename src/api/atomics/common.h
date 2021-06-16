/* For license: see LICENSE file at top-level */

#ifndef SHMEM_AMO_COMMON_H
#define SHMEM_AMO_COMMON_H 1

#include <bits/wordsize.h>
#include <sys/types.h>
#include <stdint.h>
#include <stddef.h>

/*
 * blocking
 */

#define API_DEF_CONST_AMO1(_op, _name, _type)                           \
    _type shmem_##_name##_atomic_##_op(const _type *target, int pe)     \
    {                                                                   \
        return shmem_ctx_##_name##_atomic_##_op(SHMEM_CTX_DEFAULT,      \
                                                target, pe);            \
    }

#define API_DEF_AMO1(_op, _name, _type)                                 \
    _type shmem_##_name##_atomic_##_op(_type *target, int pe)           \
    {                                                                   \
        return shmem_ctx_##_name##_atomic_##_op(SHMEM_CTX_DEFAULT,      \
                                                target, pe);            \
    }

#define API_DEF_AMO2(_op, _name, _type)                                 \
    _type shmem_##_name##_atomic_##_op(_type *target, _type value,      \
                                       int pe)                          \
    {                                                                   \
        return shmem_ctx_##_name##_atomic_##_op(SHMEM_CTX_DEFAULT,      \
                                                target, value, pe);     \
    }

#define API_DEF_AMO3(_op, _name, _type)                                 \
    _type shmem_##_name##_atomic_##_op(_type *target,                   \
                                       _type cond, _type value,         \
                                       int pe)                          \
    {                                                                   \
        return shmem_ctx_##_name##_atomic_##_op(SHMEM_CTX_DEFAULT,      \
                                                target, cond, value, pe); \
    }

#define API_DEF_VOID_AMO1(_op, _name, _type)                            \
    void shmem_##_name##_atomic_##_op(_type *target, int pe)            \
    {                                                                   \
        shmem_ctx_##_name##_atomic_##_op(SHMEM_CTX_DEFAULT,             \
                                         target, pe);                   \
    }

#define API_DEF_VOID_AMO2(_op, _name, _type)                            \
    void shmem_##_name##_atomic_##_op(_type *target, _type value,       \
                                      int pe)                           \
    {                                                                   \
        shmem_ctx_##_name##_atomic_##_op(SHMEM_CTX_DEFAULT,             \
                                         target, value, pe);            \
    }

#define SHMEM_CTX_TYPE_BITWISE(_opname, _name, _type)                   \
    void                                                                \
    shmem_ctx_##_name##_atomic_##_opname(shmem_ctx_t ctx,               \
                                         _type *target,                 \
                                         _type value, int pe)           \
    {                                                                   \
        SHMEMT_MUTEX_NOPROTECT(shmemc_ctx_##_opname(ctx, target,        \
                                                    &value, sizeof(value), \
                                                    pe));               \
    }

#define SHMEM_CTX_TYPE_FETCH_BITWISE(_opname, _name, _type)             \
    _type                                                               \
    shmem_ctx_##_name##_atomic_fetch_##_opname(shmem_ctx_t ctx,         \
                                               _type *target,           \
                                               _type value, int pe)     \
    {                                                                   \
        _type v;                                                        \
                                                                        \
        SHMEMT_MUTEX_NOPROTECT(shmemc_ctx_fetch_##_opname(ctx,          \
                                                          target,       \
                                                          &value,       \
                                                          sizeof(value), \
                                                          pe, &v));     \
        return v;                                                       \
    }

/*
 * non-blocking
 */

#define API_DEF_CONST_AMO1_NBI(_op, _name, _type)                       \
    void shmem_##_name##_atomic_##_op##_nbi(_type *fetch,               \
                                            const _type *target,        \
                                            int pe)                     \
    {                                                                   \
        shmem_ctx_##_name##_atomic_##_op##_nbi(SHMEM_CTX_DEFAULT,       \
                                               fetch,                   \
                                               target, pe);             \
    }

#define API_DEF_AMO1_NBI(_op, _name, _type)                             \
    void shmem_##_name##_atomic_##_op##_nbi(_type *fetch,               \
                                            _type *target,              \
                                            int pe)                     \
    {                                                                   \
        shmem_ctx_##_name##_atomic_##_op##_nbi(SHMEM_CTX_DEFAULT,       \
                                               fetch,                   \
                                               target, pe);             \
    }

#define API_DEF_AMO2_NBI(_op, _name, _type)                             \
    void shmem_##_name##_atomic_##_op##_nbi(_type *fetch,               \
                                            _type *target, _type value, \
                                            int pe)                     \
    {                                                                   \
        shmem_ctx_##_name##_atomic_##_op##_nbi(SHMEM_CTX_DEFAULT,       \
                                               fetch,                   \
                                               target, value, pe);      \
    }

#define API_DEF_AMO3_NBI(_op, _name, _type)                             \
    void shmem_##_name##_atomic_##_op##_nbi(_type *fetch,               \
                                            _type *target,              \
                                            _type cond,                 \
                                            _type value,                \
                                            int pe)                     \
    {                                                                   \
        shmem_ctx_##_name##_atomic_##_op##_nbi(SHMEM_CTX_DEFAULT,       \
                                               fetch,                   \
                                               target,                  \
                                               cond, value, pe);        \
    }

#define SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(_opname, _name, _type)         \
    void                                                                \
    shmem_ctx_##_name##_atomic_fetch_##_opname##_nbi(shmem_ctx_t ctx,   \
                                                     _type *fetch,      \
                                                     _type *target,     \
                                                     _type value,       \
                                                     int pe)            \
    {                                                                   \
        SHMEMT_MUTEX_NOPROTECT(shmemc_ctx_fetch_##_opname(ctx,          \
                                                          target,       \
                                                          &value,       \
                                                          sizeof(value), \
                                                          pe, fetch));  \
    }


#endif /* ! SHMEM_AMO_COMMON_H */
