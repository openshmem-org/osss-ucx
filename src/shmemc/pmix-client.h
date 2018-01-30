/* For license: see LICENSE file at top-level */

#ifndef _SHMEMC_PMIX_CLIENT_H
#define _SHMEMC_PMIX_CLIENT_H 1

void shmemc_pmix_client_init(void);
void shmemc_pmix_client_finalize(void);
void shmemc_pmix_client_abort(const char *msg, int status);

void shmemc_pmix_barrier_all(void);

#ifndef ENABLE_ALIGNED_ADDRESSES
void shmemc_pmix_publish_heap_info(void);
void shmemc_pmix_exchange_heap_info(void);
#endif /* ! ENABLE_ALIGNED_ADDRESSES */

void shmemc_pmix_publish_worker(void);
void shmemc_pmix_exchange_workers(void);

void shmemc_pmix_publish_my_rkeys(void);
void shmemc_pmix_exchange_all_rkeys(void);

#endif /* ! _SHMEMC_PMIX_CLIENT_H */
