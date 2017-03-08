#ifndef _SHMEMI_PMI2_H
#define _SHMEMI_PMI2_H 2

void shmemi_finalize_pmi2(void);
void shmemi_init_pmi2(void);

int shmemi_my_pe_pmi2(void);
int shmemi_n_pes_pmi2(void);

#endif /* ! _SHMEMI_PMI2_H */
