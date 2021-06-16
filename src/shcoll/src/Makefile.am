# For license: see LICENSE file at top-level

SOURCES                 = alltoall.c \
				alltoalls.c \
				barrier.c \
				broadcast.c \
				collect.c \
				fcollect.c \
				reduction.c
SOURCES                += util/bithacks.c \
				util/broadcast-size.c \
				util/rotate.c \
				util/scan.c \
				util/trees.c

FIND_SHMEM_H            = -I$(top_srcdir)/include \
				-I../../../include

# disable warnings here (should detect how to do this)
# BUILD_CFLAGS            = -w
BUILD_CFLAGS            =
BUILD_CFLAGS           += -I.. $(FIND_SHMEM_H)

lib_LTLIBRARIES         = libshcoll.la
libshcoll_la_SOURCES    = $(SOURCES)
libshcoll_la_CFLAGS     = $(BUILD_CFLAGS)

nobase_include_HEADERS  = shcoll.h \
				shcoll/alltoall.h \
				shcoll/alltoalls.h \
				shcoll/barrier.h \
				shcoll/broadcast.h \
				shcoll/collect.h \
				shcoll/common.h \
				shcoll/fcollect.h \
				shcoll/reduction.h

EXTRA_DIST              = shcoll/compat.h
