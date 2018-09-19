# For license: see LICENSE file at top-level

AC_ARG_ENABLE([progress],
	AS_HELP_STRING([--enable-progress],
			[Run a progress thread? @<:@default=no@:>@]))

AS_IF([test "x$enable_progress" = "xyes"],
	[AC_DEFINE([ENABLE_PROGRESS], [1], [Enable progress thread])
	 AC_SUBST([ENABLE_PROGRESS], [1]),
	 AC_MSG_NOTICE([Enabling progress thread])
	 ]
	 )

AM_CONDITIONAL([ENABLE_PROGRESS], [test "x$enable_progress" = "xyes"])
