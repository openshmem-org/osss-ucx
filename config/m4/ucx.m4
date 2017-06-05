#
# TODO make this optional because we might choose to use GASNet
# instead.  But we *will* need one of them.
#

ucx_happy=no

AC_ARG_WITH([ucx], AS_HELP_STRING([--with-ucx], [Use UCX library])])

AS_IF([test -d "$with_ucx"],
      [
          AS_IF([test -r "$with_ucx/include/ucp/api/ucp.h"],
          [
	      CPPFLAGS="-I$with_ucx/include $CPPFLAGS"
	      LDFLAGS="-L$with_ucx/lib -lucp $LDFLAGS"
	      AC_DEFINE([HAVE_UCX], [1], [UCX support])
	      UCX_DIR="$with_ucx"
	      AC_DEFINE_UNQUOTED([UCX_DIR], ["$UCX_DIR"], [UCX installation directory])
	      AC_SUBST([UCX_DIR])
	      ucx_happy=yes
	      AC_MSG_NOTICE([Selecting UCX])
          ]
          )
      ]
      )
      
AM_CONDITIONAL([HAVE_UCX], [test "x$ucx_happy" != xno])
