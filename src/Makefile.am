lib_LTLIBRARIES = libshmem.la
libshmem_la_SOURCES = heapx.c init.c logger.c malloc.c pe.c shmemi_pmi1.c shmemi_pmi2.c shmemi_pmix.c timer.c
libshmem_la_LDFLAGS = -version-info 0:0:0
libshmem_la_CFLAGS = $(AM_CFLAGS)
libshmem_la_CPPFLAGS = $(PMIX_CPPFLAGS)

lib_LIBRARIES = libshmem.a
libshmem_a_SOURCES = heapx.c init.c logger.c malloc.c pe.c shmemi_pmi1.c shmemi_pmi2.c shmemi_pmix.c timer.c
libshmem_a_CPPFLAGS = $(PMIX_CPPFLAGS)

include_HEADERS = shmem.h
