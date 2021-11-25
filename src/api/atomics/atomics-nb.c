/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem_mutex.h"
#include "shmemu.h"
#include "shmemc.h"

#include <bits/wordsize.h>
#include <sys/types.h>
#include <stdint.h>
#include <stddef.h>

/*
 * -- context-free
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
                                      _type *target,                    \
                                      int pe)                           \
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

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_atomic_fetch_nbi = pshmem_int_atomic_fetch_nbi
#define shmem_int_atomic_fetch_nbi pshmem_int_atomic_fetch_nbi
#pragma weak shmem_long_atomic_fetch_nbi = pshmem_long_atomic_fetch_nbi
#define shmem_long_atomic_fetch_nbi pshmem_long_atomic_fetch_nbi
#pragma weak shmem_longlong_atomic_fetch_nbi = pshmem_longlong_atomic_fetch_nbi
#define shmem_longlong_atomic_fetch_nbi pshmem_longlong_atomic_fetch_nbi
#pragma weak shmem_float_atomic_fetch_nbi = pshmem_float_atomic_fetch_nbi
#define shmem_float_atomic_fetch_nbi pshmem_float_atomic_fetch_nbi
#pragma weak shmem_double_atomic_fetch_nbi = pshmem_double_atomic_fetch_nbi
#define shmem_double_atomic_fetch_nbi pshmem_double_atomic_fetch_nbi
#pragma weak shmem_uint_atomic_fetch_nbi = pshmem_uint_atomic_fetch_nbi
#define shmem_uint_atomic_fetch_nbi pshmem_uint_atomic_fetch_nbi
#pragma weak shmem_ulong_atomic_fetch_nbi = pshmem_ulong_atomic_fetch_nbi
#define shmem_ulong_atomic_fetch_nbi pshmem_ulong_atomic_fetch_nbi
#pragma weak shmem_ulonglong_atomic_fetch_nbi = pshmem_ulonglong_atomic_fetch_nbi
#define shmem_ulonglong_atomic_fetch_nbi pshmem_ulonglong_atomic_fetch_nbi
#pragma weak shmem_int32_atomic_fetch_nbi = pshmem_int32_atomic_fetch_nbi
#define shmem_int32_atomic_fetch_nbi pshmem_int32_atomic_fetch_nbi
#pragma weak shmem_int64_atomic_fetch_nbi = pshmem_int64_atomic_fetch_nbi
#define shmem_int64_atomic_fetch_nbi pshmem_int64_atomic_fetch_nbi
#pragma weak shmem_uint32_atomic_fetch_nbi = pshmem_uint32_atomic_fetch_nbi
#define shmem_uint32_atomic_fetch_nbi pshmem_uint32_atomic_fetch_nbi
#pragma weak shmem_uint64_atomic_fetch_nbi = pshmem_uint64_atomic_fetch_nbi
#define shmem_uint64_atomic_fetch_nbi pshmem_uint64_atomic_fetch_nbi
#pragma weak shmem_size_atomic_fetch_nbi = pshmem_size_atomic_fetch_nbi
#define shmem_size_atomic_fetch_nbi pshmem_size_atomic_fetch_nbi
#pragma weak shmem_ptrdiff_atomic_fetch_nbi = pshmem_ptrdiff_atomic_fetch_nbi
#define shmem_ptrdiff_atomic_fetch_nbi pshmem_ptrdiff_atomic_fetch_nbi
#endif /* ENABLE_PSHMEM */

