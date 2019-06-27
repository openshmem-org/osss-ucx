# For license: see LICENSE file at top-level

# Prerequisites

## PMIx

v1.2.x, or newer, release; or from github master:

[Get PMIx](https://github.com/pmix/pmix/)

### Sample author config

```sh
$ .../configure --prefix=/path/to/install/pmix
```

## UCX

Tested with all releases and from github master:

[Get UCX](https://github.com/openucx/ucx/)

If you want to build threading support, UCX should be installed with
`--enable-mt`.

## SHCOLL

The OpenSHMEM Collectives Library.

Tested with github master:

[Get SHCOLL](https://github.com/tonycurtis/OpenSHMEM-Collective-Routines)

SHCOLL is also bundled with the OSSS-UCX distribution to avoid
requiring an external installation, and will be used by default.

### Sample author config

```sh
.../configure \
        --prefix=/path/to/install/shcoll
```

## OpenMPI

For PMIx-aware "mpiexec" launcher:

[Get OpenMPI](https://www.open-mpi.org/software/ompi/)

### Sample author config

```sh
.../configure \
        --prefix=/path/to/install/openmpi \
        --with-knem \
        --with-ucx=/path/to/install/ucx \
        --with-tm
```

## Combinations

For all users, any version of UCX is fine.

The interaction with most impact is that of PMIx and Open-MPI:

* For end-users, the most stable combinations are
    * PMIx 1.2.5 and Open-MPI 2.1.3
    * PMIx 2.1.1 and Open-MPI 3.1.0, 4.0.0
* For developers, the github master versions throughout will track
  all the latest updates.

A PRRTE-based launcher is in development, but as PRRTE is in flux,
this will not be documented here for now, see [prrte.txt](./prrte.md)
for more information.

## Building Sequence

In the top-level source directory, run

```sh
./autogen.sh
```

You can build this package either in-source-tree or
out-of-source-tree.

The configure script can be told where PMIx and UCX live, and other
options, e.g.

```sh
 .../configure \
       --prefix=/path/to/install/osss-ucx \
       --with-pmix=/path/to/install/pmix \
       --with-ucx=/path/to/install/ucx \
       --with-shcoll=/path/to/install/shcoll \
       --enable-debug \
       --enable-logging \
       --with-heap-size=8G
```

Then

```sh
$ make install
```
