#
# needs to be if'd for different malloc implementations
#

noinst_LIBRARIES       = libmalloc.a
libmalloc_a_SOURCES    = dlmalloc.c
libmalloc_a_CFLAGS     = -DONLY_MSPACES=1 -DHAVE_MORECORE=0 -DHAVE_MMAP=0 -DUSE_LOCKS=1

