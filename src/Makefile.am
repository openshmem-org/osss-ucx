shmemc_dir             = ./shmemc
shmemc_cppflags        = -I$(top_srcdir)/src/$(shmemc_dir)
shmemc_ldflags         = -L$(shmemc_dir)
shmemc_libs            = -lshmemc-ucx

shmemu_dir             = ./shmemu
shmemu_cppflags        = -I$(top_srcdir)/src/$(shmemu_dir)
shmemu_ldflags         = -L$(shmemu_dir)
shmemu_libs            = -lshmemu

SUBDIRS                = $(shmemc_dir) $(shmemu_dir)

MY_INCLUDE             = -I../include

all_cppflags           = $(shmemc_cppflags) $(shmemu_cppflags) $(MY_INCLUDE)

MY_SOURCES             = \
				dlmalloc.c \
				memalloc.c \
				init.c \
				globalexit.c \
				shmalloc.c \
				version.c \
				atomics.c lock.c waituntil.c \
				pe-query.c \
				ptr.c \
				accessible.c \
				quiet.c \
				put.c get.c \
				put-strided.c get-strided.c \
				put-nbi.c get-nbi.c \
				barrier.c \
				reductions.c \
				fcollect.c collect.c \
				broadcast.c \
				alltoall.c \
				cache.c


if ENABLE_FORTRAN
MY_SOURCES            += fortran/fortran.c fortran/fortran-mem.c
endif

if ENABLE_EXPERIMENTAL
MY_SOURCES            += wtime.c
  if ENABLE_FORTRAN
    MY_SOURCES        += fortran/fortranx.c
  endif
endif

if ENABLE_PSHMEM
MY_SOURCES            += profiling.c
endif

LIBSHMEM_SOURCES       = $(MY_SOURCES)

lib_LTLIBRARIES        = libshmem.la
libshmem_la_SOURCES    = $(LIBSHMEM_SOURCES)
libshmem_la_CFLAGS     = $(AM_CFLAGS)
libshmem_la_CPPFLAGS   = $(all_cppflags)
libshmem_la_LDFLAGS    = -version-info 0:0:0

lib_LIBRARIES          = libshmem.a
libshmem_a_SOURCES     = $(LIBSHMEM_SOURCES)
libshmem_a_CPPFLAGS    = $(all_cppflags)

bin_PROGRAMS           = osh_info
osh_info_SOURCES       = osh_info.c
osh_info_CPPFLAGS      = $(all_cppflags)
osh_info_LDFLAGS       = $(shmemu_ldflags) $(shmemu_libs)
