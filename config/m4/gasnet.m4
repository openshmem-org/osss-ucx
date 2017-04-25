AC_ARG_WITH([gasnet],
	[AS_HELP_STRING([--with-gasnet], [Use GASNet library])])
AS_IF([test "x$with_gasnet" = "xyes"], [
	    		    PKG_CHECK_MODULES([GASNET], [gasnet], [], [])
		],
		[AC_MSG_ERROR([GASNet library required])]
		)
AC_DEFINE([HAVE_GASNET], [1], [Using GASNet])
AC_SUBST([GASNET_CFLAGS])
AC_SUBST([GASNET_LIBS])

AC_ARG_WITH([gasnet-conduit],
		[AS_HELP_STRING([--with-gasnet-conduit@<:@=NAME@:>@], [GASNet conduit choice])],
		[with_gasnet_conduit=$withval]
		)
AS_IF([test -d $with_gasnet_conduit],
	[
		GASENET_CONDUIT="$with_gasnet_conduit"
		AC_CHECK_HEADER([$with_gasnet/include/$with_gasnet_conduit-conduit/gasnet_core.h],
				[AC_DEFINE([GASNET_CONDUIT_H], [1])
				AC_SUBST(GASNET_CONDUIT)],
				[AC_MSG_ERROR([Cannot find GASNet conduit header file])]
				)
		CPPFLAGS="$save_cppflags"
	],
	[AC_MSG_ERROR([Cannot find GASNet conduit header file])]
	)
