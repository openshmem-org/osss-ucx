# For license: see LICENSE file at top-level

AC_ARG_WITH([shcoll],
            [AS_HELP_STRING([--with-shcoll@<:@=DIR@:>@], [Use SHCOLL library])])

shcoll_type="external"

AS_IF([test "x$with_shcoll" != "x" -a "x$with_shcoll" != "xinternal"],
      [
          AC_MSG_NOTICE([Checking for external SHCOLL])
          shcoll_hdr="$with_shcoll/include/shcoll.h"
          AS_IF([test -r "$shcoll_hdr"],
          [
              SHCOLL_DIR="$with_shcoll"
              AC_DEFINE_UNQUOTED([SHCOLL_DIR],
				 ["$SHCOLL_DIR"],
				 [SHCOLL installation directory])
              AC_DEFINE([HAVE_SHCOLL_EXTERNAL], [1], [External SHCOLL support])
	      AC_SUBST([HAVE_SHCOLL_EXTERNAL], [1])
	      SHCOLL_DIR="$with_shcoll"
	      AS_BOX([Using SHCOLL in $with_shcoll])
          ],
          [
            AC_MSG_ERROR([Unable to find SHCOLL header file in $shcoll_hdr])
          ]
          )
      ],
      [
          AC_DEFINE([HAVE_SHCOLL_INTERNAL], [1], [Internal SHCOLL support])
	  shcoll_type="internal"
	  SHCOLL_DIR="$srcdir/src/collectives/shcoll/src"
	  AC_SUBST([HAVE_SHCOLL_INTERNAL], [1])
	  AS_BOX([Using $shcoll_type SHCOLL])
      ]
      )

SHCOLL_LIBS="-L$with_shcoll/lib64 -Wl,-rpath -Wl,$with_shcoll/lib64"
SHCOLL_LIBS="$SHCOLL_LIBS -L$with_shcoll/lib -Wl,-rpath -Wl,$with_shcoll/lib"
SHCOLL_LIBS="$SHCOLL_LIBS -lshcoll"

AC_DEFINE([HAVE_SHCOLL], [1], [SHCOLL support])
AC_SUBST([SHCOLL_DIR])
AC_SUBST([SHCOLL_LIBS])

AM_CONDITIONAL([HAVE_SHCOLL_INTERNAL], [test "$shcoll_type" = "internal"])
