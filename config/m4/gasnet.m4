# For license: see LICENSE file at top-level

gasnet_happy=no

AC_ARG_WITH([gasnet], [AS_HELP_STRING([--with-gasnet@<:@=DIR@:>@], [Use GASNet library])])

AS_IF([test -d "$with_gasnet"], [
    AC_ARG_WITH([gasnet-conduit],
                [AS_HELP_STRING([--with-gasnet-conduit=NAME],
                                [GASNet conduit choice (if not auto-detected)])],
		[],
		[AC_MSG_ERROR([Must specify GASNet conduit name])])

    conduit_dir="$with_gasnet/include/${with_gasnet_conduit}-conduit"
    threnv=par
    AS_IF([test -d "$conduit_dir"], [
        gasnet_h_dir=$with_gasnet/include
    	GASNET_CONDUIT="$with_gasnet_conduit"
    	GASNET_DIR="$with_gasnet"
    	GASNET_MAKE_FRAGMENT="$conduit_dir/$with_gasnet_conduit-$threnv.mak"
    	AS_IF([test -r "$gasnet_h_dir/gasnet.h"],
            [gasnet_happy=yes
	     AC_MSG_NOTICE([Selecting GASNet as communications layer])
	     AC_DEFINE_UNQUOTED([GASNET_DIR], ["$GASNET_DIR"], [GASNet installation directory])
             AC_DEFINE_UNQUOTED([GASNET_CONDUIT], ["$with_gasnet_conduit"], [GASNet conduit in use])
             AC_SUBST(GASNET_MAKE_FRAGMENT)
             AC_SUBST(GASNET_CONDUIT)
             AC_SUBST(GASNET_DIR)
	     AC_DEFINE([HAVE_GASNET], [1], [Using GASNet])
            ],
            [AC_MSG_ERROR([Cannot find GASNet conduit header file])]
            )
        ],
    [AC_MSG_ERROR([Cannot find GASNet conduit directory])]
    )
    ]
)

AM_CONDITIONAL([HAVE_GASNET], [test "x$gasnet_happy" != xno])
