# retain tree
nobase_include_HEADERS = shmem.h \
				mpp/shmem.h mpp/shmemx.h

if ENABLE_FORTRAN
nobase_include_HEADERS += shmem_defines.fh shmem.fh mpp/shmem.fh
endif

if ENABLE_EXPERIMENTAL
SHMEMX_HEADER          = shmemx.h
else
SHMEMX_HEADER          = null-shmemx.h
endif

install-data-hook:
	cp -p $(SHMEMX_HEADER) $(includedir)/shmemx.h
