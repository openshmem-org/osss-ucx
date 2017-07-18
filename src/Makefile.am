shmemu_dir             = shmemu
shmemu_cppflags        = -I$(top_srcdir)/src/$(shmemu_dir)
shmemu_ldflags         = -L$(shmemu_dir)
shmemu_libs            = -lshmemu

shmemc_dir             = shmemc
shmemc_cppflags        = -I$(top_srcdir)/src/$(shmemc_dir)
shmemc_ldflags         =
shmemc_libs            =

#shmemi_dir             = shmemi
#shmemi_cppflags        = -I$(top_srcdir)/src/$(shmemi_dir)
#shmemi_ldflags         = -L$(shmemi_dir)
#shmemi_libs            = -lshmemi

SUBDIRS                = $(shmemu_dir) $(shmemc_dir)


MY_SOURCES             = \
				init.c \
				globalexit.c \
				malloc.c \
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
MY_SOURCES            += fortran/fortran.c \
				fortran/fortran-mem.c
endif

if ENABLE_EXPERIMENTAL
MY_SOURCES            += wtime.c xor.c

if ENABLE_FORTRAN
MY_SOURCES            += fortran/fortranx.c
endif

endif

if ENABLE_PSHMEM
MY_SOURCES            += profiling.c
endif

LIBSHMEM_SOURCES       = $(MY_SOURCES)

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
