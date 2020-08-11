/* For license: see LICENSE file at top-level */

#ifndef _SHMEMU_INIT_H
#define _SHMEMU_INIT_H 1

#ifdef ENABLE_LOGGING

void shmemu_deprecate_init(void);
void shmemu_deprecate_finalize(void);

void shmemu_logger_init(void);
void shmemu_logger_finalize(void);

#else  /* ENABLE_LOGGING */

# define shmemu_logger_init()
# define shmemu_logger_finalize()

# define shmemu_deprecate_init()
# define shmemu_deprecate_finalize()

#endif  /* ENABLE_LOGGING */

void shmemu_timer_init(void);
void shmemu_timer_finalize(void);

#endif /* ! _SHMEMU_INIT_H */
