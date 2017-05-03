include @GASNET_MAKE_FRAGMENT@

lib_LIBRARIES          = libshmemc-gasnet.a
libshmemc_gasnet_a_SOURCES    = comms.c
libshmemc_gasnet_a_CPPFLAGS = $(GASNET_CPPFLAGS)
# libshmemc_gasnet_a_CFLAGS = $(GASNET_CFLAGS)

MY_WRAPPERS       = oshcc oshrun

oshcc:		oshcc.tmpl
	sed \
		-e 's,%GASNET_LDFLAGS%,$(GASNET_LDFLAGS),' \
		-e 's,%GASNET_LIBS%,$(GASNET_LIBS),' \
		< $< > $@


oshrun:		oshrun.tmpl
	sed \
		-e 's,%GASNET_LDFLAGS%,$(GASNET_LDFLAGS),' \
		-e 's,%GASNET_LIBS%,$(GASNET_LIBS),' \
		< $< > $@

if ENABLE_CXX

MY_WRAPPERS      += oshcxx

oshcxx:		oshcxx.tmpl
	sed \
		-e 's,%GASNET_LDFLAGS%,$(GASNET_LDFLAGS),' \
		-e 's,%GASNET_LIBS%,$(GASNET_LIBS),' \
		< $< > $@

install-exec-hook:
	cd $(bindir); \
	cp -p oshcxx oshc++

endif

if ENABLE_FORTRAN

MY_WRAPPERS      += oshfort

oshfort:	oshfort.tmpl
	sed \
		-e 's,%GASNET_LDFLAGS%,$(GASNET_LDFLAGS),' \
		-e 's,%GASNET_LIBS%,$(GASNET_LIBS),' \
		< $< > $@

endif

bin_SCRIPTS = $(MY_WRAPPERS)