API_DEF_CONST_AMO1_NBI(fetch, float, float)
API_DEF_CONST_AMO1_NBI(fetch, double, double)
API_DEF_CONST_AMO1_NBI(fetch, int, int)
API_DEF_CONST_AMO1_NBI(fetch, long, long)
API_DEF_CONST_AMO1_NBI(fetch, longlong, long long)
API_DEF_CONST_AMO1_NBI(fetch, uint, unsigned int)
API_DEF_CONST_AMO1_NBI(fetch, ulong, unsigned long)
API_DEF_CONST_AMO1_NBI(fetch, ulonglong, unsigned long long)
API_DEF_CONST_AMO1_NBI(fetch, int32, int32_t)
API_DEF_CONST_AMO1_NBI(fetch, int64, int64_t)
API_DEF_CONST_AMO1_NBI(fetch, uint32, uint32_t)
API_DEF_CONST_AMO1_NBI(fetch, uint64, uint64_t)
API_DEF_CONST_AMO1_NBI(fetch, size, size_t)
API_DEF_CONST_AMO1_NBI(fetch, ptrdiff, ptrdiff_t)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_atomic_fetch_inc_nbi = pshmem_int_atomic_fetch_inc_nbi
#define shmem_int_atomic_fetch_inc_nbi pshmem_int_atomic_fetch_inc_nbi
#pragma weak shmem_long_atomic_fetch_inc_nbi = pshmem_long_atomic_fetch_inc_nbi
#define shmem_long_atomic_fetch_inc_nbi pshmem_long_atomic_fetch_inc_nbi
#pragma weak shmem_longlong_atomic_fetch_inc_nbi = pshmem_longlong_atomic_fetch_inc_nbi
#define shmem_longlong_atomic_fetch_inc_nbi pshmem_longlong_atomic_fetch_inc_nbi
#pragma weak shmem_uint_atomic_fetch_inc_nbi = pshmem_uint_atomic_fetch_inc_nbi
#define shmem_uint_atomic_fetch_inc_nbi pshmem_uint_atomic_fetch_inc_nbi
#pragma weak shmem_ulong_atomic_fetch_inc_nbi = pshmem_ulong_atomic_fetch_inc_nbi
#define shmem_ulong_atomic_fetch_inc_nbi pshmem_ulong_atomic_fetch_inc_nbi
#pragma weak shmem_ulonglong_atomic_fetch_inc_nbi = pshmem_ulonglong_atomic_fetch_inc_nbi
#define shmem_ulonglong_atomic_fetch_inc_nbi pshmem_ulonglong_atomic_fetch_inc_nbi
#pragma weak shmem_int32_atomic_fetch_inc_nbi = pshmem_int32_atomic_fetch_inc_nbi
#define shmem_int32_atomic_fetch_inc_nbi pshmem_int32_atomic_fetch_inc_nbi
#pragma weak shmem_int64_atomic_fetch_inc_nbi = pshmem_int64_atomic_fetch_inc_nbi
#define shmem_int64_atomic_fetch_inc_nbi pshmem_int64_atomic_fetch_inc_nbi
#pragma weak shmem_uint32_atomic_fetch_inc_nbi = pshmem_uint32_atomic_fetch_inc_nbi
#define shmem_uint32_atomic_fetch_inc_nbi pshmem_uint32_atomic_fetch_inc_nbi
#pragma weak shmem_uint64_atomic_fetch_inc_nbi = pshmem_uint64_atomic_fetch_inc_nbi
#define shmem_uint64_atomic_fetch_inc_nbi pshmem_uint64_atomic_fetch_inc_nbi
#pragma weak shmem_size_atomic_fetch_inc_nbi = pshmem_size_atomic_fetch_inc_nbi
#define shmem_size_atomic_fetch_inc_nbi pshmem_size_atomic_fetch_inc_nbi
#pragma weak shmem_ptrdiff_atomic_fetch_inc_nbi = pshmem_ptrdiff_atomic_fetch_inc_nbi
#define shmem_ptrdiff_atomic_fetch_inc_nbi pshmem_ptrdiff_atomic_fetch_inc_nbi
#endif /* ENABLE_PSHMEM */

