AC_ARG_WITH([gasnet], [AS_HELP_STRING([--with-gasnet@<:@=DIR@:>@], [Use GASNet library])])

gasnet_selected=no

AS_IF([test "x$with_gasnet" != "xno"], [
    AS_IF([test ! -d "$with_gasnet"], [
        PKG_CHECK_MODULES([GASNET], [gasnet], [
            AC_DEFINE([HAVE_GASNET], [1], [Using GASNet])
	    gasnet_selected=yes
	],
	[]
	)
    ])
    ]
)

AC_ARG_WITH([gasnet-conduit],
    [AS_HELP_STRING([--with-gasnet-conduit=NAME],
                    [GASNet conduit choice (if not auto-detected)])],
                    []
)

conduit_dir=$with_gasnet/include/${with_gasnet_conduit}-conduit
threnv=par

AS_IF([test -d $conduit_dir], [
    gasnet_h_dir=$with_gasnet/include
    save_CPPFLAGS="$CPPFLAGS"
    CPPFLAGS="-I$gasnet_h_dir $CPPFLAGS"
    GASNET_CONDUIT="$with_gasnet_conduit"
    GASNET_DIR="$with_gasnet"
    GASNET_MAKE_FRAGMENT="$conduit_dir/$with_gasnet_conduit-$threnv.mak"
    AS_IF([test -f $gasnet_h_dir/gasnet.h],
        [AC_DEFINE_UNQUOTED([GASNET_DIR],
	                    ["$GASNET_DIR"],
			    [GASNet installation directory])
         AC_DEFINE_UNQUOTED([GASNET_CONDUIT],
	                    ["$with_gasnet_conduit"],
			    [GASNet conduit in use])
         AC_SUBST(GASNET_MAKE_FRAGMENT)
         AC_SUBST(GASNET_CONDUIT)
         AC_SUBST(GASNET_DIR)
	 AC_DEFINE([HAVE_GASNET], [1], [Using GASNet])
        ],
        [AC_MSG_ERROR([Cannot find GASNet conduit header file])]
    )
    CPPFLAGS="$save_CPPFLAGS"
    ],
    [AC_MSG_ERROR([Cannot find GASNet conduit header file])]
)
