PMIX_CFLAGS            = @PMIX_CFLAGS@

OTHER_CPPFLAGS         = -I$(srcdir)/../../shmemu \
				-I../../include

lib_LIBRARIES          = libshmemc-ucx.a
libshmemc_ucx_a_SOURCES    = comms.c \
				put-strided.c get-strided.c \
				pmi-init.c \
				pmix-client.c

libshmemc_ucx_a_CFLAGS = $(UCX_CFLAGS) $(PMIX_CFLAGS) $(OTHER_CPPFLAGS)

MY_WRAPPERS       = oshcc oshrun

oshcc:		oshcc.tmpl
	cp $^ $@

oshrun:		oshrun.tmpl
	cp $^ $@

if ENABLE_CXX

MY_WRAPPERS      += oshcxx

oshcxx:		oshcxx.tmpl
	cp $^ $@

install-exec-hook:
	cd $(bindir); \
	cp -p oshcxx oshc++

endif

if ENABLE_FORTRAN

MY_WRAPPERS      += oshfort

oshfort:	oshcc.tmpl
	cp $^ $@

endif

bin_SCRIPTS = $(MY_WRAPPERS)

