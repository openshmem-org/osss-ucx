#ifndef _SHMEMI_PMIX_H
#define _SHMEMI_PMIX_H 1

void shmemi_finalize_pmix(void);
void shmemi_init_pmix(void);

void shmemi_setup_heaps_pmix(void); /* although maybe int:nheaps? */

#endif /* ! _SHMEMI_PMIX_H */