API_DEF_AMO1_NBI(fetch_inc, int, int)
API_DEF_AMO1_NBI(fetch_inc, long, long)
API_DEF_AMO1_NBI(fetch_inc, longlong, long long)
API_DEF_AMO1_NBI(fetch_inc, uint, unsigned int)
API_DEF_AMO1_NBI(fetch_inc, ulong, unsigned long)
API_DEF_AMO1_NBI(fetch_inc, ulonglong, unsigned long long)
API_DEF_AMO1_NBI(fetch_inc, int32, int32_t)
API_DEF_AMO1_NBI(fetch_inc, int64, int64_t)
API_DEF_AMO1_NBI(fetch_inc, uint32, uint32_t)
API_DEF_AMO1_NBI(fetch_inc, uint64, uint64_t)
API_DEF_AMO1_NBI(fetch_inc, size, size_t)
API_DEF_AMO1_NBI(fetch_inc, ptrdiff, ptrdiff_t)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_int_atomic_fetch_add_nbi = pshmem_int_atomic_fetch_add_nbi
#define shmem_int_atomic_fetch_add_nbi pshmem_int_atomic_fetch_add_nbi
#pragma weak shmem_long_atomic_fetch_add_nbi = pshmem_long_atomic_fetch_add_nbi
#define shmem_long_atomic_fetch_add_nbi pshmem_long_atomic_fetch_add_nbi
#pragma weak shmem_longlong_atomic_fetch_add_nbi = pshmem_longlong_atomic_fetch_add_nbi
#define shmem_longlong_atomic_fetch_add_nbi pshmem_longlong_atomic_fetch_add_nbi
#pragma weak shmem_uint_atomic_fetch_add_nbi = pshmem_uint_atomic_fetch_add_nbi
#define shmem_uint_atomic_fetch_add_nbi pshmem_uint_atomic_fetch_add_nbi
#pragma weak shmem_ulong_atomic_fetch_add_nbi = pshmem_ulong_atomic_fetch_add_nbi
#define shmem_ulong_atomic_fetch_add_nbi pshmem_ulong_atomic_fetch_add_nbi
#pragma weak shmem_ulonglong_atomic_fetch_add_nbi = pshmem_ulonglong_atomic_fetch_add_nbi
#define shmem_ulonglong_atomic_fetch_add_nbi pshmem_ulonglong_atomic_fetch_add_nbi
#pragma weak shmem_int32_atomic_fetch_add_nbi = pshmem_int32_atomic_fetch_add_nbi
#define shmem_int32_atomic_fetch_add_nbi pshmem_int32_atomic_fetch_add_nbi
#pragma weak shmem_int64_atomic_fetch_add_nbi = pshmem_int64_atomic_fetch_add_nbi
#define shmem_int64_atomic_fetch_add_nbi pshmem_int64_atomic_fetch_add_nbi
#pragma weak shmem_uint32_atomic_fetch_add_nbi = pshmem_uint32_atomic_fetch_add_nbi
#define shmem_uint32_atomic_fetch_add_nbi pshmem_uint32_atomic_fetch_add_nbi
#pragma weak shmem_uint64_atomic_fetch_add_nbi = pshmem_uint64_atomic_fetch_add_nbi
#define shmem_uint64_atomic_fetch_add_nbi pshmem_uint64_atomic_fetch_add_nbi
#pragma weak shmem_size_atomic_fetch_add_nbi = pshmem_size_atomic_fetch_add_nbi
#define shmem_size_atomic_fetch_add_nbi pshmem_size_atomic_fetch_add_nbi
#pragma weak shmem_ptrdiff_atomic_fetch_add_nbi = pshmem_ptrdiff_atomic_fetch_add_nbi
#define shmem_ptrdiff_atomic_fetch_add_nbi pshmem_ptrdiff_atomic_fetch_add_nbi
#endif /* ENABLE_PSHMEM */

