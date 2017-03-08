#ifndef _TIMER_H
#define _TIMER_H 1

void shmemi_timer_init(void);
void shmemi_timer_finalize(void);
long shmemi_timer_get_elapsed_us(void);

#endif /* ! _TIMER_H 1 */
