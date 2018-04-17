# For license: see LICENSE file at top-level

Prerequisites
=============

This package must be compiled with --enable-debug

Usage
=====

In the calling environment, the variable

    SHMEM_DEBUG

must be set to a "true" value ("1", "y").

E.g.

    $ export SHMEM_DEBUG=y
    $ oshrun ...

