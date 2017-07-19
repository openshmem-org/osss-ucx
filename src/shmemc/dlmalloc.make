DLMALLOC_CPPFLAGS = \
			-DONLY_MSPACES=1 \
			-DHAVE_MORECORE=0 \
			-DHAVE_MMAP=0 \
			-DUSE_LOCKS=1

libshmemc_ucx_a-dlmalloc.o:	dlmalloc.c
	$(CC) $(DLMALLOC_CPPFLAGS) -c -o $@ $^
