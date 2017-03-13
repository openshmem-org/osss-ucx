#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([shmem-library], [1.3], [anthony.curtis@stonybrook.edu])
AC_CONFIG_SRCDIR([autogen.sh])
AC_CONFIG_HEADERS([config.h])
dnl AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC_C99
AM_PROG_CC_C_O
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
	[AC_SUBST([ENABLE_EXPERIMENTAL], [1])],
	[AC_SUBST([ENABLE_EXPERIMENTAL], [0])]
	)

AM_CONDITIONAL([ENABLE_EXPERIMENTAL], [test "x$enable_experimental" = "xyes"])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h sys/time.h sys/types.h assert.h stdarg.h stdbool.h pmi.h pmi2.h pmix.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([atexit gettimeofday])

AM_INIT_AUTOMAKE

AC_SUBST(SHMEM_MAJOR_VERSION, 1)
AC_SUBST(SHMEM_MINOR_VERSION, 3)
AC_SUBST(SHMEM_MAX_NAME_LEN, 64)
AC_SUBST(SHMEM_VENDOR_STRING, '"OSSS/SBU Reference Implementation"')

AC_CONFIG_FILES([Makefile
		src/Makefile src/shmemi/Makefile
		src/shmem.h src/shmemx.h src/version.h
		scripts/Makefile scripts/noshcc scripts/noshrun
		src/shmemi/dlmalloc/Makefile])

AC_OUTPUT
