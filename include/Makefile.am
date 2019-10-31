# For license: see LICENSE file at top-level

SHMEM_HDRS             =  \
				shmem/api.h \
				shmem/cache.h \
				shmem/defs.h \
				shmem/defs_subst.h \
				shmem/depr.h \
				shmem/generics.h \
				shmem/teams.h \
				shmem.h

#
# deprecated locations of files from earlier specs.  Always required.
#
SHMEM_HDRS             += mpp/shmem.h mpp/shmemx.h

#
# add profiling interface if requested
#
if ENABLE_PSHMEM
SHMEM_HDRS             += pshmem.h
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
