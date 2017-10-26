# For license: see LICENSE file at top-level

OTHER_CPPFLAGS            = -I../../include -I$(srcdir)/../shmemc
LIBSHMEMU_SOURCES         = hostname.c logger.c timer.c unitparse.c \
				shmemu-init.c

lib_LIBRARIES             = libshmemu.a
libshmemu_a_SOURCES       = $(LIBSHMEMU_SOURCES)
libshmemu_a_CFLAGS        = $(OTHER_CPPFLAGS)
