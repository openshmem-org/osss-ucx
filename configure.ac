#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# For license: see LICENSE file at top-level

m4_define([pkg_name],       [osss-ucx])
m4_define([pkg_major],      [1])
m4_define([pkg_minor],      [0])
m4_define([pkg_version],    [pkg_major.pkg_minor])
m4_define([pkg_bugreport],  [anthony.curtis@stonybrook.edu])
m4_define([pkg_url],        [https://bitbucket.org/sbuopenshmem/osss-ucx.git])

m4_define([spec_major_version],     [1])
m4_define([spec_minor_version],     [4])
m4_define([spec_max_name_len],      [64])
# quoted to make C string literal
m4_define([spec_vendor_string],     ["pkg_name"])

# unless overridden by user
m4_define([shmem_default_heap_size], ["32M"])

shmem_heap_size=""

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
# AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_MACRO_DIR([m4])
# for multiple levels of autoconf packages (embedding)
# AC_CONFIG_AUX_DIR([.])
# AC_CONFIG_SUBDIRS([src/shcoll])
AM_INIT_AUTOMAKE([subdir-objects tar-pax])

AM_SILENT_RULES([yes])

AC_PROG_LN_S

# C
AC_PROG_CC_C99
AC_PROG_CC_C_O
AC_PROG_INSTALL
AM_PROG_CC_C_O

# Checks for libraries.
# for building shared libraries
LT_INIT

m4_include([config/m4/pmi.m4])
m4_include([config/m4/pmix.m4])
m4_include([config/m4/ucx.m4])
m4_include([config/m4/shcoll.m4])

m4_include([config/m4/confargs.m4])
# m4_include([config/m4/progress.m4])

m4_include([config/m4/ax_gcc_func_attribute.m4])
m4_include([config/m4/attribute_checks.m4])

# c++ -specific checks
m4_include([config/m4/cxx.m4])

# Fortran-specific checks
m4_include([config/m4/fortran.m4])

# static and shared library detection
AS_IF([test "x$enable_static" = "xyes"],
	    [AC_DEFINE([ENABLE_STATIC], [1], [Enable static libraries])]
	    )
AS_IF([test "x$enable_shared" = "xyes"],
	    [AC_DEFINE([ENABLE_SHARED], [1], [Enable shared libraries])]
	    )

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h unistd.h string.h time.h sys/time.h sys/utsname.h sys/types.h fcntl.h stddef.h sys/param.h assert.h stdarg.h stdbool.h sched.h])

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
AC_CHECK_FUNCS([atexit _exit exit gettimeofday gethostname uname memset strlcat strlcpy sched_yield nanosleep])
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
# see if compiler knows about builtin_expect
#
m4_include([config/m4/ax_gcc_builtin.m4])
AX_GCC_BUILTIN(__builtin_expect)

#
# Stub for doxygen
#
m4_include([config/m4/ax_prog_doxygen.m4])

DX_INIT_DOXYGEN([openshmem])

#
# figure out the heap size
#

AS_IF([test -z "$shmem_heap_size"],
      [shmem_heap_size=m4_expand([shmem_default_heap_size])]
      )

# for the oshrun man page
AC_SUBST([SHMEM_DEFAULT_HEAP_SIZE], [$shmem_heap_size])
# C string
AC_DEFINE_UNQUOTED([SHMEM_DEFAULT_HEAP_SIZE],
			["$shmem_heap_size"],
			[Default size of symmetric heap])

#
# all the output files
#

AC_CONFIG_FILES([
	Makefile
	man/Makefile
	man/man1/Makefile
	man/man1/oshcc.1 man/man1/oshcxx.1 man/man1/oshfort.1
	man/man1/oshrun.1 man/man1/osh_info.1 man/man1/osh_intro.1
	include/Makefile
	include/shmem/defs.h
	include/shmem/defs.fh
	pkgconfig/Makefile
	pkgconfig/osss-ucx.pc
	src/Makefile
	src/osh_info/Makefile
	src/collectives/shcoll/Makefile
	src/collectives/shcoll/src/Makefile
	src/shmemc/Makefile
	src/shmemc/osh_common
	src/shmemu/Makefile
        ])

AC_OUTPUT

#
# Courtesy info
#

echo
echo "Summary"
echo "-------"
echo
echo "                  UCX: version $UCX_VERSION_STRING in $UCX_DIR"
echo "                 PMIx: version $PMIX_VERSION_STRING in $PMIX_DIR"

case "$shcoll_happy" in
    no)
        ;;
    *)
        echo "               SHCOLL: in $SHCOLL_DIR"
	;;
esac
case "$SHMEM_LAUNCHER" in
    "")
        ;;
    *)
        echo "             Launcher: $SHMEM_LAUNCHER"
	;;
esac

echo "    Default heap size: $shmem_heap_size"
echo
echo
