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


bin_SCRIPTS       = $(MY_WRAPPERS)
