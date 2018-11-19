
# For license: see LICENSE file at top-level

AC_MSG_NOTICE([Checking for how we handle collectives])

AC_ARG_WITH([shcoll],
            [AS_HELP_STRING([--with-shcoll@<:@=DIR@:>@],
	    [Use SHCOLL library from DIR, or "internal" to use bundled version [default: internal]])])

SHCOLL_TYPE="internal"
# don't evaluate top_srcdir until we're compiling
SHCOLL_DIR='$(top_srcdir)/src/collectives/shcoll/src'

AS_IF([test "x$with_shcoll" != "x" -a "x$with_shcoll" != "xinternal"],
      [
          AC_MSG_NOTICE([Checking for external SHCOLL])
          shcoll_hdr="$with_shcoll/include/shcoll.h"
          AS_IF([test -r "$shcoll_hdr"],
          [
	      SHCOLL_TYPE="external"
              SHCOLL_DIR="$with_shcoll"
              AC_DEFINE([HAVE_SHCOLL_EXTERNAL], [1], [External SHCOLL support])
	      AC_SUBST([HAVE_SHCOLL_EXTERNAL], [1])
	      AS_BOX([Using SHCOLL in $with_shcoll])
          ],
          [
            AC_MSG_ERROR([Unable to find SHCOLL header file in $shcoll_hdr])
          ]
          )
      ],
      [
          AC_DEFINE([HAVE_SHCOLL_INTERNAL], [1], [Internal SHCOLL support])
	  AC_SUBST([HAVE_SHCOLL_INTERNAL], [1])
	  AS_BOX([Using $SHCOLL_TYPE SHCOLL])
      ]
)

#
# make sure we can find shcoll, wherever it is
#
SHCOLL_LIBS=""
AS_IF([test "$SHCOLL_TYPE" = "external"],
      [
          SHCOLL_LIBS="$SHCOLL_LIBS -L$with_shcoll/lib64"
          SHCOLL_LIBS="$SHCOLL_LIBS -Wl,-rpath -Wl,$with_shcoll/lib64"
          SHCOLL_LIBS="$SHCOLL_LIBS -L$with_shcoll/lib"
          SHCOLL_LIBS="$SHCOLL_LIBS -Wl,-rpath -Wl,$with_shcoll/lib"
      ]
      )
SHCOLL_LIBS="$SHCOLL_LIBS -lshcoll"
AC_SUBST([SHCOLL_LIBS])

AC_DEFINE([HAVE_SHCOLL], [1], [SHCOLL support])

AC_DEFINE_UNQUOTED([SHCOLL_DIR],
		   ["$SHCOLL_DIR"],
		   [SHCOLL installation directory])

AC_SUBST([SHCOLL_DIR])
AC_SUBST([SHCOLL_TYPE])

#
# need to compile differently if we have our own shcoll
#
AM_CONDITIONAL([HAVE_SHCOLL_INTERNAL], [test "$SHCOLL_TYPE" = "internal"])
