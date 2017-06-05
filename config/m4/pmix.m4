#
# TODO: needs some work like for ucx/gasnet to allow location passed in too
#


AC_ARG_WITH([pmix],
	[AS_HELP_STRING([--with-pmix], [Use PMIx library])])
AS_IF([test "x$with_pmix" = "xyes"], [
	    	    PKG_CHECK_MODULES([PMIX], [pmix])
		    ]
	    )

AC_DEFINE_UNQUOTED([HAVE_PMIX], [1], [Using PMIx])

AC_DEFINE_UNQUOTED([PMIX_CFLAGS], ["$PMIX_CFLAGS"], [PMIx Compile Flags])
AC_DEFINE_UNQUOTED([PMIX_LIBS], ["$PMIX_LIBS"], [PMIx Compile Libraries])

AC_SUBST(PMIX_CFLAGS)
AC_SUBST(PMIX_LIBS)
