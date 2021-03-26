# For license: see LICENSE file at top-level

At a minimum, make sure OSSS-UCX, and a suitable launcher, see
[Building](building.md), are on your PATH in that priority:

```shell
$ export PATH=/path/to/install/openmpi/bin:$PATH
$ export PATH=/path/to/install/osss-ucx/bin:$PATH
```

and test:

```shell
$ which mpiexec
/path/to/install/openmpi/bin/mpiexec

$ which oshcc
/path/to/install/osss-ucx/bin/oshcc
```

You can set up a modulefile (http://modules.sourceforge.net/) to
simplify this process and set up other SHMEM environment variables
that might be of interest, e.g. something like:

```shell
#%Module1.0######

set             openmpi_root      /path/to/install/openmpi
prepend-path    PATH              $openmpi_root/bin
prepend-path    MANPATH           $openmpi_root/share/man
prepend-path    PKG_CONFIG_PATH   $openmpi_root/lib/pkgconfig

set             shmem_root        /path/to/install/osss-ucx
prepend-path    PATH              $shmem_root/bin
prepend-path    MANPATH           $shmem_root/share/man
prepend-path    PKG_CONFIG_PATH   $shmem_root/lib/pkgconfig

setenv          SHMEM_INFO        yes
```


The details of actually running an OpenSHMEM program with OSSS-UCX
will vary greatly depending on your environment, be it LSF, SLURM,
Torque, standalone...
