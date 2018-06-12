# For license: see LICENSE file at top-level

#
# TODO: needs some work like for ucx/gasnet to allow location passed in too
#

pmix_happy=no

AC_ARG_WITH([pmix],
	[AS_HELP_STRING([--with-pmix@<:@=DIR@:>@], [Use PMIx library])])

PKG_CHECK_MODULES([PMIX], [pmix], [
	pmix_happy=yes
	AC_MSG_NOTICE([Using PMIx from pkg-config for process management])
], [
   AS_IF([test -d "$with_pmix"],
   	       [
	       		pmix_hdr="$with_pmix/include/pmix.h"
	       		AS_IF([test -r "$pmix_hdr"],
			[
				PMIX_CFLAGS="-I$with_pmix/include"
				PMIX_LIBS="-L$with_pmix/lib64 -Wl,-rpath -Wl,$with_pmix/lib64"
				PMIX_LIBS="$PMIX_LIBS -L$with_pmix/lib -Wl,-rpath -Wl,$with_pmix/lib"
				PMIX_LIBS="$PMIX_LIBS -lpmix"
				PMIX_DIR="$with_pmix"
				pmix_happy=yes
				AC_MSG_NOTICE([PMIx: found installation directory])
			], [
			     AC_MSG_ERROR([Unable to find PMIx header file in $pmix_hdr])
		           ]
		)
		], [
			AC_MSG_ERROR([Unable to find PMIx in $with_pmix])
		]
		)
]
)

AS_IF([test "x$pmix_happy" != "xno"], [
	AC_DEFINE_UNQUOTED([PMIX_DIR], ["$PMIX_DIR"], [PMIx installation directory])
	AC_SUBST([PMIX_DIR])
	AC_DEFINE_UNQUOTED([PMIX_CFLAGS], ["$PMIX_CFLAGS"], [PMIx Compile Flags])
	AC_DEFINE_UNQUOTED([PMIX_LIBS], ["$PMIX_LIBS"], [PMIx Compile Libraries])
	AC_SUBST(PMIX_CFLAGS)
	AC_SUBST(PMIX_LIBS)
	AC_DEFINE([HAVE_PMIX], [1], [PMIx support])
	#
	# so we have pmix, but is it v1 or >= v2 ?
	#
	save_CFLAGS="$CFLAGS"
	CFLAGS="$PMIX_CFLAGS"
	AC_COMPILE_IFELSE(
		[AC_LANG_PROGRAM([[#include <pmix.h>]],
		 	[[pmix_proc_t p; PMIx_Init(&p); PMIx_Finalize();]])
		],
		[pmix1=yes], [pmix1=no]
		)
	CFLAGS="$save_CFLAGS"
	AS_IF([test "x$pmix1" = "xyes" ],
		    [AC_DEFINE([HAVE_PMIX_NO_INIT_HINTS], [1], [PMIx v1 series])])
	]
)

AS_IF([test "x$pmix_happy" = "xyes"],
      [hdr="$PMIX_DIR/include/pmix_version.h"
       maj=`awk '$2 == "PMIX_VERSION_MAJOR" {print strtonum($3)}' $hdr`
       min=`awk '$2 == "PMIX_VERSION_MINOR" {print strtonum($3)}' $hdr`

       PMIX_VERSION_STRING=`printf "%lu.%lu" $maj $min`
       AS_BOX(PMIx version is $PMIX_VERSION_STRING)

       AC_DEFINE_UNQUOTED([PMIX_VERSION_STRING], ["$PMIX_VERSION_STRING"], [Version of PMIx])
       AC_SUBST([PMIX_VERSION_STRING])
      ]
)

AM_CONDITIONAL([HAVE_PMIX], [test "x$pmix_happy" != "xno"])
