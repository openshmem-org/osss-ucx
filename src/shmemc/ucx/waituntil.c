/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"
#include "shmemc.h"

#include "yielder.h"

#include <ucp/api/ucp.h>

#if 0
#define VOLATILIZE(_type, _var) (* ( volatile _type *) (_var))
#endif

#define COMMS_CTX_WAIT_SIZE(_size, _opname)                             \
    void                                                                \
    shmemc_ctx_wait_until_##_opname##_size(shmem_ctx_t ctx,             \
                                           int##_size##_t *var,         \
                                           int##_size##_t value)        \
    {                                                                   \
        shmemc_context_h ch = (shmemc_context_h) ctx;                   \
                                                                        \
        while (shmemc_ctx_test_##_opname##_size(ctx, var, value) == 0) { \
            ucp_worker_wait_mem(ch->w, var);                            \
            yielder();                                                  \
        }                                                               \
    }

COMMS_CTX_WAIT_SIZE(16, eq)
COMMS_CTX_WAIT_SIZE(32, eq)
COMMS_CTX_WAIT_SIZE(64, eq)

COMMS_CTX_WAIT_SIZE(16, ne)
COMMS_CTX_WAIT_SIZE(32, ne)
COMMS_CTX_WAIT_SIZE(64, ne)

COMMS_CTX_WAIT_SIZE(16, gt)
COMMS_CTX_WAIT_SIZE(32, gt)
COMMS_CTX_WAIT_SIZE(64, gt)

COMMS_CTX_WAIT_SIZE(16, le)
COMMS_CTX_WAIT_SIZE(32, le)
COMMS_CTX_WAIT_SIZE(64, le)

COMMS_CTX_WAIT_SIZE(16, lt)
COMMS_CTX_WAIT_SIZE(32, lt)
COMMS_CTX_WAIT_SIZE(64, lt)

COMMS_CTX_WAIT_SIZE(16, ge)
COMMS_CTX_WAIT_SIZE(32, ge)
COMMS_CTX_WAIT_SIZE(64, ge)

#define COMMS_CTX_WAIT_UNTIL_ALL_SIZE(_size, _opname)                   \
    void                                                                \
    shmemc_ctx_wait_until_all_##_opname##_size(shmem_ctx_t ctx,         \
                                               int##_size##_t *vars,    \
                                               size_t nelems,           \
                                               const int *status,       \
                                               int##_size##_t value)    \
    {                                                                   \
        size_t n = 0;                                                   \
        size_t i;                                                       \
                                                                        \
        do {                                                            \
            for (i = 0; i < nelems; ++i) {                              \
                if ( (status != NULL) && ( status[i] != 0) ) {          \
                    continue;                                           \
                }                                                       \
                if (shmemc_ctx_test_##_opname##_size(ctx,               \
                                                     &(vars[i]),        \
                                                     value) != 0) {     \
                    ++n;                                                \
                }                                                       \
                yielder();                                              \
            }                                                           \
        } while (n < nelems);                                           \
    }

COMMS_CTX_WAIT_UNTIL_ALL_SIZE(16, eq)
COMMS_CTX_WAIT_UNTIL_ALL_SIZE(32, eq)
COMMS_CTX_WAIT_UNTIL_ALL_SIZE(64, eq)

COMMS_CTX_WAIT_UNTIL_ALL_SIZE(16, ne)
COMMS_CTX_WAIT_UNTIL_ALL_SIZE(32, ne)
COMMS_CTX_WAIT_UNTIL_ALL_SIZE(64, ne)

COMMS_CTX_WAIT_UNTIL_ALL_SIZE(16, gt)
COMMS_CTX_WAIT_UNTIL_ALL_SIZE(32, gt)
COMMS_CTX_WAIT_UNTIL_ALL_SIZE(64, gt)

