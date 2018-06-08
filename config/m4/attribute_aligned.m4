# For license: see LICENSE file at top-level

AC_COMPILE_IFELSE(
	[AC_LANG_SOURCE([[struct foo { char text[4]; }  __attribute__ ((__aligned__(8)));]])],
	[
	  AC_DEFINE([ATTRIBUTE_ALIGNED], [__attribute__ ((__aligned__(8)))], [Support compiler attribute aligned])
	],
	[
	  AC_DEFINE([ATTRIBUTE_ALIGNED], [], [Do not support compiler attribute aligned])
	]
	)
