#ifndef _SHMEM_SHEMMU_H
#define _SHMEM_SHEMMU_H 1

void shmemu_init(void);
void shmemu_finalize(void);

void shmemu_timer_init(void);
void shmemu_timer_finalize(void);
double shmemu_timer(void);

char *shmemu_gethostname(void);

#endif /* ! _SHMEM_SHEMMU_H */
