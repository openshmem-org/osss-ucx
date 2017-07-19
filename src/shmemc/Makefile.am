PMIX_CFLAGS            = @PMIX_CFLAGS@

OTHER_CPPFLAGS         = -I$(srcdir)/../shmemu \
				-I../../include

lib_LIBRARIES          = libshmemc-ucx.a
libshmemc_ucx_a_SOURCES    = \
				hostname.c logger.c timer.c unitparse.c \
				pmix-client.c \
				ucx-init.c \
				heapx.c \
				ranks.c \
				comms.c \
				mcs-lock.c \
				dlmalloc.c \
				memalloc.c \
				shmemu-init.c

libshmemc_ucx_a_CFLAGS = $(UCX_CFLAGS) $(PMIX_CFLAGS) $(OTHER_CPPFLAGS)

# support C by default, plus something to run it with

MY_WRAPPERS       = oshcc oshrun

if ENABLE_CXX

MY_WRAPPERS      += oshcxx

install-exec-hook:
	cd $(bindir); \
	cp -p oshcxx oshc++

endif

if ENABLE_FORTRAN

MY_WRAPPERS      += oshfort

endif

bin_SCRIPTS = $(MY_WRAPPERS)

#
# special override rule
#
include ./dlmalloc.make
