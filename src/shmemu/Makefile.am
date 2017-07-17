LIBSHMEMU_SOURCES      = shmemu.c hostname.c timer.c unitparse.c logger.c

SHMEMX_CPPFLAGS        = -I$(srcdir)/../shmemi -I$(srcdir)/../shmemc

MY_INCLUDES            = -I../../include

# lib_LTLIBRARIES        = libshmemu.la
# libshmemu_la_SOURCES   = $(LIBSHMEMU_SOURCES)
# libshmemu_la_CFLAGS    = $(AM_CFLAGS)
# libshmemu_la_CPPFLAGS  = $(SHMEMX_CPPFLAGS) $(MY_INCLUDES)
# libshmemu_la_LDFLAGS   = -version-info 0:0:0

lib_LIBRARIES          = libshmemu.a
libshmemu_a_SOURCES    = $(LIBSHMEMU_SOURCES)
libshmemu_a_CPPFLAGS   = $(SHMEMX_CPPFLAGS) $(MY_INCLUDES)
