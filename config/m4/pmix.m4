#
# TODO: needs some work like for ucx/gasnet to allow location passed in too
#

pmix_happy=no

AC_ARG_WITH([pmix],
	[AS_HELP_STRING([--with-pmix], [Use PMIx library])])

PKG_CHECK_MODULES([PMIX], [pmix], [
			pmix_happy=yes
			AC_MSG_NOTICE([Using PMIx from pkg-config for process management])
		], [
		   AS_IF([test -d "$with_pmix"],
		   	       [
			       		AS_IF([test -r $with_pmix/include/pmix.h],
					[
						PMIX_CFLAGS="-I$with_pmix/include"
						PMIX_LIBS="-L$with_pmix/lib -Wl,-rpath,$with_pmix/lib -lpmix"
						PMIX_DIR="$with_pmix"
						pmix_happy=yes
						AC_MSG_NOTICE([PMIx: no pkg-config, but found installation directory])
					]
				)
				], [
					AC_MSG_ERROR([Unable to find PMIx])
				]
				)
	]
	)

AS_IF([test "x$pmix_happy" != "xno"], [
			# AC_DEFINE_UNQUOTED([PMIX_DIR], ["$PMIX_DIR"], [PMIx installation directory])
			# AC_SUBST([PMIX_DIR])
			AC_DEFINE_UNQUOTED([PMIX_CFLAGS], ["$PMIX_CFLAGS"], [PMIx Compile Flags])
			AC_DEFINE_UNQUOTED([PMIX_LIBS], ["$PMIX_LIBS"], [PMIx Compile Libraries])
			AC_SUBST(PMIX_CFLAGS)
			AC_SUBST(PMIX_LIBS)
			AC_DEFINE([HAVE_PMIX], [1], [PMIx support])
		]
	)

AM_CONDITIONAL([HAVE_PMIX], [test "x$pmix_happy" != "xno"])
