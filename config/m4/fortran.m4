# For license: see LICENSE file at top-level

fortran_happy=no

AC_PROG_FC

AC_ARG_ENABLE([fortran],
	AS_HELP_STRING([--enable-fortran],
			[Enable deprecated Fortran API support @<:@default=no@:>@]))

if test "x$enable_fortran" = "xyes"
then
    if test "x$FC" != "x"
    then
      AC_PROG_FC_C_O
      AC_FC_LIBRARY_LDFLAGS
      AC_FC_WRAPPERS
      fortran_happy=yes
    fi
fi

AS_IF([test "x$fortran_happy" = "xyes"],
	[AC_DEFINE([ENABLE_FORTRAN], [1], [Enable Fortran API])
	 AC_SUBST([ENABLE_FORTRAN], [1])
	],
	[AC_SUBST([ENABLE_FORTRAN], [0])]
	)

AM_CONDITIONAL([ENABLE_FORTRAN], [test "x$fortran_happy" = "xyes"])
