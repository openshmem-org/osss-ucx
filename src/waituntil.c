#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem/defs.h"
#include "shmemu.h"
#include "shmemc.h"

#ifdef ENABLE_PSHMEM
#pragma weak shmem_short_wait_until = pshmem_short_wait_until
#define shmem_short_wait_until pshmem_short_wait_until
#pragma weak shmem_int_wait_until = pshmem_int_wait_until
#define shmem_int_wait_until pshmem_int_wait_until
#pragma weak shmem_long_wait_until = pshmem_long_wait_until
#define shmem_long_wait_until pshmem_long_wait_until
#pragma weak shmem_longlong_wait_until = pshmem_longlong_wait_until
#define shmem_longlong_wait_until pshmem_longlong_wait_until


#pragma weak shmem_short_wait = pshmem_short_wait
#define shmem_short_wait pshmem_short_wait
#pragma weak shmem_int_wait = pshmem_int_wait
#define shmem_int_wait pshmem_int_wait
#pragma weak shmem_long_wait = pshmem_long_wait
#define shmem_long_wait pshmem_long_wait
#pragma weak shmem_longlong_wait = pshmem_longlong_wait
#define shmem_longlong_wait pshmem_longlong_wait
#endif /* ENABLE_PSHMEM */


/**
 * wait_until with operator dispatchers, type-parameterized.
 */
#define SHMEM_TYPE_WAIT_UNTIL(_name, _type)                             \
    void                                                                \
    shmem_##_name##_wait_until(volatile _type *ivar, int cmp, _type cmp_value) \
    {                                                                   \
        switch (cmp) {                                                  \
        case SHMEM_CMP_EQ:                                              \
            shmemc_##_name##_wait_eq_until(ivar, cmp, cmp_value);       \
            break;                                                      \
        case SHMEM_CMP_NE:                                              \
            shmemc_##_name##_wait_ne_until(ivar, cmp, cmp_value);       \
            break;                                                      \
        case SHMEM_CMP_GT:                                              \
            shmemc_##_name##_wait_gt_until(ivar, cmp, cmp_value);       \
            break;                                                      \
        case SHMEM_CMP_LE:                                              \
            shmemc_##_name##_wait_le_until(ivar, cmp, cmp_value);       \
            break;                                                      \
        case SHMEM_CMP_LT:                                              \
            shmemc_##_name##_wait_lt_until(ivar, cmp, cmp_value);       \
            break;                                                      \
        case SHMEM_CMP_GE:                                              \
            shmemc_##_name##_wait_ge_until(ivar, cmp, cmp_value);       \
            break;                                                      \
        default:                                                        \
            logger(LOG_FATAL,                                           \
                   "unknown operator (code %d) in \"%s\"",              \
                   cmp,                                                 \
                   __func__                                             \
                   );                                                   \
            return;                                                     \
            /* NOT REACHED */                                           \
            break;                                                      \
        }                                                               \
    }

SHMEM_TYPE_WAIT_UNTIL(short, short)
SHMEM_TYPE_WAIT_UNTIL(int, int)
SHMEM_TYPE_WAIT_UNTIL(long, long)
SHMEM_TYPE_WAIT_UNTIL(longlong, long long)

/**
 * wait is just wait_until with inequality/change test
 */

#define SHMEM_TYPE_WAIT(_name, _type)                                 \
    void                                                              \
    shmem_##_name##_wait(volatile _type *ivar, _type cmp_value)       \
    {                                                                 \
        shmemc_##_name##_wait_ne_until(ivar, SHMEM_CMP_NE, cmp_value);  \
    }

SHMEM_TYPE_WAIT(short, short)
SHMEM_TYPE_WAIT(int, int)
SHMEM_TYPE_WAIT(long, long)
SHMEM_TYPE_WAIT(longlong, long long)
