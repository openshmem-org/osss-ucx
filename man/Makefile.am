man1_MANS        = oshcc.1 oshrun.1 osh_info.1 osh_intro.1

if ENABLE_CXX

man1_MANS       += oshcxx.1

install-data-hook:
	cd $(man1dir); \
	cp -p oshcxx.1 oshc++.1

endif

if ENABLE_FORTRAN

man1_MANS       += oshfort.1

endif
