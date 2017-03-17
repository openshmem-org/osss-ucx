#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([OSSS/SBU Reference Implementation],
	[1.0],
	[anthony.curtis@stonybrook.edu],
	[],
	[https://github.com/openshmem-org/])
AC_CONFIG_SRCDIR([autogen.sh])
AC_CONFIG_HEADERS([config.h])
dnl AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC_C99
AC_PROG_CC_C_O
AC_PROG_CXX
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
				[AC_DEFINE([HAVE_PMIX_H], [1])
				AC_SUBST(PMIX_CPPFLAGS)
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
AC_ARG_ENABLE([pshmem],
	AS_HELP_STRING([--enable-pshmem],
			[Enable Profiling interface @<:@default=no@:>@]))
AS_IF([test "x$enable_pshmem" = "xyes"],
	[AC_DEFINE([ENABLE_PSHMEM], [1], [Enable profiling interface])
	 AC_SUBST([ENABLE_PSHMEM], [1])],
	[AC_SUBST([ENABLE_PSHMEM], [0])]
	)

AM_CONDITIONAL([ENABLE_EXPERIMENTAL], [test "x$enable_experimental" = "xyes"])
AM_CONDITIONAL([ENABLE_PSHMEM], [test "x$enable_pshmem" = "xyes"])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h unistd.h string.h time.h sys/time.h  sys/types.h fcntl.h stddef.h sys/param.h assert.h stdarg.h stdbool.h pmi.h pmi2.h pmix.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_MMAP
AC_CHECK_FUNCS([atexit gettimeofday gethostname getpagesize memset])

AM_INIT_AUTOMAKE

#
# the header file constants
#
AC_SUBST([SHMEM_MAJOR_VERSION], 1)
AC_SUBST([SHMEM_MINOR_VERSION], 3)
AC_SUBST([SHMEM_MAX_NAME_LEN],  64)
AC_SUBST([SHMEM_VENDOR_STRING], ['AC_PACKAGE_NAME'])

AC_CONFIG_FILES([Makefile
		src/shmemi/dlmalloc/Makefile
		man/oshcc.1 man/oshcxx.1 man/oshrun.1
		src/Makefile src/shmemi/Makefile
		scripts/Makefile
		man/Makefile
		scripts/oshcc scripts/oshcxx scripts/oshrun
		src/shmem.h src/shmemx.h src/version.h
		])

AC_OUTPUT