API_DEF_AMO2_NBI(fetch_add, int, int)
API_DEF_AMO2_NBI(fetch_add, long, long)
API_DEF_AMO2_NBI(fetch_add, longlong, long long)
API_DEF_AMO2_NBI(fetch_add, uint, unsigned int)
API_DEF_AMO2_NBI(fetch_add, ulong, unsigned long)
API_DEF_AMO2_NBI(fetch_add, ulonglong, unsigned long long)
API_DEF_AMO2_NBI(fetch_add, int32, int32_t)
API_DEF_AMO2_NBI(fetch_add, int64, int64_t)
API_DEF_AMO2_NBI(fetch_add, uint32, uint32_t)
API_DEF_AMO2_NBI(fetch_add, uint64, uint64_t)
API_DEF_AMO2_NBI(fetch_add, size, size_t)
API_DEF_AMO2_NBI(fetch_add, ptrdiff, ptrdiff_t)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_uint_atomic_fetch_xor = pshmem_uint_atomic_fetch_xor
#define shmem_uint_atomic_fetch_xor pshmem_uint_atomic_fetch_xor
#pragma weak shmem_ulong_atomic_fetch_xor = pshmem_ulong_atomic_fetch_xor
#define shmem_ulong_atomic_fetch_xor pshmem_ulong_atomic_fetch_xor
#pragma weak shmem_ulonglong_atomic_fetch_xor = pshmem_ulonglong_atomic_fetch_xor
#define shmem_ulonglong_atomic_fetch_xor pshmem_ulonglong_atomic_fetch_xor
#pragma weak shmem_int32_atomic_fetch_xor = pshmem_int32_atomic_fetch_xor
#define shmem_int32_atomic_fetch_xor pshmem_int32_atomic_fetch_xor
#pragma weak shmem_int64_atomic_fetch_xor = pshmem_int64_atomic_fetch_xor
#define shmem_int64_atomic_fetch_xor pshmem_int64_atomic_fetch_xor
#pragma weak shmem_uint32_atomic_fetch_xor = pshmem_uint32_atomic_fetch_xor
#define shmem_uint32_atomic_fetch_xor pshmem_uint32_atomic_fetch_xor
#pragma weak shmem_uint64_atomic_fetch_xor = pshmem_uint64_atomic_fetch_xor
#define shmem_uint64_atomic_fetch_xor pshmem_uint64_atomic_fetch_xor

#pragma weak shmem_uint_atomic_fetch_or = pshmem_uint_atomic_fetch_or
#define shmem_uint_atomic_fetch_or pshmem_uint_atomic_fetch_or
#pragma weak shmem_ulong_atomic_fetch_or = pshmem_ulong_atomic_fetch_or
#define shmem_ulong_atomic_fetch_or pshmem_ulong_atomic_fetch_or
#pragma weak shmem_ulonglong_atomic_fetch_or = pshmem_ulonglong_atomic_fetch_or
#define shmem_ulonglong_atomic_fetch_or pshmem_ulonglong_atomic_fetch_or
#pragma weak shmem_int32_atomic_fetch_or = pshmem_int32_atomic_fetch_or
#define shmem_int32_atomic_fetch_or pshmem_int32_atomic_fetch_or
#pragma weak shmem_int64_atomic_fetch_or = pshmem_int64_atomic_fetch_or
#define shmem_int64_atomic_fetch_or pshmem_int64_atomic_fetch_or
#pragma weak shmem_uint32_atomic_fetch_or = pshmem_uint32_atomic_fetch_or
#define shmem_uint32_atomic_fetch_or pshmem_uint32_atomic_fetch_or
#pragma weak shmem_uint64_atomic_fetch_or = pshmem_uint64_atomic_fetch_or
#define shmem_uint64_atomic_fetch_or pshmem_uint64_atomic_fetch_or

#pragma weak shmem_uint_atomic_fetch_and = pshmem_uint_atomic_fetch_and
#define shmem_uint_atomic_fetch_and pshmem_uint_atomic_fetch_and
#pragma weak shmem_ulong_atomic_fetch_and = pshmem_ulong_atomic_fetch_and
#define shmem_ulong_atomic_fetch_and pshmem_ulong_atomic_fetch_and
#pragma weak shmem_ulonglong_atomic_fetch_and = pshmem_ulonglong_atomic_fetch_and
#define shmem_ulonglong_atomic_fetch_and pshmem_ulonglong_atomic_fetch_and
#pragma weak shmem_int32_atomic_fetch_and = pshmem_int32_atomic_fetch_and
#define shmem_int32_atomic_fetch_and pshmem_int32_atomic_fetch_and
#pragma weak shmem_int64_atomic_fetch_and = pshmem_int64_atomic_fetch_and
#define shmem_int64_atomic_fetch_and pshmem_int64_atomic_fetch_and
#pragma weak shmem_uint32_atomic_fetch_and = pshmem_uint32_atomic_fetch_and
#define shmem_uint32_atomic_fetch_and pshmem_uint32_atomic_fetch_and
#pragma weak shmem_uint64_atomic_fetch_and = pshmem_uint64_atomic_fetch_and
#define shmem_uint64_atomic_fetch_and pshmem_uint64_atomic_fetch_and
#endif /* ENABLE_PSHMEM */

