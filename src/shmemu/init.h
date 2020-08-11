/* For license: see LICENSE file at top-level */

#ifndef _SHMEMU_INIT_H
#define _SHMEMU_INIT_H 1

void shmemu_deprecate_init(void);
void shmemu_deprecate_finalize(void);

void shmemu_timer_init(void);
void shmemu_timer_finalize(void);

void shmemu_logger_init(void);
void shmemu_logger_finalize(void);

#endif /* ! _SHMEMU_INIT_H */
