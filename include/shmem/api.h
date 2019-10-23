/* For license: see LICENSE file at top-level */

#ifndef _SHMEM_API_H
#define _SHMEM_API_H 1

#include <shmem/defs.h>
#include <shmem/depr.h>

#include <sys/types.h>
#include <stddef.h>               /* ptrdiff_t */
#include <stdint.h>               /* sized int types */
#include <stdarg.h>

/*
 * for handling the "I" (upper-case eye) macro for complex numbers
 *
 * and see end of file for tidy-up
 */
#ifdef I
# define shmemi_h_I_already_defined__
#endif  /* I */

#ifdef __cplusplus
extern "C"
{
#endif  /* __cplusplus */

    /*
     * start/stop & query
     */

    /**
     * @brief initializes the OpenSHMEM environment on the calling PE.
     *
     * @page start_pes
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void start_pes(int npes);
     @endcode
     *
     * @param npes the number of PEs participating in the program.  This
     * is ignored and should be set to 0.
     *
     * @section Effect
     * Initializes the OpenSHMEM environment on the calling PE.
     *
     * @section Return
     * None.
     *
     * @section Deprecated
     * by \ref shmem_init()
     *
     */
    void start_pes(int npes)
        _DEPRECATED_BY(shmem_init, 1.2);

    /**
     * @brief initializes the OpenSHMEM environment on the calling PE.
     * @page shmem_init
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_init(void);
     @endcode
     *
     * @section Effect
     * Initializes the OpenSHMEM environment on the calling PE.
     *
     * @section Return
     * None.
     *
     */
    void shmem_init(void);

    /**
     * @brief initializes the OpenSHMEM environment on the calling PE
     * and requests a threading support level
     *
     * @page shmem_init_thread
     *
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     int shmem_init_thread(int requested, int *provided);
     @endcode
     *
     * @section Effect
     *
     * Initializes the OpenSHMEM environment on the calling PE,
     * requesting thread support level "requested".  Routine returns
     * thread support level "provided".
     *
     * @section Return zero on success, non-zero otherwise
     *
     */
    int shmem_init_thread(int requested, int *provided);

    /**
     * @brief finalizes the OpenSHMEM environment on the calling PE.
     * @page shmem_finalize
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_finalize(void);
     @endcode
     *
     * @section Effect
     * A collective finalization of the OpenSHMEM environment on the
     * calling PE.  After a finalize call, no further OpenSHMEM calls
     * are permitted.  Any subsequent use has undefined effects.
     *
     * @section Return
     * None.
     *
     */
    void shmem_finalize(void);

    /**
     * @brief causes immediate exit from the OpenSHMEM program on all PEs.
     * @page shmem_global_exit
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_global_exit(int status);
     @endcode
     *
     * @section Effect
     * Called on 1 or more PEs, Causes immediate termination of the
     * program on all PEs.  Pending communication is flushed, files are
     * closed.  "status" allows the call to pass back information to the
     * execution environment.
     *
     * @section Return
     * None.
     *
     */
    void shmem_global_exit(int status);

    /**
     * @brief returns the "rank" or identity of the calling PE
     * @page _my_pe
     * @section Deprecated
     * by \ref shmem_my_pe()
     *
     */
    int _my_pe(void)
        _WUR _DEPRECATED_BY(shmem_my_pe, 1.2);

    /**
     * @brief returns the "rank" or identity of the calling PE
     * @page shmem_my_pe
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     int shmem_my_pe(void);
     @endcode
     *
     * @section Effect
     * None.
     *
     * @section Return
     * Rank of calling PE
     *
     */
    int shmem_my_pe(void) _WUR;

    /**
     * @brief These routines return the number of PEs in the program
     *
     * @section Deprecated by by shmem_n_pes()
     *
     */
    int _num_pes(void)
        _WUR _DEPRECATED_BY(shmem_n_pes, 1.2);

    /**
     * @brief returns the number of PEs in the program
     *
     * @page shmem_n_pes
     *
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     int shmem_n_pes(void);
     @endcode
     *
     * @section Effect
     * None.
     *
     * @section Return
     * Number of PEs in program
     *
     */
    int shmem_n_pes(void) _WUR;

    /**
     * @brief Supplies the supported threading level to the caller
     *
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_query_thread(int *provided);
     @endcode
     *
     * @section Effect
     * Supplies the supported threading level to the caller
     *
     * @section Return
     * None.
     *
     */
    void shmem_query_thread(int *provided);

    /**
     * @brief determines the major.minor version numbers of this release.
     * @page shmem_info_get_version
     * @section Synopsis
     *
     * @subsection c C
     @code
     void shmem_info_get_version(int *maj, int *min);
     @endcode
     *
     * @param[out] maj set to the release's major version number
     * @param[out] min set to the release's minor version number
     *
     * @section Effect
     * None.
     *
     * @section Return
     * None.
     *
     */
    void shmem_info_get_version(int *major, int *minor);

    /**
     * @brief determines a vandor-supplied name for this release.
     * @page shmem_info_get_name
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_info_get_name(char *name);
     @endcode
     *
     * @param[out] name contains the vendor-supplied release name
     *
     * @section Effect
     * None.
     *
     * @section Return
     * None.
     *
     */
    void shmem_info_get_name(char *name);

    /**
     * @brief Allows the user to control profiling
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_pcontrol(const int level, ...)
     @endcode
     *
     * @param[in] level Profilong level to use
     *
     * @section Effect
     * Sets the library's internal profiling behavior.
     *
     * @section Return
     * None.
     *
     */
    void shmem_pcontrol(const int level, ...);

    /*
     * I/O
     */

#define API_DECL_CTX_PUTGET(_opname, _name, _type)                      \
    /* see \ref shmem_ctx_long_##_opname() */                           \
    void shmem_ctx_##_name##_##_opname(shmem_ctx_t ctx,                 \
                                       _type *dest, const _type *src,   \
                                       size_t nelems, int pe);          \
    /* see \ref shmem_ctx_long_i##_opname() */                          \
    void shmem_ctx_##_name##_i##_opname(shmem_ctx_t ctx,                \
                                        _type *dest,                    \
                                        const _type *src,               \
                                        ptrdiff_t tst, ptrdiff_t sst,   \
                                        size_t nelems, int pe);         \
    /* see \ref shmem_ctx_long_##_opname##_nbi() */                     \
    void shmem_ctx_##_name##_##_opname##_nbi(shmem_ctx_t ctx,           \
                                             _type *dest,               \
                                             const _type *src,          \
                                             size_t nelems, int pe);

    API_DECL_CTX_PUTGET(put, float, float)
    API_DECL_CTX_PUTGET(put, double, double)
    API_DECL_CTX_PUTGET(put, longdouble, long double)
    API_DECL_CTX_PUTGET(put, schar, signed char)
    API_DECL_CTX_PUTGET(put, char, char)
    API_DECL_CTX_PUTGET(put, short, short)
    API_DECL_CTX_PUTGET(put, int, int)
    API_DECL_CTX_PUTGET(put, long, long)
    API_DECL_CTX_PUTGET(put, longlong, long long)
    API_DECL_CTX_PUTGET(put, uchar, unsigned char)
    API_DECL_CTX_PUTGET(put, ushort, unsigned short)
    API_DECL_CTX_PUTGET(put, uint, unsigned int)
    API_DECL_CTX_PUTGET(put, ulong, unsigned long)
    API_DECL_CTX_PUTGET(put, ulonglong, unsigned long long)
    API_DECL_CTX_PUTGET(put, int8, int8_t)
    API_DECL_CTX_PUTGET(put, int16, int16_t)
    API_DECL_CTX_PUTGET(put, int32, int32_t)
    API_DECL_CTX_PUTGET(put, int64, int64_t)
    API_DECL_CTX_PUTGET(put, uint8, uint8_t)
    API_DECL_CTX_PUTGET(put, uint16, uint16_t)
    API_DECL_CTX_PUTGET(put, uint32, uint32_t)
    API_DECL_CTX_PUTGET(put, uint64, uint64_t)
    API_DECL_CTX_PUTGET(put, size, size_t)
    API_DECL_CTX_PUTGET(put, ptrdiff, ptrdiff_t)

    API_DECL_CTX_PUTGET(get, float, float)
    API_DECL_CTX_PUTGET(get, double, double)
    API_DECL_CTX_PUTGET(get, longdouble, long double)
    API_DECL_CTX_PUTGET(get, schar, signed char)
    API_DECL_CTX_PUTGET(get, char, char)
    API_DECL_CTX_PUTGET(get, short, short)
    API_DECL_CTX_PUTGET(get, int, int)
    API_DECL_CTX_PUTGET(get, long, long)
    API_DECL_CTX_PUTGET(get, longlong, long long)
    API_DECL_CTX_PUTGET(get, uchar, unsigned char)
    API_DECL_CTX_PUTGET(get, ushort, unsigned short)
    API_DECL_CTX_PUTGET(get, uint, unsigned int)
    API_DECL_CTX_PUTGET(get, ulong, unsigned long)
    API_DECL_CTX_PUTGET(get, ulonglong, unsigned long long)
    API_DECL_CTX_PUTGET(get, int8, int8_t)
    API_DECL_CTX_PUTGET(get, int16, int16_t)
    API_DECL_CTX_PUTGET(get, int32, int32_t)
    API_DECL_CTX_PUTGET(get, int64, int64_t)
    API_DECL_CTX_PUTGET(get, uint8, uint8_t)
    API_DECL_CTX_PUTGET(get, uint16, uint16_t)
    API_DECL_CTX_PUTGET(get, uint32, uint32_t)
    API_DECL_CTX_PUTGET(get, uint64, uint64_t)
    API_DECL_CTX_PUTGET(get, size, size_t)
    API_DECL_CTX_PUTGET(get, ptrdiff, ptrdiff_t)

#undef API_DECL_CTX_PUTGET

#define API_DECL_PUTGET(_opname, _name, _type)                          \
    void shmem_##_name##_##_opname(_type *dest, const _type *src,       \
                              size_t nelems, int pe);                   \
    void shmem_##_name##_i##_opname(_type *dest,                        \
                               const _type *src,                        \
                               ptrdiff_t tst, ptrdiff_t sst,            \
                               size_t nelems, int pe);                  \
    void shmem_##_name##_##_opname##_nbi(_type *dest,                   \
                                    const _type *src,                   \
                                    size_t nelems, int pe);

    API_DECL_PUTGET(put, float, float)
    API_DECL_PUTGET(put, double, double)
    API_DECL_PUTGET(put, longdouble, long double)
    API_DECL_PUTGET(put, schar, signed char)
    API_DECL_PUTGET(put, char, char)
    API_DECL_PUTGET(put, short, short)
    API_DECL_PUTGET(put, int, int)
    API_DECL_PUTGET(put, long, long)
    API_DECL_PUTGET(put, longlong, long long)
    API_DECL_PUTGET(put, uchar, unsigned char)
    API_DECL_PUTGET(put, ushort, unsigned short)
    API_DECL_PUTGET(put, uint, unsigned int)
    API_DECL_PUTGET(put, ulong, unsigned long)
    API_DECL_PUTGET(put, ulonglong, unsigned long long)
    API_DECL_PUTGET(put, int8, int8_t)
    API_DECL_PUTGET(put, int16, int16_t)
    API_DECL_PUTGET(put, int32, int32_t)
    API_DECL_PUTGET(put, int64, int64_t)
    API_DECL_PUTGET(put, uint8, uint8_t)
    API_DECL_PUTGET(put, uint16, uint16_t)
    API_DECL_PUTGET(put, uint32, uint32_t)
    API_DECL_PUTGET(put, uint64, uint64_t)
    API_DECL_PUTGET(put, size, size_t)
    API_DECL_PUTGET(put, ptrdiff, ptrdiff_t)

    API_DECL_PUTGET(get, float, float)
    API_DECL_PUTGET(get, double, double)
    API_DECL_PUTGET(get, longdouble, long double)
    API_DECL_PUTGET(get, schar, signed char)
    API_DECL_PUTGET(get, char, char)
    API_DECL_PUTGET(get, short, short)
    API_DECL_PUTGET(get, int, int)
    API_DECL_PUTGET(get, long, long)
    API_DECL_PUTGET(get, longlong, long long)
    API_DECL_PUTGET(get, uchar, unsigned char)
    API_DECL_PUTGET(get, ushort, unsigned short)
    API_DECL_PUTGET(get, uint, unsigned int)
    API_DECL_PUTGET(get, ulong, unsigned long)
    API_DECL_PUTGET(get, ulonglong, unsigned long long)
    API_DECL_PUTGET(get, int8, int8_t)
    API_DECL_PUTGET(get, int16, int16_t)
    API_DECL_PUTGET(get, int32, int32_t)
    API_DECL_PUTGET(get, int64, int64_t)
    API_DECL_PUTGET(get, uint8, uint8_t)
    API_DECL_PUTGET(get, uint16, uint16_t)
    API_DECL_PUTGET(get, uint32, uint32_t)
    API_DECL_PUTGET(get, uint64, uint64_t)
    API_DECL_PUTGET(get, size, size_t)
    API_DECL_PUTGET(get, ptrdiff, ptrdiff_t)

#undef API_DECL_PUTGET

