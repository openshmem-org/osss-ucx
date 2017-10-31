/* For license: see LICENSE file at top-level */

#include "state.h"

#include <ucp/api/ucp.h>

#if 0
#define VOLATILIZE(_type, _var) (* ( volatile _type *) (_var))
#endif

#define COMMS_TEST_SIZE(_size, _opname, _op)                            \
    int                                                                 \
    shmemc_test_##_opname##_size(int##_size##_t *var,                   \
                                 int##_size##_t value)                  \
    {                                                                   \
        ucp_worker_wait_mem(proc.comms.wrkr, var);                      \
        return ( *var _op value ) ? 1 : 0;                              \
    }

COMMS_TEST_SIZE(16, eq, ==)
COMMS_TEST_SIZE(32, eq, ==)
COMMS_TEST_SIZE(64, eq, ==)

COMMS_TEST_SIZE(16, ne, !=)
COMMS_TEST_SIZE(32, ne, !=)
COMMS_TEST_SIZE(64, ne, !=)

COMMS_TEST_SIZE(16, gt, >)
COMMS_TEST_SIZE(32, gt, >)
COMMS_TEST_SIZE(64, gt, >)

COMMS_TEST_SIZE(16, le, <=)
COMMS_TEST_SIZE(32, le, <=)
COMMS_TEST_SIZE(64, le, <=)

COMMS_TEST_SIZE(16, lt, <)
COMMS_TEST_SIZE(32, lt, <)
COMMS_TEST_SIZE(64, lt, <)

COMMS_TEST_SIZE(16, ge, >=)
COMMS_TEST_SIZE(32, ge, >=)
COMMS_TEST_SIZE(64, ge, >=)

#define COMMS_WAIT_SIZE(_size, _opname, _op)                            \
    void                                                                \
    shmemc_wait_##_opname##_until##_size(int##_size##_t *var,           \
                                         int##_size##_t value)          \
    {                                                                   \
        while (1) {                                                     \
            ucp_worker_wait_mem(proc.comms.wrkr, var);                  \
            if ( *var _op value ) {                                     \
                return;                                                 \
            }                                                           \
        }                                                               \
    }

COMMS_WAIT_SIZE(16, eq, ==)
COMMS_WAIT_SIZE(32, eq, ==)
COMMS_WAIT_SIZE(64, eq, ==)

COMMS_WAIT_SIZE(16, ne, !=)
COMMS_WAIT_SIZE(32, ne, !=)
COMMS_WAIT_SIZE(64, ne, !=)

COMMS_WAIT_SIZE(16, gt, >)
COMMS_WAIT_SIZE(32, gt, >)
COMMS_WAIT_SIZE(64, gt, >)

COMMS_WAIT_SIZE(16, le, <=)
COMMS_WAIT_SIZE(32, le, <=)
COMMS_WAIT_SIZE(64, le, <=)

COMMS_WAIT_SIZE(16, lt, <)
COMMS_WAIT_SIZE(32, lt, <)
COMMS_WAIT_SIZE(64, lt, <)

COMMS_WAIT_SIZE(16, ge, >=)
COMMS_WAIT_SIZE(32, ge, >=)
COMMS_WAIT_SIZE(64, ge, >=)
