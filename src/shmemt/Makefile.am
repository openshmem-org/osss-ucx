# For license: see LICENSE file at top-level

MY_INCLUDE             = -I../../include \
				-I$(top_srcdir)/include

MY_SOURCES             = threading.c

if ENABLE_THREADS
MY_INCLUDE            += -I$(top_srcdir)/src/shmemc
MY_SOURCES            += shmem_mutex.c
endif # threads

lib_LTLIBRARIES        = libshmemt.la
libshmemt_la_SOURCES   = $(MY_SOURCES)
libshmemt_la_CFLAGS    = $(AM_CFLAGS)
libshmemt_la_CPPFLAGS  = $(MY_INCLUDE)
libshmemt_la_LDFLAGS   = -version-info 0:0:0

# lib_LIBRARIES          = libshmemt.a
# libshmemt_a_SOURCES    = $(MY_SOURCES)
# libshmemt_a_CPPFLAGS   = $(MY_INCLUDE)
