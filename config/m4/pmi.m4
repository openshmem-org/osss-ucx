# For license: see LICENSE file at top-level

pmi_happy=no

AC_ARG_WITH([pmi],
	[AS_HELP_STRING([--with-pmi@<:@=DIR@:>@], [Use PMI1 library])])

PKG_CHECK_MODULES([PMI], [pmi], [
			pmi_happy=yes
			AC_MSG_NOTICE([Using PMI1 from pkg-config for process management])
		], [
		   AS_IF([test -d "$with_pmi"],
	   	       [
	       		AS_IF([test -r $with_pmi/include/pmi.h],
				[
					PMI_CFLAGS="-I$with_pmi/include"
					PMI_LIBS="-L$with_pmi/lib64 -Wl,-rpath, -Wl$with_pmi/lib64"
					PMI_LIBS="$PMI_LIBS -L$with_pmi/lib -Wl,-rpath -Wl,$with_pmi/lib"
					PMI_LIBS="$PMI_LIBS -lpmi"
					PMI_DIR="$with_pmi"
					pmi_happy=yes
					AC_MSG_NOTICE([PMI: no pkg-config, but found installation directory])
				]
			)
			], [	]
		)
	]
	)

AS_IF([test "x$pmi_happy" != "xno"], [
			AC_DEFINE_UNQUOTED([PMI_DIR], ["$PMI_DIR"], [PMI installation directory])
			AC_SUBST([PMI_DIR])
			AC_DEFINE_UNQUOTED([PMI_CFLAGS], ["$PMI_CFLAGS"], [PMI Compile Flags])
			AC_DEFINE_UNQUOTED([PMI_LIBS], ["$PMI_LIBS"], [PMI Compile Libraries])
			AC_SUBST(PMI_CFLAGS)
			AC_SUBST(PMI_LIBS)
			AC_DEFINE([HAVE_PMI], [1], [PMI support])
		]
	)

AM_CONDITIONAL([HAVE_PMI], [test "x$pmi_happy" != "xno"])
