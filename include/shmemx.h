/* For license: see LICENSE file at top-level */

#ifndef _SHMEMX_H
#define _SHMEMX_H 1

#include <shmem.h>

#ifdef __cplusplus
extern "C"
{
#endif  /* __cplusplus */

    /*
     * new ideas (not part of formal 1.x API)
     */

    /*
     * wallclock time
     *
     */

    /**
     * @brief returns the number of seconds since the program
     * started running
     *
     * @section Synopsis:
     *
     * @substitute c C/C++
     * @code
     double shmemx_wtime(void)
     * @endcode
     *
     * @return Returns the number of seconds since the program started(epoch).
     *
     * @section Note: shmemx_wtime does not indicate any error code; if it
     * is unable to detect the elapsed time, the return value is
     * undefined.  The time may be different on each PE, but the epoch
     * from which the time is measured will not change while OpenSHMEM is
     * active.
     *
     */
    double shmemx_wtime(void);

    /*
     * profiling
     */
    void shmemx_pcontrol(int level);

    /*
     * address translation
     *
     */

    /**
     * @brief returns the symmetric address on another PE corresponding to
     * the symmetric address on this PE
     *
     * @section Synopsis:
     *
     * @substitute c C/C++
     * @code
     void *shmemx_lookup_remote_addr(void *addr, int pe)
     * @endcode
     *
     * @return Returns the address corresponding to "addr" on PE "pe"
     *
     *
     */
    void *shmemx_lookup_remote_addr(void *addr, int pe);

    /*
     * non-blocking fence/quiet
     *
     */

    /**
     * @brief check whether all communication operations issued prior to
     * this call have satisfied the fence semantic.
     *
     * @section Synopsis:
     *
     * @substitute c C/C++
     * @code
     int shmemx_ctx_fence_test(shmem_ctx_t ctx)
     * @endcode
     *
     * @return Returns non-zero if all communication operations issued
     * prior to this call have satisfied the fence semantic for the
     * given context, 0 otherwise.
     *
     */
    int shmemx_ctx_fence_test(shmem_ctx_t ctx);

    /**
     * @brief check whether all communication operations issued prior to
     * this call have satisfied the quiet semantic.
     *
     * @section Synopsis:
     *
     * @substitute c C/C++
     * @code
     int shmemx_ctx_quiet_test(shmem_ctx_t ctx)
     * @endcode
     *
     * @return Returns non-zero if all communication operations issued
     * prior to this call have satisfied the quiet semantic for the
     * given context, 0 otherwise.
     *
     */
    int shmemx_ctx_quiet_test(shmem_ctx_t ctx);

    /**
     * @brief check whether all communication operations issued prior to
     * this call have satisfied the fence semantic.
     *
     * @section Synopsis:
     *
     * @substitute c C/C++
     * @code
     int shmemx_fence_test(void)
     * @endcode
     *
     * @return Returns non-zero if all communication operations issued
     * prior to this call have satisfied the fence semantic for the
     * default context, 0 otherwise.
     *
     */
    int shmemx_fence_test(void);

    /**
     * @brief check whether all communication operations issued prior to
     * this call have satisfied the quiet semantic.
     *
     * @section Synopsis:
     *
     * @substitute c C/C++
     * @code
     int shmemx_quiet_test(void)
     * @endcode
     *
     * @return Returns non-zero if all communication operations issued
     * prior to this call have satisfied the quiet semantic for the
     * default context, 0 otherwise.
     *
     */
    int shmemx_quiet_test(void);

    /*
     * context sessions
     */
    void shmemx_ctx_start_session(shmem_ctx_t ctx);
    void shmemx_ctx_end_session(shmem_ctx_t ctx);

    /*
     * multiple symmetric heap support
     */
    typedef int shmemx_heap_index_t;

    shmemx_heap_index_t shmemx_name_to_index(const char *name);
    const char *shmemx_index_to_name(shmemx_heap_index_t index);

    void *shmemx_malloc_by_index(shmemx_heap_index_t index,
                                 size_t s);
    void *shmemx_calloc_by_index(shmemx_heap_index_t index,
                                 size_t n, size_t s);
    void shmemx_free_by_index(shmemx_heap_index_t index,
                              void *p);
    void *shmemx_realloc_by_index(shmemx_heap_index_t index,
                                  void *p, size_t s);
    void *shmemx_align_by_index(shmemx_heap_index_t index,
                                size_t a, size_t s);
    void *shmemx_malloc_by_name(const char *name,
                                size_t s);
    void *shmemx_calloc_by_name(const char *name,
                                size_t n, size_t s);
    void shmemx_free_by_name(const char *name,
                             void *p);
    void *shmemx_realloc_by_name(const char *name,
                                 void *p, size_t s);
    void *shmemx_align_by_name(const char *name,
                               size_t a, size_t s);

#if SHMEM_HAS_C11

#define shmemx_malloc(_arg1, _s)                            \
    _Generic(_arg1,                                         \
             shmemx_heap_index_t: shmemx_malloc_by_index,   \
             char *: shmemx_malloc_by_name                  \
             )(_arg1, _s)

#define shmemx_calloc(_arg1, _n, _s)                        \
    _Generic(_arg1,                                         \
             shmemx_heap_index_t: shmemx_calloc_by_index,   \
             char *: shmemx_calloc_by_name                  \
             )(_arg1, _n, _s)

#define shmemx_free(_arg1, _p)                          \
    _Generic(_arg1,                                     \
             shmemx_heap_index_t: shmemx_free_by_index, \
             char *: shmemx_free_by_name                \
             )(_arg1, _p)

#define shmemx_realloc(_arg1, _p, _s)                       \
    _Generic(_arg1,                                         \
             shmemx_heap_index_t: shmemx_realloc_by_index,  \
             char *: shmemx_realloc_by_name                 \
             )(_arg1, _p, _s)

#define shmemx_align(_arg1, _a, _s)                         \
    _Generic(_arg1,                                         \
             shmemx_heap_index_t: shmemx_align_by_index,    \
             char *: shmemx_align_by_name                   \
             )(_arg1, _a, _s)

#endif /* SHMEM_HAS_C11 */

#define APIX_DECL_CTX_PUTGET_SIGNAL(_opname, _name, _type)           \
    void                                                             \
    shmemx_ctx_##_name##_##_opname##_signal(shmem_ctx_t ctx,         \
                                            _type *dest,             \
                                            const _type *src,        \
                                            size_t nelems,           \
                                            uint64_t *sig,           \
                                            uint64_t sigval,         \
                                            int pe)

    APIX_DECL_CTX_PUTGET_SIGNAL(put, float, float);
    APIX_DECL_CTX_PUTGET_SIGNAL(put, double, double);
    APIX_DECL_CTX_PUTGET_SIGNAL(put, longdouble, long double);
    APIX_DECL_CTX_PUTGET_SIGNAL(put, schar, signed char);
    APIX_DECL_CTX_PUTGET_SIGNAL(put, char, char);
    APIX_DECL_CTX_PUTGET_SIGNAL(put, short, short);
    APIX_DECL_CTX_PUTGET_SIGNAL(put, int, int);
    APIX_DECL_CTX_PUTGET_SIGNAL(put, long, long);
    APIX_DECL_CTX_PUTGET_SIGNAL(put, longlong, long long);
    APIX_DECL_CTX_PUTGET_SIGNAL(put, uchar, unsigned char);
    APIX_DECL_CTX_PUTGET_SIGNAL(put, ushort, unsigned short);
    APIX_DECL_CTX_PUTGET_SIGNAL(put, uint, unsigned int);
    APIX_DECL_CTX_PUTGET_SIGNAL(put, ulong, unsigned long);
    APIX_DECL_CTX_PUTGET_SIGNAL(put, ulonglong, unsigned long long);
    APIX_DECL_CTX_PUTGET_SIGNAL(put, int8, int8_t);
    APIX_DECL_CTX_PUTGET_SIGNAL(put, int16, int16_t);
    APIX_DECL_CTX_PUTGET_SIGNAL(put, int32, int32_t);
    APIX_DECL_CTX_PUTGET_SIGNAL(put, int64, int64_t);
    APIX_DECL_CTX_PUTGET_SIGNAL(put, uint8, uint8_t);
    APIX_DECL_CTX_PUTGET_SIGNAL(put, uint16, uint16_t);
    APIX_DECL_CTX_PUTGET_SIGNAL(put, uint32, uint32_t);
    APIX_DECL_CTX_PUTGET_SIGNAL(put, uint64, uint64_t);
    APIX_DECL_CTX_PUTGET_SIGNAL(put, size, size_t);
    APIX_DECL_CTX_PUTGET_SIGNAL(put, ptrdiff, ptrdiff_t);

    APIX_DECL_CTX_PUTGET_SIGNAL(get, float, float);
    APIX_DECL_CTX_PUTGET_SIGNAL(get, double, double);
    APIX_DECL_CTX_PUTGET_SIGNAL(get, longdouble, long double);
    APIX_DECL_CTX_PUTGET_SIGNAL(get, schar, signed char);
    APIX_DECL_CTX_PUTGET_SIGNAL(get, char, char);
    APIX_DECL_CTX_PUTGET_SIGNAL(get, short, short);
    APIX_DECL_CTX_PUTGET_SIGNAL(get, int, int);
    APIX_DECL_CTX_PUTGET_SIGNAL(get, long, long);
    APIX_DECL_CTX_PUTGET_SIGNAL(get, longlong, long long);
    APIX_DECL_CTX_PUTGET_SIGNAL(get, uchar, unsigned char);
    APIX_DECL_CTX_PUTGET_SIGNAL(get, ushort, unsigned short);
    APIX_DECL_CTX_PUTGET_SIGNAL(get, uint, unsigned int);
    APIX_DECL_CTX_PUTGET_SIGNAL(get, ulong, unsigned long);
    APIX_DECL_CTX_PUTGET_SIGNAL(get, ulonglong, unsigned long long);
    APIX_DECL_CTX_PUTGET_SIGNAL(get, int8, int8_t);
    APIX_DECL_CTX_PUTGET_SIGNAL(get, int16, int16_t);
    APIX_DECL_CTX_PUTGET_SIGNAL(get, int32, int32_t);
    APIX_DECL_CTX_PUTGET_SIGNAL(get, int64, int64_t);
    APIX_DECL_CTX_PUTGET_SIGNAL(get, uint8, uint8_t);
    APIX_DECL_CTX_PUTGET_SIGNAL(get, uint16, uint16_t);
    APIX_DECL_CTX_PUTGET_SIGNAL(get, uint32, uint32_t);
    APIX_DECL_CTX_PUTGET_SIGNAL(get, uint64, uint64_t);
    APIX_DECL_CTX_PUTGET_SIGNAL(get, size, size_t);
    APIX_DECL_CTX_PUTGET_SIGNAL(get, ptrdiff, ptrdiff_t);

