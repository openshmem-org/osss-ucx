# For license: see LICENSE file at top-level

shmemc_dir             = ./shmemc
shmemc_cppflags        = -I$(top_srcdir)/src/$(shmemc_dir)
shmemc_ldflags         = -L$(shmemc_dir)
shmemc_libs            = -lshmemc-ucx

shmemu_dir             = ./shmemu
shmemu_cppflags        = -I$(top_srcdir)/src/$(shmemu_dir)
shmemu_ldflags         = -L$(shmemu_dir)
shmemu_libs            = -lshmemu

shmemt_dir             = ./shmemt
shmemt_cppflags        = -I$(top_srcdir)/src/$(shmemt_dir)
shmemt_ldflags         = -L$(shmemt_dir)
shmemt_libs            = -lshmemt

MY_INCLUDE             = -I../../../include -I$(top_srcdir)/include

all_cppflags           = $(shmemc_cppflags) \
				$(shmemu_cppflags) \
				$(shmemt_cppflags) \
				$(MY_INCLUDE)

AMO_SOURCES_B          = \
				add.c \
				and.c \
				cswap.c \
				fetch.c \
				inc.c \
				or.c \
				set.c \
				swap.c \
				xor.c \
				fetch-add.c \
				fetch-and.c \
				fetch-inc.c \
				fetch-or.c \
				fetch-xor.c

AMO_SOURCES_NBI        = \
				cswap-nbi.c \
				fetch-add-nbi.c \
				fetch-and-nbi.c \
				fetch-inc-nbi.c \
				fetch-nbi.c \
				fetch-or-nbi.c \
				fetch-xor-nbi.c \
				swap-nbi.c

LIBSHMEM_AMO_SOURCES   = $(AMO_SOURCES_B) $(AMO_SOURCES_NBI)

lib_LTLIBRARIES            = libshmem-amo.la
libshmem_amo_la_SOURCES    = $(LIBSHMEM_AMO_SOURCES)
libshmem_amo_la_CFLAGS     = $(AM_CFLAGS)
libshmem_amo_la_CPPFLAGS   = $(all_cppflags)
libshmem_amo_la_LDFLAGS    = -version-info 0:0:0
