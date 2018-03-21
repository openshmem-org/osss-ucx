# For license: see LICENSE file at top-level

# yes, I really do want to override here
CFLAGS=
LDFLAGS=

bin_PROGRAMS           = osh_info

osh_info_SOURCES       = osh_info.c ../info.c
osh_info_CPPFLAGS      = -I$(top_srcdir)/src
osh_info_LDFLAGS       =
osh_info_LDADD         =

banana:
	@echo CFLAGS=$(CFLAGS) LDFLAGS=$(LDFLAGS)
