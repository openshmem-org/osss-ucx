LIBSHMEMI_SOURCES      = shmemi.c \
				pmi1-client.c pmi2-client.c pmix-client.c \
				malloc.c logger.c heapx.c timer.c pe.c

lib_LTLIBRARIES        = libshmemi.la
libshmemi_la_SOURCES   = $(LIBSHMEMI_SOURCES)
libshmemi_la_CFLAGS    = $(AM_CFLAGS)
libshmemi_la_CPPFLAGS  = $(PMIX_CPPFLAGS)
libshmemi_la_LDFLAGS   = -version-info 0:0:0

lib_LIBRARIES          = libshmemi.a
libshmemi_a_SOURCES    = $(LIBSHMEMI_SOURCES)
libshmemi_a_CPPFLAGS   = $(PMIX_CPPFLAGS)
