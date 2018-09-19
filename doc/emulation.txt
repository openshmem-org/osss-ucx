
# For license: see LICENSE file at top-level

UCX Emulation
=============

Applies to: UCX github master on or after 09/17/2018.

When UCX is used in environments such as

    o TCP (inter-node, no IB devices)
    o legacy IB hardware (e.g ConnectX-3)
    o shared memory (no IB hardware)

it requires an emulation mode for RMA and AMO that uses active messages.

Programs that rely on passive progress, in which PEs do not engage in
blocking/synchronization OpenSHMEM calls for a long time, or even not
at all, will require a progress thread to service the active messages.

OSSS-UCX will start its own progress threads to handle this situation
if the environment variable SHMEM_PROGRESS_THREADS is set to either a
list of (ranges of) PEs, or the case-insensitive value "all", e.g.

    $ export SHMEM_PROGRESS_THREAD=all
    $ export SHMEM_PROGRESS_THREAD=0,1,6,7
    $ export SHMEM_PROGRESS_THREAD=0-3,7
