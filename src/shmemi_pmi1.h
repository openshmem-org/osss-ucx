#ifndef _SHMEMI_PMI1_H
#define _SHMEMI_PMI1_H 1

void shmemi_finalize_pmi1(void);
void shmemi_init_pmi1(void);

int shmemi_my_pe_pmi1(void);
int shmemi_n_pes_pmi1(void);

#endif /* ! _SHMEMI_PMI1_H */
