#
# TODO make this optional because we might choose to use GASNet
# instead.  But we *will* need one of them.
#
AC_ARG_WITH([ucx],
	[AS_HELP_STRING([--with-ucx], [Use UCX library])])
AS_IF([test "x$with_ucx" = "xyes"], [
	    		 PKG_CHECK_MODULES([UCX], [ucx])
	    ],
	    [AC_MSG_ERROR([UCX library required])]
	    )
AC_DEFINE([HAVE_UCX], [1], [Using UCX])
AC_SUBST([UCX_CFLAGS])
AC_SUBST([UCX_LIBS])
