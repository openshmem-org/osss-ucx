#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "shmem/defs.h"
#include "shmemi/shmemi.h"
#include "shmemu/shmemu.h"
#include "shmemi/shmemi.h"
#include "fortran-common.h"

/**
 *
 * we've removed this from the C API since it's a hold-over from an
 * old SGI version.  Left it in the code for now, though, (a) for
 * Fortran to use and (b) in case people want it back.
 */
extern long malloc_error;

/**
 * readable error message for error code "e"
 */

typedef struct
{
    long code;
    char *msg;
} malloc_error_code_t;

static malloc_error_code_t error_table[] =
    {
        {SHMEM_MALLOC_FAIL,
         "symmetric memory allocation failed"},
        {SHMEM_MALLOC_ALREADY_FREE,
         "attempt to free already null symmetric memory address"},
        {SHMEM_MALLOC_MEMALIGN_FAILED,
         "attempt to align symmetric memory address failed"},
        {SHMEM_MALLOC_REALLOC_FAILED,
         "attempt to reallocate symmetric memory address failed"},
        {SHMEM_MALLOC_SYMMSIZE_FAILED,
         "asymmetric sizes passed to symmetric memory allocator"},
        {SHMEM_MALLOC_BAD_SIZE,
         "size of data to allocate can not be negative"},
        {SHMEM_MALLOC_NOT_ALIGNED,
         "address is not block-aligned"},
        {SHMEM_MALLOC_NOT_IN_SYMM_HEAP,
         "address falls outside of symmetric heap"},
        {SHMEM_MALLOC_OK,
         NULL},
    };

/**
 * Return human-readable error message
 */
char *
sherror(void)
{
    malloc_error_code_t *etp = error_table;

    while (etp->code != SHMEM_MALLOC_OK) {
        if (malloc_error == etp->code) {
            return etp->msg;
            /* NOT REACHED */
        }
        etp += 1;
    }

    return "unknown memory error";
}

/*
 * Fortran symmetric memory operations
 */

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

    symm_addr = shmemi_malloc(*length * scale);

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

    shmemi_free(*addr);

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
    *addr = shmemi_realloc(*addr, *length);

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