API_DEF_AMO2_NBI(fetch_xor, uint, unsigned int)
API_DEF_AMO2_NBI(fetch_xor, ulong, unsigned long)
API_DEF_AMO2_NBI(fetch_xor, ulonglong, unsigned long long)
API_DEF_AMO2_NBI(fetch_xor, int32, int32_t)
API_DEF_AMO2_NBI(fetch_xor, int64, int64_t)
API_DEF_AMO2_NBI(fetch_xor, uint32, uint32_t)
API_DEF_AMO2_NBI(fetch_xor, uint64, uint64_t)

API_DEF_AMO2_NBI(fetch_or, uint, unsigned int)
API_DEF_AMO2_NBI(fetch_or, ulong, unsigned long)
API_DEF_AMO2_NBI(fetch_or, ulonglong, unsigned long long)
API_DEF_AMO2_NBI(fetch_or, int32, int32_t)
API_DEF_AMO2_NBI(fetch_or, int64, int64_t)
API_DEF_AMO2_NBI(fetch_or, uint32, uint32_t)
API_DEF_AMO2_NBI(fetch_or, uint64, uint64_t)

API_DEF_AMO2_NBI(fetch_and, uint, unsigned int)
API_DEF_AMO2_NBI(fetch_and, ulong, unsigned long)
API_DEF_AMO2_NBI(fetch_and, ulonglong, unsigned long long)
API_DEF_AMO2_NBI(fetch_and, int32, int32_t)
API_DEF_AMO2_NBI(fetch_and, int64, int64_t)
API_DEF_AMO2_NBI(fetch_and, uint32, uint32_t)
API_DEF_AMO2_NBI(fetch_and, uint64, uint64_t)

#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_int_atomic_fetch_add_nbi = pshmem_ctx_int_atomic_fetch_add_nbi
#define shmem_ctx_int_atomic_fetch_add_nbi pshmem_ctx_int_atomic_fetch_add_nbi
#pragma weak shmem_ctx_long_atomic_fetch_add_nbi = pshmem_ctx_long_atomic_fetch_add_nbi
#define shmem_ctx_long_atomic_fetch_add_nbi pshmem_ctx_long_atomic_fetch_add_nbi
#pragma weak shmem_ctx_longlong_atomic_fetch_add_nbi = pshmem_ctx_longlong_atomic_fetch_add_nbi
#define shmem_ctx_longlong_atomic_fetch_add_nbi pshmem_ctx_longlong_atomic_fetch_add_nbi
#pragma weak shmem_ctx_uint_atomic_fetch_add_nbi = pshmem_ctx_uint_atomic_fetch_add_nbi
#define shmem_ctx_uint_atomic_fetch_add_nbi pshmem_ctx_uint_atomic_fetch_add_nbi
#pragma weak shmem_ctx_ulong_atomic_fetch_add_nbi = pshmem_ctx_ulong_atomic_fetch_add_nbi
#define shmem_ctx_ulong_atomic_fetch_add_nbi pshmem_ctx_ulong_atomic_fetch_add_nbi
#pragma weak shmem_ctx_ulonglong_atomic_fetch_add_nbi = pshmem_ctx_ulonglong_atomic_fetch_add_nbi
#define shmem_ctx_ulonglong_atomic_fetch_add_nbi pshmem_ctx_ulonglong_atomic_fetch_add_nbi
#pragma weak shmem_ctx_int32_atomic_fetch_add_nbi = pshmem_ctx_int32_atomic_fetch_add_nbi
#define shmem_ctx_int32_atomic_fetch_add_nbi pshmem_ctx_int32_atomic_fetch_add_nbi
#pragma weak shmem_ctx_int64_atomic_fetch_add_nbi = pshmem_ctx_int64_atomic_fetch_add_nbi
#define shmem_ctx_int64_atomic_fetch_add_nbi pshmem_ctx_int64_atomic_fetch_add_nbi
#pragma weak shmem_ctx_uint32_atomic_fetch_add_nbi = pshmem_ctx_uint32_atomic_fetch_add_nbi
#define shmem_ctx_uint32_atomic_fetch_add_nbi pshmem_ctx_uint32_atomic_fetch_add_nbi
#pragma weak shmem_ctx_uint64_atomic_fetch_add_nbi = pshmem_ctx_uint64_atomic_fetch_add_nbi
#define shmem_ctx_uint64_atomic_fetch_add_nbi pshmem_ctx_uint64_atomic_fetch_add_nbi
#pragma weak shmem_ctx_size_atomic_fetch_add_nbi = pshmem_ctx_size_atomic_fetch_add_nbi
#define shmem_ctx_size_atomic_fetch_add_nbi pshmem_ctx_size_atomic_fetch_add_nbi
#pragma weak shmem_ctx_ptrdiff_atomic_fetch_add_nbi = pshmem_ctx_ptrdiff_atomic_fetch_add_nbi
#define shmem_ctx_ptrdiff_atomic_fetch_add_nbi pshmem_ctx_ptrdiff_atomic_fetch_add_nbi
#endif /* ENABLE_PSHMEM */

