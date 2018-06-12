# For license: see LICENSE file at top-level

OTHER_CPPFLAGS            = -I$(srcdir)/../shmemu \
				-I../../include -I$(srcdir)/..
LIBSHMEMC_SOURCES         = \
				barrier.c \
				broadcast.c \
				collalgo.c \
				collect.c \
				comms.c \
				contexts.c \
				fcollect.c \
				globalexit.c \
				locks.c \
				readenv.c \
				shmemc-init.c \
				state.c \
				threading.c \
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

lib_LTLIBRARIES           = libshmemc-ucx.la
libshmemc_ucx_la_SOURCES  = $(LIBSHMEMC_SOURCES)
libshmemc_ucx_la_CFLAGS   = $(UCX_CFLAGS) $(EXTRA_CFLAGS) $(OTHER_CPPFLAGS)

lib_LIBRARIES             = libshmemc-ucx.a
libshmemc_ucx_a_SOURCES   = $(LIBSHMEMC_SOURCES)
libshmemc_ucx_a_CFLAGS    = $(UCX_CFLAGS) $(EXTRA_CFLAGS) $(OTHER_CPPFLAGS)

# support C by default, plus something to run it with

MY_WRAPPERS               = oshcc oshrun

if ENABLE_CXX
MY_WRAPPERS              += oshcxx
endif # c++

if ENABLE_FORTRAN
MY_WRAPPERS              += oshfort
endif # fortran

#
# create alternate name for C++ compiler wrapper
# (if configured)
#
if ENABLE_CXX

install-cxx-alt:	uninstall-cxx-alt
	cd $(bindir); \
	$(LN_S) oshcxx oshc++

uninstall-cxx-alt:
	cd $(bindir); \
	$(RM) oshc++

else

install-cxx-alt:

uninstall-cxx-alt:

endif # c++

#
# create alternate name for launcher
#
install-run-alt:	uninstall-run-alt
	cd $(bindir); \
	$(LN_S) oshrun oshexec

uninstall-run-alt:
	cd $(bindir); \
	$(RM) oshexec

install-exec-hook:	install-cxx-alt install-run-alt

uninstall-local:	uninstall-cxx-alt uninstall-run-alt

#
# and install these
#
bin_SCRIPTS               = $(MY_WRAPPERS)
