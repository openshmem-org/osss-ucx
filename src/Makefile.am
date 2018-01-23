# For license: see LICENSE file at top-level

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
                                accessible.c \
                                alltoall.c \
                                atomics.c \
                                barrier.c \
                                broadcast.c \
                                cache.c \
                                collect.c \
				contexts.c \
                                fcollect.c \
                                globalexit.c \
                                info.c \
                                init.c \
                                lock.c \
                                ptr.c \
                                putget.c \
                                quiet.c \
				ranks.c  \
                                reductions.c \
                                shmalloc.c \
				shmem_mutex.c \
                                version.c \
                                waituntil.c

#
# separate out the memory allocation routines
#
liballocator_a_SOURCES = allocator/dlmalloc.c allocator/memalloc.c
liballocator_a_CFLAGS  = \
			-DONLY_MSPACES=1 \
			-DHAVE_MORECORE=0 \
			-DUSE_LOCKS=1 \
			-DREALLOC_ZERO_BYTES_FREES=1
liballocator_la_SOURCES = $(liballocator_a_SOURCES)
liballocator_la_CFLAGS  = $(liballocator_a_CFLAGS)
noinst_LIBRARIES       = liballocator.a
noinst_LTLIBRARIES     = liballocator.la

if ENABLE_FORTRAN
MY_SOURCES            += fortran/fortran.c
MY_SOURCES            += fortran/fortran-mem.c
MY_SOURCES            += fortran/fortran-cache.c
endif # fortran

if ENABLE_EXPERIMENTAL
MY_SOURCES            += wtime.c

endif # experimental

if ENABLE_PSHMEM
MY_SOURCES            += profiling.c
endif # pshmem

LIBSHMEM_SOURCES       = $(MY_SOURCES)

lib_LTLIBRARIES        = libshmem.la
libshmem_la_SOURCES    = $(LIBSHMEM_SOURCES)
libshmem_la_CFLAGS     = $(AM_CFLAGS)
libshmem_la_CPPFLAGS   = $(all_cppflags)
libshmem_la_LDFLAGS    = -version-info 0:0:0
libshmem_la_LIBADD     = liballocator.la

lib_LIBRARIES          = libshmem.a
libshmem_a_SOURCES     = $(LIBSHMEM_SOURCES)
libshmem_a_CPPFLAGS    = $(all_cppflags)
libshmem_a_LIBADD      = liballocator.a

bin_PROGRAMS           = osh_info
osh_info_SOURCES       = osh_info.c info.c
osh_info_CPPFLAGS      = $(all_cppflags)
osh_info_LDFLAGS       = $(shmemu_ldflags) $(shmemu_libs)
