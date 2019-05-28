/* For license: see LICENSE file at top-level */

#include <stdio.h>
#include <sys/utsname.h>

#include <shmem.h>

int
main(void)
{
    struct utsname u;
    int me, npes;

    uname(&u);

    shmem_init();

    me = shmem_my_pe();
    npes = shmem_n_pes();

    printf("%s: Hello from PE %4d of %4d\n", u.nodename, me, npes);

    shmem_finalize();

    return 0;
}
