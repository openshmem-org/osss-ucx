/* For license: see LICENSE file at top-level */

#include "shmemu.h"

void
shmemu_init(void)
{
    shmemu_timer_init();
    shmemu_logger_init();
}

void
shmemu_finalize(void)
{
    shmemu_logger_finalize();
    shmemu_timer_finalize();
}
