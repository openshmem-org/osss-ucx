/* For license: see LICENSE file at top-level */

#include "state.h"

#include <ucp/api/ucp.h>

#define VOLATILIZE(_type, _var) (* ( volatile _type *) (_var))

#define COMMS_WAIT_TYPE(_name, _type, _opname, _op)                     \
    void                                                                \
    shmemc_##_name##_wait_##_opname##_until(_type *var,                 \
                                            _type cmp_value)            \
    {                                                                   \
        while (1) {                                                     \
            ucp_worker_wait_mem(proc.comms.wrkr, var);                  \
            if (VOLATILIZE(_type, var) _op cmp_value ) {                \
                return;                                                 \
            }                                                           \
        }                                                               \
    }

COMMS_WAIT_TYPE(short, short, eq, ==)
COMMS_WAIT_TYPE(int, int, eq, ==)
COMMS_WAIT_TYPE(long, long, eq, ==)
COMMS_WAIT_TYPE(longlong, long long, eq, ==)

COMMS_WAIT_TYPE(short, short, ne, !=)
COMMS_WAIT_TYPE(int, int, ne, !=)
COMMS_WAIT_TYPE(long, long, ne, !=)
COMMS_WAIT_TYPE(longlong, long long, ne, !=)

COMMS_WAIT_TYPE(short, short, gt, >)
COMMS_WAIT_TYPE(int, int, gt, >)
COMMS_WAIT_TYPE(long, long, gt, >)
COMMS_WAIT_TYPE(longlong, long long, gt, >)

COMMS_WAIT_TYPE(short, short, le, <=)
COMMS_WAIT_TYPE(int, int, le, <=)
COMMS_WAIT_TYPE(long, long, le, <=)
COMMS_WAIT_TYPE(longlong, long long, le, <=)

COMMS_WAIT_TYPE(short, short, lt, <)
COMMS_WAIT_TYPE(int, int, lt, <)
COMMS_WAIT_TYPE(long, long, lt, <)
COMMS_WAIT_TYPE(longlong, long long, lt, <)

COMMS_WAIT_TYPE(short, short, ge, >=)
COMMS_WAIT_TYPE(int, int, ge, >=)
COMMS_WAIT_TYPE(long, long, ge, >=)
COMMS_WAIT_TYPE(longlong, long long, ge, >=)
