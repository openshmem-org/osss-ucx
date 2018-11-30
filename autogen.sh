#!/bin/sh
# For license: see LICENSE file at top-level

set -e

mkdir -p m4
autoreconf -ivf
rm -rf autom4te.cache
