# For license: see LICENSE file at top-level

SHMEM_HDRS             =  \
				shmem/defs.h \
				shmem/depr.h \
				shmem/cache.h \
				shmem/api.h \
				shmem.h

#
# deprecated locations of files from earlier specs.  Always required.
#
SHMEM_HDRS             += mpp/shmem.h mpp/shmemx.h

#
# must install if fortran requested
#
if ENABLE_FORTRAN
SHMEM_HDRS             += shmem/defs.fh shmem.fh
SHMEM_HDRS             += mpp/shmem.fh
endif # fortran

#
# add profiling interface if requested
#
if ENABLE_PSHMEM
SHMEM_HDRS             += pshmem.h

if ENABLE_FORTRAN
SHMEM_HDRS             += pshmem.fh
endif # fortran

endif # pshmem

#
# need presence of *a* shmemx header file whether features requested
# or not.
#
if ENABLE_EXPERIMENTAL
SHMEMX_HEADER_C         = shmemx.h
else
SHMEMX_HEADER_C         = null-shmemx.h
endif # experimental

# retain tree
nobase_include_HEADERS  = $(SHMEM_HDRS)

# these are installed regardless of whether SHMEMX_ enabled

install-data-hook:
	$(INSTALL_DATA) $(srcdir)/$(SHMEMX_HEADER_C) $(includedir)/shmemx.h

uninstall-local:
	$(RM) $(includedir)/shmemx.h