#define API_DECL_CTX_PUTGET_SIZE(_opname, _size)                        \
    /* see \ref shmem_ctx_long_##_opname() */                           \
    void shmem_ctx_##_opname##_size(shmem_ctx_t ctx,                    \
                                    void *dest, const void *src,        \
                                    size_t nelems, int pe);             \
    /* see \ref shmem_ctx_long_i##_opname() */                          \
    void shmem_ctx_i##_opname##_size(shmem_ctx_t ctx,                   \
                                     void *dest,                        \
                                     const void *src,                   \
                                     ptrdiff_t tst, ptrdiff_t sst,      \
                                     size_t nelems, int pe);            \
    /* see \ref shmem_ctx_long_##_opname() */                           \
    void shmem_ctx_##_opname##_size##_nbi(shmem_ctx_t ctx,              \
                                          void *dest, const void *src,  \
                                          size_t nelems, int pe);

    API_DECL_CTX_PUTGET_SIZE(put, 8)
    API_DECL_CTX_PUTGET_SIZE(put, 16)
    API_DECL_CTX_PUTGET_SIZE(put, 32)
    API_DECL_CTX_PUTGET_SIZE(put, 64)
    API_DECL_CTX_PUTGET_SIZE(put, 128)

    API_DECL_CTX_PUTGET_SIZE(get, 8)
    API_DECL_CTX_PUTGET_SIZE(get, 16)
    API_DECL_CTX_PUTGET_SIZE(get, 32)
    API_DECL_CTX_PUTGET_SIZE(get, 64)
    API_DECL_CTX_PUTGET_SIZE(get, 128)

#undef API_DECL_CTX_PUTGET_SIZE

#define API_DECL_PUTGET_SIZE(_opname, _size)                            \
    void shmem_##_opname##_size(void *dest, const void *src,            \
                           size_t nelems, int pe);                      \
    void shmem_i##_opname##_size(void *dest, const void *src,           \
                            ptrdiff_t tst, ptrdiff_t sst,               \
                            size_t nelems, int pe);                     \
    void shmem_##_opname##_size##_nbi(void *dest, const void *src,      \
                                      size_t nelems, int pe);           \

    API_DECL_PUTGET_SIZE(put, 8)
    API_DECL_PUTGET_SIZE(put, 16)
    API_DECL_PUTGET_SIZE(put, 32)
    API_DECL_PUTGET_SIZE(put, 64)
    API_DECL_PUTGET_SIZE(put, 128)

    API_DECL_PUTGET_SIZE(get, 8)
    API_DECL_PUTGET_SIZE(get, 16)
    API_DECL_PUTGET_SIZE(get, 32)
    API_DECL_PUTGET_SIZE(get, 64)
    API_DECL_PUTGET_SIZE(get, 128)

#undef API_DECL_PUTGET_SIZE

#define API_DECL_CTX_PUTGET_MEM(_opname)                                \
    /* see \ref shmem_ctx_long_##_opname() */                           \
    void shmem_ctx_##_opname##mem(shmem_ctx_t ctx,                      \
                                  void *dest, const void *src,          \
                                  size_t nelems, int pe);               \
    /* see \ref shmem_ctx_long_##_opname() */                           \
    void shmem_ctx_##_opname##mem##_nbi(shmem_ctx_t ctx,                \
                                        void *dest, const void *src,    \
                                        size_t nelems, int pe);         \

    API_DECL_CTX_PUTGET_MEM(put)
    API_DECL_CTX_PUTGET_MEM(get)

#undef API_DECL_CTX_PUTGET_MEM

#define API_DECL_PUTGET_MEM(_opname)                                    \
    void shmem_##_opname##mem(void *dest, const void *src,              \
                              size_t nelems, int pe);                   \
    void shmem_##_opname##mem_nbi(void *dest, const void *src,          \
                                  size_t nelems, int pe);

    API_DECL_PUTGET_MEM(put)
    API_DECL_PUTGET_MEM(get)

#undef API_DECL_PUTGET_MEM

#define API_CTX_DECL_P(_name, _type)                                    \
    /* see \ref shmem_ctx_long_p() */                                   \
    void shmem_ctx_##_name##_p(shmem_ctx_t ctx,                         \
                               _type *dest, _type src, int pe);

    API_CTX_DECL_P(float, float)
    API_CTX_DECL_P(double, double)
    API_CTX_DECL_P(longdouble, long double)
    API_CTX_DECL_P(schar, signed char)
    API_CTX_DECL_P(char, char)
    API_CTX_DECL_P(short, short)
    API_CTX_DECL_P(int, int)
    API_CTX_DECL_P(long, long)
    API_CTX_DECL_P(longlong, long long)
    API_CTX_DECL_P(uchar, unsigned char)
    API_CTX_DECL_P(ushort, unsigned short)
    API_CTX_DECL_P(uint, unsigned int)
    API_CTX_DECL_P(ulong, unsigned long)
    API_CTX_DECL_P(ulonglong, unsigned long long)
    API_CTX_DECL_P(int8, int8_t)
    API_CTX_DECL_P(int16, int16_t)
    API_CTX_DECL_P(int32, int32_t)
    API_CTX_DECL_P(int64, int64_t)
    API_CTX_DECL_P(uint8, uint8_t)
    API_CTX_DECL_P(uint16, uint16_t)
    API_CTX_DECL_P(uint32, uint32_t)
    API_CTX_DECL_P(uint64, uint64_t)
    API_CTX_DECL_P(size, size_t)
    API_CTX_DECL_P(ptrdiff, ptrdiff_t)

#undef API_CTX_DECL_P

#define API_DECL_P(_name, _type)                            \
    void shmem_##_name##_p(_type *dest, _type src, int pe);

    API_DECL_P(float, float)
    API_DECL_P(double, double)
    API_DECL_P(longdouble, long double)
    API_DECL_P(schar, signed char)
    API_DECL_P(char, char)
    API_DECL_P(short, short)
    API_DECL_P(int, int)
    API_DECL_P(long, long)
    API_DECL_P(longlong, long long)
    API_DECL_P(uchar, unsigned char)
    API_DECL_P(ushort, unsigned short)
    API_DECL_P(uint, unsigned int)
    API_DECL_P(ulong, unsigned long)
    API_DECL_P(ulonglong, unsigned long long)
    API_DECL_P(int8, int8_t)
    API_DECL_P(int16, int16_t)
    API_DECL_P(int32, int32_t)
    API_DECL_P(int64, int64_t)
    API_DECL_P(uint8, uint8_t)
    API_DECL_P(uint16, uint16_t)
    API_DECL_P(uint32, uint32_t)
    API_DECL_P(uint64, uint64_t)
    API_DECL_P(size, size_t)
    API_DECL_P(ptrdiff, ptrdiff_t)

#undef API_DECL_P

#define API_CTX_DECL_G(_name, _type)                        \
    /* see \ref shmem_ctx_long_g() */                       \
    _type shmem_ctx_##_name##_g(shmem_ctx_t ctx,            \
                                const _type *src, int pe);

    API_CTX_DECL_G(float, float)
    API_CTX_DECL_G(double, double)
    API_CTX_DECL_G(longdouble, long double)
    API_CTX_DECL_G(schar, signed char)
    API_CTX_DECL_G(char, char)
    API_CTX_DECL_G(short, short)
    API_CTX_DECL_G(int, int)
    API_CTX_DECL_G(long, long)
    API_CTX_DECL_G(longlong, long long)
    API_CTX_DECL_G(uchar, unsigned char)
    API_CTX_DECL_G(ushort, unsigned short)
    API_CTX_DECL_G(uint, unsigned int)
    API_CTX_DECL_G(ulong, unsigned long)
    API_CTX_DECL_G(ulonglong, unsigned long long)
    API_CTX_DECL_G(int8, int8_t)
    API_CTX_DECL_G(int16, int16_t)
    API_CTX_DECL_G(int32, int32_t)
    API_CTX_DECL_G(int64, int64_t)
    API_CTX_DECL_G(uint8, uint8_t)
    API_CTX_DECL_G(uint16, uint16_t)
    API_CTX_DECL_G(uint32, uint32_t)
    API_CTX_DECL_G(uint64, uint64_t)
    API_CTX_DECL_G(size, size_t)
    API_CTX_DECL_G(ptrdiff, ptrdiff_t)

#undef API_CTX_DECL_G

#define API_DECL_G(_name, _type)                            \
    _type shmem_##_name##_g(const _type *src, int pe);

    API_DECL_G(float, float)
    API_DECL_G(double, double)
    API_DECL_G(longdouble, long double)
    API_DECL_G(schar, signed char)
    API_DECL_G(char, char)
    API_DECL_G(short, short)
    API_DECL_G(int, int)
    API_DECL_G(long, long)
    API_DECL_G(longlong, long long)
    API_DECL_G(uchar, unsigned char)
    API_DECL_G(ushort, unsigned short)
    API_DECL_G(uint, unsigned int)
    API_DECL_G(ulong, unsigned long)
    API_DECL_G(ulonglong, unsigned long long)
    API_DECL_G(int8, int8_t)
    API_DECL_G(int16, int16_t)
    API_DECL_G(int32, int32_t)
    API_DECL_G(int64, int64_t)
    API_DECL_G(uint8, uint8_t)
    API_DECL_G(uint16, uint16_t)
    API_DECL_G(uint32, uint32_t)
    API_DECL_G(uint64, uint64_t)
    API_DECL_G(size, size_t)
    API_DECL_G(ptrdiff, ptrdiff_t)

#undef API_DECL_G

#define API_DECL_CTX_PUT_SIGNAL(_name, _type)           \
    void                                                \
    shmem_ctx_##_name##_put_signal(shmem_ctx_t ctx,     \
                                   _type *dest,         \
                                   const _type *src,    \
                                   size_t nelems,       \
                                   uint64_t *sig_addr,  \
                                   uint64_t signal,     \
                                   int sig_op,          \
                                   int pe)

    API_DECL_CTX_PUT_SIGNAL(float, float);
    API_DECL_CTX_PUT_SIGNAL(double, double);
    API_DECL_CTX_PUT_SIGNAL(longdouble, long double);
    API_DECL_CTX_PUT_SIGNAL(schar, signed char);
    API_DECL_CTX_PUT_SIGNAL(char, char);
    API_DECL_CTX_PUT_SIGNAL(short, short);
    API_DECL_CTX_PUT_SIGNAL(int, int);
    API_DECL_CTX_PUT_SIGNAL(long, long);
    API_DECL_CTX_PUT_SIGNAL(longlong, long long);
    API_DECL_CTX_PUT_SIGNAL(uchar, unsigned char);
    API_DECL_CTX_PUT_SIGNAL(ushort, unsigned short);
    API_DECL_CTX_PUT_SIGNAL(uint, unsigned int);
    API_DECL_CTX_PUT_SIGNAL(ulong, unsigned long);
    API_DECL_CTX_PUT_SIGNAL(ulonglong, unsigned long long);
    API_DECL_CTX_PUT_SIGNAL(int8, int8_t);
    API_DECL_CTX_PUT_SIGNAL(int16, int16_t);
    API_DECL_CTX_PUT_SIGNAL(int32, int32_t);
    API_DECL_CTX_PUT_SIGNAL(int64, int64_t);
    API_DECL_CTX_PUT_SIGNAL(uint8, uint8_t);
    API_DECL_CTX_PUT_SIGNAL(uint16, uint16_t);
    API_DECL_CTX_PUT_SIGNAL(uint32, uint32_t);
    API_DECL_CTX_PUT_SIGNAL(uint64, uint64_t);
    API_DECL_CTX_PUT_SIGNAL(size, size_t);
    API_DECL_CTX_PUT_SIGNAL(ptrdiff, ptrdiff_t);

#define API_DECL_CTX_PUT_SIGNAL_NBI(_name, _type)           \
    void                                                    \
    shmem_ctx_##_name##_put_signal_nbi(shmem_ctx_t ctx,     \
                                       _type *dest,         \
                                       const _type *src,    \
                                       size_t nelems,       \
                                       uint64_t *sig_addr,  \
                                       uint64_t signal,     \
                                       int sig_op,          \
                                       int pe)

    API_DECL_CTX_PUT_SIGNAL_NBI(float, float);
    API_DECL_CTX_PUT_SIGNAL_NBI(double, double);
    API_DECL_CTX_PUT_SIGNAL_NBI(longdouble, long double);
    API_DECL_CTX_PUT_SIGNAL_NBI(schar, signed char);
    API_DECL_CTX_PUT_SIGNAL_NBI(char, char);
    API_DECL_CTX_PUT_SIGNAL_NBI(short, short);
    API_DECL_CTX_PUT_SIGNAL_NBI(int, int);
    API_DECL_CTX_PUT_SIGNAL_NBI(long, long);
    API_DECL_CTX_PUT_SIGNAL_NBI(longlong, long long);
    API_DECL_CTX_PUT_SIGNAL_NBI(uchar, unsigned char);
    API_DECL_CTX_PUT_SIGNAL_NBI(ushort, unsigned short);
    API_DECL_CTX_PUT_SIGNAL_NBI(uint, unsigned int);
    API_DECL_CTX_PUT_SIGNAL_NBI(ulong, unsigned long);
    API_DECL_CTX_PUT_SIGNAL_NBI(ulonglong, unsigned long long);
    API_DECL_CTX_PUT_SIGNAL_NBI(int8, int8_t);
    API_DECL_CTX_PUT_SIGNAL_NBI(int16, int16_t);
    API_DECL_CTX_PUT_SIGNAL_NBI(int32, int32_t);
    API_DECL_CTX_PUT_SIGNAL_NBI(int64, int64_t);
    API_DECL_CTX_PUT_SIGNAL_NBI(uint8, uint8_t);
    API_DECL_CTX_PUT_SIGNAL_NBI(uint16, uint16_t);
    API_DECL_CTX_PUT_SIGNAL_NBI(uint32, uint32_t);
    API_DECL_CTX_PUT_SIGNAL_NBI(uint64, uint64_t);
    API_DECL_CTX_PUT_SIGNAL_NBI(size, size_t);
    API_DECL_CTX_PUT_SIGNAL_NBI(ptrdiff, ptrdiff_t);

