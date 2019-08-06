# For license: see LICENSE file at top-level

man1_MANS        = oshcc.1 oshrun.1 osh_info.1 osh_intro.1

if ENABLE_CXX

man1_MANS       += oshcxx.1

install-cxx-alt:	uninstall-cxx-alt
	cd $(man1dir); \
	$(LN_S) oshcxx.1 oshc++.1

uninstall-cxx-alt:
	cd $(man1dir); \
	$(RM) oshc++.1

else

install-cxx-alt:

uninstall-cxx-alt:

endif # c++

install-run-alt:	uninstall-run-alt
	cd $(man1dir); \
	$(LN_S) oshrun.1 oshexec.1

uninstall-run-alt:
	cd $(man1dir); \
	$(RM) oshexec.1

install-data-hook:	install-cxx-alt	install-run-alt

uninstall-local:	uninstall-cxx-alt	uninstall-run-alt
