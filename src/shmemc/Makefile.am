SUBDIRS                = gasnet

LIBSHMEMC_SOURCES      = put.c get.c lock.c getenv.c

lib_LTLIBRARIES        = libshmemc.la
libshmemc_la_SOURCES   = $(LIBSHMEMC_SOURCES)
libshmemc_la_CFLAGS    = $(AM_CFLAGS)
libshmemc_la_LDFLAGS   = -version-info 0:0:0

lib_LIBRARIES          = libshmemc.a
libshmemc_a_SOURCES    = $(LIBSHMEMC_SOURCES)
