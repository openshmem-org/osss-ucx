#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# For license: see LICENSE file at top-level

m4_define([pkg_name],       [OSSS/SBU Reference Implementation])
m4_define([pkg_major],      [1])
m4_define([pkg_minor],      [0])
m4_define([pkg_version],    [pkg_major.pkg_minor])
m4_define([pkg_bugreport],  [anthony.curtis@stonybrook.edu])
m4_define([pkg_url],        [https://bitbucket.org/sbuopenshmem/osss-ucx.git])

m4_define([spec_major_version],     [1])
m4_define([spec_minor_version],     [4])
m4_define([spec_max_name_len],      [64])
dnl quoted to make C string literal
m4_define([spec_vendor_string],     ["pkg_name"])

AC_PREREQ([2.69])
AC_INIT([pkg_name], [pkg_version], [pkg_bugreport], [],	[pkg_url])

config_flags="$*"
AC_DEFINE_UNQUOTED([CONFIG_FLAGS],
			["$config_flags"],
			[Flags passed to configure])
	
AC_SUBST([SHMEM_MAJOR_VERSION], [spec_major_version])
AC_SUBST([SHMEM_MINOR_VERSION], [spec_minor_version])
AC_SUBST([SHMEM_MAX_NAME_LEN],  [spec_max_name_len])
AC_SUBST([SHMEM_VENDOR_STRING], [spec_vendor_string])

AC_DEFINE_UNQUOTED([SHMEM_MAJOR_VERSION],
			[spec_major_version],
			[Major version of package])
AC_DEFINE_UNQUOTED([SHMEM_MINOR_VERSION],
			[spec_minor_version],
			[Minor version of package])
AC_DEFINE_UNQUOTED([SHMEM_MAX_NAME_LEN],
			[spec_max_name_len],
			[Maximum length of vendor string])
AC_DEFINE_UNQUOTED([SHMEM_VENDOR_STRING],
			[spec_vendor_string],
			[Vendor-supplied package name])

AC_CONFIG_SRCDIR([autogen.sh])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])
dnl AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([subdir-objects])

AM_SILENT_RULES([yes])

# C/C++
AC_PROG_CC_C99
AC_PROG_CC_C_O
AC_PROG_INSTALL
AM_PROG_CC_C_O

AC_PROG_CXX
AC_PROG_CXX_C_O
AC_PROG_CXXCPP

# Fortran-specific checks
AC_PROG_FC
AC_PROG_FC_C_O
AC_FC_LIBRARY_LDFLAGS
AC_FC_MAIN
AC_FC_DUMMY_MAIN
AC_FC_WRAPPERS

# Checks for libraries.
# for building shared libraries
LT_INIT

m4_include([config/m4/pmi.m4])
m4_include([config/m4/pmix.m4])
m4_include([config/m4/ucx.m4])

m4_include([config/m4/confargs.m4])

# static and shared library detection
AS_IF([test "x$enable_static" = "xyes"],
	    [AC_DEFINE([ENABLE_STATIC], [1], [Enable static libraries])]
	    )
AS_IF([test "x$enable_shared" = "xyes"],
	    [AC_DEFINE([ENABLE_SHARED], [1], [Enable shared libraries])]
	    )

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h unistd.h string.h time.h sys/time.h sys/utsname.h sys/types.h fcntl.h stddef.h sys/param.h assert.h stdarg.h stdbool.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

AC_CHECK_TYPES([int64_t])
AC_CHECK_TYPES([uint64_t])
AC_CHECK_TYPES([ptrdiff_t])

#
# Checks for library functions.
#

# AC_FUNC_MALLOC
# AC_FUNC_REALLOC
AC_FUNC_MMAP
AC_CHECK_FUNCS([atexit gettimeofday gethostname uname memset strlcat])
AC_CHECK_LIB([m], [log10])

AM_INIT_AUTOMAKE

now="`date`"
AC_DEFINE_UNQUOTED([CONFIG_BUILD_DATE],["$now"],[When this package was compiled])
host="`hostname`"
AC_DEFINE_UNQUOTED([CONFIG_BUILD_HOST],["$host"],[Where this package was compiled])

#
# check for pthreads and how to set it up
#
m4_include([config/m4/ax_pthread.m4])

AX_PTHREAD

AC_SUBST([PTHREAD_CC])
AC_SUBST([PTHREAD_CFLAGS])
AC_SUBST([PTHREAD_LIBS])

#
# Stub for doxygen
#

m4_include([config/m4/ax_prog_doxygen.m4])

DX_INIT_DOXYGEN([openshmem])

#
# figure out the heap size
#

if test "x$shmem_heap_size" = "x"
then
    shmem_heap_size=32M
fi

AC_SUBST([SHMEM_DEFAULT_HEAP_SIZE], [$shmem_heap_size])
dnl C string
AC_DEFINE_UNQUOTED([SHMEM_DEFAULT_HEAP_SIZE],
			["$shmem_heap_size"],
			[Default size of symmetric heap])

#
# all the output files
#

AC_CONFIG_FILES([Makefile])

AC_CONFIG_FILES([man/man1/Makefile
		 man/man1/oshcc.1 man/man1/oshcxx.1 man/man1/oshfort.1
		 man/man1/oshrun.1 man/man1/osh_info.1 man/man1/osh_intro.1
		])

AC_CONFIG_FILES([man/Makefile
		])

AC_CONFIG_FILES([include/Makefile
		 include/shmem/defs.h
		 include/shmem/depr.h
		 include/shmem/cache.h
		 include/shmem/api.h
		 include/shmem.h
		 include/pshmem.h include/pshmemx.h
		 include/shmemx.h include/null-shmemx.h
		])

AC_CONFIG_FILES([include/shmemx.fh include/null-shmemx.fh
		 include/shmem/defs.fh
		 include/shmem.fh include/pshmem.fh
		 include/pshmemx.fh
		])

AC_CONFIG_FILES([pkgconfig/Makefile
		 pkgconfig/osss-ucx.pc
		])

AC_CONFIG_FILES([src/Makefile
		])

AC_CONFIG_FILES([src/shmemc/Makefile
		 src/shmemc/heapsize.h
		 src/shmemc/oshcc src/shmemc/oshcxx src/shmemc/oshfort
		 src/shmemc/oshrun
               ])

AC_CONFIG_FILES([src/shmemu/Makefile
               ])

AC_OUTPUT
