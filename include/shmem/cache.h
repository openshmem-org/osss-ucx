/* For license: see LICENSE file at top-level */

#ifndef _SHMEM_CACHE_H
#define _SHMEM_CACHE_H 1

#include <shmem/defs.h>
#include <shmem/depr.h>

/**
 * cache flushing(deprecated)
 */

#ifdef __cplusplus
extern "C"
{
#endif  /* __cplusplus */

    /**
     * @brief shmem_clear_cache_inv has no effect.
     * @page shmem_clear_cache_inv
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_clear_cache_inv(void);
     @endcode
     *
     * @section Effect
     *
     * None.
     *
     * @section Deprecated
     * Included for legacy use only, these calls have no
     * effect in OpenSHMEM.
     *
     * @section Return
     * None.
     *
     */
    void shmem_clear_cache_inv(void) _DEPRECATED;

    /**
     * @brief shmem_set_cache_inv has no effect.
     * @page shmem_set_cache_inv
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_set_cache_inv(void);
     @endcode
     *
     * @section Effect
     *
     * None.
     *
     * @section Deprecated
     * Included for legacy use only, these calls have no
     * effect in OpenSHMEM.
     *
     * @section Return
     * None.
     *
     */
    void shmem_set_cache_inv(void) _DEPRECATED;

    /**
     * @brief shmem_clear_cache_line_inv has no effect.
     * @page shmem_clear_cache_line_inv
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_clear_cache_line_inv(void *target);
     @endcode
     *
     * @param target is the address of the cache line.
     *
     * @section Effect
     *
     * None.
     *
     * @section Deprecated
     * Included for legacy use only, these calls have no
     * effect in OpenSHMEM.
     *
     * @section Return
     * None.
     *
     */
    void shmem_clear_cache_line_inv(void *target) _DEPRECATED;

    /**
     * @brief shmem_set_cache_line_inv has no effect.
     * @page shmem_set_cache_line_inv
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_set_cache_line_inv(void *target);
     @endcode
     *
     * @param target is the address of the cache line.
     *
     * @section Effect
     *
     * None.
     *
     * @section Deprecated
     * Included for legacy use only, these calls have no
     * effect in OpenSHMEM.
     *
     * @section Return
     * None.
     *
     */
    void shmem_set_cache_line_inv(void *target) _DEPRECATED;

    /**
     * @brief shmem_udcflush has no effect.
     * @page shmem_udcflush
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_udcflush(void);
     @endcode
     *
     * @section Effect
     *
     * None.
     *
     * @section Deprecated
     * Included for legacy use only, these calls have no
     * effect in OpenSHMEM.
     *
     * @section Return
     * None.
     *
     */
    void shmem_udcflush(void) _DEPRECATED;

    /**
     * @brief shmem_udcflush_line has no effect.
     * @page shmem_udcflush_line
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_udcflush_line(void *target);
     @endcode
     *
     * @param target is the address of the cache line.
     *
     * @section Effect
     *
     * None.
     *
     * @section Deprecated
     * Included for legacy use only, these calls have no
     * effect in OpenSHMEM.
     *
     * @section Return
     * None.
     *
     */
    void shmem_udcflush_line(void *target) _DEPRECATED;

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* ! _SHMEM_CACHE_H */
