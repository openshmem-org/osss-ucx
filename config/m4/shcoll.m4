# For license: see LICENSE file at top-level

shcoll_happy=no

AC_ARG_WITH([shcoll],
            [AS_HELP_STRING([--with-shcoll@<:@=DIR@:>@], [Use SHCOLL library])])

AS_IF([test "x$with_shcoll" != "x"],
      [
          AC_MSG_NOTICE([Checking for SHCOLL])
          shcoll_hdr="$with_shcoll/include/shcoll.h"
          AS_IF([test -r "$shcoll_hdr"],
          [
	      SHCOLL_LIBS="-L$with_shcoll/lib64 -Wl,-rpath -Wl,$with_shcoll/lib64"
	      SHCOLL_LIBS="$SHCOLL_LIBS -L$with_shcoll/lib -Wl,-rpath -Wl,$with_shcoll/lib"
	      SHCOLL_LIBS="$SHCOLL_LIBS -lshcoll"
	      AC_DEFINE([HAVE_SHCOLL], [1], [SHCOLL support])
	      SHCOLL_DIR="$with_shcoll"
	      AC_DEFINE_UNQUOTED([SHCOLL_DIR], ["$SHCOLL_DIR"], [SHCOLL installation directory])
	      AC_SUBST([SHCOLL_DIR])
	      AC_SUBST([SHCOLL_LIBS])
	      shcoll_happy=yes
	      AS_BOX([Using SHCOLL in $with_shcoll])
          ],
	  [
	    AC_MSG_ERROR([Unable to find SHCOLL header file in $shcoll_hdr])
          ]
          )
      ]
      )

AM_CONDITIONAL([HAVE_SHCOLL], [test "x$shcoll_happy" != xno])
