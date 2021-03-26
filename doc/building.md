# For license: see LICENSE file at top-level

## Combinations

For all users, any version of UCX should be fine.  Features are
auto-detected by our configure script.

The interaction with most impact is that of PMIx and the launcher.
Here is a table showing tested configurations:

|Launcher|PMIx|Stability level|Note|
|---:|---:|---:|---:|
|OMPI 4.0.5|3.2.2||Author's more stable env|
|OMPI 4.1.5|github master|||
|OMPI github master|4.0.0|||
|OMPI github master|github master|||
|PRRTE github master|github master|lowest|Author's bleeding-edge env|

# Prerequisites

## PMIx

v2 is no longer supported.

v3, or newer, release; or from github master:

[Get PMIx](https://github.com/openpmix/openpmix/)

### Sample author config

```shell
$ .../configure --prefix=/path/to/install/pmix
```

## UCX

Tested with all releases and from github master:

[Get UCX](https://github.com/openucx/ucx/)

If you want to build threading support, UCX should be installed with
`--enable-mt`.  UCX has some sample configurations in its `contrib`
directory.

## SHCOLL

The OpenSHMEM Collectives Library.

Tested with github master:

[Get SHCOLL](https://github.com/tonycurtis/shcoll)

SHCOLL is also bundled with the OSSS-UCX distribution to avoid
requiring an external installation, and will be used by default
(RECOMMENDED).

### Sample author config

```shell
.../configure \
        --prefix=/path/to/install/shcoll
```

## Launcher: OpenMPI, PRRTE, SLURM

### Open-MPI

For PMIx-aware "mpiexec" launcher:

[Get OpenMPI](https://www.open-mpi.org/software/ompi/)

### Sample author config

```shell
.../configure \
        --prefix=/path/to/install/openmpi \
        --without-verbs \
        --with-ucx=/path/to/install/ucx \
        --with-tm
```

### PRRTE

Open-MPI and PRRTE are basically "becoming one".  If you use the
latest Open-MPI from GitHub, the OSSS-UCX launcher will find the
Open-MPI `prte`/`prun` commands and use those, interpreting the setup
as PRRTE-based.  But it should continue to work "as is".

PRRTE is the server-side of PMIx.  It can be used instead of Open-MPI
as the launcher.

[Get PRRTE](https://github.com/openpmix/prrte/)

#### Sample author config

```shell
.../configure \
        --with-tm \
        --prefix=/path/to/install/prrte \
        --with-pmix=/path/to/install/pmix
```

OSSS-UCX's oshrun command will detect PRRTE's `prun` command and use
it instead of mpiexec.

In case of problems, the environment variable OSHRUN_DEBUG can be set
to y|1 to provide debugging output during oshrun start-up for
detection of PRRTE/Open-MPI.

### SLURM

SLURM can be configured with a PMIx plugin.  You can see if this is
supported with the command:

```shell
$ srun --mpi=list
srun: MPI types are...
srun: pmix
srun: pmix_v2
srun: none
srun: openmpi
srun: pmi2
```

With this setup, you should be able to run OSSS-UCX OpenSHMEM programs
directly with `srun`, avoiding Open-MPI/PRRTE and `oshrun` altogether.

Other job schedulers, e.g. LSF, also provide PMIx-aware launchers like
`jsrun`.

## OSSS-UCX Building Sequence

In the top-level source directory, run

```shell
./autogen.sh
```

You can build this package either in-source-tree or
out-of-source-tree.

The configure script can be told where PMIx and UCX live, and other
options, e.g.

```shell
 .../configure \
       --prefix=/path/to/install/osss-ucx \
       --with-pmix=/path/to/install/pmix \
       --with-ucx=/path/to/install/ucx \
       --with-shcoll=/path/to/install/shcoll \
       --enable-debug \
       --enable-logging \
       --enable-aligned-addresses \
       --with-heap-size=8G
```

`--enabled-aligned-addresses` is an optimization for systems in which
all the compute nodes have address space randomization turned off.

Then

```shell
$ make install
```

See [Running](running.md) about using OSSS-UCX.
