# For license: see LICENSE file at top-level

ucx_happy=no

AC_ARG_WITH([ucx],
            [AS_HELP_STRING([--with-ucx@<:@=DIR@:>@], [Use UCX library])])

AS_IF([test "x$with_ucx" = "xyes"], [
        PKG_CHECK_MODULES([UCX], [ucx], [
              ucx_include_dir="`pkg-config --variable=includedir ucx`"
              ucp_include_dir="$ucx_include_dir/ucp/api"
              ucp_hdr="$ucp_include_dir/ucp.h"
              ucx_happy=yes
            ]
          )
        ], [
            AS_IF([test -d "$with_ucx"], [
                 ucx_include_dir="$with_ucx/include"
                 ucp_include_dir="$ucx_include_dir/ucp/api"
                 ucp_hdr="$ucp_include_dir/ucp.h"
                 AS_IF([test -r "$ucp_hdr"], [
                         UCX_CFLAGS="-I$ucx_include_dir"
                         UCX_LIBS="-L$with_ucx/lib64 -Wl,-rpath -Wl,$with_ucx/lib64"
                         UCX_LIBS="$UCX_LIBS -L$with_ucx/lib -Wl,-rpath -Wl,$with_ucx/lib"
                         UCX_LIBS="$UCX_LIBS -lucp"
                         ucx_happy=yes
                     ]
                 )
               ]
             )
           ]
)

