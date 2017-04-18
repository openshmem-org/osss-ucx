shmemu_dir             = shmemu
shmemu_cppflags        = -I$(srcdir)/$(shmemu_dir)
shmemu_ldflags         = -L$(shmemu_dir)
shmemu_libs            = -lshmemu

shmemc_dir             = shmemc
shmemc_cppflags        = -I$(srcdir)/$(shmemc_dir)
shmemc_ldflags         = -L$(shmemc_dir)
shmemc_libs            = -lshmemc

shmemi_dir             = shmemi
shmemi_cppflags        = -I$(srcdir)/$(shmemi_dir)
shmemi_ldflags         = -L$(shmemi_dir)
shmemi_libs            = -lshmemi

SUBDIRS                = $(shmemu_dir) \
				$(shmemc_dir) \
				$(shmemi_dir)

LIBSHMEM_SOURCES       = \
				init.c \
				malloc.c \
				version.c \
				atomics.c lock.c waituntil.c \
				pe.c \
				ptr.c \
				accessible.c \
				put.c get.c \
				put-strided.c get-strided.c \
				put-nbi.c get-nbi.c \
				reductions.c \
				fcollect.c collect.c \
				broadcast.c \
				barrier.c \
				cache.c


if ENABLE_FORTRAN
LIBSHMEM_SOURCES      += fortran/fortran.c \
				fortran/fortran-mem.c
endif

if ENABLE_EXPERIMENTAL
LIBSHMEM_SOURCES      += wtime.c xor.c \
				fortran/fortranx.c
endif

MY_INCLUDE             = -I../include

lib_LTLIBRARIES        = libshmem.la
libshmem_la_SOURCES    = $(LIBSHMEM_SOURCES)
libshmem_la_CFLAGS     = $(AM_CFLAGS)
libshmem_la_CPPFLAGS   = $(shmemu_cppflags) $(MY_INCLUDE)
libshmem_la_LDFLAGS    = -version-info 0:0:0

lib_LIBRARIES          = libshmem.a
libshmem_a_SOURCES     = $(LIBSHMEM_SOURCES)
libshmem_a_CPPFLAGS    = $(shmemu_cppflags) $(MY_INCLUDE)

bin_PROGRAMS           = osh_info
osh_info_SOURCES       = osh_info.c
osh_info_LDFLAGS       = $(shmemu_ldflags) $(shmemu_libs)
