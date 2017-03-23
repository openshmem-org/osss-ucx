#include "shmemu.h"

shmemu_init(void)
{
    shmemu_timer_init();
}

void
shmemu_finalize(void)
{
    shmemu_timer_finalize();
}
