# For license: see LICENSE file at top-level

## Combinations

For all users, any version of UCX should be fine.

The interaction with most impact is that of PMIx and Open-MPI.  Here
is a table showing tested configurations:

|Launcher|PMIx|Stability level|Note|
|---:|---:|---:|---:|
|OMPI 3.1.3|2.1.1|highest||
|OMPI 4.0.0|2.1.1|||
|OMPI 4.0.1|3.1.2||Author's more stable env|
|OMPI 4.0.1|github master|||
|OMPI github master|3.1.2|||
|OMPI github master|github master|||
|PRRTE github master|github master|lowest|Author's bleeding-edge env|

For more information about the PRRTE-based launcher, see
[prrte.md](./prrte.md).

# Prerequisites

## PMIx

v1.2.x, or newer, release; or from github master:

[Get PMIx](https://github.com/openpmix/pmix/)

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

[Get SHCOLL](https://github.com/tonycurtis/shcoll)

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
	--without-verbs \
        --with-ucx=/path/to/install/ucx \
        --with-tm
```

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
