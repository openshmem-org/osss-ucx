# For license: see LICENSE file at top-level

ucx_happy=no

AC_ARG_WITH([ucx],
            [AS_HELP_STRING([--with-ucx@<:@=DIR@:>@], [Use UCX library])])

AS_IF([test -d "$with_ucx"],
      [
          ucp_hdr="$with_ucx/include/ucp/api/ucp.h"
          AS_IF([test -r "$ucp_hdr"],
          [
	      CPPFLAGS="-I$with_ucx/include $CPPFLAGS"
	      UCX_LIBS="-L$with_ucx/lib64 -Wl,-rpath -Wl,$with_ucx/lib64"
	      UCX_LIBS="$UCX_LIBS -L$with_ucx/lib -Wl,-rpath -Wl,$with_ucx/lib"
	      UCX_LIBS="$UCX_LIBS -lucp -luct -lucs"
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
	      # we'll use the following as a representative of
	      # supporting bitwise AMOs
	      AS_IF([fgrep -q UCP_ATOMIC_POST_OP_XOR $ucp_hdr],
		[AC_MSG_NOTICE([UCX: native bit-wise atomics found])
 	         AC_DEFINE([HAVE_UCP_BITWISE_ATOMICS], [1], [UCX has bitwise atomics])
		],
		[AC_MSG_NOTICE([UCX: native bit-wise atomics NOT found])
		])
	      AC_LANG_POP([C])
	      UCX_DIR="$with_ucx"
	      AC_DEFINE_UNQUOTED([UCX_DIR], ["$UCX_DIR"], [UCX installation directory])
	      AC_SUBST([UCX_DIR])
	      AC_SUBST([UCX_LIBS])
	      ucx_happy=yes
          ]
          )
      ]
      )

AS_IF([test "x$ucx_happy" = "xyes"],
      [hdr="$UCX_DIR/include/ucp/api/ucp_version.h"
       maj=`awk '$2 == "UCP_API_MAJOR" {print $3}' $hdr`
       min=`awk '$2 == "UCP_API_MINOR" {print $3}' $hdr`

       UCX_VERSION=`printf "%u.%u" $maj $min`
       AS_BOX(UCX version is $UCX_VERSION)

       AC_DEFINE_UNQUOTED([UCX_VERSION], ["$UCX_VERSION"], [Version of UCX])
       AC_SUBST([UCX_VERSION])
       
       # AC_MSG_NOTICE([Selecting UCX as communications layer])
      ]
)

AM_CONDITIONAL([HAVE_UCX], [test "x$ucx_happy" != xno])
