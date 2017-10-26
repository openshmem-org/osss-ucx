# For license: see LICENSE file at top-level

SHMEM_HDRS             =  shmem/defs.h \
				shmem/depr.h \
				shmem/api.h \
				shmem.h



#
# deprecated locations of files from spec 1.0
#
SHMEM_HDRS            += mpp/shmem.h



#
# must install if fortran requested
#
if ENABLE_FORTRAN
SHMEM_HDRS            += shmem/defs.fh shmem.fh mpp/shmem.fh
endif # fortran



#
# need presence of shmemx header file whether features requested or not.
# Also add fortran one if enabled.
#
if ENABLE_EXPERIMENTAL
SHMEMX_HEADER          = shmemx.h
else
SHMEMX_HEADER          = null-shmemx.h
endif # experimental



#
# extensions, maybe for fortran too
#
if ENABLE_EXPERIMENTAL

if ENABLE_FORTRAN
SHMEM_HDRS            += shmemx.fh

if ENABLE_PSHMEM
SHMEM_HDRS            += pshmemx.h
endif # pshmem

endif # fortran

endif # experimental



#
# add profiling interface if requested
#
if ENABLE_PSHMEM
SHMEM_HDRS            += pshmem.h

if ENABLE_FORTRAN
SHMEM_HDRS            += pshmem.fh
endif # fortran

endif # pshmem



# retain tree
nobase_include_HEADERS = $(SHMEM_HDRS)

install-data-hook:
	cp -p $(SHMEMX_HEADER) $(includedir)/shmemx.h
