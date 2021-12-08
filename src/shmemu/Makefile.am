# For license: see LICENSE file at top-level

OTHER_CPPFLAGS            = 	-I../../include \
				-I$(top_srcdir)/include \
				-I$(srcdir)/../shmemc \
				-I$(srcdir)/../shmemt \
				-I$(srcdir)/../api/allocator

LIBSHMEMU_SOURCES         =	init.c \
				carp.c \
				memcheck.c \
				parse_csv.c \
				plural.c \
				progress.c \
				threadlevels.c \
				timer.c \
				unitparse.c

if ENABLE_ALIGNED_ADDRESSES
LIBSHMEMU_SOURCES        += asr.c
endif

if ENABLE_LOGGING
LIBSHMEMU_SOURCES        += deprecate.c logger.c
endif

lib_LTLIBRARIES           = libshmemu.la
libshmemu_la_SOURCES      = $(LIBSHMEMU_SOURCES)
libshmemu_la_CFLAGS       = $(OTHER_CPPFLAGS)

# lib_LIBRARIES             = libshmemu.a
# libshmemu_a_SOURCES       = $(LIBSHMEMU_SOURCES)
# libshmemu_a_CFLAGS        = $(OTHER_CPPFLAGS)
