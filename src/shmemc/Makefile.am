# For license: see LICENSE file at top-level

PMIX_CFLAGS               = @PMIX_CFLAGS@

OTHER_CPPFLAGS            = -I$(srcdir)/../shmemu \
				-I../../include -I$(srcdir)/..
LIBSHMEMC_SOURCES         = \
				progress.c \
				state.c \
				pmix-client.c \
				ucx-init.c \
				ranks.c \
				barrier.c \
				broadcast.c \
				comms.c \
				wait.c \
				mcs-lock.c \
				shmemc-init.c

lib_LIBRARIES             = libshmemc-ucx.a
libshmemc_ucx_a_SOURCES   = $(LIBSHMEMC_SOURCES)
libshmemc_ucx_a_CFLAGS    = $(UCX_CFLAGS) $(PMIX_CFLAGS) $(OTHER_CPPFLAGS)

# support C by default, plus something to run it with

MY_WRAPPERS               = oshcc oshrun

if ENABLE_CXX

MY_WRAPPERS              += oshcxx

install-exec-hook:
	cd $(bindir); \
	cp -p oshcxx oshc++

endif

if ENABLE_FORTRAN

MY_WRAPPERS              += oshfort

endif

bin_SCRIPTS               = $(MY_WRAPPERS)
