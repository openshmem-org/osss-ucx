# For license: see LICENSE file at top-level

Prerequisites
=============

This package must be compiled with --enable-logging

Usage
=====

In the calling environment, the variables

    SHMEM_LOGGING

must be set to a "true" value ("1", "y"), and

    SHMEM_LOGGING_EVENTS

must be set to a list of requested message categories (see "oshrun"
man page for a full list).

E.g.

    $ export SHMEM_LOGGING=y
    $ export SHMEM_LOGGING_EVENTS=all
    $ oshrun ...

Output Format
=============

Log lines are formatted, e.g.

    [0:cn082:60765:0.000128] INIT: init_thread_helper(requested=0, provided->0)

which is

    [ PE# : hostname : pid : elapsed-time ] event-type message

The message is often

    routine ( arg=value, ... ) [ -> return-value ]
