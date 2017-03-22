shmemu_dir             = ./shmemu
shmemu_cppflags        = -I$(shmemu_dir)
shmemu_ldflags         = -L$(shmemu_dir)
shmemu_libs            = -lshmemu

shmemi_dir             = ./shmemi

SUBDIRS                = $(shmemu_dir) \
				$(shmemi_dir) # TBD comms

LIBSHMEM_SOURCES       = init.c malloc.c version.c atomics.c cache.c \
				pe.c ptr.c accessible.c

if ENABLE_EXPERIMENTAL
LIBSHMEM_SOURCES      += wtime.c xor.c
endif


lib_LTLIBRARIES        = libshmem.la
libshmem_la_SOURCES    = $(LIBSHMEM_SOURCES)
libshmem_la_CFLAGS     = $(AM_CFLAGS)
libshmem_la_CPPFLAGS   = $(shmemu_cppflags)
libshmem_la_LDFLAGS    = -version-info 0:0:0

lib_LIBRARIES          = libshmem.a
libshmem_a_SOURCES     = $(LIBSHMEM_SOURCES)
libshmem_a_CPPFLAGS    = $(shmemu_cppflags)

# retain tree
nobase_include_HEADERS = shmem.h mpp/shmem.h shmemx.h mpp/shmemx.h

bin_PROGRAMS           = osh_info
osh_info_SOURCES       = osh_info.c
osh_info_LDFLAGS       = $(shmemu_ldflags) $(shmemu_libs)
