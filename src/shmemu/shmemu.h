#ifndef _SHMEM_SHEMMU_H
#define _SHMEM_SHEMMU_H 1

#include <sys/types.h>

void shmemu_init(void);
void shmemu_finalize(void);

void shmemu_timer_init(void);
void shmemu_timer_finalize(void);
double shmemu_timer(void);

char *shmemu_gethostname(void);

void shmemu_parse_size(char *size_str, size_t *bytes_p, int *ok_p);

/*
 * how many elements in array T?
 *
 */
#define TABLE_SIZE(_t) ( sizeof(_t) / sizeof((_t)[0]) )

#endif /* ! _SHMEM_SHEMMU_H */
