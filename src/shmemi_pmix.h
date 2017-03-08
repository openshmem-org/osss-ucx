#ifndef _SHMEMI_PMIX_H
#define _SHMEMI_PMIX_H 1

void shmemi_finalize_pmix(void);
void shmemi_init_pmix(void);

int shmemi_my_pe_pmix(void);
int shmemi_n_pes_pmix(void);

void shmemi_setup_heaps_pmix(void); /* although maybe int:nheaps? */

#endif /* ! _SHMEMI_PMIX_H */