#define APIX_DECL_PUTGET_SIGNAL(_opname, _name, _type)              \
        void                                                        \
        shmemx_##_name##_##_opname##_signal(_type *dest,            \
                                            const _type *src,       \
                                            size_t nelems,          \
                                            uint64_t *sig,          \
                                            uint64_t sigval,        \
                                            int pe)

    APIX_DECL_PUTGET_SIGNAL(put, float, float);
    APIX_DECL_PUTGET_SIGNAL(put, double, double);
    APIX_DECL_PUTGET_SIGNAL(put, longdouble, long double);
    APIX_DECL_PUTGET_SIGNAL(put, schar, signed char);
    APIX_DECL_PUTGET_SIGNAL(put, char, char);
    APIX_DECL_PUTGET_SIGNAL(put, short, short);
    APIX_DECL_PUTGET_SIGNAL(put, int, int);
    APIX_DECL_PUTGET_SIGNAL(put, long, long);
    APIX_DECL_PUTGET_SIGNAL(put, longlong, long long);
    APIX_DECL_PUTGET_SIGNAL(put, uchar, unsigned char);
    APIX_DECL_PUTGET_SIGNAL(put, ushort, unsigned short);
    APIX_DECL_PUTGET_SIGNAL(put, uint, unsigned int);
    APIX_DECL_PUTGET_SIGNAL(put, ulong, unsigned long);
    APIX_DECL_PUTGET_SIGNAL(put, ulonglong, unsigned long long);
    APIX_DECL_PUTGET_SIGNAL(put, int8, int8_t);
    APIX_DECL_PUTGET_SIGNAL(put, int16, int16_t);
    APIX_DECL_PUTGET_SIGNAL(put, int32, int32_t);
    APIX_DECL_PUTGET_SIGNAL(put, int64, int64_t);
    APIX_DECL_PUTGET_SIGNAL(put, uint8, uint8_t);
    APIX_DECL_PUTGET_SIGNAL(put, uint16, uint16_t);
    APIX_DECL_PUTGET_SIGNAL(put, uint32, uint32_t);
    APIX_DECL_PUTGET_SIGNAL(put, uint64, uint64_t);
    APIX_DECL_PUTGET_SIGNAL(put, size, size_t);
    APIX_DECL_PUTGET_SIGNAL(put, ptrdiff, ptrdiff_t);

    APIX_DECL_PUTGET_SIGNAL(get, float, float);
    APIX_DECL_PUTGET_SIGNAL(get, double, double);
    APIX_DECL_PUTGET_SIGNAL(get, longdouble, long double);
    APIX_DECL_PUTGET_SIGNAL(get, schar, signed char);
    APIX_DECL_PUTGET_SIGNAL(get, char, char);
    APIX_DECL_PUTGET_SIGNAL(get, short, short);
    APIX_DECL_PUTGET_SIGNAL(get, int, int);
    APIX_DECL_PUTGET_SIGNAL(get, long, long);
    APIX_DECL_PUTGET_SIGNAL(get, longlong, long long);
    APIX_DECL_PUTGET_SIGNAL(get, uchar, unsigned char);
    APIX_DECL_PUTGET_SIGNAL(get, ushort, unsigned short);
    APIX_DECL_PUTGET_SIGNAL(get, uint, unsigned int);
    APIX_DECL_PUTGET_SIGNAL(get, ulong, unsigned long);
    APIX_DECL_PUTGET_SIGNAL(get, ulonglong, unsigned long long);
    APIX_DECL_PUTGET_SIGNAL(get, int8, int8_t);
    APIX_DECL_PUTGET_SIGNAL(get, int16, int16_t);
    APIX_DECL_PUTGET_SIGNAL(get, int32, int32_t);
    APIX_DECL_PUTGET_SIGNAL(get, int64, int64_t);
    APIX_DECL_PUTGET_SIGNAL(get, uint8, uint8_t);
    APIX_DECL_PUTGET_SIGNAL(get, uint16, uint16_t);
    APIX_DECL_PUTGET_SIGNAL(get, uint32, uint32_t);
    APIX_DECL_PUTGET_SIGNAL(get, uint64, uint64_t);
    APIX_DECL_PUTGET_SIGNAL(get, size, size_t);
    APIX_DECL_PUTGET_SIGNAL(get, ptrdiff, ptrdiff_t);

    enum interoperability {
        UPC_THREADS_ARE_PES = 0,
        FORTRAN_IMAGES_ARE_PES,
        MPI_PROCESSES_ARE_PES,
        SHMEM_INITIALIZES_MPI,
        MPI_INITIALIZES_SHMEM
    };

    int shmemx_query_interoperability(int property);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _SHMEMX_H */
