AC_ARG_WITH([pmix],
	[AS_HELP_STRING([--with-pmix], [Use PMIx library])])
AS_IF([test "x$with_pmix" = "xyes"], [
	    	    PKG_CHECK_MODULES([PMIX], [pmix])
		    ]
	    )
AC_DEFINE([HAVE_PMIX], [1], [Using PMIx])
AC_SUBST(PMIX_CFLAGS)
AC_SUBST(PMIX_LIBS)
