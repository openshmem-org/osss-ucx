#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([OSSS/SBU Reference Implementation],
	[1.0],
	[anthony.curtis@stonybrook.edu],
	[],
	[https://github.com/openshmem-org/])

config_flags="$*"
AC_DEFINE_UNQUOTED([CONFIG_FLAGS],["$config_flags"],[Flags passed to configure])

AC_CONFIG_SRCDIR([autogen.sh])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])
dnl AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([subdir-objects])

# Checks for programs.
AC_PROG_CC_C99
AC_PROG_CC_C_O
AC_PROG_CXX
AC_PROG_FC
AC_PROG_FC_C_O
AC_FC_LIBRARY_LDFLAGS
AC_FC_MAIN
AC_FC_WRAPPERS
AC_PROG_INSTALL

# Checks for libraries.
# for building shared libraries
LT_INIT

# package checks
AC_ARG_WITH([pmix],
		[AS_HELP_STRING([--with-pmix@<:@=DIR@:>@], [PMIx library location])],
		[with_pmix=$withval],
		[AC_MSG_ERROR([Cannot find PMIx])])
AS_IF([test -d $with_pmix],
	[
		PMIX_CPPFLAGS="-I$with_pmix/include"
		save_cppflags="$CPPFLAGS"
		CPPFLAGS="$CPPFLAGS $PMIX_CPPFLAGS"
		pmix_libdir="$with_pmix/lib"
		PMIX_LDFLAGS="-L$pmix_libdir -Wl,-rpath,$pmix_libdir -lpmix"
		AC_CHECK_HEADER([$with_pmix/include/pmix.h],
				[AC_DEFINE([HAVE_PMIX_H], [1])
				AC_SUBST(PMIX_CPPFLAGS)
				AC_SUBST(PMIX_LDFLAGS)],
				[AC_MSG_ERROR([Cannot find PMIx include file])]
				)
		CPPFLAGS="$save_cppflags"
	],
	[AC_MSG_ERROR([Cannot find PMIx])]
	)

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

AC_ARG_ENABLE([fortran],
	AS_HELP_STRING([--enable-fortran],
			[Enable Fortran API support @<:@default=no@:>@]))
AS_IF([test "x$enable_fortran" = "xyes"],
	[AC_DEFINE([ENABLE_FORTRAN], [1], [Enable Fortran API])
	 AC_SUBST([ENABLE_FORTRAN], [1])],
	[AC_SUBST([ENABLE_FORTRAN], [0])]
	)
AM_CONDITIONAL([ENABLE_FORTRAN], [test "x$enable_fortran" = "xyes"])

AC_ARG_ENABLE([cxx],
	AS_HELP_STRING([--enable-cxx],
			[Enable C++ API support @<:@default=no@:>@]))
AS_IF([test "x$enable_cxx" = "xyes"],
	[AC_DEFINE([ENABLE_CXX], [1], [Enable C++ API])
	 AC_SUBST([ENABLE_CXX], [1])],
	[AC_SUBST([ENABLE_CXX], [0])]
	)
AM_CONDITIONAL([ENABLE_CXX], [test "x$enable_cxx" = "xyes"])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h unistd.h string.h time.h sys/time.h sys/utsname.h sys/types.h fcntl.h stddef.h sys/param.h assert.h stdarg.h stdbool.h pmi.h pmi2.h pmix.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_MMAP
AC_CHECK_FUNCS([atexit gettimeofday gethostname uname getpagesize memset])

AM_INIT_AUTOMAKE

#
# the header file constants
#
AC_SUBST([SHMEM_MAJOR_VERSION], 1)
AC_SUBST([SHMEM_MINOR_VERSION], 3)
AC_SUBST([SHMEM_MAX_NAME_LEN],  64)
AC_SUBST([SHMEM_VENDOR_STRING], ['AC_PACKAGE_NAME'])

now="`date`"
AC_DEFINE_UNQUOTED([CONFIG_BUILD_DATE],["$now"],[When this package was compiled])
host="`hostname`"
AC_DEFINE_UNQUOTED([CONFIG_BUILD_HOST],["$host"],[Where this package was compiled])

AC_CONFIG_FILES([Makefile
		man/Makefile
		man/oshcc.1 man/oshcxx.1 man/oshfort.1
		man/oshrun.1 man/osh_info.1 man/osh_intro.1
		include/Makefile
		include/shmemx.h include/null-shmemx.h
		include/shmem_defines.h
		include/shmem_depr.h
		include/shmem_api.h
		include/shmem.h
		include/shmem_defines.fh
		src/Makefile
		src/shmemu/Makefile
		src/shmemc/Makefile
			src/shmemc/ucx/Makefile
			src/shmemc/gasnet/Makefile
		src/shmemi/Makefile
			src/shmemi/dlmalloc/Makefile
		src/version.h
		pkgconfig/Makefile
		pkgconfig/osss-openshmem.pc
		scripts/Makefile
		scripts/oshcc scripts/oshcxx scripts/oshfort scripts/oshrun
		])

AC_OUTPUT
