SHMEM_HDRS             =  shmem/defs.h \
				shmem/depr.h \
				shmem/api.h \
				shmem.h
SHMEM_HDRS            += mpp/shmem.h mpp/shmemx.h

if ENABLE_FORTRAN
SHMEM_HDRS            += shmem/defs.fh shmem.fh mpp/shmem.fh
endif

if ENABLE_EXPERIMENTAL
SHMEMX_HEADER          = shmemx.h
else
SHMEMX_HEADER          = null-shmemx.h
endif

# retain tree
nobase_include_HEADERS = $(SHMEM_HDRS)

install-data-hook:
	cp -p $(SHMEMX_HEADER) $(includedir)/shmemx.h
