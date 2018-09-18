# For license: see LICENSE file at top-level

shmemc_dir             = ./shmemc
shmemc_cppflags        = -I$(top_srcdir)/src/$(shmemc_dir)
shmemc_ldflags         = -L$(shmemc_dir)
shmemc_libs            = -lshmemc-ucx

shmemu_dir             = ./shmemu
shmemu_cppflags        = -I$(top_srcdir)/src/$(shmemu_dir)
shmemu_ldflags         = -L$(shmemu_dir)
shmemu_libs            = -lshmemu

MY_INCLUDE             = -I../include -I$(top_srcdir)/include

all_cppflags           = $(shmemc_cppflags) $(shmemu_cppflags) $(MY_INCLUDE)

MY_SOURCES             = \
                                accessible.c \
                                atomics.c \
                                cache.c \
				contexts.c \
                                globalexit.c \
				info.c \
                                init.c \
                                lock.c \
				progress.c \
                                ptr.c \
                                putget.c \
                                quiet.c \
				ranks.c  \
                                shmalloc.c \
				tl_query.c \
                                version.c \
                                waituntil.c

SUBDIRS                = $(shmemu_dir) $(shmemc_dir) osh_info

#
# -- begin: SHCOLL --
#
# shcoll is bundled with package, but handled differently if pulled in
# from somewhere else.
#

if HAVE_SHCOLL_INTERNAL
SUBDIRS               += collectives/shcoll

all_cppflags          += -I@SHCOLL_DIR@
else
all_cppflags          += -I@SHCOLL_DIR@/include
endif

#
# -- end: SHCOLL --
#
# But we need the shim no matter what

MY_SOURCES            += collectives/shcoll-shim.c

if ENABLE_THREADS
MY_SOURCES            += shmem_mutex.c
endif # threads

#
# separate out the memory allocation routines
#
liballocator_a_SOURCES = allocator/dlmalloc.c allocator/memalloc.c

if ENABLE_EXPERIMENTAL
liballocator_a_SOURCES += allocator/xmemalloc.c
endif # experimental

liballocator_a_CFLAGS  = \
			-DONLY_MSPACES=1 \
			-DHAVE_MORECORE=0 \
			-DHAVE_MMAP=0 \
			-DUSE_LOCKS=1 \
			-DREALLOC_ZERO_BYTES_FREES=1
liballocator_la_SOURCES = $(liballocator_a_SOURCES)
liballocator_la_CFLAGS  = $(liballocator_a_CFLAGS)
noinst_LIBRARIES       = liballocator.a
noinst_LTLIBRARIES     = liballocator.la

if ENABLE_FORTRAN
MY_SOURCES            += fortran.c
endif # fortran

if ENABLE_EXPERIMENTAL

MY_SOURCES            += \
			extensions/putget_signal.c \
			extensions/shmalloc.c \
			extensions/wtime.c \
			extensions/interop.c

all_cppflags          += -I$(srcdir)/extensions

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