COMMS_CTX_WAIT_UNTIL_ALL_SIZE(16, le)
COMMS_CTX_WAIT_UNTIL_ALL_SIZE(32, le)
COMMS_CTX_WAIT_UNTIL_ALL_SIZE(64, le)

COMMS_CTX_WAIT_UNTIL_ALL_SIZE(16, lt)
COMMS_CTX_WAIT_UNTIL_ALL_SIZE(32, lt)
COMMS_CTX_WAIT_UNTIL_ALL_SIZE(64, lt)

COMMS_CTX_WAIT_UNTIL_ALL_SIZE(16, ge)
COMMS_CTX_WAIT_UNTIL_ALL_SIZE(32, ge)
COMMS_CTX_WAIT_UNTIL_ALL_SIZE(64, ge)

#define COMMS_CTX_WAIT_UNTIL_ANY_SIZE(_size, _opname)                   \
    size_t                                                              \
    shmemc_ctx_wait_until_any_##_opname##_size(shmem_ctx_t ctx,         \
                                               int##_size##_t * vars,   \
                                               size_t nelems,           \
                                               const int *status,       \
                                               int##_size##_t value)    \
    {                                                                   \
        size_t winner;                                                  \
        size_t i;                                                       \
                                                                        \
        while (1) {                                                     \
            for (i = 0; i < nelems; ++i) {                              \
                if ( (status != NULL) && (status[i] != 0) ) {           \
                    continue;                                           \
                }                                                       \
                                                                        \
                if (shmemc_ctx_test_##_opname##_size(ctx,               \
                                                     &(vars[i]),        \
                                                     value) != 0) {     \
                    winner = i;                                         \
                    goto ret;                                           \
                    /* NOT REACHED */                                   \
                }                                                       \
                yielder();                                              \
            }                                                           \
        }                                                               \
                                                                        \
    ret:                                                                \
        return winner;                                                  \
    }

COMMS_CTX_WAIT_UNTIL_ANY_SIZE(16, eq)
COMMS_CTX_WAIT_UNTIL_ANY_SIZE(32, eq)
COMMS_CTX_WAIT_UNTIL_ANY_SIZE(64, eq)

COMMS_CTX_WAIT_UNTIL_ANY_SIZE(16, ne)
COMMS_CTX_WAIT_UNTIL_ANY_SIZE(32, ne)
COMMS_CTX_WAIT_UNTIL_ANY_SIZE(64, ne)

COMMS_CTX_WAIT_UNTIL_ANY_SIZE(16, gt)
COMMS_CTX_WAIT_UNTIL_ANY_SIZE(32, gt)
COMMS_CTX_WAIT_UNTIL_ANY_SIZE(64, gt)

COMMS_CTX_WAIT_UNTIL_ANY_SIZE(16, le)
COMMS_CTX_WAIT_UNTIL_ANY_SIZE(32, le)
COMMS_CTX_WAIT_UNTIL_ANY_SIZE(64, le)

COMMS_CTX_WAIT_UNTIL_ANY_SIZE(16, lt)
COMMS_CTX_WAIT_UNTIL_ANY_SIZE(32, lt)
COMMS_CTX_WAIT_UNTIL_ANY_SIZE(64, lt)

COMMS_CTX_WAIT_UNTIL_ANY_SIZE(16, ge)
COMMS_CTX_WAIT_UNTIL_ANY_SIZE(32, ge)
COMMS_CTX_WAIT_UNTIL_ANY_SIZE(64, ge)