#define API_DECL_PUT_SIGNAL(_name, _type)           \
    void                                            \
    shmem_##_name##_put_signal(_type *dest,         \
                               const _type *src,    \
                               size_t nelems,       \
                               uint64_t *sig_addr,  \
                               uint64_t signal,     \
                               int sig_op,          \
                               int pe)

    API_DECL_PUT_SIGNAL(float, float);
    API_DECL_PUT_SIGNAL(double, double);
    API_DECL_PUT_SIGNAL(longdouble, long double);
    API_DECL_PUT_SIGNAL(schar, signed char);
    API_DECL_PUT_SIGNAL(char, char);
    API_DECL_PUT_SIGNAL(short, short);
    API_DECL_PUT_SIGNAL(int, int);
    API_DECL_PUT_SIGNAL(long, long);
    API_DECL_PUT_SIGNAL(longlong, long long);
    API_DECL_PUT_SIGNAL(uchar, unsigned char);
    API_DECL_PUT_SIGNAL(ushort, unsigned short);
    API_DECL_PUT_SIGNAL(uint, unsigned int);
    API_DECL_PUT_SIGNAL(ulong, unsigned long);
    API_DECL_PUT_SIGNAL(ulonglong, unsigned long long);
    API_DECL_PUT_SIGNAL(int8, int8_t);
    API_DECL_PUT_SIGNAL(int16, int16_t);
    API_DECL_PUT_SIGNAL(int32, int32_t);
    API_DECL_PUT_SIGNAL(int64, int64_t);
    API_DECL_PUT_SIGNAL(uint8, uint8_t);
    API_DECL_PUT_SIGNAL(uint16, uint16_t);
    API_DECL_PUT_SIGNAL(uint32, uint32_t);
    API_DECL_PUT_SIGNAL(uint64, uint64_t);
    API_DECL_PUT_SIGNAL(size, size_t);
    API_DECL_PUT_SIGNAL(ptrdiff, ptrdiff_t);

#define API_DECL_PUT_SIGNAL_NBI(_name, _type)           \
    void                                                \
    shmem_##_name##_put_signal_nbi(_type *dest,         \
                                   const _type *src,    \
                                   size_t nelems,       \
                                   uint64_t *sig_addr,  \
                                   uint64_t signal,     \
                                   int sig_op,          \
                                   int pe)

    API_DECL_PUT_SIGNAL_NBI(float, float);
    API_DECL_PUT_SIGNAL_NBI(double, double);
    API_DECL_PUT_SIGNAL_NBI(longdouble, long double);
    API_DECL_PUT_SIGNAL_NBI(schar, signed char);
    API_DECL_PUT_SIGNAL_NBI(char, char);
    API_DECL_PUT_SIGNAL_NBI(short, short);
    API_DECL_PUT_SIGNAL_NBI(int, int);
    API_DECL_PUT_SIGNAL_NBI(long, long);
    API_DECL_PUT_SIGNAL_NBI(longlong, long long);
    API_DECL_PUT_SIGNAL_NBI(uchar, unsigned char);
    API_DECL_PUT_SIGNAL_NBI(ushort, unsigned short);
    API_DECL_PUT_SIGNAL_NBI(uint, unsigned int);
    API_DECL_PUT_SIGNAL_NBI(ulong, unsigned long);
    API_DECL_PUT_SIGNAL_NBI(ulonglong, unsigned long long);
    API_DECL_PUT_SIGNAL_NBI(int8, int8_t);
    API_DECL_PUT_SIGNAL_NBI(int16, int16_t);
    API_DECL_PUT_SIGNAL_NBI(int32, int32_t);
    API_DECL_PUT_SIGNAL_NBI(int64, int64_t);
    API_DECL_PUT_SIGNAL_NBI(uint8, uint8_t);
    API_DECL_PUT_SIGNAL_NBI(uint16, uint16_t);
    API_DECL_PUT_SIGNAL_NBI(uint32, uint32_t);
    API_DECL_PUT_SIGNAL_NBI(uint64, uint64_t);
    API_DECL_PUT_SIGNAL_NBI(size, size_t);
    API_DECL_PUT_SIGNAL_NBI(ptrdiff, ptrdiff_t);

#define API_DECL_CTX_PUT_SIGNAL_SIZE(_size)                         \
    /* see \ref shmem_ctx_long_put_signal() */                      \
    void shmem_ctx_put##_size##_signal(shmem_ctx_t ctx,             \
                                       void *dest,                  \
                                       const void *src,             \
                                       size_t nelems,               \
                                       uint64_t *sig_addr,          \
                                       uint64_t signal,             \
                                       int sig_op,                  \
                                       int pe);                     \
    /* see \ref shmem_ctx_long_put_signal_nbi() */                  \
    void shmem_ctx_put##_size##_signal_nbi(shmem_ctx_t ctx,         \
                                           void *dest,              \
                                           const void *src,         \
                                           size_t nelems,           \
                                           uint64_t *sig_addr,      \
                                           uint64_t signal,         \
                                           int sig_op,              \
                                           int pe);

    API_DECL_CTX_PUT_SIGNAL_SIZE(8)
    API_DECL_CTX_PUT_SIGNAL_SIZE(16)
    API_DECL_CTX_PUT_SIGNAL_SIZE(32)
    API_DECL_CTX_PUT_SIGNAL_SIZE(64)
    API_DECL_CTX_PUT_SIGNAL_SIZE(128)

#undef API_DECL_CTX_PUT_SIGNAL_SIZE

#define API_DECL_CTX_PUTMEM_SIGNAL()                                \
    void shmem_ctx_putmem_signal(shmem_ctx_t ctx,                   \
                                 void *dest,                        \
                                 const void *src,                   \
                                 size_t nelems,                     \
                                 uint64_t *sig_addr,                \
                                 uint64_t signal,                   \
                                 int sig_op,                        \
                                 int pe);                           \
    void shmem_ctx_putmem_signal_nbi(shmem_ctx_t ctx,               \
                                     void *dest,                    \
                                     const void *src,               \
                                     size_t nelems,                 \
                                     uint64_t *sig_addr,            \
                                     uint64_t signal,               \
                                     int sig_op,                    \
                                     int pe);

    API_DECL_CTX_PUTMEM_SIGNAL()

#undef API_DECL_CTX_PUTMEM_SIGNAL

#define API_DECL_PUT_SIGNAL_SIZE(_size)                     \
    /* see \ref shmem_long_put_signal() */                  \
    void shmem_put##_size##_signal(void *dest,              \
                                   const void *src,         \
                                   size_t nelems,           \
                                   uint64_t *sig_addr,      \
                                   uint64_t signal,         \
                                   int sig_op,              \
                                   int pe);                 \
    /* see \ref shmem_long_put_signal_nbi() */              \
    void shmem_put##_size##_signal_nbi(void *dest,          \
                                       const void *src,     \
                                       size_t nelems,       \
                                       uint64_t *sig_addr,  \
                                       uint64_t signal,     \
                                       int sig_op,          \
                                       int pe);

    API_DECL_PUT_SIGNAL_SIZE(8)
    API_DECL_PUT_SIGNAL_SIZE(16)
    API_DECL_PUT_SIGNAL_SIZE(32)
    API_DECL_PUT_SIGNAL_SIZE(64)
    API_DECL_PUT_SIGNAL_SIZE(128)

#undef API_DECL_PUT_SIGNAL_SIZE

#define API_DECL_PUTMEM_SIGNAL()                        \
    void shmem_putmem_signal(void *dest,                \
                             const void *src,           \
                             size_t nelems,             \
                             uint64_t *sig_addr,        \
                             uint64_t signal,           \
                             int sig_op,                \
                             int pe);                   \
    void shmem_putmem_signal_nbi(void *dest,            \
                                 const void *src,       \
                                 size_t nelems,         \
                                 uint64_t *sig_addr,    \
                                 uint64_t signal,       \
                                 int sig_op,            \
                                 int pe);

    API_DECL_PUTMEM_SIGNAL()

#undef API_DECL_PUTMEM_SIGNAL

    /**
     * @brief fetches value of the signal object.
     * @page shmem_signel_fetch
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     uint64_t shmem_signal_fetch(const uint64_t *sig_addr);
     @endcode
     *
     * @param[in] sig_addr The address of the remotely accessible signal object
     *
     * @section Effect
     * None.
     *
     * @section Return
     * The data value.
     *
     */
    uint64_t shmem_signal_fetch(const uint64_t *sig_addr);

    /*
     * barriers & syncs
     */

    /**
     * @brief causes an active set of PEs to synchronize.  Local memory stores
     * complete.
     * @page shmem_sync
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_sync(int PE_start, int logPE_stride, int PE_size,
                     long *pSync);
     @endcode
     *
     * @param[in] PE_start first PE of the active set
     * @param[in] logPE_stride log2 of stride between PEs
     * @param[in] PE_size number of PEs in the active set
     * @param[in, out] pSync symmetric work array
     *
     * @section Effect
     * PEs in the active set defined by (PE_start, logPE_stride,
     * PE_size) synchronize: no PE from this active set can leave the
     * global barrier until all have arrived.  Local memory loads and store
     * complete before return.  PEs not in the active set do not call
     * shmem_sync().  pSync must be initialized everywhere before
     * use, and, if modified, must be reset to its state before the
     * call.
     *
     * @section Return
     * None.
     *
     */
    void shmem_sync(int PE_start, int logPE_stride, int PE_size,
                    long *pSync);

    /**
     * @brief causes all PEs to synchronize
     * @page shmem_sync_all
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_sync_all(void);
     @endcode
     *
     * @section Effect
     * All PEs synchronize: no PE can leave the global barrier until all
     * have arrived.  Local memory loads and store complete before return.
     *
     * @section Return
     * None.
     *
     */
    void shmem_sync_all(void);

    /**
     * @brief causes an active set of PEs to synchronize
     * @page shmem_barrier
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_barrier(int PE_start, int logPE_stride, int PE_size,
                        long *pSync);
     @endcode
     *
     * @param[in] PE_start first PE of the active set
     * @param[in] logPE_stride log2 of stride between PEs
     * @param[in] PE_size number of PEs in the active set
     * @param[in, out] pSync symmetric work array
     *
     * @section Effect
     * PEs in the active set defined by (PE_start, logPE_stride,
     * PE_size) synchronize: no PE from this active set can leave the
     * global barrier until all have arrived.  Communication is also
     * flushed before return.  PEs not in the active set do not call
     * shmem_barrier().  pSync must be initialized everywhere before
     * use, and, if modified, must be reset to its state before the
     * call.
     *
     * @section Return
     * None.
     *
     */
    void shmem_barrier(int PE_start, int logPE_stride, int PE_size,
                       long *pSync);


    /**
     * @brief causes all PEs to synchronize
     * @page shmem_barrier_all
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_barrier_all(void);
     @endcode
     *
     * @section Effect
     * All PEs synchronize: no PE can leave the global barrier until all
     * have arrived.
     *
     * @section Return
     * None.
     *
     */
    void shmem_barrier_all(void);

    /**
     * @brief outbound communication completes before any subsequent
     * communication is sent.
     * @page shmem_ctx_fence
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_ctx_fence(shmem_ctx_t ctx);
     void shmem_fence(void);
     @endcode
     *
     * @section Effect
     * Local ordering
     *
     * @section Return
     * None.
     *
     */
    void shmem_ctx_fence(shmem_ctx_t ctx);
    void shmem_fence(void);

    /**
     * @brief causes outbound communication to complete before
     * subsequent puts are sent.
     * @page shmem_ctx_quiet
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_ctx_quiet(shmem_ctx_t ctx);
     void shmem_quiet(void);
     @endcode
     *
     * @section Effect
     * Remote completion
     *
     * @section Return
     * None.
     *
     */
    void shmem_ctx_quiet(shmem_ctx_t ctx);
    void shmem_quiet(void);

    /*
     * accessibility
     */

    /**
     * @brief checks whether the caller PE can communicate with the named PE
     * @page shmem_pe_accessible
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     int shmem_pe_accessible(int pe);
     @endcode
     *
     * @section Effect
     * None
     *
     * @section Return
     * non-zero if "pe" can be communicated with.  0 if not.
     *
     */
    int shmem_pe_accessible(int pe) _WUR;

    /**
     * @brief checks whether the caller PE can communicate with a memory
     * address on the named PE
     * @page shmem_addr_accessible
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     int shmem_addr_accessible(const void *addr, int pe);
     @endcode
     *
     * @param addr address to check
     * @param pe PE to check
     *
     * @section Effect
     * None
     *
     * @section Return
     * non-zero if address "addr" can be used for communication
     * on PE "pe".  0 if not.
     *
     */
    int shmem_addr_accessible(const void *addr, int pe) _WUR;

    /**
     * @brief checks whether an address on a target PE can be accessed
     * with a simple load/store operation.
     * @page shmem_ptr
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void *shmem_ptr(const void *addr, int pe);
     @endcode
     *
     * @section Effect
     * None
     *
     * @section Return
     * a pointer to a memory location corresponding to the
     * address on the target PE if that address can be accessed with
     * load/store operations by the calling PE.  NULL if not.
     *
     */
    void *shmem_ptr(const void *target, int pe) _WUR;

    /*
     * symmetric memory management
     */

    /* deprecated calls from 1.2 ++ */

    /**
     * @brief dynamically allocates symmetric memory
     *
     * @section Deprecated by by \ref shmem_malloc()
     *
     */
    void *shmalloc(size_t size)
        _WUR _DEPRECATED_BY(shmem_malloc, 1.2);

    /**
     * @brief dynamically allocates symmetric memory
     *
     * @section Deprecated by by \ref shmem_free()
     *
     */
    void shfree(void *ptr)
        _DEPRECATED_BY(shmem_free, 1.2);

    /**
     * @brief dynamically allocates symmetric memory
     *
     * @section Deprecated by by \ref shmem_realloc()
     *
     */
    void *shrealloc(void *ptr, size_t size)
        _WUR _DEPRECATED_BY(shmem_realloc, 1.2);

    /**
     * @brief dynamically allocates symmetric memory
     *
     * @section Deprecated by by \ref shmem_align()
     *
     */
    void *shmemalign(size_t alignment, size_t size)
        _WUR _DEPRECATED_BY(shmem_align, 1.2);

    /**
     * @brief dynamically allocates symmetric memory
     * @page shmem_malloc
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void *shmem_malloc(size_t size);
     @endcode
     *
     * @param size number of bytes requested
     *
     * @section Effect
     * Allocates "size" bytes of contiguous memory from the PE's
     * symmetric heap.
     *
     * @section Return
     * a pointer to the requested memory location, or NULL if
     * the requested memory is not available.
     *
     */
    void *shmem_malloc(size_t size) _WUR;

    /**
     * @brief dynamically allocates zeroed symmetric memory
     * @page shmem_calloc
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void *shmem_calloc(size_t count, size_t size);
     @endcode
     *
     * @param count number of elements to allcoate
     * @param size how big each element is (bytes)
     *
     * @section Effect
     * Allocates "count" lots of "size" bytes of contiguous memory
     * from the PE's symmetric heap.  Memory is zeroed before return.
     *
     * @section Return
     * a pointer to the requested memory location, or NULL if
     * the requested memory is not available.
     *
     */
    void *shmem_calloc(size_t count, size_t size) _WUR;

    /**
     * @brief dynamically allocates symmetric memory
     * @page shmem_free
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_free(void *ptr);
     @endcode
     *
     * @param ptr symmetric memory pointer
     *
     * @section Effect
     * Frees a previous symmetric allocation.
     *
     * @section Return
     * None.
     */
    void shmem_free(void *ptr);

    /**
     * @brief dynamically allocates symmetric memory
     * @page shmem_realloc
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void *shmem_realloc(void *ptr, size_t size);
     @endcode
     *
     * @param ptr symmetric memory pointer
     * @param size number of bytes
     *
     * @section Effect
     * Resizes a previous symmetric memory allocation starting at "ptr"
     * to "size" bytes.
     *
     * @section Return
     * a pointer to the resized area, or NULL if this is not
     * possible.
     *
     */
    void *shmem_realloc(void *ptr, size_t size) _WUR;

    /**
     * @brief aligns already allocated symmetric memory
     * @page shmem_align
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void *shmem_align(void *ptr, size_t size);
     @endcode
     *
     * @param ptr symmetric memory pointer
     * @param size number of bytes
     *
     * @section Effect
     * Resizes a previous symmetric memory allocation starting at "ptr"
     * to "size" bytes.
     *
     * @section Return
     * a pointer to the resized area, or NULL if this is not
     * possible.
     *
     */
    void *shmem_align(size_t alignment, size_t size) _WUR;