/*
 * fetch-and-add
 */

#undef SHMEM_CTX_TYPE_FADD_NBI



#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_int_atomic_fetch_inc_nbi = pshmem_ctx_int_atomic_fetch_inc_nbi
#define shmem_ctx_int_atomic_fetch_inc_nbi pshmem_ctx_int_atomic_fetch_inc_nbi
#pragma weak shmem_ctx_long_atomic_fetch_inc_nbi = pshmem_ctx_long_atomic_fetch_inc_nbi
#define shmem_ctx_long_atomic_fetch_inc_nbi pshmem_ctx_long_atomic_fetch_inc_nbi
#pragma weak shmem_ctx_longlong_atomic_fetch_inc_nbi = pshmem_ctx_longlong_atomic_fetch_inc_nbi
#define shmem_ctx_longlong_atomic_fetch_inc_nbi pshmem_ctx_longlong_atomic_fetch_inc_nbi
#pragma weak shmem_ctx_uint_atomic_fetch_inc_nbi = pshmem_ctx_uint_atomic_fetch_inc_nbi
#define shmem_ctx_uint_atomic_fetch_inc_nbi pshmem_ctx_uint_atomic_fetch_inc_nbi
#pragma weak shmem_ctx_ulong_atomic_fetch_inc_nbi = pshmem_ctx_ulong_atomic_fetch_inc_nbi
#define shmem_ctx_ulong_atomic_fetch_inc_nbi pshmem_ctx_ulong_atomic_fetch_inc_nbi
#pragma weak shmem_ctx_ulonglong_atomic_fetch_inc_nbi = pshmem_ctx_ulonglong_atomic_fetch_inc_nbi
#define shmem_ctx_ulonglong_atomic_fetch_inc_nbi pshmem_ctx_ulonglong_atomic_fetch_inc_nbi
#pragma weak shmem_ctx_int32_atomic_fetch_inc_nbi = pshmem_ctx_int32_atomic_fetch_inc_nbi
#define shmem_ctx_int32_atomic_fetch_inc_nbi pshmem_ctx_int32_atomic_fetch_inc_nbi
#pragma weak shmem_ctx_int64_atomic_fetch_inc_nbi = pshmem_ctx_int64_atomic_fetch_inc_nbi
#define shmem_ctx_int64_atomic_fetch_inc_nbi pshmem_ctx_int64_atomic_fetch_inc_nbi
#pragma weak shmem_ctx_uint32_atomic_fetch_inc_nbi = pshmem_ctx_uint32_atomic_fetch_inc_nbi
#define shmem_ctx_uint32_atomic_fetch_inc_nbi pshmem_ctx_uint32_atomic_fetch_inc_nbi
#pragma weak shmem_ctx_uint64_atomic_fetch_inc_nbi = pshmem_ctx_uint64_atomic_fetch_inc_nbi
#define shmem_ctx_uint64_atomic_fetch_inc_nbi pshmem_ctx_uint64_atomic_fetch_inc_nbi
#pragma weak shmem_ctx_size_atomic_fetch_inc_nbi = pshmem_ctx_size_atomic_fetch_inc_nbi
#define shmem_ctx_size_atomic_fetch_inc_nbi pshmem_ctx_size_atomic_fetch_inc_nbi
#pragma weak shmem_ctx_ptrdiff_atomic_fetch_inc_nbi = pshmem_ctx_ptrdiff_atomic_fetch_inc_nbi
#define shmem_ctx_ptrdiff_atomic_fetch_inc_nbi pshmem_ctx_ptrdiff_atomic_fetch_inc_nbi
#endif /* ENABLE_PSHMEM */

