#ifndef _SHMEMI_PMIX_H
#define _SHMEMI_PMIX_H 1

void shmemc_pmix_client_finalize(void);
void shmemc_pmix_client_init(void);

void shmemc_pmix_barrier_all(void);

void shmemc_pmix_publish_heap_info(void);
void shmemc_pmix_exchange_heap_info(void);

void shmemc_pmix_publish_worker(void);
void shmemc_pmix_exchange_workers(void);

void shmemc_pmix_publish_rkey(void);
void shmemc_pmix_exchange_rkeys(void);

#endif /* ! _SHMEMI_PMIX_H */
