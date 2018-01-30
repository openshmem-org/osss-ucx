# For license: see LICENSE file at top-level

OTHER_CPPFLAGS            = -I$(srcdir)/../shmemu \
				-I../../include -I$(srcdir)/..
LIBSHMEMC_SOURCES         = \
				barrier.c \
				broadcast.c \
				collect.c \
				comms.c \
				contexts.c \
				fcollect.c \
				globalexit.c \
				locks.c \
				readenv.c \
				shmemc-init.c \
				state.c \
				ucx-init.c \
				waituntil.c

if HAVE_PMI
LIBSHMEMC_SOURCES         += pmi1-client.c
EXTRA_CFLAGS                 = @PMI_CFLAGS@
endif
if HAVE_PMIX
LIBSHMEMC_SOURCES         += pmix-client.c
EXTRA_CFLAGS                 = @PMIX_CFLAGS@
endif

lib_LIBRARIES             = libshmemc-ucx.a
libshmemc_ucx_a_SOURCES   = $(LIBSHMEMC_SOURCES)
libshmemc_ucx_a_CFLAGS    = $(UCX_CFLAGS) $(EXTRA_CFLAGS) $(OTHER_CPPFLAGS)

# support C by default, plus something to run it with

MY_WRAPPERS               = oshcc oshrun

if ENABLE_CXX

MY_WRAPPERS              += oshcxx

install-exec-hook:
	cd $(bindir); \
	cp -p oshcxx oshc++

endif # c++

if ENABLE_FORTRAN

MY_WRAPPERS              += oshfort

endif # fortran


# install these

bin_SCRIPTS               = $(MY_WRAPPERS)
