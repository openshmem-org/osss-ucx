#ifndef _SHMEMI_H
#define _SHMEMI_H 1

void shmemi_finalize(void);
void shmemi_init(void);

void shmemi_setup_heaps(void); /* although maybe int:nheaps? */

#endif /* ! _SHMEMI_H */
