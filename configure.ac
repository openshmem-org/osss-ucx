#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([shmem-library], [1.0], [anthony.curtis@stonybrook.edu])
AC_CONFIG_SRCDIR([heapx.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])

# Checks for programs.
AC_PROG_CC_C99
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
		AC_CHECK_HEADER([$with_pmix/include/pmix.h],
				[AC_SUBST(PMIX_CPPFLAGS)
				AC_SUBST(PMIX_LIBDIR, $with_pmix/lib)],
				[AC_MSG_ERROR([Cannot find PMIx include file])]
				)
		CPPFLAGS="$save_cppflags"
	],
	[AC_MSG_ERROR([Can't find PMIx])]
	)

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([atexit gettimeofday])

AM_INIT_AUTOMAKE

AC_CONFIG_FILES([Makefile shmem.h bin/Makefile bin/noshcc bin/noshrun])
AC_OUTPUT