#define API_DECL_TEST_AND_WAIT_UNTIL(_opname, _rettype, _name, _type)   \
    /* see \ref shmem_##_name##_opname() */                             \
    _rettype shmem_##_name##_##_opname(_type *ivar,                     \
                                       int cmp,                         \
                                       _type cmp_value);

    /**
     * @brief test for symmetric variable to change value
     * @page shmem_long_test
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     int shmem_long_test(long *ivar, int cmp, long cmp_value);
     @endcode
     *
     * @section Effect
     *
     * ivar may be updated by another PE
     *
     * @section Return
     * 1 if the comparison is true, 0 if not
     *
     */
    API_DECL_TEST_AND_WAIT_UNTIL(test, int, longdouble, long double)
    API_DECL_TEST_AND_WAIT_UNTIL(test, int, schar, signed char)
    API_DECL_TEST_AND_WAIT_UNTIL(test, int, char, char)
    API_DECL_TEST_AND_WAIT_UNTIL(test, int, short, short)
    API_DECL_TEST_AND_WAIT_UNTIL(test, int, int, int)
    API_DECL_TEST_AND_WAIT_UNTIL(test, int, long, long)
    API_DECL_TEST_AND_WAIT_UNTIL(test, int, longlong, long long)
    API_DECL_TEST_AND_WAIT_UNTIL(test, int, uchar, unsigned char)
    API_DECL_TEST_AND_WAIT_UNTIL(test, int, ushort, unsigned short)
    API_DECL_TEST_AND_WAIT_UNTIL(test, int, uint, unsigned int)
    API_DECL_TEST_AND_WAIT_UNTIL(test, int, ulong, unsigned long)
    API_DECL_TEST_AND_WAIT_UNTIL(test, int, ulonglong, unsigned long long)
    API_DECL_TEST_AND_WAIT_UNTIL(test, int, int32, int32_t)
    API_DECL_TEST_AND_WAIT_UNTIL(test, int, int64, int64_t)
    API_DECL_TEST_AND_WAIT_UNTIL(test, int, uint32, uint32_t)
    API_DECL_TEST_AND_WAIT_UNTIL(test, int, uint64, uint64_t)
    API_DECL_TEST_AND_WAIT_UNTIL(test, int, size, size_t)
    API_DECL_TEST_AND_WAIT_UNTIL(test, int, ptrdiff, ptrdiff_t)

    /**
     * @brief wait for a symmetric variable to change value with
     *        specified condition
     * @page shmmem_long_wait_until
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_long_wait_until(long *ivar, int cmp, long cmp_value);
     @endcode
     *
     * @section Effect
     *
     * ivar updated by another PE, wait for that to happen
     *
     * @section Return
     * None.
     *
     */
    API_DECL_TEST_AND_WAIT_UNTIL(wait_until, void, longdouble, long double)
    API_DECL_TEST_AND_WAIT_UNTIL(wait_until, void, schar, signed char)
    API_DECL_TEST_AND_WAIT_UNTIL(wait_until, void, char, char)
    API_DECL_TEST_AND_WAIT_UNTIL(wait_until, void, short, short)
    API_DECL_TEST_AND_WAIT_UNTIL(wait_until, void, int, int)
    API_DECL_TEST_AND_WAIT_UNTIL(wait_until, void, long, long)
    API_DECL_TEST_AND_WAIT_UNTIL(wait_until, void, longlong, long long)
    API_DECL_TEST_AND_WAIT_UNTIL(wait_until, void, uchar, unsigned char)
    API_DECL_TEST_AND_WAIT_UNTIL(wait_until, void, ushort, unsigned short)
    API_DECL_TEST_AND_WAIT_UNTIL(wait_until, void, uint, unsigned int)
    API_DECL_TEST_AND_WAIT_UNTIL(wait_until, void, ulong, unsigned long)
    API_DECL_TEST_AND_WAIT_UNTIL(wait_until, void, ulonglong,
                                 unsigned long long)
    API_DECL_TEST_AND_WAIT_UNTIL(wait_until, void, int32, int32_t)
    API_DECL_TEST_AND_WAIT_UNTIL(wait_until, void, int64, int64_t)
    API_DECL_TEST_AND_WAIT_UNTIL(wait_until, void, uint32, uint32_t)
    API_DECL_TEST_AND_WAIT_UNTIL(wait_until, void, uint64, uint64_t)
    API_DECL_TEST_AND_WAIT_UNTIL(wait_until, void, size, size_t)
    API_DECL_TEST_AND_WAIT_UNTIL(wait_until, void, ptrdiff, ptrdiff_t)

#undef API_DECL_TEST_AND_WAIT_UNTIL

#define API_DECL_TEST_ALL(_opname, _type)                   \
    int                                                     \
    shmem_##_opname##_test_all(_type *ivars, size_t nelems, \
                               int cmp, _type cmp_value)

    API_DECL_TEST_ALL(short, short);
    API_DECL_TEST_ALL(int, int);
    API_DECL_TEST_ALL(long, long);
    API_DECL_TEST_ALL(longlong, long long);
    API_DECL_TEST_ALL(ushort, unsigned short);
    API_DECL_TEST_ALL(uint, unsigned int);
    API_DECL_TEST_ALL(ulong, unsigned long);
    API_DECL_TEST_ALL(ulonglong, unsigned long long);
    API_DECL_TEST_ALL(int32, int32_t);
    API_DECL_TEST_ALL(int64, int64_t);
    API_DECL_TEST_ALL(uint32, uint32_t);
    API_DECL_TEST_ALL(uint64, uint64_t);
    API_DECL_TEST_ALL(size, size_t);
    API_DECL_TEST_ALL(ptrdiff, ptrdiff_t);

#define API_DECL_TEST_ANY(_opname, _type)                   \
    size_t                                                  \
    shmem_##_opname##_test_any(_type *ivars, size_t nelems, \
                               int *status,                 \
                               int cmp, _type cmp_value)

    API_DECL_TEST_ANY(short, short);
    API_DECL_TEST_ANY(int, int);
    API_DECL_TEST_ANY(long, long);
    API_DECL_TEST_ANY(longlong, long long);
    API_DECL_TEST_ANY(ushort, unsigned short);
    API_DECL_TEST_ANY(uint, unsigned int);
    API_DECL_TEST_ANY(ulong, unsigned long);
    API_DECL_TEST_ANY(ulonglong, unsigned long long);
    API_DECL_TEST_ANY(int32, int32_t);
    API_DECL_TEST_ANY(int64, int64_t);
    API_DECL_TEST_ANY(uint32, uint32_t);
    API_DECL_TEST_ANY(uint64, uint64_t);
    API_DECL_TEST_ANY(size, size_t);
    API_DECL_TEST_ANY(ptrdiff, ptrdiff_t);

#define API_DECL_TEST_SOME(_opname, _type)                      \
    size_t                                                      \
    shmem_##_opname##_test_some(_type *ivars, size_t nelems,    \
                                size_t *indices,                \
                                int *status,                    \
                                int cmp, _type cmp_value)

    API_DECL_TEST_SOME(short, short);
    API_DECL_TEST_SOME(int, int);
    API_DECL_TEST_SOME(long, long);
    API_DECL_TEST_SOME(longlong, long long);
    API_DECL_TEST_SOME(ushort, unsigned short);
    API_DECL_TEST_SOME(uint, unsigned int);
    API_DECL_TEST_SOME(ulong, unsigned long);
    API_DECL_TEST_SOME(ulonglong, unsigned long long);
    API_DECL_TEST_SOME(int32, int32_t);
    API_DECL_TEST_SOME(int64, int64_t);
    API_DECL_TEST_SOME(uint32, uint32_t);
    API_DECL_TEST_SOME(uint64, uint64_t);
    API_DECL_TEST_SOME(size, size_t);
    API_DECL_TEST_SOME(ptrdiff, ptrdiff_t);

#define API_DECL_WAIT_UNTIL_ALL(_opname, _type)                     \
    void                                                            \
    shmem_##_opname##_wait_until_all(_type *ivars, size_t nelems,   \
                                     int cmp, _type cmp_value)

    API_DECL_WAIT_UNTIL_ALL(short, short);
    API_DECL_WAIT_UNTIL_ALL(int, int);
    API_DECL_WAIT_UNTIL_ALL(long, long);
    API_DECL_WAIT_UNTIL_ALL(longlong, long long);
    API_DECL_WAIT_UNTIL_ALL(ushort, unsigned short);
    API_DECL_WAIT_UNTIL_ALL(uint, unsigned int);
    API_DECL_WAIT_UNTIL_ALL(ulong, unsigned long);
    API_DECL_WAIT_UNTIL_ALL(ulonglong, unsigned long long);
    API_DECL_WAIT_UNTIL_ALL(int32, int32_t);
    API_DECL_WAIT_UNTIL_ALL(int64, int64_t);
    API_DECL_WAIT_UNTIL_ALL(uint32, uint32_t);
    API_DECL_WAIT_UNTIL_ALL(uint64, uint64_t);
    API_DECL_WAIT_UNTIL_ALL(size, size_t);
    API_DECL_WAIT_UNTIL_ALL(ptrdiff, ptrdiff_t);

#define API_DECL_WAIT_UNTIL_ANY(_opname, _type)                     \
    size_t                                                          \
    shmem_##_opname##_wait_until_any(_type *ivars, size_t nelems,   \
                                     int *status,                   \
                                     int cmp, _type cmp_value)

    API_DECL_WAIT_UNTIL_ANY(short, short);
    API_DECL_WAIT_UNTIL_ANY(int, int);
    API_DECL_WAIT_UNTIL_ANY(long, long);
    API_DECL_WAIT_UNTIL_ANY(longlong, long long);
    API_DECL_WAIT_UNTIL_ANY(ushort, unsigned short);
    API_DECL_WAIT_UNTIL_ANY(uint, unsigned int);
    API_DECL_WAIT_UNTIL_ANY(ulong, unsigned long);
    API_DECL_WAIT_UNTIL_ANY(ulonglong, unsigned long long);
    API_DECL_WAIT_UNTIL_ANY(int32, int32_t);
    API_DECL_WAIT_UNTIL_ANY(int64, int64_t);
    API_DECL_WAIT_UNTIL_ANY(uint32, uint32_t);
    API_DECL_WAIT_UNTIL_ANY(uint64, uint64_t);
    API_DECL_WAIT_UNTIL_ANY(size, size_t);
    API_DECL_WAIT_UNTIL_ANY(ptrdiff, ptrdiff_t);

