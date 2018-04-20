#!/bin/sh
# For license: see LICENSE file at top-level

mkdir -p m4
autoreconf -ivf
rm -rf autom4te.cache