AS_IF([test "x$ucx_happy" = "xno"], [
            AC_MSG_ERROR([Cannot find required UCX support])
    ], [
            CPPFLAGS="-I$ucx_include_dir $CPPFLAGS"
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
            #
            # check for non/blocking put/get.
            # The blocking routines are now deprecated.
            #
            AC_COMPILE_IFELSE(
                [AC_LANG_PROGRAM([[#include <ucp/api/ucp.h>]], [ucp_put])],
                [AC_MSG_NOTICE([UCX: ucp_put found])
               AC_DEFINE([HAVE_UCP_PUT], [1], [UCX has blocking ucp_put])
                ],
                [AC_MSG_NOTICE([UCX: ucp_put NOT found])
                ])
            AC_COMPILE_IFELSE(
                [AC_LANG_PROGRAM([[#include <ucp/api/ucp.h>]], [ucp_get])],
                [AC_MSG_NOTICE([UCX: ucp_get found])
               AC_DEFINE([HAVE_UCP_GET], [1], [UCX has blocking ucp_get])
                ],
                [AC_MSG_NOTICE([UCX: ucp_get NOT found])
                ])
            AC_COMPILE_IFELSE(
                [AC_LANG_PROGRAM([[#include <ucp/api/ucp.h>]], [ucp_put_nb])],
                [AC_MSG_NOTICE([UCX: ucp_put_nb found])
               AC_DEFINE([HAVE_UCP_PUT_NB], [1], [UCX has non-blocking ucp_put])
                ],
                [AC_MSG_NOTICE([UCX: ucp_put_nb NOT found])
                ])
            AC_COMPILE_IFELSE(
                [AC_LANG_PROGRAM([[#include <ucp/api/ucp.h>]], [ucp_get_nb])],
                [AC_MSG_NOTICE([UCX: ucp_get_nb found])
               AC_DEFINE([HAVE_UCP_GET_NB], [1], [UCX has non-blocking ucp_get])
                ],
                [AC_MSG_NOTICE([UCX: ucp_get_nb NOT found])
                ])
            AC_COMPILE_IFELSE(
                [AC_LANG_PROGRAM([[#include <ucp/api/ucp.h>]], [ucp_put_nbi])],
                [AC_MSG_NOTICE([UCX: ucp_put_nbi found])
               AC_DEFINE([HAVE_UCP_PUT_NBI], [1], [UCX has non-blocking implicit ucp_put])
                ],
                [AC_MSG_NOTICE([UCX: ucp_put_nbi NOT found])
                ])
            AC_COMPILE_IFELSE(
                [AC_LANG_PROGRAM([[#include <ucp/api/ucp.h>]], [ucp_get_nbi])],
                [AC_MSG_NOTICE([UCX: ucp_get_nbi found])
               AC_DEFINE([HAVE_UCP_GET_NBI], [1], [UCX has non-blocking implicit ucp_get])
                ],
                [AC_MSG_NOTICE([UCX: ucp_get_nbi NOT found])
                ])
            # see if contexts can be named
            AS_IF([fgrep -q UCP_PARAM_FIELD_NAME $ucp_hdr],
                [AC_MSG_NOTICE([UCX: contexts can be named])
               AC_DEFINE([HAVE_UCP_PARAM_FIELD_NAME], [1], [UCX contexts can be named])
                ],
                [AC_MSG_NOTICE([UCX: no context name field])
                ])
            AC_COMPILE_IFELSE(
                [AC_LANG_PROGRAM([[#include <ucp/api/ucp.h>]], [ucp_ep_close_nbx])],
                [AC_MSG_NOTICE([UCX: ucp_ep_close_nbx found])
               AC_DEFINE([HAVE_UCP_EP_CLOSE_NBX], [1], [UCX has extended ucp_ep_close_nbx])
                ],
                [AC_MSG_NOTICE([UCX: ucp_ep_close_nbx NOT found])
                ])
            AC_COMPILE_IFELSE(
                [AC_LANG_PROGRAM([[#include <ucp/api/ucp.h>]], [ucp_ep_flush_nbx])],
                [AC_MSG_NOTICE([UCX: ucp_ep_flush_nbx found])
               AC_DEFINE([HAVE_UCP_EP_FLUSH_NBX], [1], [UCX has extended ucp_ep_flush_nbx])
                ],
                [AC_MSG_NOTICE([UCX: ucp_ep_flush_nbx NOT found])
                ])
            AC_COMPILE_IFELSE(
                [AC_LANG_PROGRAM([[#include <ucp/api/ucp.h>]], [ucp_put_nbx])],
                [AC_MSG_NOTICE([UCX: ucp_put_nbx found])
               AC_DEFINE([HAVE_UCP_PUT_NBX], [1], [UCX has non-blocking extended ucp_put])
                ],
                [AC_MSG_NOTICE([UCX: ucp_put_nbx NOT found])
                ])
            AC_COMPILE_IFELSE(
                [AC_LANG_PROGRAM([[#include <ucp/api/ucp.h>]], [ucp_get_nbx])],
                [AC_MSG_NOTICE([UCX: ucp_get_nbx found])
               AC_DEFINE([HAVE_UCP_GET_NBX], [1], [UCX has non-blocking extended ucp_get])
                ],
                [AC_MSG_NOTICE([UCX: ucp_get_nbx NOT found])
                ])
            AC_COMPILE_IFELSE(
                [AC_LANG_PROGRAM([[#include <ucp/api/ucp.h>]], [ucp_worker_flush_nbx])],
		[AC_MSG_NOTICE([UCX: ucp_worker_flush_nbx found])
               AC_DEFINE([HAVE_UCP_WORKER_FLUSH_NBX], [1], [UCX has non-blocking extended ucp_worker_flush])
                ],
                [AC_MSG_NOTICE([UCX: ucp_worker_flush_nbx NOT found])
                ])
            AC_LANG_POP([C])
            AC_SUBST([UCX_LIBS])

            hdr="$ucp_include_dir/ucp_version.h"
            maj=`awk '$2 == "UCP_API_MAJOR" {print $3}' $hdr`
            min=`awk '$2 == "UCP_API_MINOR" {print $3}' $hdr`

            UCX_VERSION_STRING=`printf "%u.%u" $maj $min`
            AS_BOX(UCX version is $UCX_VERSION_STRING)

            AC_DEFINE_UNQUOTED([UCX_VERSION_STRING], ["$UCX_VERSION_STRING"], [Version of UCX])
            AC_SUBST([UCX_VERSION_STRING])
    ]
)

AM_CONDITIONAL([HAVE_UCX], [test "x$ucx_happy" != xno])
