# For license: see LICENSE file at top-level

ucx_happy=no

AC_ARG_WITH([ucx],
            [AS_HELP_STRING([--with-ucx@<:@=DIR@:>@], [Use UCX library])])

AS_IF([test -d "$with_ucx"],
      [
          AS_IF([test -r "$with_ucx/include/ucp/api/ucp.h"],
          [
	      CPPFLAGS="-I$with_ucx/include $CPPFLAGS"
	      UCX_LIBS="-L$with_ucx/lib -Wl,-rpath,$with_ucx/lib -lucp -luct -lucs"
	      LDFLAGS="$UCX_LIBS $LDFLAGS"
	      AC_DEFINE([HAVE_UCX], [1], [UCX support])
	      AC_MSG_NOTICE([UCX: checking for API features...])
	      AC_LANG_PUSH([C])
	      AC_COMPILE_IFELSE(
		[AC_LANG_PROGRAM([[#include <ucp/api/ucp.h>]], [ucp_ep_close_nb])],
		[AC_MSG_NOTICE([UCX: ucp_ep_close_nb found])
 	         AC_DEFINE([HAVE_UCP_EP_CLOSE_NB], [1], [UCX has ucp_ep_close_nb])
		],
		[AC_MSG_NOTICE([UCX: ucp_ep_close_nb NOT found])
		])
	      AC_COMPILE_IFELSE(
		[AC_LANG_PROGRAM([[#include <ucp/api/ucp.h>]], [ucp_rkey_ptr])],
		[AC_MSG_NOTICE([UCX: ucp_rkey_ptr found])
 	         AC_DEFINE([HAVE_UCP_RKEY_PTR], [1], [UCX has ucp_rkey_ptr])
		],
		[AC_MSG_NOTICE([UCX: ucp_rkey_ptr NOT found])
		])
	      AC_COMPILE_IFELSE(
		[AC_LANG_PROGRAM([[#include <ucp/api/ucp.h>]], [ucp_request_check_status])],
		[AC_MSG_NOTICE([UCX: ucp_request_check_status found])
 	         AC_DEFINE([HAVE_UCP_REQUEST_CHECK_STATUS], [1], [UCX has ucp_request_check_status])
		],
		[AC_MSG_NOTICE([UCX: ucp_request_check_status NOT found])
		])
	      AC_COMPILE_IFELSE(
		[AC_LANG_PROGRAM([[#include <ucp/api/ucp.h>]], [ucp_atomic_and64])],
		[AC_MSG_NOTICE([UCX: ucp_atomic_and64 found])
 	         AC_DEFINE([HAVE_UCP_BITWISE_ATOMICS], [1], [UCX has bitwise atomics])
		],
		[AC_MSG_NOTICE([UCX: ucp_atomic_and64 NOT found])
		])
	      AC_LANG_POP([C])
	      UCX_DIR="$with_ucx"
	      AC_DEFINE_UNQUOTED([UCX_DIR], ["$UCX_DIR"], [UCX installation directory])
	      AC_SUBST([UCX_DIR])
	      AC_SUBST([UCX_LIBS])
	      ucx_happy=yes
	      AC_MSG_NOTICE([Selecting UCX as communications layer])
          ]
          )
      ]
      )
      
AM_CONDITIONAL([HAVE_UCX], [test "x$ucx_happy" != xno])
