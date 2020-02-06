# For license: see LICENSE file at top-level

# Feature checks

#
# debugging checks: disabled by default
#
AC_ARG_ENABLE([debug],
	AS_HELP_STRING([--enable-debug], [Enable library debuging @<:@default=no@:>@]),
	AS_IF([test "x$enableval" = "xyes"],
		    AC_DEFINE([ENABLE_DEBUG], [1], [Enable debug])
		    ),
	[])
AM_CONDITIONAL([ENABLE_DEBUG], [test "x$enable_debug" = "xyes"])

AS_IF([test "x$enable_debug" != "xyes"], [enable_debug=no])

#
# logging output: disabled by default
#
AC_ARG_ENABLE([logging],
	AS_HELP_STRING([--enable-logging], [Enable library logging outptut @<:@default=no@:>@]),
	AS_IF([test "x$enableval" = "xyes"],
		    AC_DEFINE([ENABLE_LOGGING], [1], [Enable logging])
		    ),
	[])
AM_CONDITIONAL([ENABLE_LOGGING], [test "x$enable_logging" = "xyes"])

AS_IF([test "x$enable_logging" != "xyes"], [enable_logging=no])

#
# non-standard/proposed API features (shmemx): disabled by default
#
AC_ARG_ENABLE([experimental],
	AS_HELP_STRING([--enable-experimental],
			[Enable non-standard extensions @<:@default=no@:>@]))
AS_IF([test "x$enable_experimental" = "xyes"],
	[AC_DEFINE([ENABLE_EXPERIMENTAL], [1], [Enable non-standard extensions])
	AC_SUBST([ENABLE_EXPERIMENTAL], [1])],
	[AC_SUBST([ENABLE_EXPERIMENTAL], [0])]
	)
AM_CONDITIONAL([ENABLE_EXPERIMENTAL], [test "x$enable_experimental" = "xyes"])

AS_IF([test "x$enable_experimental" != "xyes"], [enable_experimental=no])

#
# profiling API: disabled by default
#
AC_ARG_ENABLE([pshmem],
	AS_HELP_STRING([--enable-pshmem],
			[Enable Profiling interface @<:@default=no@:>@]))
AS_IF([test "x$enable_pshmem" = "xyes"],
	[AC_DEFINE([ENABLE_PSHMEM], [1], [Enable profiling interface])
	 AC_SUBST([ENABLE_PSHMEM], [1])],
	[AC_SUBST([ENABLE_PSHMEM], [0])]
	)
AM_CONDITIONAL([ENABLE_PSHMEM], [test "x$enable_pshmem" = "xyes"])

AS_IF([test "x$enable_pshmem" != "xyes"], [enable_pshmem=no])

#
# address translation: disabled by default
#
AC_ARG_ENABLE([aligned-addresses],
	AS_HELP_STRING([--enable-aligned-addresses],
			[Symmetric addresses are identical everywhere @<:@default=no@:>@]))
AS_IF([test "x$enable_aligned_addresses" = "xyes"],
	[AC_DEFINE([ENABLE_ALIGNED_ADDRESSES], [1], [Enable aligned symmetric addresses])
	 AC_SUBST([ENABLE_ALIGNED_ADDRESSES], [1])],
	[AC_SUBST([ENABLE_ALIGNED_ADDRESSES], [0])]
	)
AM_CONDITIONAL([ENABLE_ALIGNED_ADDRESSES], [test "x$enable_aligned_addresses" = "xyes"])

AS_IF([test "x$enable_aligned_addresses" != "xyes"],
	    [enable_aligned_addresses=no])

#
# threads: enabled by default
#
AC_ARG_ENABLE([threads],
	AS_HELP_STRING([--disable-threads],
		       [Are threading levels supported? @<:@default=yes@:>@]))
AS_IF([test "x$enable_threads" != "xno"],
	[AC_DEFINE([ENABLE_THREADS], [1], [Enable threading support])
	 AC_SUBST([ENABLE_THREADS], [1])],
	[AC_SUBST([ENABLE_THREADS], [0])]
	)
AM_CONDITIONAL([ENABLE_THREADS], [test "x$enable_threads" != "xno"])

AS_IF([test "x$enable_threads" != "xno"], [enable_threads=yes])

#
# Default symmetric heap size
#
AC_ARG_WITH([heap-size],
	AS_HELP_STRING([--with-heap-size=VALUE],
			[Make the default symmetric heap size VALUE @<:@default=shmem_default_heap_size@:>@]))
AS_IF([test "x$with_heap_size" != "x"],
	[shmem_heap_size="$with_heap_size"])
AM_CONDITIONAL([SHMEM_DEFAULT_HEAP_SIZE], [test "x$with_heapsize" != "x"])

#
# specify launcher program
#
AC_ARG_WITH([launcher],
	AS_HELP_STRING([--with-launcher=NAME],
			[Use program NAME as OpenSHMEM launcher @<:@default=search@:>@]))
AS_IF([test "x$with_launcher" != "x"],
	[AC_DEFINE([SHMEM_LAUNCHER], [$with_launcher], [OpenSHMEM launcher])
	 AC_SUBST([SHMEM_LAUNCHER], [$with_launcher])]
	 )
AM_CONDITIONAL([SHMEM_LAUNCHER], [test "x$with_launcher" != "x"])
