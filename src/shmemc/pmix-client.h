#ifndef _SHMEMI_PMIX_H
#define _SHMEMI_PMIX_H 1

void pmix_client_finalize(void);
void pmix_client_init(void);

void pmix_publish_heap_info(void);
void pmix_exchange_heap_info(void);

#endif /* ! _SHMEMI_PMIX_H */
