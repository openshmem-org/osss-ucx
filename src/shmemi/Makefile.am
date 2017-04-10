SUBDIRS                = dlmalloc

LIBSHMEMI_SOURCES      = init.c \
				pmi1-client.c pmi2-client.c pmix-client.c \
				malloc.c heapx.c pe.c logger.c \
				accessible.c

SHMEMX_CPPFLAGS        = -I$(srcdir)/../shmemu -I$(srcdir)/../shmemc

PMIX_CPPFLAGS          = @PMIX_CPPFLAGS@

lib_LTLIBRARIES        = libshmemi.la
libshmemi_la_SOURCES   = $(LIBSHMEMI_SOURCES)
libshmemi_la_CFLAGS    = $(AM_CFLAGS)
libshmemi_la_CPPFLAGS  = $(PMIX_CPPFLAGS) $(SHMEMX_CPPFLAGS)
libshmemi_la_LDFLAGS   = -version-info 0:0:0

lib_LIBRARIES          = libshmemi.a
libshmemi_a_SOURCES    = $(LIBSHMEMI_SOURCES)
libshmemi_a_CPPFLAGS   = $(PMIX_CPPFLAGS) $(SHMEMX_CPPFLAGS)