#define COMMS_CTX_WAIT_UNTIL_SOME_SIZE(_size, _opname)                  \
    size_t                                                              \
    shmemc_ctx_wait_until_some_##_opname##_size(shmem_ctx_t ctx,        \
                                                int##_size##_t * vars,  \
                                                size_t nelems,          \
                                                size_t * idxs,          \
                                                const int *status,      \
                                                int##_size##_t value)   \
    {                                                                   \
        size_t i;                                                       \
        size_t hits = 0;                                                \
                                                                        \
        /* find any one (there may be others too, further up) */        \
        (void) shmemc_ctx_wait_until_any_##_opname##_size(ctx,          \
                                                          vars,         \
                                                          nelems,       \
                                                          status,       \
                                                          value);       \
        /* see which ones match */                                      \
        for (i = 0; i < nelems; ++i) {                                  \
            if ( (status != NULL) && (status[i] != 0) ) {               \
                continue;                                               \
            }                                                           \
                                                                        \
            if (shmemc_ctx_test_##_opname##_size(ctx,                   \
                                                 &(vars[i]),            \
                                                 value) != 0) {         \
                idxs[hits] = i;                                         \
                ++hits;                                                 \
            }                                                           \
            yielder();                                                  \
        }                                                               \
        return hits;                                                    \
    }

COMMS_CTX_WAIT_UNTIL_SOME_SIZE(16, eq)
COMMS_CTX_WAIT_UNTIL_SOME_SIZE(32, eq)
COMMS_CTX_WAIT_UNTIL_SOME_SIZE(64, eq)

COMMS_CTX_WAIT_UNTIL_SOME_SIZE(16, ne)
COMMS_CTX_WAIT_UNTIL_SOME_SIZE(32, ne)
COMMS_CTX_WAIT_UNTIL_SOME_SIZE(64, ne)

COMMS_CTX_WAIT_UNTIL_SOME_SIZE(16, gt)
COMMS_CTX_WAIT_UNTIL_SOME_SIZE(32, gt)
COMMS_CTX_WAIT_UNTIL_SOME_SIZE(64, gt)

COMMS_CTX_WAIT_UNTIL_SOME_SIZE(16, le)
COMMS_CTX_WAIT_UNTIL_SOME_SIZE(32, le)
COMMS_CTX_WAIT_UNTIL_SOME_SIZE(64, le)

COMMS_CTX_WAIT_UNTIL_SOME_SIZE(16, lt)
COMMS_CTX_WAIT_UNTIL_SOME_SIZE(32, lt)
COMMS_CTX_WAIT_UNTIL_SOME_SIZE(64, lt)

COMMS_CTX_WAIT_UNTIL_SOME_SIZE(16, ge)
COMMS_CTX_WAIT_UNTIL_SOME_SIZE(32, ge)
COMMS_CTX_WAIT_UNTIL_SOME_SIZE(64, ge)

#define COMMS_CTX_WAIT_UNTIL_ALL_VECTOR_SIZE(_size, _opname)            \
    void                                                                \
    shmemc_ctx_wait_until_all_vector_##_opname##_size(shmem_ctx_t ctx,  \
                                                      int##_size##_t *vars, \
                                                      size_t nelems,    \
                                                      const int *status, \
                                                      void *values)     \
    {                                                                   \
        int##_size##_t *vs = (int##_size##_t *) values;                 \
        size_t n = 0;                                                   \
        size_t i;                                                       \
                                                                        \
        do {                                                            \
            for (i = 0; i < nelems; ++i) {                              \
                if ( (status != NULL) && ( status[i] != 0) ) {          \
                    continue;                                           \
                }                                                       \
                if (shmemc_ctx_test_##_opname##_size(ctx,               \
                                                     &(vars[i]),        \
                                                     vs[i]) != 0) {     \
                    ++n;                                                \
                }                                                       \
                yielder();                                              \
            }                                                           \
        } while (n < nelems);                                           \
    }

COMMS_CTX_WAIT_UNTIL_ALL_VECTOR_SIZE(16, eq)
COMMS_CTX_WAIT_UNTIL_ALL_VECTOR_SIZE(32, eq)
COMMS_CTX_WAIT_UNTIL_ALL_VECTOR_SIZE(64, eq)

