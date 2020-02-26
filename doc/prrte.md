# For license: see LICENSE file at top-level

# Using PRRTE as launcher

## NOTE:

Open-MPI and PRRTE are basically "becoming one".  If you use the
latest Open-MPI from GitHub, the OSSS-UCX launcher will find the
Open-MPI `prte`/`prun` commands and use those, interpreting the setup
as PRRTE-based.  But it should continue to work "as is".

PRRTE is the server-side of PMIx.  It can be used instead of Open-MPI
as the launcher.

[Get PRRTE](https://github.com/openpmix/prrte/)

## Sample author config

```sh
.../configure \
        --with-tm \
        --prefix=/path/to/install/prrte \
        --with-pmix=/path/to/install/pmix
```

OSSS-UCX's oshrun command will detect PRRTE's prun command and use it
instead of mpiexec.

In case of problems, the environment variable OSHRUN_DEBUG can be set
to y|1 to provide debugging output during oshrun start-up for
detection of PRRTE/Open-MPI.
