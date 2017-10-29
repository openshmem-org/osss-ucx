#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# For license: see LICENSE file at top-level

AC_PREREQ([2.69])
AC_INIT([OSSS/SBU Reference Implementation],
	[1.0],
	[anthony.curtis@stonybrook.edu],
	[],
	[https://github.com/openshmem-org/])
AC_SUBST([SHMEM_MAJOR_VERSION], 1)
AC_SUBST([SHMEM_MINOR_VERSION], 4)
AC_SUBST([SHMEM_MAX_NAME_LEN],  64)
AC_SUBST([SHMEM_VENDOR_STRING], ['AC_PACKAGE_NAME'])

config_flags="$*"
AC_DEFINE_UNQUOTED([CONFIG_FLAGS],["$config_flags"],[Flags passed to configure])

AC_CONFIG_SRCDIR([autogen.sh])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])
dnl AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([subdir-objects])

AM_SILENT_RULES([yes])

# C/C++
AC_PROG_CC_C99
AC_PROG_CC_C_O
AC_PROG_CXX
AC_PROG_CXX_C_O
AC_PROG_CXXCPP
AC_PROG_INSTALL

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

m4_include([config/m4/pmix.m4])
m4_include([config/m4/ucx.m4])

# Feature checks
AC_ARG_ENABLE([debug],
	AS_HELP_STRING([--enable-debug], [Enable library debug @<:@default=no@:>@]),
	AS_IF([test "x$enableval" = "xyes"],
		    AC_DEFINE([ENABLE_DEBUG], [1], [Enable debug])
		    ),
	[])
AC_ARG_ENABLE([experimental],
	AS_HELP_STRING([--enable-experimental],
			[Enable non-standard extensions @<:@default=no@:>@]))
AS_IF([test "x$enable_experimental" = "xyes"],
	[AC_DEFINE([ENABLE_EXPERIMENTAL], [1], [Enable non-standard extensions])
	AC_SUBST([ENABLE_EXPERIMENTAL], [1])],
	[AC_SUBST([ENABLE_EXPERIMENTAL], [0])]
	)
AM_CONDITIONAL([ENABLE_EXPERIMENTAL], [test "x$enable_experimental" = "xyes"])

AC_ARG_ENABLE([pshmem],
	AS_HELP_STRING([--enable-pshmem],
			[Enable Profiling interface @<:@default=no@:>@]))
AS_IF([test "x$enable_pshmem" = "xyes"],
	[AC_DEFINE([ENABLE_PSHMEM], [1], [Enable profiling interface])
	 AC_SUBST([ENABLE_PSHMEM], [1])],
	[AC_SUBST([ENABLE_PSHMEM], [0])]
	)
AM_CONDITIONAL([ENABLE_PSHMEM], [test "x$enable_pshmem" = "xyes"])

#
# disabled by default
#
AC_ARG_ENABLE([fortran],
	AS_HELP_STRING([--enable-fortran],
			[Enable Fortran API support @<:@default=no@:>@]))
AS_IF([test "x$enable_fortran" = "xyes"],
	[AC_DEFINE([ENABLE_FORTRAN], [1], [Enable Fortran API])
	 AC_SUBST([ENABLE_FORTRAN], [1])],
	[AC_SUBST([ENABLE_FORTRAN], [0])]
	)
AM_CONDITIONAL([ENABLE_FORTRAN], [test "x$enable_fortran" = "xyes"])

#
# enabled by default
#
AC_ARG_ENABLE([cxx],
	AS_HELP_STRING([--disable-cxx],
			[Disable C++ compiler support @<:@default=yes@:>@]))
AS_IF([test "x$enable_cxx" != "xno"],
	[AC_DEFINE([ENABLE_CXX], [1], [Enable C++ compiler])
	 AC_SUBST([ENABLE_CXX], [1])],
	[AC_SUBST([ENABLE_CXX], [0])]
	)
AM_CONDITIONAL([ENABLE_CXX], [test "x$enable_cxx" != "xno"])

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

AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
# AC_FUNC_MALLOC
# AC_FUNC_REALLOC
AC_FUNC_MMAP
AC_CHECK_FUNCS([atexit gettimeofday gethostname uname getpagesize memset])

AM_INIT_AUTOMAKE

now="`date`"
AC_DEFINE_UNQUOTED([CONFIG_BUILD_DATE],["$now"],[When this package was compiled])
host="`hostname`"
AC_DEFINE_UNQUOTED([CONFIG_BUILD_HOST],["$host"],[Where this package was compiled])

AC_CONFIG_FILES([Makefile])

AC_CONFIG_FILES([man/Makefile
		 man/oshcc.1 man/oshcxx.1 man/oshfort.1
		 man/oshrun.1 man/osh_info.1 man/osh_intro.1
		])

AC_CONFIG_FILES([include/Makefile
		 include/shmem/defs.h
		 include/shmem/depr.h
		 include/shmem/api.h
		 include/shmem.h
		 include/pshmem.h include/pshmemx.h
		 include/shmemx.h include/null-shmemx.h
		 include/shmem/defs.fh
		 include/shmem.fh include/pshmem.fh
		 include/shmemx.fh include/pshmemx.fh
		])

AC_CONFIG_FILES([pkgconfig/Makefile
		 pkgconfig/osss-ucx.pc
		])

AC_CONFIG_FILES([src/Makefile
		 src/version.h
		])

AC_CONFIG_FILES([src/shmemc/Makefile
		 src/shmemc/oshcc src/shmemc/oshcxx src/shmemc/oshfort
		 src/shmemc/oshrun
               ])

AC_CONFIG_FILES([src/shmemu/Makefile
               ])

AC_OUTPUT
