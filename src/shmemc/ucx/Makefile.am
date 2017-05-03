lib_LIBRARIES          = libshmemc-ucx.a
libshmemc_ucx_a_SOURCES    = comms.c
libshmemc_ucx_a_CFLAGS = $(UCX_CFLAGS)

MY_WRAPPERS       = oshcc oshrun

oshcc:		oshcc.tmpl
	sed \
		< $< > $@


oshrun:		oshrun.tmpl
	sed \
		< $< > $@

if ENABLE_CXX

MY_WRAPPERS      += oshcxx

oshcxx:		oshcxx.tmpl
	sed \
		< $< > $@

install-exec-hook:
	cd $(bindir); \
	cp -p oshcxx oshc++

endif

if ENABLE_FORTRAN

MY_WRAPPERS      += oshfort

oshfort:	oshcc.tmpl
	sed \
		< $< > $@

endif

bin_SCRIPTS = $(MY_WRAPPERS)
