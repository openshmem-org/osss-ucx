/* For license: see LICENSE file at top-level */

#ifndef _PSHMEMX_H
#define _PSHMEMX_H 1

#include <pshmem.h>
#include <shmemx.h>

#ifdef __cplusplus
extern "C"
{
#endif  /* __cplusplus */

    /*
     * new ideas (not part of formal 1.x API)
     */

    void pshmemx_short_put_nb(short *target, const short *source,
                              size_t nelems, int pe,
                              shmemx_request_handle_t *desc);
    void pshmemx_int_put_nb(int *target, const int *source,
                            size_t nelems, int pe,
                            shmemx_request_handle_t *desc);
    void pshmemx_long_put_nb(long *target, const long *source,
                             size_t nelems, int pe,
                             shmemx_request_handle_t *desc);
    void pshmemx_longlong_put_nb(long long *target,
                                 const long long *source, size_t nelems,
                                 int pe, shmemx_request_handle_t *desc);
    void pshmemx_longdouble_put_nb(long double *target,
                                   const long double *source,
                                   size_t nelems, int pe,
                                   shmemx_request_handle_t *desc);
    void pshmemx_double_put_nb(double *target, const double *source,
                               size_t nelems, int pe,
                               shmemx_request_handle_t *desc);
    void pshmemx_float_put_nb(float *target, const float *source,
                              size_t nelems, int pe,
                              shmemx_request_handle_t *desc);
    void pshmemx_putmem_nb(void *target, const void *source,
                           size_t nelems, int pe,
                           shmemx_request_handle_t *desc);
    void pshmemx_put32_nb(void *target, const void *source,
                          size_t nelems, int pe,
                          shmemx_request_handle_t *desc);
    void pshmemx_put64_nb(void *target, const void *source,
                          size_t nelems, int pe,
                          shmemx_request_handle_t *desc);
    void pshmemx_put128_nb(void *target, const void *source,
                           size_t nelems, int pe,
                           shmemx_request_handle_t *desc);

    void pshmemx_short_get_nb(short *dest, const short *src, size_t nelems,
                              int pe, shmemx_request_handle_t *desc);
    void pshmemx_int_get_nb(int *dest, const int *src, size_t nelems,
                            int pe, shmemx_request_handle_t *desc);
    void pshmemx_long_get_nb(long *dest, const long *src, size_t nelems,
                             int pe, shmemx_request_handle_t *desc);
    void pshmemx_longlong_get_nb(long long *dest, const long long *src,
                                 size_t nelems, int pe,
                                 shmemx_request_handle_t *desc);
    void pshmemx_longdouble_get_nb(long double *dest,
                                   const long double *src,
                                   size_t nelems, int pe,
                                   shmemx_request_handle_t *desc);
    void pshmemx_double_get_nb(double *dest, const double *src,
                               size_t nelems, int pe,
                               shmemx_request_handle_t *desc);
    void pshmemx_complexd_get_nb(COMPLEXIFY(double) * dest,
                                 const COMPLEXIFY(double) * src,
                                 size_t nelems, int pe,
                                 shmemx_request_handle_t *desc);
    void pshmemx_float_get_nb(float *dest, const float *src, size_t nelems,
                              int pe, shmemx_request_handle_t *desc);
    void pshmemx_getmem_nb(void *dest, const void *src, size_t nelems,
                           int pe, shmemx_request_handle_t *desc);
    void pshmemx_get32_nb(void *dest, const void *src, size_t nelems,
                          int pe, shmemx_request_handle_t *desc);
    void pshmemx_get64_nb(void *dest, const void *src, size_t nelems,
                          int pe, shmemx_request_handle_t *desc);
    void pshmemx_get128_nb(void *dest, const void *src, size_t nelems,
                           int pe, shmemx_request_handle_t *desc);

    void pshmemx_wait_req(shmemx_request_handle_t desc);
    void pshmemx_test_req(shmemx_request_handle_t desc, int *flag);


    /*
     * symmetric memory management
     */
    void *pshmalloc_nb(size_t size);
    void  pshfree_nb(void *addr);

#define pshmemx_malloc_nb(s)   pshmalloc_nb(s)
#define pshmemx_free_nb(a)     pshfree_nb(a)

    /*
     * wallclock time
     *
     */
    double pshmemx_wtime(void);

    /*
     * symmetric address translation
     *
     */
    void *pshmemx_lookup_remote_addr(void *addr, int pe);

    /*
     * non-blocking fence and quiet checks
     *
     */
    int shmemx_ctx_fence_test(shmem_ctx_t ctx);
    int shmemx_ctx_quiet_test(shmem_ctx_t ctx);

    int pshmemx_fence_test(void);
    int pshmemx_quiet_test(void);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _PSHMEMX_H */
