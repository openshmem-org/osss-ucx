/* For license: see LICENSE file at top-level */

#ifndef _SHMEM_CACHE_H
#define _SHMEM_CACHE_H 1

#include "shmem/defs.h"
#include "shmem/depr.h"

    /**
     * cache flushing(deprecated)
     */

    /*
     * @brief shmem_clear_cache_inv has no effect.
     *
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_clear_cache_inv(void);
     @endcode
     *
     * @subsection f Fortran
     @code
     CALL SHMEM_CLEAR_CACHE_INV()
     @endcode
     *
     * @section Effect
     *
     * None.
     *
     * @deprecated Included for legacy use only, these calls have no
     * effect in OpenSHMEM.
     *
     * @return None.
     *
     */
    void shmem_clear_cache_inv(void) _DEPRECATED;

    /*
     * @brief shmem_set_cache_inv has no effect.
     *
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_set_cache_inv(void);
     @endcode
     *
     * @subsection f Fortran
     @code
     CALL SHMEM_SET_CACHE_INV()
     @endcode
     *
     * @section Effect
     *
     * None.  Included for legacy use only, these calls have no
     * effect in OpenSHMEM.
     *
     * @deprecated Included for legacy use only.
     *
     * @return None.
     *
     */
    void shmem_set_cache_inv(void) _DEPRECATED;

    /*
     * @brief shmem_clear_cache_line_inv has no effect.
     *
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_clear_cache_line_inv(void *target);
     @endcode
     *
     * @subsection f Fortran
     @code
     CALL SHMEM_CLEAR_CACHE_LINE_INV(target)
     @endcode
     *
     * @param target is the address of the cache line.
     *
     * @section Effect
     *
     * None.
     *
     * @deprecated Included for legacy use only, these calls have no
     * effect in OpenSHMEM.
     *
     * @return None.
     *
     */
    void shmem_clear_cache_line_inv(void *target) _DEPRECATED;

    /*
     * @brief shmem_set_cache_line_inv has no effect.
     *
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_set_cache_line_inv(void *target);
     @endcode
     *
     * @subsection f Fortran
     @code
     CALL SHMEM_SET_CACHE_LINE_INV(target)
     @endcode
     *
     * @param target is the address of the cache line.
     *
     * @section Effect
     *
     * None.
     *
     * @deprecated Included for legacy use only, these calls have no
     * effect in OpenSHMEM.
     *
     * @return None.
     *
     */
    void shmem_set_cache_line_inv(void *target) _DEPRECATED;

    /*
     * @brief shmem_udcflush has no effect.
     *
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_udcflush(void);
     @endcode
     *
     * @subsection f Fortran
     @code
     CALL SHMEM_UDCFLUSH()
     @endcode
     *
     * @section Effect
     *
     * None.
     *
     * @deprecated Included for legacy use only, these calls have no
     * effect in OpenSHMEM.
     *
     * @return None.
     *
     */
    void shmem_udcflush(void) _DEPRECATED;

    /*
     * @brief shmem_udcflush_line has no effect.
     *
     * @section Synopsis
     *
     * @subsection c C/C++
     @code
     void shmem_udcflush_line(void *target);
     @endcode
     *
     * @subsection f Fortran
     @code
     CALL SHMEM_UDCFLUSH_LINE(target)
     @endcode
     *
     * @param target is the address of the cache line.
     *
     * @section Effect
     *
     * None.
     *
     * @deprecated Included for legacy use only, these calls have no
     * effect in OpenSHMEM.
     *
     * @return None.
     *
     */
    void shmem_udcflush_line(void *target) _DEPRECATED;

#endif  /* ! _SHMEM_CACHE_H */
