/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "state.h"
#include "shmemu.h"

#include "shmem/defs.h"

#include "yielder.h"

#include <ucp/api/ucp.h>

#if 0
#define VOLATILIZE(_type, _var) (* ( volatile _type *) (_var))
#endif

/*
 * return 1 if the memory location changed w.r.t "value", otherwise 0
 */

#define COMMS_CTX_TEST_SIZE(_size, _opname, _op)                        \
    int                                                                 \
    shmemc_ctx_test_##_opname##_size(shmem_ctx_t ctx,                   \
                                     int##_size##_t *var,               \
                                     int##_size##_t value)              \
    {                                                                   \
        NO_WARN_UNUSED(ctx);                                            \
                                                                        \
        return ( (*var) _op (value) ) ? 1 : 0;                          \
    }

COMMS_CTX_TEST_SIZE(16, eq, ==)
COMMS_CTX_TEST_SIZE(32, eq, ==)
COMMS_CTX_TEST_SIZE(64, eq, ==)

COMMS_CTX_TEST_SIZE(16, ne, !=)
COMMS_CTX_TEST_SIZE(32, ne, !=)
COMMS_CTX_TEST_SIZE(64, ne, !=)

COMMS_CTX_TEST_SIZE(16, gt, >)
COMMS_CTX_TEST_SIZE(32, gt, >)
COMMS_CTX_TEST_SIZE(64, gt, >)

COMMS_CTX_TEST_SIZE(16, le, <=)
COMMS_CTX_TEST_SIZE(32, le, <=)
COMMS_CTX_TEST_SIZE(64, le, <=)

COMMS_CTX_TEST_SIZE(16, lt, <)
COMMS_CTX_TEST_SIZE(32, lt, <)
COMMS_CTX_TEST_SIZE(64, lt, <)

COMMS_CTX_TEST_SIZE(16, ge, >=)
COMMS_CTX_TEST_SIZE(32, ge, >=)
COMMS_CTX_TEST_SIZE(64, ge, >=)

#define COMMS_CTX_WAIT_SIZE(_size, _opname)                             \
    void                                                                \
    shmemc_ctx_wait_until_##_opname##_size(shmem_ctx_t ctx,             \
                                           int##_size##_t *var,         \
                                           int##_size##_t value)        \
    {                                                                   \
        shmemc_context_h ch = (shmemc_context_h) ctx;                   \
                                                                        \
        do {                                                            \
            shmemc_progress();                                          \
            yielder();                                                  \
            ucp_worker_wait_mem(ch->w, var);                            \
        } while (shmemc_ctx_test_##_opname##_size(ctx, var, value) == 0); \
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


/*
 * return 1 if all the memory locations changed w.r.t "value",
 * otherwise 0
 */

#define COMMS_CTX_TEST_ALL_SIZE(_size, _opname, _op)                    \
    int                                                                 \
    shmemc_ctx_test_all_##_opname##_size(shmem_ctx_t ctx,               \
                                         int##_size##_t *vars,          \
                                         size_t nelems,                 \
                                         int##_size##_t value)          \
    {                                                                   \
        size_t i;                                                       \
        int n = 0;                                                      \
                                                                        \
        for (i = 0; i < nelems; ++i) {                                  \
            n += shmemc_ctx_test_##_opname##_size(ctx,                  \
                                                  &(vars[i]), value);   \
        }                                                               \
                                                                        \
        return ((size_t) n == nelems) ? 1 : 0;                          \
    }

COMMS_CTX_TEST_ALL_SIZE(16, eq, ==)
COMMS_CTX_TEST_ALL_SIZE(32, eq, ==)
COMMS_CTX_TEST_ALL_SIZE(64, eq, ==)

COMMS_CTX_TEST_ALL_SIZE(16, ne, !=)
COMMS_CTX_TEST_ALL_SIZE(32, ne, !=)
COMMS_CTX_TEST_ALL_SIZE(64, ne, !=)

COMMS_CTX_TEST_ALL_SIZE(16, gt, >)
COMMS_CTX_TEST_ALL_SIZE(32, gt, >)
COMMS_CTX_TEST_ALL_SIZE(64, gt, >)

COMMS_CTX_TEST_ALL_SIZE(16, le, <=)
COMMS_CTX_TEST_ALL_SIZE(32, le, <=)
COMMS_CTX_TEST_ALL_SIZE(64, le, <=)

COMMS_CTX_TEST_ALL_SIZE(16, lt, <)
COMMS_CTX_TEST_ALL_SIZE(32, lt, <)
COMMS_CTX_TEST_ALL_SIZE(64, lt, <)

COMMS_CTX_TEST_ALL_SIZE(16, ge, >=)
COMMS_CTX_TEST_ALL_SIZE(32, ge, >=)
COMMS_CTX_TEST_ALL_SIZE(64, ge, >=)

/*
 * return how many memory locations changed w.r.t "value", otherwise 0
 */

#define COMMS_CTX_TEST_SOME_SIZE(_size, _opname, _op)                   \
    size_t                                                              \
    shmemc_ctx_test_some_##_opname##_size(shmem_ctx_t ctx,              \
                                          int##_size##_t *vars,         \
                                          size_t nelems,                \
                                          size_t * restrict indices,    \
                                          int * restrict status,        \
                                          int##_size##_t value)         \
    {                                                                   \
        size_t n;                                                       \
        size_t i = 0;                                                   \
                                                                        \
        for (n = 0; n < nelems; ++n) {                                  \
            if ( (status != NULL) && ( status[n] != 0) ) {              \
                continue;                                               \
            }                                                           \
                                                                        \
            if (shmemc_ctx_test_##_opname##_size(ctx,                   \
                                                 &(vars[n]),            \
                                                 value) != 0) {         \
                indices[i++] = n;                                       \
            }                                                           \
        }                                                               \
        return i;                                                       \
    }

