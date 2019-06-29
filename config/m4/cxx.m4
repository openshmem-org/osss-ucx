# For license: see LICENSE file at top-level

cxx_happy=no

# C++
AC_PROG_CXX

if test "x$CXX" != "x"
then
    AC_PROG_CXX_C_O
    AC_PROG_CXXCPP

    cxx_happy=yes
fi

AC_ARG_ENABLE([cxx],
	AS_HELP_STRING([--disable-cxx],
			[Provide OpenSHMEM C++ compiler wrapper? @<:@default=yes@:>@]))

AS_IF([test "x$cxx_happy" = "xyes" -a "x$enable_cxx" != "xno"],
	[AC_DEFINE([ENABLE_CXX], [1], [Enable C++ compiler])
	 AC_SUBST([ENABLE_CXX], [1])
	 AC_SUBST([CXX], $CXX)],
	[AC_SUBST([ENABLE_CXX], [0])
	 cxx_happy=no
	]
	)

AM_CONDITIONAL([ENABLE_CXX], [test "x$cxx_happy" = "xyes"])
