# For license: see LICENSE file at top-level

OTHER_CPPFLAGS            = -I$(srcdir)/../shmemu \
				-I../../include -I$(srcdir)/..
LIBSHMEMC_SOURCES         = \
				contexts.c \
				globalexit.c \
				locks.c \
				readenv.c \
				shmemc_init.c \
				state.c
#
# -- begin: UCX sources --
#
LIBSHMEMC_SOURCES        += \
				ucx/comms.c \
				ucx/contexts.c \
				ucx/init.c \
				ucx/waituntil.c

if HAVE_PMI
LIBSHMEMC_SOURCES        += ucx/pmi1_client.c
EXTRA_CFLAGS              = @PMI_CFLAGS@
endif
if HAVE_PMIX
LIBSHMEMC_SOURCES        += ucx/pmix_client.c
EXTRA_CFLAGS              = @PMIX_CFLAGS@
endif
#
# -- end: UCX sources --
#

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

#
# to build wrapper scripts
#

COMPILER_SED_SCRIPT = 's%\@PTHREAD_CC\@%@PTHREAD_CC@%g'

oshcc:	oshcc.in	osh_common
	cat $^ | sed -e $(COMPILER_SED_SCRIPT) > $@

if ENABLE_CXX
oshcxx:	oshcxx.in	osh_common
	cat $^ > $@
endif # c++

if ENABLE_FORTRAN
oshfort:	oshfort.in	osh_common
	cat $^ > $@
endif # fortran

oshrun:	oshrun.in
	sed -e 's%\@SHMEM_LAUNCHER\@%@SHMEM_LAUNCHER@%g' < $^ > $@
