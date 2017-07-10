SUBDIRS                = dlmalloc

LIBSHMEMI_SOURCES      = init.c \
				malloc.c heapx.c pe.c logger.c

SHMEMX_CPPFLAGS        = -I$(srcdir)/../shmemu -I$(srcdir)/../shmemc

MY_INCLUDES            = -I../../include

lib_LTLIBRARIES        = libshmemi.la
libshmemi_la_SOURCES   = $(LIBSHMEMI_SOURCES)
libshmemi_la_CFLAGS    = $(AM_CFLAGS)
libshmemi_la_CPPFLAGS  = $(SHMEMX_CPPFLAGS) $(MY_INCLUDES)
libshmemi_la_LDFLAGS   = -version-info 0:0:0

lib_LIBRARIES          = libshmemi.a
libshmemi_a_SOURCES    = $(LIBSHMEMI_SOURCES)
libshmemi_a_CFLAGS     = $(AM_CFLAGS)
libshmemi_a_CPPFLAGS   = $(SHMEMX_CPPFLAGS) $(MY_INCLUDES)
