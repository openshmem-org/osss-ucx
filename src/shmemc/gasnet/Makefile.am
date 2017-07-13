include @GASNET_MAKE_FRAGMENT@

lib_LIBRARIES          = libshmemc-gasnet.a

libshmemc_gasnet_a_SOURCES    = bail.c \
				service.c \
				locality.c \
				comms.c \
				gvhack.c \
				put-strided.c \
				get-strided.c \
				lock.c

libshmemc_gasnet_a_CPPFLAGS = $(GASNET_CPPFLAGS) \
				-I$(top_srcdir)/src/shmemi \
				-I$(top_srcdir)/src/shmemu \
				-I$(top_srcdir)/src/shmemc \
				-I../../../include
# libshmemc_gasnet_a_CFLAGS = $(GASNET_CFLAGS)

MY_WRAPPERS       = oshcc oshrun

oshcc:		oshcc.tmpl
	sed \
		-e 's,%GASNET_LDFLAGS%,$(GASNET_LDFLAGS),' \
		-e 's,%GASNET_LIBS%,$(GASNET_LIBS),' \
		-e 's,%GASNET_LD%,$(GASNET_LD),' \
		-e 's,%GASNET_CC%,$(GASNET_CC),' \
		< $< > $@


oshrun:		oshrun.tmpl
	sed \
		-e 's,%GASNET_LDFLAGS%,$(GASNET_LDFLAGS),' \
		-e 's,%GASNET_PREFIX%,$(GASNET_PREFIX),' \
		-e 's,%GASNET_LIBS%,$(GASNET_LIBS),' \
		< $< > $@

if ENABLE_CXX

MY_WRAPPERS      += oshcxx

oshcxx:		oshcxx.tmpl
	sed \
		-e 's,%GASNET_LDFLAGS%,$(GASNET_LDFLAGS),' \
		-e 's,%GASNET_LIBS%,$(GASNET_LIBS),' \
		-e 's,%GASNET_LD%,$(GASNET_LD),' \
		-e 's,%GASNET_CXX%,$(GASNET_CXX),' \
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
		-e 's,%GASNET_LD%,$(GASNET_LD),' \
		< $< > $@

endif

bin_SCRIPTS = $(MY_WRAPPERS)
