/* For license: see LICENSE file at top-level */

#ifndef _SHMEMC_PMI_CLIENT_H
#define _SHMEMC_PMI_CLIENT_H 1

/* what the PMI[12x] interface needs to do */

void shmemc_pmi_client_init(void);
void shmemc_pmi_client_finalize(void);
void shmemc_pmi_client_abort(const char *msg, int status);

void shmemc_pmi_barrier_all(void);

/*
 * pub
 */
#ifndef ENABLE_ALIGNED_ADDRESSES
void shmemc_pmi_publish_heap_info(void);
#endif /* ! ENABLE_ALIGNED_ADDRESSES */

void shmemc_pmi_publish_worker(void);
void shmemc_pmi_publish_keys(void);

/*
 * exchange
 */

#ifndef ENABLE_ALIGNED_ADDRESSES
void shmemc_pmi_exchange_heap_info(void);
#endif /* ! ENABLE_ALIGNED_ADDRESSES */

void shmemc_pmi_exchange_workers(void);
void shmemc_pmi_exchange_rkeys(void);

#endif /* ! _SHMEMC_PMIX_CLIENT_H */