COMMS_CTX_WAIT_UNTIL_ALL_VECTOR_SIZE(16, ne)
COMMS_CTX_WAIT_UNTIL_ALL_VECTOR_SIZE(32, ne)
COMMS_CTX_WAIT_UNTIL_ALL_VECTOR_SIZE(64, ne)

COMMS_CTX_WAIT_UNTIL_ALL_VECTOR_SIZE(16, gt)
COMMS_CTX_WAIT_UNTIL_ALL_VECTOR_SIZE(32, gt)
COMMS_CTX_WAIT_UNTIL_ALL_VECTOR_SIZE(64, gt)

COMMS_CTX_WAIT_UNTIL_ALL_VECTOR_SIZE(16, le)
COMMS_CTX_WAIT_UNTIL_ALL_VECTOR_SIZE(32, le)
COMMS_CTX_WAIT_UNTIL_ALL_VECTOR_SIZE(64, le)

COMMS_CTX_WAIT_UNTIL_ALL_VECTOR_SIZE(16, lt)
COMMS_CTX_WAIT_UNTIL_ALL_VECTOR_SIZE(32, lt)
COMMS_CTX_WAIT_UNTIL_ALL_VECTOR_SIZE(64, lt)

COMMS_CTX_WAIT_UNTIL_ALL_VECTOR_SIZE(16, ge)
COMMS_CTX_WAIT_UNTIL_ALL_VECTOR_SIZE(32, ge)
COMMS_CTX_WAIT_UNTIL_ALL_VECTOR_SIZE(64, ge)

#define COMMS_CTX_WAIT_UNTIL_ANY_VECTOR_SIZE(_size, _opname)            \
    size_t                                                              \
    shmemc_ctx_wait_until_any_vector_##_opname##_size(shmem_ctx_t ctx,  \
                                                      int##_size##_t * vars, \
                                                      size_t nelems,    \
                                                      const int *status, \
                                                      void *values)     \
    {                                                                   \
        int##_size##_t *vs = (int##_size##_t *) values;                 \
        size_t winner;                                                  \
        size_t i;                                                       \
                                                                        \
        while (1) {                                                     \
            for (i = 0; i < nelems; ++i) {                              \
                if ( (status != NULL) && (status[i] != 0) ) {           \
                    continue;                                           \
                }                                                       \
                                                                        \
                if (shmemc_ctx_test_##_opname##_size(ctx,               \
                                                     &(vars[i]),        \
                                                     vs[i]) != 0) {     \
                    winner = i;                                         \
                    goto ret;                                           \
                    /* NOT REACHED */                                   \
                }                                                       \
                yielder();                                              \
            }                                                           \
        }                                                               \
                                                                        \
    ret:                                                                \
        return winner;                                                  \
    }

COMMS_CTX_WAIT_UNTIL_ANY_VECTOR_SIZE(16, eq)
COMMS_CTX_WAIT_UNTIL_ANY_VECTOR_SIZE(32, eq)
COMMS_CTX_WAIT_UNTIL_ANY_VECTOR_SIZE(64, eq)

COMMS_CTX_WAIT_UNTIL_ANY_VECTOR_SIZE(16, ne)
COMMS_CTX_WAIT_UNTIL_ANY_VECTOR_SIZE(32, ne)
COMMS_CTX_WAIT_UNTIL_ANY_VECTOR_SIZE(64, ne)

COMMS_CTX_WAIT_UNTIL_ANY_VECTOR_SIZE(16, gt)
COMMS_CTX_WAIT_UNTIL_ANY_VECTOR_SIZE(32, gt)
COMMS_CTX_WAIT_UNTIL_ANY_VECTOR_SIZE(64, gt)

COMMS_CTX_WAIT_UNTIL_ANY_VECTOR_SIZE(16, le)
COMMS_CTX_WAIT_UNTIL_ANY_VECTOR_SIZE(32, le)
COMMS_CTX_WAIT_UNTIL_ANY_VECTOR_SIZE(64, le)

