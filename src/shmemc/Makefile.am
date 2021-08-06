# For license: see LICENSE file at top-level

OTHER_CPPFLAGS            = -I$(srcdir)/../shmemu \
				-I$(srcdir)/../shmemt \
				-I$(top_srcdir)/src/klib \
				-I$(top_srcdir)/src/api \
				-I../../include \
				-I$(top_srcdir)/include
LIBSHMEMC_SOURCES         = \
				contexts.c \
				globalexit.c \
				env.c \
				heaps.c \
				init.c \
				nodename.c \
				state.c \
				teams.c

#
# -- begin: UCX sources --
#
LIBSHMEMC_SOURCES        += \
				ucx/callbacks.c \
				ucx/comms.c \
				ucx/contexts.c \
				ucx/eps.c \
				ucx/init.c \
				ucx/teams.c \
				ucx/test.c ucx/waituntil.c

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

# support C by default, plus something to run it with

MY_WRAPPERS               = oshcc oshrun

if ENABLE_CXX
MY_WRAPPERS              += oshcxx
endif # c++

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

CC_SED_SCRIPT  = 's,\@PTHREAD_CC\@,@PTHREAD_CC@,g'

oshcc:	oshcc.in	osh_common
	cat $^ | sed -e $(CC_SED_SCRIPT) > $@

if ENABLE_CXX

CXX_SED_SCRIPT = 's,\@CXX\@,@CXX@,g'

oshcxx:	oshcxx.in	osh_common
	cat $^ | sed -e $(CXX_SED_SCRIPT) > $@
endif # c++

#
# DROPPING PERL LAUNCHER
#

OSHRUN_TEMPLATE = oshrun.py.in

oshrun: $(OSHRUN_TEMPLATE)
	sed -e 's,\@PYTHON_BIN\@,@PYTHON_BIN@,g' \
		-e 's,\@SHMEM_LAUNCHER\@,@SHMEM_LAUNCHER@,g' < $^ > $@