#undef API_DECL_TEST_ALL
#undef API_DECL_TEST_ANY
#undef API_DECL_TEST_SOME
#undef API_DECL_WAIT_UNTIL_ALL
#undef API_DECL_WAIT_UNTIL_ANY
#undef API_DECL_WAIT_UNTIL_SOME

    /**
     * @brief wait for symmetric variable to change value
     * @page shmem_long_wait
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_long_wait(long *ivar, long cmp_value);
     @endcode
     *
     * @section Effect
     *
     * ivar updated by another PE, wait for that to happen
     *
     * @section Return
     * None.
     *
     * @section Deprecated
     * by \ref shmem_long_wait_until
     *
     */
#define API_DECL_WAIT(_name, _type)                                     \
    /* see \ref shmem_##_name##_wait() */                               \
    void shmem_##_name##_##wait(_type *ivar,                            \
                                _type cmp_value)                        \
        _DEPRECATED_BY(shmem_##_name##_wait_until with compare          \
                       operator SHMEM_CMP_NE, 1.4);

    API_DECL_WAIT(longdouble, long double)
    API_DECL_WAIT(schar, signed char)
    API_DECL_WAIT(char, char)
    API_DECL_WAIT(short, short)
    API_DECL_WAIT(int, int)
    API_DECL_WAIT(long, long)
    API_DECL_WAIT(longlong, long long)
    API_DECL_WAIT(uchar, unsigned char)
    API_DECL_WAIT(ushort, unsigned short)
    API_DECL_WAIT(uint, unsigned int)
    API_DECL_WAIT(ulong, unsigned long)
    API_DECL_WAIT(ulonglong, unsigned long long)
    API_DECL_WAIT(int32, int32_t)
    API_DECL_WAIT(int64, int64_t)
    API_DECL_WAIT(uint32, uint32_t)
    API_DECL_WAIT(uint64, uint64_t)
    API_DECL_WAIT(size, size_t)
    API_DECL_WAIT(ptrdiff, ptrdiff_t)

    /*
     * atomic swaps
     */

    /**
     * @brief swap value into symmetric variable, fetch back old value
     * @page shmem_long_atomic_swap
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     long shmem_ctx_long_atomic_swap(shmem_ctx_t ctx, long *target, long value, int pe);
     @endcode
     *
     * @section Effect
     *
     * swaps contents of remote variable with supplied value
     *
     * @section Return
     * None.
     *
     */
#define API_CTX_DECL_SWAP(_name, _type)                                 \
    /* see \ref shmem_ctx_long_atomic_swap() */                         \
    _type shmem_ctx_##_name##_atomic_swap(shmem_ctx_t ctx,              \
                                          _type *target,                \
                                          _type value,                  \
                                          int pe) _WUR;                 \
    /* see \ref shmem_long_atomic_swap() */                             \
    _type shmem_##_name##_atomic_swap(_type *target, _type value, int pe);

    API_CTX_DECL_SWAP(float, float)
    API_CTX_DECL_SWAP(double, double)
    API_CTX_DECL_SWAP(int, int)
    API_CTX_DECL_SWAP(long, long)
    API_CTX_DECL_SWAP(longlong, long long)
    API_CTX_DECL_SWAP(uchar, unsigned char)
    API_CTX_DECL_SWAP(uint, unsigned int)
    API_CTX_DECL_SWAP(ulong, unsigned long)
    API_CTX_DECL_SWAP(ulonglong, unsigned long long)
    API_CTX_DECL_SWAP(int32, int32_t)
    API_CTX_DECL_SWAP(int64, int64_t)
    API_CTX_DECL_SWAP(uint32, uint32_t)
    API_CTX_DECL_SWAP(uint64, uint64_t)
    API_CTX_DECL_SWAP(size, size_t)
    API_CTX_DECL_SWAP(ptrdiff, ptrdiff_t)

#undef API_CTX_DECL_SWAP

    long shmem_long_swap(long *target, long value, int pe)
        _DEPRECATED_BY(shmem_long_atomic_swap, 1.4) _WUR;
    int shmem_int_swap(int *target, int value, int pe)
        _DEPRECATED_BY(shmem_int_atomic_swap, 1.4) _WUR;
    long long shmem_longlong_swap(long long *target, long long value,
                                  int pe)
        _DEPRECATED_BY(shmem_longlong_atomic_swap, 1.4) _WUR;
    float shmem_float_swap(float *target, float value, int pe)
        _DEPRECATED_BY(shmem_float_atomic_swap, 1.4) _WUR;
    double shmem_double_swap(double *target, double value, int pe)
        _DEPRECATED_BY(shmem_double_atomic_swap, 1.4) _WUR;

    /**
     * @brief conditionally swap value into symmetric variable, fetch
     * back old value
     * @page shmem_long_atomic_compare_swap
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     long shmem_ctx_long_atomic_compare_swap(shmem_ctx_t ctx,
                                             long *target,
                                             long cond, long value,
                                             int pe);
     @endcode
     *
     * @section Effect
     *
     * Performs an atomic swap of @a value but only if @a cond is
     * equal to the old value on the remote PE.
     *
     * @section Return
     * None.
     *
     */
#define API_CTX_DECL_CSWAP(_name, _type)                                \
    /* see \ref shmem_ctx_long_atomic_compare_swap() */                 \
    _type shmem_ctx_##_name##_atomic_compare_swap(shmem_ctx_t ctx,      \
                                                  _type *target,        \
                                                  _type cond, _type value, \
                                                  int pe) _WUR;         \
    /* see \ref shmem_long_atomic_compare_swap() */                     \
    _type shmem_##_name##_atomic_compare_swap(_type *target,            \
                                              _type cond,_type value,   \
                                              int pe);

    /* no reals */
    API_CTX_DECL_CSWAP(int, int)
    API_CTX_DECL_CSWAP(long, long)
    API_CTX_DECL_CSWAP(longlong, long long)
    API_CTX_DECL_CSWAP(uint, unsigned int)
    API_CTX_DECL_CSWAP(ulong, unsigned long)
    API_CTX_DECL_CSWAP(ulonglong, unsigned long long)
    API_CTX_DECL_CSWAP(int32, int32_t)
    API_CTX_DECL_CSWAP(int64, int64_t)
    API_CTX_DECL_CSWAP(uint32, uint32_t)
    API_CTX_DECL_CSWAP(uint64, uint64_t)
    API_CTX_DECL_CSWAP(size, size_t)
    API_CTX_DECL_CSWAP(ptrdiff, ptrdiff_t)

