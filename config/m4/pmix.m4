# For license: see LICENSE file at top-level

pmix_happy=no

AC_ARG_WITH([pmix],
	[AS_HELP_STRING([--with-pmix@<:@=DIR@:>@], [Use PMIx library])])

AS_IF([test "x$with_pmix" = "xyes"], [
	PKG_CHECK_MODULES([PMIX], [pmix], [
              pmix_include_dir="`pkg-config --variable=includedir pmix`"
              pmix_happy=yes
            ]
	  )
	], [
            AS_IF([test -d "$with_pmix"], [
   	         pmix_include_dir="$with_pmix/include"
                 pmix_hdr="$pmix_include_dir/pmix.h"
            	 AS_IF([test -r "$pmix_hdr"], [
		        PMIX_CFLAGS="-I$pmix_include_dir"
		        PMIX_LIBS="-L$with_pmix/lib64 -Wl,-rpath -Wl,$with_pmix/lib64"
		        PMIX_LIBS="$PMIX_LIBS -L$with_pmix/lib -Wl,-rpath -Wl,$with_pmix/lib"
		        PMIX_LIBS="$PMIX_LIBS -lpmix"
		        AC_MSG_NOTICE([PMIx: found installation])
			pmix_happy=yes
                    ]
		 )
	      ]
            )
	 ]
)

AS_IF([test "x$pmix_happy" = "xno"], [
        AC_MSG_ERROR([Cannot find required PMIx support])
    ], [
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
	  		         	    [[pmix_proc_t p; PMIx_Init(NULL); PMIx_Finalize();]])],
	 	    [pmix1=yes], [pmix1=no]
		    )

	CFLAGS="$save_CFLAGS"
	AS_IF([test "x$pmix1" = "xyes" ],
		    [AC_DEFINE([HAVE_PMIX_NO_INIT_HINTS], [1], [PMIx v1 series])])

	hdr="$pmix_include_dir/pmix_version.h"

	maj=`awk '$2 == "PMIX_VERSION_MAJOR"   {print 0 + $3}' $hdr`
	min=`awk '$2 == "PMIX_VERSION_MINOR"   {print 0 + $3}' $hdr`
	rel=`awk '$2 == "PMIX_VERSION_RELEASE" {print 0 + $3}' $hdr`

	PMIX_VERSION_STRING=`printf "%u.%u.%u" $maj $min $rel`
	AS_BOX(PMIx version is $PMIX_VERSION_STRING)

	min_pmix_version=3

	AS_IF([test $maj -lt $min_pmix_version], [
	   AC_MSG_ERROR([PMIx version $PMIX_VERSION_STRING is too old.  Use version $min_pmix_version and above])
	 ]
	)

	AC_DEFINE_UNQUOTED([PMIX_VERSION_STRING], ["$PMIX_VERSION_STRING"], [Version of PMIx])
	AC_SUBST([PMIX_VERSION_STRING])
    ]
)

AM_CONDITIONAL([HAVE_PMIX], [test "x$pmix_happy" != "xno"])
