# For license: see LICENSE file at top-level

shmemc_dir             = shmemc
shmemc_cppflags        = -I$(top_srcdir)/src/$(shmemc_dir)
shmemc_ldflags         = -L$(shmemc_dir)
shmemc_libs            = -lshmemc-ucx

shmemu_dir             = shmemu
shmemu_cppflags        = -I$(top_srcdir)/src/$(shmemu_dir)
shmemu_ldflags         = -L$(shmemu_dir)
shmemu_libs            = -lshmemu

shmemt_dir             = shmemt
shmemt_cppflags        = -I$(top_srcdir)/src/$(shmemt_dir)
shmemt_ldflags         = -L$(shmemt_dir)
shmemt_libs            = -lshmemt

atomics_dir            = ./atomics
atomics_cppflags       = -I$(top_srcdir)/src/$(shmemc_dir)

MY_INCLUDE             = -I../../include \
				-I$(top_srcdir)/include \
				-I$(top_srcdir)/src/shcoll/src

all_cppflags           = $(atomics_cppflags) \
				$(shmemc_cppflags) \
				$(shmemu_cppflags) \
				$(shmemt_cppflags) \
				$(MY_INCLUDE)

MY_SOURCES             = \
				accessible.c \
				contexts.c \
				fencequiet.c \
				globalexit.c \
				info.c \
				init.c \
				lock.c \
				profiling.c \
				ptr.c \
				putget.c \
				put_signal.c \
				ranks.c	 \
				shmalloc.c \
				teams.c \
				tl_query.c \
				version.c

MY_SOURCES            += \
				test.c \
				testall.c \
				testany.c \
				testsome.c

MY_SOURCES            += \
				waituntil.c \
				waitall.c \
				waitany.c \
				waitsome.c

MY_SOURCES            += \
				testall_vector.c \
				testany_vector.c \
				testsome_vector.c

MY_SOURCES            += \
				waitall_vector.c \
				waitany_vector.c \
				waitsome_vector.c

MY_SOURCES            += \
				deprecated/atomics.c \
				deprecated/cache.c \
				deprecated/init.c \
				deprecated/ranks.c \
				deprecated/shmalloc.c \
				deprecated/waituntil.c

SUBDIRS                = 	atomics

#
# -- begin: SHCOLL --
#
# shcoll is bundled with package, but handled differently if pulled in
# from somewhere else.
#

if HAVE_SHCOLL_INTERNAL
all_cppflags          += -I@SHCOLL_DIR@
else
all_cppflags          += -I@SHCOLL_DIR@/include
endif

#
# -- end: SHCOLL --
#
# But we need the shim no matter what

MY_SOURCES            += collectives/shcoll-shim.c
MY_SOURCES            += collectives/table.c

#
# separate out the memory allocation routines
#

liballocator_a_SOURCES    = ../ptmalloc/ptmalloc.c allocator/memalloc.c

if ENABLE_EXPERIMENTAL
liballocator_a_SOURCES   += allocator/xmemalloc.c
endif # experimental

liballocator_a_CPPFLAGS   = -I$(srcdir)/allocator
liballocator_a_CFLAGS     = \
				-DFOOTERS=1 \
				-DCORRUPTION_ERROR_ACTION=report_corruption \
				-DUSAGE_ERROR_ACTION=report_usage_error \
				-DONLY_MSPACES=1 \
				-DHAVE_MORECORE=0 \
				-DHAVE_MMAP=0 \
				-DUSE_LOCKS=1 \
				-DREALLOC_ZERO_BYTES_FREES=1
liballocator_la_SOURCES   = $(liballocator_a_SOURCES)
liballocator_la_CFLAGS    = $(liballocator_a_CFLAGS)
liballocator_la_CPPFLAGS  = $(liballocator_a_CPPFLAGS)
noinst_LIBRARIES          = liballocator.a
noinst_LTLIBRARIES        = liballocator.la

#
#
#

if ENABLE_EXPERIMENTAL

MY_SOURCES            += \
			extensions/fence.c \
			extensions/quiet.c \
			extensions/shmalloc.c \
			extensions/wtime.c \
			extensions/interop.c

all_cppflags          += -I$(srcdir)/extensions

endif # experimental

LIBSHMEM_SOURCES       = $(MY_SOURCES)

lib_LTLIBRARIES        = libshmem.la
libshmem_la_SOURCES    = $(LIBSHMEM_SOURCES)
libshmem_la_CFLAGS     = $(AM_CFLAGS)
libshmem_la_CPPFLAGS   = $(all_cppflags)
libshmem_la_LDFLAGS    = -version-info 0:0:0
libshmem_la_LIBADD     = liballocator.la
