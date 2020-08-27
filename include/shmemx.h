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

    enum interoperability {
        UPC_THREADS_ARE_PES = 0,
        MPI_PROCESSES_ARE_PES,
        SHMEM_INITIALIZES_MPI,
        MPI_INITIALIZES_SHMEM
    };

    int shmemx_query_interoperability(int property);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _SHMEMX_H */
