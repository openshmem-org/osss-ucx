#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "shmemi/shmemi.h"
#include "shmemu/shmemu.h"

void
shmem_init(void)
{
    shmemu_init();
    shmemi_init();

    /* urgh! */
}

void
shmem_finalize(void)
{
    shmemi_finalize();
    shmemu_finalize();
}

void
start_pes(int n /* unused */)
{
    deprecate(__func__);
    shmem_init();
}
