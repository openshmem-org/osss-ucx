#include <sys/types.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "fortran-common.h"

#include "shmem/api.h"
#include "shmemi/shmemi.h"

#ifdef HAVE_FEATURE_PSHMEM
#include "pshmem.h"
#endif /* HAVE_FEATURE_PSHMEM */

/*
 * used by Fortran routines, but not part of API
 */
extern char *sherror (void);

/*
 * Fortran symmetric memory operations
 */

#ifdef HAVE_FEATURE_PSHMEM
#pragma weak shpalloc_ = pshpalloc_
#define shpalloc_ pshpalloc_
#pragma weak shpdeallc_ = pshpdeallc_
#define shpdeallc_ pshpdeallc_
#pragma weak shpclmove_ = pshpclmove_
#define shpclmove_ pshpclmove_
#endif /* HAVE_FEATURE_PSHMEM */

/**
 *
 * SYNOPSIS
 *   POINTER (addr, A(1))
 *   INTEGER (length, errcode, abort)
 *   CALL SHPALLOC(addr, length, errcode, abort)
 *
 * DESCRIPTION
 *   SHPALLOC  allocates a block of memory from the program's symmetric heap
 *   that is greater than or equal  to  the  size  requested.   To  maintain
 *   symmetric  heap  consistency,  all PEs in an program must call SHPALLOC
 *   with the same value of length; if any  processing  elements  (PEs)  are
 *   missing, the program will hang.
 */

/**
 *
 * we've removed this from the C API since it's a hold-over from an
 * old SGI version.  Left it in the code for now, though, (a) for
 * Fortran to use and (b) in case people want it back.
 */
extern long malloc_error;

void
FORTRANIFY(shpalloc)(uintptr_t **addr, int *length,
                     int *errcode, int *abort)
{
    /* convert 32-bit words to bytes */
    const int scale = sizeof(int32_t);
    void *symm_addr;

    if (*length <= 0) {
        *errcode = SHMEM_MALLOC_BAD_SIZE;
        return;
        /* NOT REACHED */
    }

    symm_addr = shmem_malloc(*length * scale);

    /* pass back status code */
    *errcode = malloc_error;

    /* if malloc succeeded, nothing else to do */
    if (malloc_error == SHMEM_MALLOC_OK) {
        *addr = symm_addr;

        logger(LOG_MEMORY,
               "shpalloc(addr = %p, length = %d,"
               " errcode = %d, abort = %d)",
               addr, *length,
               *errcode, *abort
               );

        return;
        /* NOT REACHED */
    }

    /* failed somehow, we might have to abort */
    logger(*abort ? LOG_FATAL : LOG_MEMORY,
           "shpalloc() was given non-symmetric memory sizes");
    /* MAYBE NOT REACHED */

    addr = NULL;
}

/**
 *
 * SYNOPSIS
 *   POINTER (addr, A(1))
 *   INTEGER errcode, abort
 *   CALL SHPDEALLC(addr, errcode, abort)
 *
 *
 * DESCRIPTION
 *   SHPDEALLC  returns  a block of memory (allocated using SHPALLOC) to the
 *   list of available space in the symmetric heap.  To  maintain  symmetric
 *   heap  consistency, all processing elements (PEs) in a program must call
 *   SHPDEALLC with the same value of addr; if  any  PEs  are  missing,  the
 *   program hangs.
 */

void
FORTRANIFY(shpdeallc)(uintptr_t **addr, int *errcode, int *abort)
{
    logger(LOG_MEMORY,
           "shpdeallc(addr = %p, errcode = %d, abort = %d)",
           *addr, *errcode, *abort);

    shmem_free(*addr);

    /* pass back status code */
    *errcode = malloc_error;

    /* if malloc succeeded, nothing else to do */
    if (malloc_error == SHMEM_MALLOC_OK) {
        return;
        /* NOT REACHED */
    }

    /* failed somehow, we might have to abort */
    logger(*abort ? LOG_FATAL : LOG_MEMORY,
           "shpdeallc() failed: %s", sherror());
    /* MAYBE NOT REACHED */
}

/**
 *
 * SYNOPSIS
 *   POINTER (addr, A(1))
 *   INTEGER length, status, abort
 *   CALL SHPCLMOVE (addr, length, status, abort)
 *
 *
 * DESCRIPTION
 *   The SHPCLMOVE function either extends a symmetric  heap  block  if  the
 *   block  is  followed by a large enough free block or copies the contents
 *   of the existing block to a larger  block  and  returns  a  status  code
 *   indicating that the block was moved.  This function also can reduce the
 *   size of a block if the new length is less than  the  old  length.   All
 *   processing  elements  (PEs)  in  a program must call SHPCLMOVE with the
 *   same value of addr to maintain symmetric heap consistency; if  any  PEs
 *   are missing, the program hangs.
 */

void
FORTRANIFY(shpclmove)(uintptr_t **addr, int *length,
                      int *errcode, int *abort)
{
    *addr = shmem_realloc(*addr, *length);

    /* pass back status code */
    *errcode = malloc_error;

    /* if malloc succeeded, nothing else to do */
    if (malloc_error == SHMEM_MALLOC_OK) {
        return;
        /* NOT REACHED */
    }

    /* failed somehow, we might have to abort */
    logger(*abort ? LOG_FATAL : LOG_MEMORY,
           "shpdeallc() failed: %s", sherror());
    /* MAYBE NOT REACHED */
}