#undef API_CTX_DECL_SWAP

    long shmem_long_cswap(long *target,
                          long cond, long value,
                          int pe)
        _DEPRECATED_BY(shmem_long_atomic_compare_swap, 1.4) _WUR;
    int shmem_int_cswap(int *target,
                        int cond, int value,
                        int pe)
        _DEPRECATED_BY(shmem_int_atomic_compare_swap, 1.4) _WUR;
    long long shmem_longlong_cswap(long long *target,
                                   long long cond,
                                   long long value,
                                   int pe)
        _DEPRECATED_BY(shmem_longlong_atomic_compare_swap, 1.4) _WUR;

    /**
     * atomic fetch-{add,inc} & add,inc
     */

    /**
     * @brief add value to symmetric variable, fetch back old value
     * @page shmem_long_atomic_fetch_add
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     long shmem_long_atomic_fetch_add(long *target, long value, int pe);
     @endcode
     *
     * @section Effect
     *
     * atomic fetch-and-add on another PE
     *
     * @section Return
     * None.
     *
     */
    SHMEM_DECL_AMO2(fetch_add, long, long)
    /* see \ref shmem_long_atomic_fetch_add() */
    SHMEM_DECL_AMO2(fetch_add, int, int)
    /* see \ref shmem_long_atomic_fetch_add() */
    SHMEM_DECL_AMO2(fetch_add, longlong, long long)
    /* see \ref shmem_long_atomic_fetch_add() */
    SHMEM_DECL_AMO2(fetch_add, uint, unsigned int)
    /* see \ref shmem_long_atomic_fetch_add() */
    SHMEM_DECL_AMO2(fetch_add, ulong, unsigned long)
    /* see \ref shmem_long_atomic_fetch_add() */
    SHMEM_DECL_AMO2(fetch_add, ulonglong, unsigned long long)
    /* see \ref shmem_long_atomic_fetch_add() */
    SHMEM_DECL_AMO2(fetch_add, int32, int32_t)
    /* see \ref shmem_long_atomic_fetch_add() */
    SHMEM_DECL_AMO2(fetch_add, int64, int64_t)
    /* see \ref shmem_long_atomic_fetch_add() */
    SHMEM_DECL_AMO2(fetch_add, uint32, uint32_t)
    /* see \ref shmem_long_atomic_fetch_add() */
    SHMEM_DECL_AMO2(fetch_add, uint64, uint64_t)
    /* see \ref shmem_long_atomic_fetch_add() */
    SHMEM_DECL_AMO2(fetch_add, size, size_t)
    /* see \ref shmem_long_atomic_fetch_add() */
    SHMEM_DECL_AMO2(fetch_add, ptrdiff, ptrdiff_t)

    long shmem_long_fadd(long *target, long value, int pe)
        _DEPRECATED_BY(shmem_long_atomic_fetch_add, 1.4) _WUR;
    int shmem_int_fadd(int *target, int value, int pe)
        _DEPRECATED_BY(shmem_int_atomic_fetch_add, 1.4) _WUR;
    long long shmem_longlong_fadd(long long *target, long long value,
                                  int pe)
        _DEPRECATED_BY(shmem_longlong_atomic_fetch_add, 1.4) _WUR;

    /**
     * @brief increment symmetric variable, fetch back old value
     * @page shmem_long_atomic_fetch_inc
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     long shmem_long_atomic_fetch_inc(long *target, int pe);
     @endcode
     *
     * @section Effect
     *
     * atomic fetch-and-increment on another PE
     *
     * @section Return
     * None.
     *
     */
    SHMEM_DECL_AMO1(fetch_inc, long, long)
    /* see \ref shmem_long_atomic_fetch_inc() */
    SHMEM_DECL_AMO1(fetch_inc, int, int)
    /* see \ref shmem_long_atomic_fetch_inc() */
    SHMEM_DECL_AMO1(fetch_inc, longlong, long long)
    /* see \ref shmem_long_atomic_fetch_inc() */
    SHMEM_DECL_AMO1(fetch_inc, uint, unsigned int)
    /* see \ref shmem_long_atomic_fetch_inc() */
    SHMEM_DECL_AMO1(fetch_inc, ulong, unsigned long)
    /* see \ref shmem_long_atomic_fetch_inc() */
    SHMEM_DECL_AMO1(fetch_inc, ulonglong, unsigned long long)
    /* see \ref shmem_long_atomic_fetch_inc() */
    SHMEM_DECL_AMO1(fetch_inc, int32, int32_t)
    /* see \ref shmem_long_atomic_fetch_inc() */
    SHMEM_DECL_AMO1(fetch_inc, int64, int64_t)
    /* see \ref shmem_long_atomic_fetch_inc() */
    SHMEM_DECL_AMO1(fetch_inc, uint32, uint32_t)
    /* see \ref shmem_long_atomic_fetch_inc() */
    SHMEM_DECL_AMO1(fetch_inc, uint64, uint64_t)
    /* see \ref shmem_long_atomic_fetch_inc() */
    SHMEM_DECL_AMO1(fetch_inc, size, size_t)
    /* see \ref shmem_long_atomic_fetch_inc() */
    SHMEM_DECL_AMO1(fetch_inc, ptrdiff, ptrdiff_t)

    long shmem_long_finc(long *target, int pe)
        _DEPRECATED_BY(shmem_long_atomic_fetch_inc, 1.4) _WUR;
    int shmem_int_finc(int *target, int pe)
        _DEPRECATED_BY(shmem_int_atomic_fetch_inc, 1.4) _WUR;
    long long shmem_longlong_finc(long long *target, int pe)
        _DEPRECATED_BY(shmem_longlong_atomic_fetch_inc, 1.4) _WUR;

    /**
     * @brief add value to symmetric variable
     * @page shmem_long_atomic_add
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_long_atomic_add(long *target, long value, int pe);
     @endcode
     *
     * @section Effect
     *
     * atomic add on another PE
     *
     * @section Return
     * None.
     *
     */
    SHMEM_DECL_VOID_AMO2(add, long, long)
    /* see \ref shmem_long_atomic_add() */
    SHMEM_DECL_VOID_AMO2(add, int, int)
    /* see \ref shmem_long_atomic_add() */
    SHMEM_DECL_VOID_AMO2(add, longlong, long long)
    /* see \ref shmem_long_atomic_add() */
    SHMEM_DECL_VOID_AMO2(add, uint, unsigned int)
    /* see \ref shmem_long_atomic_add() */
    SHMEM_DECL_VOID_AMO2(add, ulong, unsigned long)
    /* see \ref shmem_long_atomic_add() */
    SHMEM_DECL_VOID_AMO2(add, ulonglong, unsigned long long)
    /* see \ref shmem_long_atomic_add() */
    SHMEM_DECL_VOID_AMO2(add, int32, int32_t)
    /* see \ref shmem_long_atomic_add() */
    SHMEM_DECL_VOID_AMO2(add, int64, int64_t)
    /* see \ref shmem_long_atomic_add() */
    SHMEM_DECL_VOID_AMO2(add, uint32, uint32_t)
    /* see \ref shmem_long_atomic_add() */
    SHMEM_DECL_VOID_AMO2(add, uint64, uint64_t)
    /* see \ref shmem_long_atomic_add() */
    SHMEM_DECL_VOID_AMO2(add, size, size_t)
    /* see \ref shmem_long_atomic_add() */
    SHMEM_DECL_VOID_AMO2(add, ptrdiff, ptrdiff_t)

    void shmem_long_add(long *target, long value, int pe)
        _DEPRECATED_BY(shmem_long_atomic_add, 1.4);
    void shmem_int_add(int *target, int value, int pe)
        _DEPRECATED_BY(shmem_int_atomic_add, 1.4);
    void shmem_longlong_add(long long *target, long long value, int pe)
        _DEPRECATED_BY(shmem_longlong_atomic_add, 1.4);

    /**
     * @brief These routines perform an atomic "or" operation
     * between a data value and the target data object.
     * @page shmem_ulong_atomic_or
     * @section Synopsis
     *
     * - C/C++:
     * @code
     void shmem_ulong_atomic_or(unsigned long *dest, unsigned long value, int pe);
     * @endcode
     *
     * @param dest    Address of the symmetric data object where to save
     *                    the data on the target pe.
     * @param value     The value with which the exclusive-or operation is
     *                    atomically performed with the data at address dest.
     * @param pe        An integer that indicates the PE number upon
     *                which dest is to be updated.
     *
     * @section Constraints
     *      - dest must be the address of a symmetric data object.
     *      - If using C/C++, the type of value must match that implied in
     *        the Synopsis section.
     *      - value must be the same type as the target data object.
     *      - This process must be carried out guaranteeing that it will not
     *          be interrupted by any other atomic operation on the
     *          specified type.
     *
     * @section Effect
     *
     * The atomic or routines perform a bitwise or-operation between
     * value and the data at address dest on PE pe. The operation must
     * be completed without the possibility of another process updating
     * dest at the same time.
     *
     * @section Return
     * None.
     *
     */
    SHMEM_DECL_VOID_AMO2(or, ulong, unsigned long)
    /* see \ref shmem_ulong_atomic_or() */
    SHMEM_DECL_VOID_AMO2(or, uint, unsigned int)
    /* see \ref shmem_ulong_atomic_or() */
    SHMEM_DECL_VOID_AMO2(or, ulonglong, unsigned long long)
    /* see \ref shmem_ulong_atomic_or() */
    SHMEM_DECL_VOID_AMO2(or, int32, int32_t)
    /* see \ref shmem_ulong_atomic_or() */
    SHMEM_DECL_VOID_AMO2(or, int64, int64_t)
    /* see \ref shmem_ulong_atomic_or() */
    SHMEM_DECL_VOID_AMO2(or, uint32, uint32_t)
    /* see \ref shmem_ulong_atomic_or() */
    SHMEM_DECL_VOID_AMO2(or, uint64, uint64_t)

    /**
     * @brief These routines perform an atomic "or"
     * operation between a data value and the target data object and
     * return the former remote value.
     * @page shmem_ulong_atomic_fetch_or
     * @section Synopsis
     *
     * - C/C++:
     * @code
     long shmem_ulong_atomic_fetch_or(unsigned long *dest, unsigned long value, int pe);
     * @endcode
     *
     * @param dest    Address of the symmetric data object where to save
     *                    the data on the target pe.
     * @param value     The value with which the exclusive-or operation is
     *                    atomically performed with the data at address dest.
     * @param pe        An integer that indicates the PE number upon
     *                which dest is to be updated.
     *
     * @section Constraints
     *      - dest must be the address of a symmetric data object.
     *      - If using C/C++, the type of value must match that implied in
     *        the Synopsis section.
     *      - value must be the same type as the target data object.
     *      - This process must be carried out guaranteeing that it will not
     *          be interrupted by any other atomic operation on the
     *          specified type.
     *
     * @section Effect
     *
     * The atomic and routines perform a bitwise and-operation between
     * value and the data at address dest on PE pe. The operation must
     * be completed without the possibility of another process updating
     * dest at the same time.
     *
     * @section Return
     * Value stored previously in remote location.
     *
     */

    SHMEM_DECL_AMO2(fetch_or, ulong, unsigned long)
    /* see \ref shmem_ulong_atomic_fetch_or() */
    SHMEM_DECL_AMO2(fetch_or, uint, unsigned int)
    /* see \ref shmem_ulong_atomic_fetch_or() */
    SHMEM_DECL_AMO2(fetch_or, ulonglong, unsigned long long)
    /* see \ref shmem_ulong_atomic_fetch_or() */
    SHMEM_DECL_AMO2(fetch_or, int32, int32_t)
    /* see \ref shmem_ulong_atomic_fetch_or() */
    SHMEM_DECL_AMO2(fetch_or, int64, int64_t)
    /* see \ref shmem_ulong_atomic_fetch_or() */
    SHMEM_DECL_AMO2(fetch_or, uint32, uint32_t)
    /* see \ref shmem_ulong_atomic_fetch_or() */
    SHMEM_DECL_AMO2(fetch_or, uint64, uint64_t)

    /**
     * @brief These routines perform an atomic "and" operation
     * between a data value and the target data object.
     * @page shmem_ulong_atomic_and
     * @section Synopsis
     *
     * - C/C++:
     * @code
     void shmem_ulong_atomic_and(unsigned long *dest, unsigned long value, int pe);
     * @endcode
     *
     * @param dest    Address of the symmetric data object where to save
     *                    the data on the target pe.
     * @param value     The value with which the exclusive-or operation is
     *                    atomically performed with the data at address dest.
     * @param pe        An integer that indicates the PE number upon
     *                which dest is to be updated.
     *
     * @section Constraints
     *      - dest must be the address of a symmetric data object.
     *      - If using C/C++, the type of value must match that implied in
     *        the Synopsis section.
     *      - value must be the same type as the target data object.
     *      - This process must be carried out guaranteeing that it will not
     *          be interrupted by any other atomic operation on the
     *          specified type.
     *
     * @section Effect
     *
     * The atomic exclusive-or routines perform an and-operation between
     * value and the data at address dest on PE pe. The operation must
     * be completed without the possibility of another process updating
     * dest between the time of the fetch and the update.
     *
     * @section Return
     * None.
     *
     */

    SHMEM_DECL_VOID_AMO2(and, ulong, unsigned long)
    /* see \ref shmem_ulong_atomic_and() */
    SHMEM_DECL_VOID_AMO2(and, uint, unsigned int)
    /* see \ref shmem_ulong_atomic_and() */
    SHMEM_DECL_VOID_AMO2(and, ulonglong, unsigned long long)
    /* see \ref shmem_ulong_atomic_and() */
    SHMEM_DECL_VOID_AMO2(and, int32, int32_t)
    /* see \ref shmem_ulong_atomic_and() */
    SHMEM_DECL_VOID_AMO2(and, int64, int64_t)
    /* see \ref shmem_ulong_atomic_and() */
    SHMEM_DECL_VOID_AMO2(and, uint32, uint32_t)
    /* see \ref shmem_ulong_atomic_and() */
    SHMEM_DECL_VOID_AMO2(and, uint64, uint64_t)

    /**
     * @brief These routines perform an atomic "and"
     * operation between a data value and the target data object and
     * return the former remote value.
     * @page shmem_ulong_atomic_fetch_and
     * @section Synopsis
     *
     * - C/C++:
     * @code
     long shmem_ulong_atomic_fetch_and(unsigned long *dest, unsigned long value, int pe);
     * @endcode
     *
     * @param dest    Address of the symmetric data object where to save
     *                    the data on the target pe.
     * @param value     The value with which the exclusive-or operation is
     *                    atomically performed with the data at address dest.
     * @param pe        An integer that indicates the PE number upon
     *                which dest is to be updated.
     *
     * @section Constraints
     *      - dest must be the address of a symmetric data object.
     *      - If using C/C++, the type of value must match that implied in
     *        the Synopsis section.
     *      - value must be the same type as the target data object.
     *      - This process must be carried out guaranteeing that it will not
     *          be interrupted by any other atomic operation on the
     *          specified type.
     *
     * @section Effect
     *
     * The atomic exclusive-or routines perform an and-operation between
     * value and the data at address dest on PE pe. The operation must
     * be completed without the possibility of another process updating
     * dest between the time of the fetch and the update.
     *
     * @section Return
     * Value stored previously in remote location.
     *
     */

    SHMEM_DECL_AMO2(fetch_and, ulong, unsigned long)
    /* see \ref shmem_ulong_atomic_fetch_and() */
    SHMEM_DECL_AMO2(fetch_and, uint, unsigned int)
    /* see \ref shmem_ulong_atomic_fetch_and() */
    SHMEM_DECL_AMO2(fetch_and, ulonglong, unsigned long long)
    /* see \ref shmem_ulong_atomic_fetch_and() */
    SHMEM_DECL_AMO2(fetch_and, int32, int32_t)
    /* see \ref shmem_ulong_atomic_fetch_and() */
    SHMEM_DECL_AMO2(fetch_and, int64, int64_t)
    /* see \ref shmem_ulong_atomic_fetch_and() */
    SHMEM_DECL_AMO2(fetch_and, uint32, uint32_t)
    /* see \ref shmem_ulong_atomic_fetch_and() */
    SHMEM_DECL_AMO2(fetch_and, uint64, uint64_t)

    /**
     * @brief These routines perform an atomic "xor" operation
     * between a data value and the target data object.
     * @page shmem_ulong_atomic_xor
     * @section Synopsis
     *
     * - C/C++:
     * @code
     void shmem_ulong_atomic_xor(unsigned long *dest, unsigned long value, int pe);
     * @endcode
     *
     * @param dest    Address of the symmetric data object where to save
     *                    the data on the target pe.
     * @param value     The value with which the exclusive-or operation is
     *                    atomically performed with the data at address dest.
     * @param pe        An integer that indicates the PE number upon
     *                which dest is to be updated.
     *
     * @section Constraints
     *      - dest must be the address of a symmetric data object.
     *      - If using C/C++, the type of value must match that implied in
     *        the Synopsis section.
     *      - value must be the same type as the target data object.
     *      - This process must be carried out guaranteeing that it will not
     *          be interrupted by any other atomic operation on the
     *          specified type.
     *
     * @section Effect
     *
     * The atomic exclusive-or routines perform an xor-operation between
     * value and the data at address dest on PE pe. The operation must
     * be completed without the possibility of another process updating
     * dest between the time of the fetch and the update.
     *
     * @section Return
     * None.
     *
     */

    SHMEM_DECL_VOID_AMO2(xor, ulong, unsigned long)
    /* see \ref shmem_ulong_atomic_xor() */
    SHMEM_DECL_VOID_AMO2(xor, uint, unsigned int)
    /* see \ref shmem_ulong_atomic_xor() */
    SHMEM_DECL_VOID_AMO2(xor, ulonglong, unsigned long long)
    /* see \ref shmem_ulong_atomic_xor() */
    SHMEM_DECL_VOID_AMO2(xor, int32, int32_t)
    /* see \ref shmem_ulong_atomic_xor() */
    SHMEM_DECL_VOID_AMO2(xor, int64, int64_t)
    /* see \ref shmem_ulong_atomic_xor() */
    SHMEM_DECL_VOID_AMO2(xor, uint32, uint32_t)
    /* see \ref shmem_ulong_atomic_xor() */
    SHMEM_DECL_VOID_AMO2(xor, uint64, uint64_t)

    /**
     * @brief These routines perform an atomic "xor"
     * operation between a data value and the target data object and
     * return the former remote value.
     * @page shmem_ulong_atomic_fetch_xor
     * @section Synopsis
     *
     * - C/C++:
     * @code
     long shmem_ulong_atomic_fetch_xor(unsigned long *dest, unsigned long value, int pe);
     * @endcode
     *
     * @param dest    Address of the symmetric data object where to save
     *                    the data on the target pe.
     * @param value     The value with which the exclusive-or operation is
     *                    atomically performed with the data at address dest.
     * @param pe        An integer that indicates the PE number upon
     *                which dest is to be updated.
     *
     * @section Constraints
     *      - dest must be the address of a symmetric data object.
     *      - If using C/C++, the type of value must match that implied in
     *        the Synopsis section.
     *      - value must be the same type as the target data object.
     *      - This process must be carried out guaranteeing that it will not
     *          be interrupted by any other atomic operation on the
     *          specified type.
     *
     * @section Effect
     *
     * The atomic exclusive-or routines perform an xor-operation between
     * value and the data at address dest on PE pe. The operation must
     * be completed without the possibility of another process updating
     * dest between the time of the fetch and the update.
     *
     * @section Return
     *
     * Value stored previously in remote location.
     *
     */

    SHMEM_DECL_AMO2(fetch_xor, uint, unsigned int)
    /* see \ref shmem_ulong_atomic_fetch_xor() */
    SHMEM_DECL_AMO2(fetch_xor, ulonglong, unsigned long long)
    /* see \ref shmem_ulong_atomic_fetch_xor() */
    SHMEM_DECL_AMO2(fetch_xor, int32, int32_t)
    /* see \ref shmem_ulong_atomic_fetch_xor() */
    SHMEM_DECL_AMO2(fetch_xor, int64, int64_t)
   /* see \ref shmem_ulong_atomic_fetch_xor() */
    SHMEM_DECL_AMO2(fetch_xor, uint32, uint32_t)
    /* see \ref shmem_ulong_atomic_fetch_xor() */
    SHMEM_DECL_AMO2(fetch_xor, uint64, uint64_t)

    /**
     * @brief increment symmetric variable
     * @page shmem_long_atomic_inc
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_long_atomic_inc(long *target, int pe);
     @endcode
     *
     * @section Effect
     *
     * atomic increment on another PE
     *
     * @section Return
     * None.
     *
     */
    SHMEM_DECL_VOID_AMO1(inc, long, long)
    /* see \ref shmem_long_atomic_inc() */
    SHMEM_DECL_VOID_AMO1(inc, int, int)
    /* see \ref shmem_long_atomic_inc() */
    SHMEM_DECL_VOID_AMO1(inc, longlong, long long)
    /* see \ref shmem_long_atomic_inc() */
    SHMEM_DECL_VOID_AMO1(inc, uint, unsigned int)
    /* see \ref shmem_long_atomic_inc() */
    SHMEM_DECL_VOID_AMO1(inc, ulong, unsigned long)
    /* see \ref shmem_long_atomic_inc() */
    SHMEM_DECL_VOID_AMO1(inc, ulonglong, unsigned long long)
    /* see \ref shmem_long_atomic_inc() */
    SHMEM_DECL_VOID_AMO1(inc, int32, int32_t)
    /* see \ref shmem_long_atomic_inc() */
    SHMEM_DECL_VOID_AMO1(inc, int64, int64_t)
    /* see \ref shmem_long_atomic_inc() */
    SHMEM_DECL_VOID_AMO1(inc, uint32, uint32_t)
    /* see \ref shmem_long_atomic_inc() */
    SHMEM_DECL_VOID_AMO1(inc, uint64, uint64_t)
    /* see \ref shmem_long_atomic_inc() */
    SHMEM_DECL_VOID_AMO1(inc, size, size_t)
    /* see \ref shmem_long_atomic_inc() */
    SHMEM_DECL_VOID_AMO1(inc, ptrdiff, ptrdiff_t)

    void shmem_long_inc(long *target, int pe)
        _DEPRECATED_BY(shmem_long_atomic_inc, 1.4);
    void shmem_int_inc(int *target, int pe)
        _DEPRECATED_BY(shmem_int_atomic_inc, 1.4);
    void shmem_longlong_inc(long long *target, int pe)
        _DEPRECATED_BY(shmem_longlong_atomic_inc, 1.4);

    /**
     * @brief These routines perform an atomic fetch from a remote PE
     * @page shmem_ctx_long_atomic_fetch
     * @section Synopsis
     *
     * - C/C++:
     * @code
     long shmem_ctx_long_atomic_fetch(shmem_ctx_t ctx, const long *dest, int pe);
     * @endcode
     *
     * @param dest    Address of the symmetric data object in which save the
     *                    data on the target pe.
     * @param pe        An integer that indicates the PE number upon
     *                    which dest is to be updated.
     *
     * @section Constraints
     *      - dest must be the address of a symmetric data object.
     *      - If using C/C++, the type of value must match that implied
     *        in the Synopsis section.
     *      - value must be the same type as the target data object.
     *      - This process must be carried out guaranteeing that it will not
     *          be interrupted by any other atomic operation on the
     *          specified type.
     *
     * @section Effect
     *
     * The atomic fetch routines atomically return the value at address
     * "dest" on PE pe. The operation must
     * be completed without the possibility of another process updating
     * dest on PE pe using the same type.
     *
     * @section Return
     * The value stored at address "dest" on PE pe.
     *
     */
    SHMEM_DECL_CONST_AMO1(fetch, long, long)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, int, int)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, float, float)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, double, double)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, longlong, long long)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, uint, unsigned int)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, ulong, unsigned long)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, ulonglong, unsigned long long)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, int32, int32_t)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, int64, int64_t)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, uint32, uint32_t)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, uint64, uint64_t)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, size, size_t)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, ptrdiff, ptrdiff_t)

    int shmem_int_fetch(const int *dest, int pe)
        _DEPRECATED_BY(shmem_int_atomic_fetch, 1.4) _WUR;
    long shmem_long_fetch(const long *dest, int pe)
        _DEPRECATED_BY(shmem_long_atomic_fetch, 1.4) _WUR;
    long long shmem_longlong_fetch(const long long *dest, int pe)
        _DEPRECATED_BY(shmem_longlong_atomic_fetch, 1.4) _WUR;
    float shmem_float_fetch(const float *dest, int pe)
        _DEPRECATED_BY(shmem_float_atomic_fetch, 1.4) _WUR;
    double shmem_double_fetch(const double *dest, int pe)
        _DEPRECATED_BY(shmem_double_atomic_fetch, 1.4) _WUR;

    /**
     * @brief These routines perform an atomic set of a variable on a
     * remote PE
     * @page shmem_long_atomic_fetch_xor
     * @section Synopsis
     *
     * - C/C++:
     * @code
     long shmem_ulong_atomic_fetch_xor(long *dest, long value, int pe);
     * @endcode
     *
     * @param dest    Address of the symmetric data object where to save
     *                    the data on the target pe.
     * @param value     The value with which the exclusive-or operation is
     *                    atomically performed with the data at address dest.
     * @param pe        An integer that indicates the PE number upon
     *                which dest is to be updated.
     *
     * @section Constraints
     *      - dest must be the address of a symmetric data object.
     *      - If using C/C++, the type of value must match that implied in
     *        the Synopsis section.
     *      - value must be the same type as the target data object.
     *      - This process must be carried out guaranteeing that it will not
     *          be interrupted by any other atomic operation on the
     *          specified type.
     *
     * @section Effect
     *
     * The atomic exclusive-or routines perform an xor-operation between
     * value and the data at address dest on PE pe. The operation must
     * be completed without the possibility of another process updating
     * dest between the time of the fetch and the update.
     *
     * @section Return
     *
     * Value stored previously in remote location.
     *
     */
    SHMEM_DECL_AMO2(fetch_xor, long, long)

    /* see \ref shmem_long_atomic_fetch_xor() */
    SHMEM_DECL_AMO2(fetch_xor, ulong, unsigned long)
    /* see \ref shmem_long_atomic_fetch_xor() */
    SHMEM_DECL_AMO2(fetch_xor, uint, unsigned int)
    /* see \ref shmem_long_atomic_fetch_xor() */
    SHMEM_DECL_AMO2(fetch_xor, ulonglong, unsigned long long)
    /* see \ref shmem_long_atomic_fetch_xor() */
    SHMEM_DECL_AMO2(fetch_xor, int32, int32_t)
    /* see \ref shmem_long_atomic_fetch_xor() */
    SHMEM_DECL_AMO2(fetch_xor, int64, int64_t)
   /* see \ref shmem_uong_atomic_fetch_xor() */
    SHMEM_DECL_AMO2(fetch_xor, uint32, uint32_t)
    /* see \ref shmem_long_atomic_fetch_xor() */
    SHMEM_DECL_AMO2(fetch_xor, uint64, uint64_t)

    /**
     * @brief increment symmetric variable
     * @page shmem_long_atomic_inc
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_long_atomic_inc(long *target, int pe);
     @endcode
     *
     * @section Effect
     *
     * atomic increment on another PE
     *
     * @section Return
     * None.
     *
     */
    SHMEM_DECL_VOID_AMO1(inc, long, long)
    /* see \ref shmem_long_atomic_inc() */
    SHMEM_DECL_VOID_AMO1(inc, int, int)
    /* see \ref shmem_long_atomic_inc() */
    SHMEM_DECL_VOID_AMO1(inc, longlong, long long)
    /* see \ref shmem_long_atomic_inc() */
    SHMEM_DECL_VOID_AMO1(inc, uint, unsigned int)
    /* see \ref shmem_long_atomic_inc() */
    SHMEM_DECL_VOID_AMO1(inc, ulong, unsigned long)
    /* see \ref shmem_long_atomic_inc() */
    SHMEM_DECL_VOID_AMO1(inc, ulonglong, unsigned long long)
    /* see \ref shmem_long_atomic_inc() */
    SHMEM_DECL_VOID_AMO1(inc, int32, int32_t)
    /* see \ref shmem_long_atomic_inc() */
    SHMEM_DECL_VOID_AMO1(inc, int64, int64_t)
    /* see \ref shmem_long_atomic_inc() */
    SHMEM_DECL_VOID_AMO1(inc, uint32, uint32_t)
    /* see \ref shmem_long_atomic_inc() */
    SHMEM_DECL_VOID_AMO1(inc, uint64, uint64_t)
    /* see \ref shmem_long_atomic_inc() */
    SHMEM_DECL_VOID_AMO1(inc, size, size_t)
    /* see \ref shmem_long_atomic_inc() */
    SHMEM_DECL_VOID_AMO1(inc, ptrdiff, ptrdiff_t)

    void shmem_long_inc(long *target, int pe)
        _DEPRECATED_BY(shmem_long_atomic_inc, 1.4);
    void shmem_int_inc(int *target, int pe)
        _DEPRECATED_BY(shmem_int_atomic_inc, 1.4);
    void shmem_longlong_inc(long long *target, int pe)
        _DEPRECATED_BY(shmem_longlong_atomic_inc, 1.4);

    /**
     * @brief These routines perform an atomic fetch from a remote PE
     * @page shmem_ctx_long_atomic_fetch
     * @section Synopsis
     *
     * - C/C++:
     * @code
     long shmem_ctx_long_atomic_fetch(shmem_ctx_t ctx, const long *dest, int pe);
     * @endcode
     *
     * @param dest    Address of the symmetric data object in which save the
     *                    data on the target pe.
     * @param pe        An integer that indicates the PE number upon
     *                    which dest is to be updated.
     *
     * @section Constraints
     *      - dest must be the address of a symmetric data object.
     *      - If using C/C++, the type of value must match that implied
     *        in the Synopsis section.
     *      - value must be the same type as the target data object.
     *      - This process must be carried out guaranteeing that it will not
     *          be interrupted by any other atomic operation on the
     *          specified type.
     *
     * @section Effect
     *
     * The atomic fetch routines atomically return the value at address
     * "dest" on PE pe. The operation must
     * be completed without the possibility of another process updating
     * dest on PE pe using the same type.
     *
     * @section Return
     * The value stored at address "dest" on PE pe.
     *
     */
    SHMEM_DECL_CONST_AMO1(fetch, long, long)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, int, int)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, float, float)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, double, double)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, longlong, long long)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, uint, unsigned int)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, ulong, unsigned long)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, ulonglong, unsigned long long)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, int32, int32_t)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, int64, int64_t)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, uint32, uint32_t)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, uint64, uint64_t)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, size, size_t)
    /* see \ref shmem_long_atomic_fetch() */
    SHMEM_DECL_CONST_AMO1(fetch, ptrdiff, ptrdiff_t)

    int shmem_int_fetch(const int *dest, int pe)
        _DEPRECATED_BY(shmem_int_atomic_fetch, 1.4) _WUR;
    long shmem_long_fetch(const long *dest, int pe)
        _DEPRECATED_BY(shmem_long_atomic_fetch, 1.4) _WUR;
    long long shmem_longlong_fetch(const long long *dest, int pe)
        _DEPRECATED_BY(shmem_longlong_atomic_fetch, 1.4) _WUR;
    float shmem_float_fetch(const float *dest, int pe)
        _DEPRECATED_BY(shmem_float_atomic_fetch, 1.4) _WUR;
    double shmem_double_fetch(const double *dest, int pe)
        _DEPRECATED_BY(shmem_double_atomic_fetch, 1.4) _WUR;

    /**
     * @brief These routines perform an atomic set of a variable on a
     * remote PE
     * @page shmem_ctx_long_atomic_set
     *
     * @section Synopsis
     *
     * - C/C++:
     * @code
     void shmem_ctx_long_atomic_set(shmem_ctx_t ctx, long *dest, long value, int pe);
     * @endcode
     *
     * @param dest    Address of the symmetric data object in which save the
     *                    data on the target pe.
     * @param value     The remote dest address is atomically set to
     *                    this value.
     * @param pe        An integer that indicates the PE number upon
     *                    which dest is to be updated.
     *
     * @section Constraints
     *      - dest must be the address of a symmetric data object.
     *      - If using C/C++, the type of value must match that implied in the
     *        Synopsis section.
     *      - value must be the same type as the dest data object.
     *      - This process must be carried out guaranteeing that it will not
     *          be interrupted by any other atomic operation on the
     *          specified type.
     *
     * @section Effect
     *
     * The atomic set routines atomically update an address to be "value" on
     * PE pe. The operation must
     * be completed without the possibility of another process updating
     * dest on PE pe using the same type.
     *
     * @section Return
     * None.
     *
     */
    SHMEM_DECL_VOID_AMO2(set, long, long)
    /* see \ref shmem_long_atomic_set() */
    SHMEM_DECL_VOID_AMO2(set, int, int)
    /* see \ref shmem_long_atomic_set() */
    SHMEM_DECL_VOID_AMO2(set, float, float)
    /* see \ref shmem_long_atomic_set() */
    SHMEM_DECL_VOID_AMO2(set, double, double)
    /* see \ref shmem_long_atomic_set() */
    SHMEM_DECL_VOID_AMO2(set, longlong, long long)
    /* see \ref shmem_long_atomic_set() */
    SHMEM_DECL_VOID_AMO2(set, uint, unsigned int)
    /* see \ref shmem_long_atomic_set() */
    SHMEM_DECL_VOID_AMO2(set, ulong, unsigned long)
    /* see \ref shmem_long_atomic_set() */
    SHMEM_DECL_VOID_AMO2(set, ulonglong, unsigned long long)
    /* see \ref shmem_long_atomic_set() */
    SHMEM_DECL_VOID_AMO2(set, int32, int32_t)
    /* see \ref shmem_long_atomic_set() */
    SHMEM_DECL_VOID_AMO2(set, int64, int64_t)
    /* see \ref shmem_long_atomic_set() */
    SHMEM_DECL_VOID_AMO2(set, uint32, uint32_t)
    /* see \ref shmem_long_atomic_set() */
    SHMEM_DECL_VOID_AMO2(set, uint64, uint64_t)
    /* see \ref shmem_long_atomic_set() */
    SHMEM_DECL_VOID_AMO2(set, size, size_t)
    /* see \ref shmem_long_atomic_set() */
    SHMEM_DECL_VOID_AMO2(set, ptrdiff, ptrdiff_t)

    void shmem_int_set(int *dest, int value, int pe)
        _DEPRECATED_BY(shmem_int_atomic_set, 1.4);
    void shmem_long_set(long *dest, long value, int pe)
        _DEPRECATED_BY(shmem_long_atomic_set, 1.4);
    void shmem_longlong_set(long long *dest, long long value, int pe)
        _DEPRECATED_BY(shmem_longlong_atomic_set, 1.4);
    void shmem_float_set(float *dest, float value, int pe)
        _DEPRECATED_BY(shmem_float_atomic_set, 1.4);
    void shmem_double_set(double *dest, double value, int pe)
        _DEPRECATED_BY(shmem_double_atomic_set, 1.4);

    /*
     * locks/critical section
     */

    /**
     * @brief claims a distributed lock
     *
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_set_lock(long *lock);
     @endcode
     *
     * @param[in, out] lock a symmetric variable
     *
     * @section Effect
     *
     * The calling PE claims a lock on the symmetric variable.  Blocks
     * until lock acquired.
     *
     * @section Return
     * None.
     *
     */
    void shmem_set_lock(long *lock);

    /**
     * @brief releases a distributed lock
     *
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_clear_lock(long *lock);
     @endcode
     *
     * @param[in, out] lock a symmetric variable
     *
     * @section Effect
     *
     * The calling PE releases a lock on the symmetric variable.
     *
     * @section Return
     * None.
     *
     */
    void shmem_clear_lock(long *lock);

    /**
     * @brief tests a distributed lock
     *
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_test_lock(long *lock);
     @endcode
     *
     * @param[in, out] lock a symmetric variable
     *
     * @section Effect
     *
     * The calling PE checks to see if lock can be acquired.  If yes,
     * the lock is claimed, otherwise the lock is not claimed and the
     * call returns immediately.  until lock acquired.
     *
     * @section Return
     * non-zero if lock acquired, 0 if not.
     *
     */
    int shmem_test_lock(long *lock) _WUR;

    /**
     * reductions
     */

    void shmem_long_sum_to_all(long *target, const long *source, int nreduce,
                               int PE_start, int logPE_stride,
                               int PE_size, long *pWrk, long *pSync);

    /* see \ref shmem_long_sum_to_all() */
    void shmem_complexd_sum_to_all(COMPLEXIFY(double) * target,
                                   const COMPLEXIFY(double) * source,
                                   int nreduce,
                                   int PE_start, int logPE_stride,
                                   int PE_size,
                                   COMPLEXIFY(double) * pWrk,
                                   long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_complexf_sum_to_all(COMPLEXIFY(float) * target,
                                   const COMPLEXIFY(float) * source,
                                   int nreduce, int PE_start,
                                   int logPE_stride, int PE_size,
                                   COMPLEXIFY(float) * pWrk,
                                   long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_double_sum_to_all(double *target, const double *source,
                                 int nreduce, int PE_start,
                                 int logPE_stride, int PE_size,
                                 double *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_float_sum_to_all(float *target, const float *source,
                                int nreduce, int PE_start,
                                int logPE_stride, int PE_size,
                                float *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_int_sum_to_all(int *target, const int *source, int nreduce,
                              int PE_start, int logPE_stride,
                              int PE_size, int *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_longdouble_sum_to_all(long double *target,
                                     const long double *source, int nreduce,
                                     int PE_start, int logPE_stride,
                                     int PE_size, long double *pWrk,
                                     long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_longlong_sum_to_all(long long *target, const long long *source,
                                   int nreduce, int PE_start,
                                   int logPE_stride, int PE_size,
                                   long long *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_short_sum_to_all(short *target, const short *source,
                                int nreduce, int PE_start,
                                int logPE_stride, int PE_size,
                                short *pWrk, long *pSync);

    /* see \ref shmem_long_sum_to_all() */
    void shmem_complexd_prod_to_all(COMPLEXIFY(double) * target,
                                    const COMPLEXIFY(double) * source,
                                    int nreduce,
                                    int PE_start, int logPE_stride,
                                    int PE_size,
                                    COMPLEXIFY(double) * pWrk,
                                    long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_complexf_prod_to_all(COMPLEXIFY(float) * target,
                                    const COMPLEXIFY(float) * source,
                                    int nreduce, int PE_start,
                                    int logPE_stride, int PE_size,
                                    COMPLEXIFY(float) * pWrk,
                                    long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_double_prod_to_all(double *target, const double *source,
                                  int nreduce, int PE_start,
                                  int logPE_stride, int PE_size,
                                  double *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_float_prod_to_all(float *target, const float *source,
                                 int nreduce, int PE_start,
                                 int logPE_stride, int PE_size,
                                 float *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_int_prod_to_all(int *target, const int *source, int nreduce,
                               int PE_start, int logPE_stride,
                               int PE_size, int *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_long_prod_to_all(long *target, const long *source, int nreduce,
                                int PE_start, int logPE_stride,
                                int PE_size, long *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_longdouble_prod_to_all(long double *target,
                                      const long double *source, int nreduce,
                                      int PE_start, int logPE_stride,
                                      int PE_size, long double *pWrk,
                                      long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_longlong_prod_to_all(long long *target,
                                    const long long *source, int nreduce,
                                    int PE_start, int logPE_stride,
                                    int PE_size, long long *pWrk,
                                    long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_short_prod_to_all(short *target, const short *source,
                                 const int nreduce, int PE_start,
                                 int logPE_stride, int PE_size,
                                 short *pWrk, long *pSync);

    /* see \ref shmem_long_sum_to_all() */
    void shmem_int_and_to_all(int *target,
                              const int *source,
                              int nreduce,
                              int PE_start, int logPE_stride,
                              int PE_size, int *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_long_and_to_all(long *target, const long *source, int nreduce,
                               int PE_start, int logPE_stride,
                               int PE_size, long *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_longlong_and_to_all(long long *target, const long long *source,
                                   int nreduce, int PE_start,
                                   int logPE_stride, int PE_size,
                                   long long *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_short_and_to_all(short *target, const short *source,
                                int nreduce, int PE_start,
                                int logPE_stride, int PE_size,
                                short *pWrk, long *pSync);

    /* see \ref shmem_long_sum_to_all() */
    void shmem_int_or_to_all(int *target,
                             const int *source,
                             int nreduce,
                             int PE_start, int logPE_stride,
                             int PE_size, int *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_long_or_to_all(long *target, const long *source, int nreduce,
                              int PE_start, int logPE_stride,
                              int PE_size, long *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_longlong_or_to_all(long long *target, const long long *source,
                                  int nreduce, int PE_start,
                                  int logPE_stride, int PE_size,
                                  long long *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_short_or_to_all(short *target, const short *source,
                               int nreduce, int PE_start,
                               int logPE_stride, int PE_size,
                               short *pWrk, long *pSync);

    /* see \ref shmem_long_sum_to_all() */
    void shmem_int_xor_to_all(int *target,
                              const int *source,
                              int nreduce,
                              int PE_start, int logPE_stride,
                              int PE_size, int *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_long_xor_to_all(long *target, const long *source, int nreduce,
                               int PE_start, int logPE_stride,
                               int PE_size, long *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_longlong_xor_to_all(long long *target, const long long *source,
                                   int nreduce, int PE_start,
                                   int logPE_stride, int PE_size,
                                   long long *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_short_xor_to_all(short *target, const short *source,
                                int nreduce, int PE_start,
                                int logPE_stride, int PE_size,
                                short *pWrk, long *pSync);

    /* see \ref shmem_long_sum_to_all() */
    void shmem_int_max_to_all(int *target,
                              const int *source,
                              int nreduce,
                              int PE_start, int logPE_stride,
                              int PE_size, int *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_long_max_to_all(long *target, const long *source, int nreduce,
                               int PE_start, int logPE_stride,
                               int PE_size, long *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_longlong_max_to_all(long long *target, const long long *source,
                                   int nreduce, int PE_start,
                                   int logPE_stride, int PE_size,
                                   long long *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_short_max_to_all(short *target, const short *source,
                                int nreduce, int PE_start,
                                int logPE_stride, int PE_size,
                                short *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_longdouble_max_to_all(long double *target,
                                     const long double *source, int nreduce,
                                     int PE_start, int logPE_stride,
                                     int PE_size, long double *pWrk,
                                     long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_float_max_to_all(float *target, const float *source,
                                int nreduce, int PE_start,
                                int logPE_stride, int PE_size,
                                float *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_double_max_to_all(double *target, const double *source,
                                 int nreduce, int PE_start,
                                 int logPE_stride, int PE_size,
                                 double *pWrk, long *pSync);

    /* see \ref shmem_long_sum_to_all() */
    void shmem_int_min_to_all(int *target,
                              const int *source,
                              int nreduce,
                              int PE_start, int logPE_stride,
                              int PE_size, int *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_long_min_to_all(long *target, const long *source, int nreduce,
                               int PE_start, int logPE_stride,
                               int PE_size, long *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_longlong_min_to_all(long long *target, const long long *source,
                                   int nreduce, int PE_start,
                                   int logPE_stride, int PE_size,
                                   long long *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_short_min_to_all(short *target, const short *source,
                                int nreduce, int PE_start,
                                int logPE_stride, int PE_size,
                                short *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_longdouble_min_to_all(long double *target,
                                     const long double *source, int nreduce,
                                     int PE_start, int logPE_stride,
                                     int PE_size, long double *pWrk,
                                     long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_float_min_to_all(float *target, const float *source,
                                int nreduce, int PE_start,
                                int logPE_stride, int PE_size,
                                float *pWrk, long *pSync);
    /* see \ref shmem_long_sum_to_all() */
    void shmem_double_min_to_all(double *target, const double *source,
                                 int nreduce, int PE_start,
                                 int logPE_stride, int PE_size,
                                 double *pWrk, long *pSync);

    /**
     * broadcasts
     */

#define API_BROADCAST_SIZE(_size)                                       \
    /* see \ref shmem_broadcast64() */                                  \
    void shmem_broadcast##_size(void *target, const void *source,       \
                                size_t nelems, int PE_root, int PE_start, \
                                int logPE_stride, int PE_size, long *pSync);

    API_BROADCAST_SIZE(32)
    API_BROADCAST_SIZE(64)

    /**
     * collects
     */

#define API_COLLECT_NAME_SIZE(_opname, _size)                           \
    /* see \ref shmem_##_opname##64() */                                \
    void shmem_##_opname##_size(void *target, const void *source,       \
                                size_t nelems, int PE_start, int logPE_stride, \
                                int PE_size, long *pSync);

    API_COLLECT_NAME_SIZE(fcollect, 32)
    API_COLLECT_NAME_SIZE(fcollect, 64)
    API_COLLECT_NAME_SIZE(collect, 32)
    API_COLLECT_NAME_SIZE(collect, 64)

    /**
     * all-to-all collectives
     */

#define API_ALLTOALL_SIZE(_size)                                        \
    /* see \ref shmem_alltoall64() */                                   \
    void shmem_alltoall##_size(void *target, const void *source, size_t nelems, \
                               int PE_start, int logPE_stride, int PE_size, \
                               long *pSync);

    API_ALLTOALL_SIZE(32)
    API_ALLTOALL_SIZE(64)

#define API_ALLTOALLS_SIZE(_size)                                       \
    /* see \ref shmem_alltoalls64() */                                  \
    void shmem_alltoalls##_size(void *target, const void *source,       \
                                ptrdiff_t dst, ptrdiff_t sst, size_t nelems, \
                                int PE_start, int logPE_stride, int PE_size, \
                                long *pSync);

    API_ALLTOALLS_SIZE(32)
    API_ALLTOALLS_SIZE(64)

    /**
     * Contexts-based API
     *
     */

    /**
     * @brief creates a new context
     * @page shmem_ctx_create
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     int shmem_ctx_create(long options, shmem_ctx_t *ctxp);
     @endcode
     *
     * @param[in] options how to create this context
     * @param[out] ctxp pointer to the newly created context
     *
     * @section Effect
     *
     * creates a new communication context and returns
     * its handle through the ctx argument.
     *
     * @section Options
     *
     * bitwise-or of SHMEM_CTX_SERIALIZED, SHMEM_CTX_PRIVATE,
     * SHMEM_CTX_NOSTORE, or zero for no special options.
     *
     * @section Return
     *
     * Zero on success, non-zero otherwise
     *
     */
    int shmem_ctx_create(long options, shmem_ctx_t *ctxp);

    /**
     * @brief creates a new context
     * @page shmem_ctx_destroy
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_ctx_destroy(shmem_ctx_t ctx);
     @endcode
     *
     * @param[in] ctx an existing context
     *
     * @section Effect
     *
     * destroys a context that was created by a call to \ref shmem_ctx_create.
     *
     * @section Notes
     *
     * it is illegal to pass the default context SHMEM_CTX_DEFAULT to
     * this routine.
     *
     * @section Return
     * None.
     *
     */
    void shmem_ctx_destroy(shmem_ctx_t ctx);

    /*
     * C11 Generics
     *
     */
#include <shmem/generics.h>

    /*
     * deprecated cache routines
     */
#include <shmem/cache.h>

#ifdef __cplusplus
}
#endif  /* __cplusplus */

/*
 * tidy-up complex "I" macro detection.
 *
 */
#ifdef  shmemi_h_I_already_defined__
# undef  shmemi_h_I_already_defined__
#else
# undef I
#endif /* shmemi_h_I_already_defined__ */

#endif  /* ! _SHMEM_API_H */
