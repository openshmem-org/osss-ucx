# For license: see LICENSE file at top-level

# Feature checks
AC_ARG_ENABLE([debug],
	AS_HELP_STRING([--enable-debug], [Enable library debug outptut @<:@default=no@:>@]),
	AS_IF([test "x$enableval" = "xyes"],
		    AC_DEFINE([ENABLE_DEBUG], [1], [Enable debug])
		    ),
	[])
AM_CONDITIONAL([ENABLE_DEBUG], [test "x$enable_debug" = "xyes"])

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
			[Enable deprecated Fortran API support @<:@default=no@:>@]))
AS_IF([test "x$enable_fortran" = "xyes"],
	[AC_DEFINE([ENABLE_FORTRAN], [1], [Enable Fortran API])
	 AC_SUBST([ENABLE_FORTRAN], [1])],
	[AC_SUBST([ENABLE_FORTRAN], [0])]
	)
AM_CONDITIONAL([ENABLE_FORTRAN], [test "x$enable_fortran" = "xyes"])

#
# address translation
#
AC_ARG_ENABLE([fixed-addresses],
	AS_HELP_STRING([--enable-fixed-addresses],
			[Symmetric addresses are identical everywhere @<:@default=no@:>@]))
AS_IF([test "x$enable_fixed_addresses" = "xyes"],
	[AC_DEFINE([ENABLE_FIXED_ADDRESSES], [1], [Enable fixed symmetric addressesI])
	 AC_SUBST([ENABLE_FIXED_ADDRESSES], [1])],
	[AC_SUBST([ENABLE_FIXED_ADDRESSES], [0])]
	)
AM_CONDITIONAL([ENABLE_FIXED_ADDRESSES], [test "x$enable_fixed_addresses" = "xyes"])

#
# enabled by default
#
AC_ARG_ENABLE([cxx],
	AS_HELP_STRING([--disable-cxx],
			[Support C++ compiler? @<:@default=yes@:>@]))
AS_IF([test "x$enable_cxx" != "xno"],
	[AC_DEFINE([ENABLE_CXX], [1], [Enable C++ compiler])
	 AC_SUBST([ENABLE_CXX], [1])],
	[AC_SUBST([ENABLE_CXX], [0])]
	)
AM_CONDITIONAL([ENABLE_CXX], [test "x$enable_cxx" != "xno"])
