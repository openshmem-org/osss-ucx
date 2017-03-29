bin_SCRIPTS      = oshcc oshrun

if ENABLE_CXX

bin_SCRIPTS     += oshcxx

install-exec-hook:
	cd $(bindir); \
	cp -p oshcxx oshc++

endif

if ENABLE_FORTRAN

bin_SCRIPTS     += oshfort

endif
