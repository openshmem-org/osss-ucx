shmemc_dir             = shmemc
shmemc_cppflags        = -I$(srcdir)/$(shmemc_dir)
shmemc_ldflags         = -L$(shmemc_dir)
shmemc_libs            = -lshmemc

SUBDIRS                = dlmalloc shmemc

LIBSHMEMI_SOURCES      = shmemi.c \
				pmi1-client.c pmi2-client.c pmix-client.c \
				malloc.c heapx.c pe.c logger.c

SHMEMX_CPPFLAGS        = -I$(srcdir)/../shmemu $(shmemc_cppflags)

lib_LTLIBRARIES        = libshmemi.la
libshmemi_la_SOURCES   = $(LIBSHMEMI_SOURCES)
libshmemi_la_CFLAGS    = $(AM_CFLAGS)
libshmemi_la_CPPFLAGS  = $(PMIX_CPPFLAGS) $(SHMEMX_CPPFLAGS)
libshmemi_la_LDFLAGS   = -version-info 0:0:0 $(PMIX_LDFLAGS)

lib_LIBRARIES          = libshmemi.a
libshmemi_a_SOURCES    = $(LIBSHMEMI_SOURCES)
libshmemi_a_CPPFLAGS   = $(PMIX_CPPFLAGS) $(SHMEMX_CPPFLAGS)