COMMS_CTX_TEST_SOME_SIZE(16, eq, ==)
COMMS_CTX_TEST_SOME_SIZE(32, eq, ==)
COMMS_CTX_TEST_SOME_SIZE(64, eq, ==)

COMMS_CTX_TEST_SOME_SIZE(16, ne, !=)
COMMS_CTX_TEST_SOME_SIZE(32, ne, !=)
COMMS_CTX_TEST_SOME_SIZE(64, ne, !=)

COMMS_CTX_TEST_SOME_SIZE(16, gt, >)
COMMS_CTX_TEST_SOME_SIZE(32, gt, >)
COMMS_CTX_TEST_SOME_SIZE(64, gt, >)

COMMS_CTX_TEST_SOME_SIZE(16, le, <=)
COMMS_CTX_TEST_SOME_SIZE(32, le, <=)
COMMS_CTX_TEST_SOME_SIZE(64, le, <=)

COMMS_CTX_TEST_SOME_SIZE(16, lt, <)
COMMS_CTX_TEST_SOME_SIZE(32, lt, <)
COMMS_CTX_TEST_SOME_SIZE(64, lt, <)

COMMS_CTX_TEST_SOME_SIZE(16, ge, >=)
COMMS_CTX_TEST_SOME_SIZE(32, ge, >=)
COMMS_CTX_TEST_SOME_SIZE(64, ge, >=)

/*
 * return the index of a memory location that changed w.r.t "value",
 * otherwise 0
 */

#define COMMS_CTX_TEST_ANY_SIZE(_size, _opname, _op)                    \
    size_t                                                              \
    shmemc_ctx_test_any_##_opname##_size(shmem_ctx_t ctx,               \
                                         int##_size##_t *vars,          \
                                         size_t nelems,                 \
                                         int * restrict status,         \
                                         int##_size##_t value)          \
    {                                                                   \
        size_t n;                                                       \
                                                                        \
        for (n = 0; n < nelems; ++n) {                                  \
            if ( (status != NULL) && ( status[n] != 0) ) {              \
                continue;                                               \
            }                                                           \
                                                                        \
            if (shmemc_ctx_test_##_opname##_size(ctx,                   \
                                                 &(vars[n]),            \
                                                 value) != 0) {         \
                return n;                                               \
            }                                                           \
        }                                                               \
        return 0;                                                       \
    }

COMMS_CTX_TEST_ANY_SIZE(16, eq, ==)
COMMS_CTX_TEST_ANY_SIZE(32, eq, ==)
COMMS_CTX_TEST_ANY_SIZE(64, eq, ==)

COMMS_CTX_TEST_ANY_SIZE(16, ne, !=)
COMMS_CTX_TEST_ANY_SIZE(32, ne, !=)
COMMS_CTX_TEST_ANY_SIZE(64, ne, !=)

COMMS_CTX_TEST_ANY_SIZE(16, gt, >)
COMMS_CTX_TEST_ANY_SIZE(32, gt, >)
COMMS_CTX_TEST_ANY_SIZE(64, gt, >)

COMMS_CTX_TEST_ANY_SIZE(16, le, <=)
COMMS_CTX_TEST_ANY_SIZE(32, le, <=)
COMMS_CTX_TEST_ANY_SIZE(64, le, <=)

COMMS_CTX_TEST_ANY_SIZE(16, lt, <)
COMMS_CTX_TEST_ANY_SIZE(32, lt, <)
COMMS_CTX_TEST_ANY_SIZE(64, lt, <)

COMMS_CTX_TEST_ANY_SIZE(16, ge, >=)
COMMS_CTX_TEST_ANY_SIZE(32, ge, >=)
COMMS_CTX_TEST_ANY_SIZE(64, ge, >=)
