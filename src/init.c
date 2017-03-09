#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "shmemi/shmemi.h"

void
shmem_init(void)
{
    shmemi_init();

    /* urgh! */
}

void
shmem_finalize(void)
{
    shmemi_finalize();
}