COMMS_CTX_WAIT_UNTIL_ANY_VECTOR_SIZE(16, lt)
COMMS_CTX_WAIT_UNTIL_ANY_VECTOR_SIZE(32, lt)
COMMS_CTX_WAIT_UNTIL_ANY_VECTOR_SIZE(64, lt)

COMMS_CTX_WAIT_UNTIL_ANY_VECTOR_SIZE(16, ge)
COMMS_CTX_WAIT_UNTIL_ANY_VECTOR_SIZE(32, ge)
COMMS_CTX_WAIT_UNTIL_ANY_VECTOR_SIZE(64, ge)

#define COMMS_CTX_WAIT_UNTIL_SOME_VECTOR_SIZE(_size, _opname)           \
    size_t                                                              \
    shmemc_ctx_wait_until_some_vector_##_opname##_size(shmem_ctx_t ctx, \
                                                       int##_size##_t * vars, \
                                                       size_t nelems,   \
                                                       size_t * idxs,   \
                                                       const int *status, \
                                                       void *values)    \
    {                                                                   \
        int##_size##_t *vs = (int##_size##_t *) values;                 \
        size_t i;                                                       \
        size_t hits = 0;                                                \
                                                                        \
        /* find any one (there may be others too, further up) */        \
        (void) shmemc_ctx_wait_until_any_vector_##_opname##_size(ctx,   \
                                                                 vars,  \
                                                                 nelems, \
                                                                 status, \
                                                                 vs);   \
        /* see which ones match */                                      \
        for (i = 0; i < nelems; ++i) {                                  \
            if ( (status != NULL) && (status[i] != 0) ) {               \
                continue;                                               \
            }                                                           \
                                                                        \
            if (shmemc_ctx_test_##_opname##_size(ctx,                   \
                                                 &(vars[i]),            \
                                                 vs[i]) != 0) {         \
                idxs[hits] = i;                                         \
                ++hits;                                                 \
            }                                                           \
            yielder();                                                  \
        }                                                               \
        return hits;                                                    \
    }

COMMS_CTX_WAIT_UNTIL_SOME_VECTOR_SIZE(16, eq)
COMMS_CTX_WAIT_UNTIL_SOME_VECTOR_SIZE(32, eq)
COMMS_CTX_WAIT_UNTIL_SOME_VECTOR_SIZE(64, eq)

COMMS_CTX_WAIT_UNTIL_SOME_VECTOR_SIZE(16, ne)
COMMS_CTX_WAIT_UNTIL_SOME_VECTOR_SIZE(32, ne)
COMMS_CTX_WAIT_UNTIL_SOME_VECTOR_SIZE(64, ne)

COMMS_CTX_WAIT_UNTIL_SOME_VECTOR_SIZE(16, gt)
COMMS_CTX_WAIT_UNTIL_SOME_VECTOR_SIZE(32, gt)
COMMS_CTX_WAIT_UNTIL_SOME_VECTOR_SIZE(64, gt)

COMMS_CTX_WAIT_UNTIL_SOME_VECTOR_SIZE(16, le)
COMMS_CTX_WAIT_UNTIL_SOME_VECTOR_SIZE(32, le)
COMMS_CTX_WAIT_UNTIL_SOME_VECTOR_SIZE(64, le)

COMMS_CTX_WAIT_UNTIL_SOME_VECTOR_SIZE(16, lt)
COMMS_CTX_WAIT_UNTIL_SOME_VECTOR_SIZE(32, lt)
COMMS_CTX_WAIT_UNTIL_SOME_VECTOR_SIZE(64, lt)

COMMS_CTX_WAIT_UNTIL_SOME_VECTOR_SIZE(16, ge)
COMMS_CTX_WAIT_UNTIL_SOME_VECTOR_SIZE(32, ge)
COMMS_CTX_WAIT_UNTIL_SOME_VECTOR_SIZE(64, ge)