/*
 * fetch-and-increment
 */

#undef SHMEM_CTX_TYPE_FINC_NBI



#undef SHMEM_CTX_TYPE_FETCH_NBI




#ifdef ENABLE_PSHMEM
#pragma weak shmem_ctx_uint_atomic_fetch_xor_nbi = pshmem_ctx_uint_atomic_fetch_xor_nbi
#define shmem_ctx_uint_atomic_fetch_xor_nbi pshmem_ctx_uint_atomic_fetch_xor_nbi
#pragma weak shmem_ctx_ulong_atomic_fetch_xor_nbi = pshmem_ctx_ulong_atomic_fetch_xor_nbi
#define shmem_ctx_ulong_atomic_fetch_xor_nbi pshmem_ctx_ulong_atomic_fetch_xor_nbi
#pragma weak shmem_ctx_ulonglong_atomic_fetch_xor_nbi = pshmem_ctx_ulonglong_atomic_fetch_xor_nbi
#define shmem_ctx_ulonglong_atomic_fetch_xor_nbi pshmem_ctx_ulonglong_atomic_fetch_xor_nbi
#pragma weak shmem_ctx_int32_atomic_fetch_xor_nbi = pshmem_ctx_int32_atomic_fetch_xor_nbi
#define shmem_ctx_int32_atomic_fetch_xor_nbi pshmem_ctx_int32_atomic_fetch_xor_nbi
#pragma weak shmem_ctx_int64_atomic_fetch_xor_nbi = pshmem_ctx_int64_atomic_fetch_xor_nbi
#define shmem_ctx_int64_atomic_fetch_xor_nbi pshmem_ctx_int64_atomic_fetch_xor_nbi
#pragma weak shmem_ctx_uint32_atomic_fetch_xor_nbi = pshmem_ctx_uint32_atomic_fetch_xor_nbi
#define shmem_ctx_uint32_atomic_fetch_xor_nbi pshmem_ctx_uint32_atomic_fetch_xor_nbi
#pragma weak shmem_ctx_uint64_atomic_fetch_xor_nbi = pshmem_ctx_uint64_atomic_fetch_xor_nbi
#define shmem_ctx_uint64_atomic_fetch_xor_nbi pshmem_ctx_uint64_atomic_fetch_xor_nbi
#pragma weak shmem_ctx_uint_atomic_fetch_or_nbi = pshmem_ctx_uint_atomic_fetch_or_nbi
#define shmem_ctx_uint_atomic_fetch_or_nbi pshmem_ctx_uint_atomic_fetch_or_nbi
#pragma weak shmem_ctx_ulong_atomic_fetch_or_nbi = pshmem_ctx_ulong_atomic_fetch_or_nbi
#define shmem_ctx_ulong_atomic_fetch_or_nbi pshmem_ctx_ulong_atomic_fetch_or_nbi
#pragma weak shmem_ctx_ulonglong_atomic_fetch_or_nbi = pshmem_ctx_ulonglong_atomic_fetch_or_nbi
#define shmem_ctx_ulonglong_atomic_fetch_or_nbi pshmem_ctx_ulonglong_atomic_fetch_or_nbi
#pragma weak shmem_ctx_int32_atomic_fetch_or_nbi = pshmem_ctx_int32_atomic_fetch_or_nbi
#define shmem_ctx_int32_atomic_fetch_or_nbi pshmem_ctx_int32_atomic_fetch_or_nbi
#pragma weak shmem_ctx_int64_atomic_fetch_or_nbi = pshmem_ctx_int64_atomic_fetch_or_nbi
#define shmem_ctx_int64_atomic_fetch_or_nbi pshmem_ctx_int64_atomic_fetch_or_nbi
#pragma weak shmem_ctx_uint32_atomic_fetch_or_nbi = pshmem_ctx_uint32_atomic_fetch_or_nbi
#define shmem_ctx_uint32_atomic_fetch_or_nbi pshmem_ctx_uint32_atomic_fetch_or_nbi
#pragma weak shmem_ctx_uint64_atomic_fetch_or_nbi = pshmem_ctx_uint64_atomic_fetch_or_nbi
#define shmem_ctx_uint64_atomic_fetch_or_nbi pshmem_ctx_uint64_atomic_fetch_or_nbi
#pragma weak shmem_ctx_uint_atomic_fetch_and_nbi = pshmem_ctx_uint_atomic_fetch_and_nbi
#define shmem_ctx_uint_atomic_fetch_and_nbi pshmem_ctx_uint_atomic_fetch_and_nbi
#pragma weak shmem_ctx_ulong_atomic_fetch_and_nbi = pshmem_ctx_ulong_atomic_fetch_and_nbi
#define shmem_ctx_ulong_atomic_fetch_and_nbi pshmem_ctx_ulong_atomic_fetch_and_nbi
#pragma weak shmem_ctx_ulonglong_atomic_fetch_and_nbi = pshmem_ctx_ulonglong_atomic_fetch_and_nbi
#define shmem_ctx_ulonglong_atomic_fetch_and_nbi pshmem_ctx_ulonglong_atomic_fetch_and_nbi
#pragma weak shmem_ctx_int32_atomic_fetch_and_nbi = pshmem_ctx_int32_atomic_fetch_and_nbi
#define shmem_ctx_int32_atomic_fetch_and_nbi pshmem_ctx_int32_atomic_fetch_and_nbi
#pragma weak shmem_ctx_int64_atomic_fetch_and_nbi = pshmem_ctx_int64_atomic_fetch_and_nbi
#define shmem_ctx_int64_atomic_fetch_and_nbi pshmem_ctx_int64_atomic_fetch_and_nbi
#pragma weak shmem_ctx_uint32_atomic_fetch_and_nbi = pshmem_ctx_uint32_atomic_fetch_and_nbi
#define shmem_ctx_uint32_atomic_fetch_and_nbi pshmem_ctx_uint32_atomic_fetch_and_nbi
#pragma weak shmem_ctx_uint64_atomic_fetch_and_nbi = pshmem_ctx_uint64_atomic_fetch_and_nbi
#define shmem_ctx_uint64_atomic_fetch_and_nbi pshmem_ctx_uint64_atomic_fetch_and_nbi
#endif /* ENABLE_PSHMEM */

/*
 * fetch-bitwise NBI
 */

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

SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(xor, uint, unsigned int)
SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(xor, ulong, unsigned long)
SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(xor, ulonglong, unsigned long long)
SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(xor, int32, int32_t)
SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(xor, int64, int64_t)
SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(xor, uint32, uint32_t)
SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(xor, uint64, uint64_t)

SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(or, uint, unsigned int)
SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(or, ulong, unsigned long)
SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(or, ulonglong, unsigned long long)
SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(or, int32, int32_t)
SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(or, int64, int64_t)
SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(or, uint32, uint32_t)
SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(or, uint64, uint64_t)

SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(and, uint, unsigned int)
SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(and, ulong, unsigned long)
SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(and, ulonglong, unsigned long long)
SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(and, int32, int32_t)
SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(and, int64, int64_t)
SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(and, uint32, uint32_t)
SHMEM_CTX_TYPE_FETCH_BITWISE_NBI(and, uint64, uint64_t)

#undef SHMEM_CTX_TYPE_FETCH_BITWISE_NBI
